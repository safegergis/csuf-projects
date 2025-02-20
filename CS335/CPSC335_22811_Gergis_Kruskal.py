def find(parent, i):
    if parent[i] == i:
        return i
    else:
        return find(parent, parent[i])

def union(parent, rank, x, y):
    root_x = find(parent, x)
    root_y = find(parent, y)

    if root_x != root_y:
        if rank[root_x] > rank[root_y]:
            parent[root_y] = root_x
        elif rank[root_x] < rank[root_y]:
            parent[root_x] = root_y
        else:
            parent[root_y] = root_x
            rank[root_x] += 1

def kruskal_algorithm(graph):
    edges = []
    for frm in graph:
        for to, cost in graph[frm].items():
            edges.append((cost, frm, to))
    edges.sort()

    vertices = list(graph.keys())
    parent = {v: v for v in vertices}
    rank = {v: 0 for v in vertices}

    mst = []
    total_weight = 0

    for cost, frm, to in edges:
        if find(parent, frm) != find(parent, to):
            union(parent, rank, frm, to)
            mst.append((frm, to, cost))
            total_weight += cost

    return mst, total_weight

def display_mst(mst, total_weight):
    print("Minimum Spanning Tree:")
    for frm, to, cost in mst:
        print(f"{frm} - {to}: (weight: {cost})")

# Example usage
graph = {
    1: {2: 2, 4: 6},
    2: {1: 2, 3: 3, 4: 8, 5: 5},
    3: {2: 3, 5: 7},
    4: {1: 6, 2: 8},
    5: {2: 5, 3: 7}
}

mst, total_weight = kruskal_algorithm(graph)
display_mst(mst, total_weight)


