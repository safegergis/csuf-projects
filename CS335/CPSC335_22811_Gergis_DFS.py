def dfs_maze_solver(maze, start, goal, path=None, visited=None):
    if path is None:
        path = [start]
    if visited is None:
        visited = set()
    
    visited.add(start)

    if start == goal:
        return path
    
    for neighbor in maze.get(start, []):
        if neighbor not in visited:
            new_path = dfs_maze_solver(maze, neighbor, goal, path + [neighbor], visited)
            if new_path:
                return new_path
    return None

maze = {
    'Start': ['A', 'B'],
    'A': ['Start', 'C'],
    'B': ['Start', 'D'],
    'C': ['A', 'Exit'],
    'D': ['B'],
    'Exit': ['C']
}

print(dfs_maze_solver(maze, 'Start', 'Exit'))