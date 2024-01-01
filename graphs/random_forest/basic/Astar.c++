#include <iostream>
#include <queue>
#include <vector>
#include <unordered_map>
#include <cmath>
#include <algorithm>
#include <limits.h>

using namespace std;

// Define a structure for a node in the grid
struct Node {
    int x, y;
    double g, h, f;
    Node* parent;
    bool isObstacle;

    Node(int x, int y) : x(x), y(y), g(INFINITY), h(0), f(INFINITY), parent(nullptr), isObstacle(false) {}

    bool operator>(const Node& other) const {
        return f > other.f; // Priority queue will prioritize lower f value
    }
};

// A* algorithm with dynamic updates and adaptive heuristics
class AStar {
public:
    AStar(int rows, int cols, pair<int, int> start, pair<int, int> goal)
        : rows(rows), cols(cols), start(start), goal(goal) {
        grid.resize(rows, vector<Node*>(cols, nullptr));
        openList = priority_queue<Node*, vector<Node*>, greater<Node*>>(); // Min-heap priority queue
        closedList.clear();

        // Create grid nodes
        for (int i = 0; i < rows; ++i) {
            for (int j = 0; j < cols; ++j) {
                grid[i][j] = new Node(i, j);
            }
        }
    }

    // Set grid cell value (0 = free, 1 = obstacle)
    void setGridCell(int x, int y, int value) {
        grid[x][y]->isObstacle = (value == 1);
    }

    // Adaptive heuristic function - can be extended for machine learning models
    double heuristic(Node* a, Node* b) {
        // Adaptive heuristic: Considering both Euclidean distance and terrain factor
        double dist = sqrt(pow(a->x - b->x, 2) + pow(a->y - b->y, 2));
        return dist * (1 + (rand() % 100) / 100.0); // Random factor for adaptive heuristic
    }

    // Reconstruct the path from start to goal
    vector<pair<int, int>> reconstructPath(Node* node) {
        vector<pair<int, int>> path;
        while (node != nullptr) {
            path.push_back({node->x, node->y});
            node = node->parent;
        }
        reverse(path.begin(), path.end());
        return path;
    }

    // Dynamic A* Search for pathfinding
    vector<pair<int, int>> search() {
        Node* startNode = grid[start.first][start.second];
        Node* goalNode = grid[goal.first][goal.second];

        startNode->g = 0;
        startNode->h = heuristic(startNode, goalNode);
        startNode->f = startNode->g + startNode->h;

        openList.push(startNode);
        closedList[{start.first, start.second}] = startNode;

        while (!openList.empty()) {
            Node* current = openList.top();
            openList.pop();

            // If the goal is reached, return the path
            if (current->x == goalNode->x && current->y == goalNode->y) {
                return reconstructPath(current);
            }

            // Explore the neighboring nodes (4 directions: up, down, left, right)
            for (const auto& direction : directions) {
                int newX = current->x + direction.first;
                int newY = current->y + direction.second;

                // Skip out-of-bounds and obstacle cells
                if (newX < 0 || newX >= rows || newY < 0 || newY >= cols || grid[newX][newY]->isObstacle)
                    continue;

                Node* neighbor = grid[newX][newY];
                double tentativeG = current->g + 1; // Uniform cost for neighbors

                // If the neighbor is not in the closed list or we found a better path to it
                if (closedList.find({newX, newY}) == closedList.end() || tentativeG < neighbor->g) {
                    neighbor->parent = current;
                    neighbor->g = tentativeG;
                    neighbor->h = heuristic(neighbor, goalNode);
                    neighbor->f = neighbor->g + neighbor->h;

                    openList.push(neighbor);
                    closedList[{newX, newY}] = neighbor;
                }
            }
        }

        return {}; // No path found
    }

private:
    int rows, cols;
    pair<int, int> start, goal;
    vector<vector<Node*>> grid;
    priority_queue<Node*, vector<Node*>, greater<Node*>> openList;
    unordered_map<pair<int, int>, Node*, hash<pair<int, int>>> closedList;

    // Directions for neighbors (up, down, left, right)
    const vector<pair<int, int>> directions = {{-1, 0}, {1, 0}, {0, -1}, {0, 1}};
};

int main() {
    int rows = 10, cols = 10;
    pair<int, int> start = {0, 0};
    pair<int, int> goal = {9, 9};

    AStar astar(rows, cols, start, goal);

    // Set some obstacles (1 represents an obstacle)
    astar.setGridCell(1, 1, 1);
    astar.setGridCell(1, 2, 1);
    astar.setGridCell(2, 2, 1);
    astar.setGridCell(3, 2, 1);
    astar.setGridCell(4, 2, 1);

    vector<pair<int, int>> path = astar.search();

    if (!path.empty()) {
        cout << "Path found: ";
        for (const auto& p : path) {
            cout << "(" << p.first << ", " << p.second << ") ";
        }
        cout << endl;
    } else {
        cout << "No path found!" << endl;
    }

    return 0;
}
