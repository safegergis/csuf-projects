from collections import deque

def bfs_shortest_path(city_graph, start, destination):
    queue = deque([start])
    visited = set()
    while queue:
        path = queue.popleft()
        vertex = path[-1]
        if vertex == destination:
            return path
        for neighbor in city_graph.get(vertex, []):
            new_path = list(path)
            new_path.append(neighbor)
            queue.append(new_path)
    return None

city_graph = {
    'A': ['B', 'C'],
    'B': ['A', 'D', 'E'],
    'C': ['A', 'D'],
    'D': ['B', 'C', 'E'],
    'E': ['B', 'D']
}

print(bfs_shortest_path(city_graph, 'A', 'E'))