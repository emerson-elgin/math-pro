#include <iostream>
#include <vector>
#include <queue>
#include <set>
using namespace std;

void bfs(vector<int> adj[], int V, int start) {
    set<int> visited;
    queue<int> q;

    visited.insert(start);
    q.push(start);

    while (!q.empty()) {
        int node = q.front();
        q.pop();
        cout << node << " ";

        for (int neighbor : adj[node]) {
            if (visited.find(neighbor) == visited.end()) {
                visited.insert(neighbor);
                q.push(neighbor);
            }
        }
    }
}

int main() {
    int V = 4;
    vector<int> adj[V];

    adj[0] = {1, 2};
    adj[1] = {2};
    adj[2] = {0, 3};
    adj[3] = {3};

    bfs(adj, V, 2);
    return 0;
}
