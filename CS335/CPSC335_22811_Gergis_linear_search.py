def linear_search(arr, target):
    result = []
    for i in range(len(arr)):
        if arr[i] == target:
            result.append("Index: " + str(i) + " Value: " + str(arr[i]))
    if result == []:
        return "Not found"
    return result
def generate_random_list(size,lower_bound,upper_bound):
    return [random.randint(lower_bound, upper_bound) for _ in range(size)]
list_size = int(input("Enter the size of the list: "))
lower_bound = int(input("Enter the lower bound: "))
upper_bound = int(input("Enter the upper bound: "))
arr = generate_random_list(list_size, lower_bound, upper_bound)
print(arr)
target = int(input("Enter the target: "))
print(linear_search(arr, target))
