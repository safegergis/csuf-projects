from collections import deque

def bfs_shortest_path(city_graph, start, destination):
    queue = deque([start])
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
def dfs_city_navigator(city_graph, start, goal, path=None, visited=None):
    if path is None:
        path = [start]
    if visited is None:
        visited = set()
    
    visited.add(start)

    if start == goal:
        return path
    
    for neighbor in city_graph.get(start, []):
        if neighbor not in visited:
            new_path = dfs_city_navigator(city_graph, neighbor, goal, path + [neighbor], visited)
            if new_path:
                return new_path
    return None

city_graph = {
    'A': ['B', 'C'],
    'B': ['A', 'D', 'E'],
    'C': ['A', 'D'],
    'D': ['B', 'C', 'E'],
    'E': ['B', 'D']
}

print('City Navigator')
print('City Map:\n')
for key, value in city_graph.items():
    print(f'{key}: {value}\n')

start = input('Enter the starting intersection: ').upper()
goal = input('Enter the destination intersection: ').upper()

method = input('Enter the search method (BFS or DFS): ').upper()

if method == 'BFS':
    path = bfs_shortest_path(city_graph, start, goal)
    for vertex in path:
        if vertex == path[0]:
            print(vertex, end='')
        else:
            print(f' -> {vertex}', end='')
elif method == 'DFS':
    path = dfs_city_navigator(city_graph, start, goal)
    for vertex in path:
        if vertex == path[0]:
            print(vertex, end='')
        else:
            print(f' -> {vertex}', end='')
else:
    print('Invalid search method. Please enter BFS or DFS.')