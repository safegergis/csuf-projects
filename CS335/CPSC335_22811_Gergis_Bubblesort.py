import random


def bubble_sort(arr):
    n = len(arr)
    for i in range(n):
        for j in range(0, n-i-1):
            if arr[j] > arr[j+1]:
                arr[j],arr[j+1] = arr[j+1], arr[j]
random_num = [random.randint(1,100) for _ in range(10)]
print("Unsorted list:",random_num)
bubble_sort(random_num)
print("Sorted list: ", random_num)