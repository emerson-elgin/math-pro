#include <iostream>
#include <algorithm>
using namespace std;

// Node structure for BST
struct Node {
    int key;
    Node* left;
    Node* right;
    int height;
};

// Function to create a new node
Node* createNode(int key) {
    Node* newNode = new Node();
    newNode->key = key;
    newNode->left = nullptr;
    newNode->right = nullptr;
    newNode->height = 1; // New node is initially added at leaf
    return newNode;
}

// Get the height of the node
int getHeight(Node* node) {
    if (!node) return 0;
    return node->height;
}

// Get the balance factor of the node
int getBalanceFactor(Node* node) {
    if (!node) return 0;
    return getHeight(node->left) - getHeight(node->right);
}

// Perform right rotation
Node* rotateRight(Node* y) {
    Node* x = y->left;
    Node* T2 = x->right;

    // Perform rotation
    x->right = y;
    y->left = T2;

    // Update heights
    y->height = max(getHeight(y->left), getHeight(y->right)) + 1;
    x->height = max(getHeight(x->left), getHeight(x->right)) + 1;

    return x;
}

// Perform left rotation
Node* rotateLeft(Node* x) {
    Node* y = x->right;
    Node* T2 = y->left;

    // Perform rotation
    y->left = x;
    x->right = T2;

    // Update heights
    x->height = max(getHeight(x->left), getHeight(x->right)) + 1;
    y->height = max(getHeight(y->left), getHeight(y->right)) + 1;

    return y;
}

// Insert a node into the AVL tree
Node* insert(Node* node, int key) {
    // Perform normal BST insertion
    if (!node) return createNode(key);

    if (key < node->key)
        node->left = insert(node->left, key);
    else if (key > node->key)
        node->right = insert(node->right, key);
    else
        return node; // Duplicate keys are not allowed

    // Update the height of this ancestor node
    node->height = 1 + max(getHeight(node->left), getHeight(node->right));

    // Get the balance factor
    int balance = getBalanceFactor(node);

    // Balance the tree if unbalanced

    // Left Left Case
    if (balance > 1 && key < node->left->key)
        return rotateRight(node);

    // Right Right Case
    if (balance < -1 && key > node->right->key)
        return rotateLeft(node);

    // Left Right Case
    if (balance > 1 && key > node->left->key) {
        node->left = rotateLeft(node->left);
        return rotateRight(node);
    }

    // Right Left Case
    if (balance < -1 && key < node->right->key) {
        node->right = rotateRight(node->right);
        return rotateLeft(node);
    }

    return node; // Return the unchanged node
}

// Find the node with the smallest key (used in deletion)
Node* findMin(Node* node) {
    Node* current = node;
    while (current->left)
        current = current->left;
    return current;
}

// Delete a node from the AVL tree
Node* deleteNode(Node* root, int key) {
    // Perform standard BST delete
    if (!root) return root;

    if (key < root->key)
        root->left = deleteNode(root->left, key);
    else if (key > root->key)
        root->right = deleteNode(root->right, key);
    else {
        // Node with only one child or no child
        if (!root->left || !root->right) {
            Node* temp = root->left ? root->left : root->right;

            // No child case
            if (!temp) {
                temp = root;
                root = nullptr;
            } else // One child case
                *root = *temp;

            delete temp;
        } else {
            // Node with two children
            Node* temp = findMin(root->right);

            // Copy the inorder successor's key
            root->key = temp->key;

            // Delete the inorder successor
            root->right = deleteNode(root->right, temp->key);
        }
    }

    // If the tree had only one node
    if (!root) return root;

    // Update height
    root->height = 1 + max(getHeight(root->left), getHeight(root->right));

    // Get the balance factor
    int balance = getBalanceFactor(root);

    // Balance the tree if unbalanced

    // Left Left Case
    if (balance > 1 && getBalanceFactor(root->left) >= 0)
        return rotateRight(root);

    // Left Right Case
    if (balance > 1 && getBalanceFactor(root->left) < 0) {
        root->left = rotateLeft(root->left);
        return rotateRight(root);
    }

    // Right Right Case
    if (balance < -1 && getBalanceFactor(root->right) <= 0)
        return rotateLeft(root);

    // Right Left Case
    if (balance < -1 && getBalanceFactor(root->right) > 0) {
        root->right = rotateRight(root->right);
        return rotateLeft(root);
    }

    return root;
}

// Inorder traversal
void inorderTraversal(Node* root) {
    if (root) {
        inorderTraversal(root->left);
        cout << root->key << " ";
        inorderTraversal(root->right);
    }
}

// Driver function
int main() {
    Node* root = nullptr;

    // Insert nodes
    root = insert(root, 50);
    root = insert(root, 30);
    root = insert(root, 20);
    root = insert(root, 40);
    root = insert(root, 70);
    root = insert(root, 60);
    root = insert(root, 80);

    cout << "Inorder traversal of the AVL tree: ";
    inorderTraversal(root);
    cout << endl;

    // Delete a node
    root = deleteNode(root, 50);

    cout << "Inorder traversal after deletion: ";
    inorderTraversal(root);
    cout << endl;

    return 0;
}
