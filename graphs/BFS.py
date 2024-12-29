from collections import deque

def bfs(graph, start):
    visited = set()
    queue = deque([start])
    visited.add(start)

    while queue:
        node = queue.popleft()
        print(node, end=" ")

        for neighbor in graph[node]:
            if neighbor not in visited:
                visited.add(neighbor)
                queue.append(neighbor)

# Example Graph as Adjacency List
graph = {
    0: [1, 2],
    1: [2],
    2: [0, 3],
    3: [3]
}
bfs(graph, 2)
