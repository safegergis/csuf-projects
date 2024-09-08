import random


def insertion_sort(arr):
    n = len(arr)
    for i in range(1, n):
        # Set current element to key
        key = arr[i]
        # Set j to index of previous element
        j = i - 1
        #while previous index exists and previous element is greater than current element
        while j >= 0 and arr[j] > key:
            #swap current element and previous element
            arr[j + 1] = arr[j]
            j = j-1
        arr[j + 1] = key
random_num = [random.randint(1,100) for _ in range(10)]
print("Unsorted list:",random_num)
insertion_sort(random_num)
print("Sorted list: ", random_num)