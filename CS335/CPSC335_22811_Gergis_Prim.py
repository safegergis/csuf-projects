import heapq

def prims_algorithm(graph):
    mst = []
    visited = set()
    
    start_node = 'A'
    visited.add(start_node)
    
    edges = [(cost, start_node, to) for to, cost in graph[start_node].items()]
    heapq.heapify(edges)
    
    while edges:
        cost, frm, to = heapq.heappop(edges)
        
        if to not in visited:
            visited.add(to)
            mst.append((frm, to, cost))
            
            for next_to, next_cost in graph[to].items():
                if next_to not in visited:
                    heapq.heappush(edges, (next_cost, to, next_to))
    
    return mst

def display_mst(mst):
    print("Minimum Spanning Tree:")
    for frm, to, cost in mst:
        print(f"{frm} - {to}: (weight: {cost})")

# Example usage
graph = {
    'A': {'B': 2, 'D': 6},
    'B': {'A': 2, 'C': 3, 'D': 8, 'E': 5},
    'C': {'B': 3, 'E': 7},
    'D': {'A': 6, 'B': 8},
    'E': {'B': 5, 'C': 7}
}

mst = prims_algorithm(graph)
display_mst(mst)
