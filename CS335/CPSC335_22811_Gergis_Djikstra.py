import heapq

def dijkstra(graph, source, destination):
    distances = {node: float('inf') for node in graph}
    distances[source] = 0

    priority_queue = [(0, source)]

    previous_nodes = {node: None for node in graph}

    while priority_queue:
        current_distance, current_node = heapq.heappop(priority_queue)
        
        if current_node == destination:
            break

        if current_distance > distances[current_node]:
            continue

        for neighbor, weight in graph[current_node].items():
            distance = current_distance + weight

            if distance < distances[neighbor]:
                distances[neighbor] = distance
                previous_nodes[neighbor] = current_node
                heapq.heappush(priority_queue, (distance, neighbor))

    path = []
    current = destination
    while current is not None:
        path.append(current)
        current = previous_nodes[current]
    path.reverse()
    return distances[destination], path

def build_city_graph():
    graph = {
        'A': {'B': 4, 'C': 2},
        'B': {'A': 4, 'D': 5},
        'C': {'A': 2, 'D': 8, 'E': 10},
        'D': {'B': 5, 'C': 8, 'E': 2, 'F': 4},
        'E': {'C': 10, 'D': 2, 'F': 3},
        'F': {'D': 4, 'E': 3}
    }
    return graph

def main():
    city_graph = build_city_graph()

    print("Available Cities/Intersection ", list(city_graph.keys()))

    source = input("Enter the starting Point: ")
    destination = input("Enter the destination point: ")

    if source not in city_graph or destination not in city_graph:
        print("Invalid Selection, please enter the valid entry.")
        return
    distance, path = dijkstra(city_graph, source, destination)

    if distance == float('inf'):
        print(f"No path found from {source} to {destination}.")
    else:
        print(f"shortest path from {source} to {destination}: {'->'.join(path)}")
        print(f"Total Distance: {distance} cost")

if __name__ == "__main__":
    main()