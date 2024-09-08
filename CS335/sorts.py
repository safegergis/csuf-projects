import random
#! BIG O(n)
def bubble_sort(arr):
    n = len(arr)
    for i in range(n):
        for j in range(0, n-i-1):
            if arr[j] > arr[j+1]:
                arr[j],arr[j+1] = arr[j+1], arr[j]

#! BIG O(n)
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
        
def merge_sort(arr):
    if len(arr) > 1:
        mid = len(arr) // 2
        left_half = arr[:mid]
        right_half = arr[mid:]

        merge_sort(left_half)
        merge_sort(right_half)

        i = j = k = 0

        while i < len(left_half) and j < len(right_half):
            if left_half[i][0] < right_half[j][0]:
                arr[k] = left_half[i]
                i += 1
            else:
                arr[k] = right_half[j]
                j += 1
            k +=1
        
        while i < len(left_half):
                arr[k] = left_half[i]
                i += 1
                k += 1
        while j < len(right_half):
                arr[k] = right_half[j]
                j += 1
                k += 1
            

#random_num = [random.randint(1,100) for _ in range(10)]
#print("Unsorted list:",random_num)
#insertion_sort(random_num)
#print("Sorted list: ", random_num)

books = [
     ("J.K. Rowling", "Harry Potter and the Philosopher's Stone"),
    ("George Orwell", "1984"),
    ("J.K. Rowling", "Harry Potter and the Chamber of Secrets"),
    ("Harper Lee", "To Kill a Mockingbird"),
    ("George Orwell", "Animal Farm"),
    ("F. Scott Fitzgerald", "The Great Gatsby"),
    ("Xane Austen", "Pride and Prejudice"),
    ("Y.R.R. Tolkien", "The Lord of the Rings"),
    ("Agatha Christie", "Murder on the Orient Express"),
    ("W.D. Salinger", "The Catcher in the Rye"),
]

print("Unsorted books: ")

for author, title in books:
     print(f"{author}: {title}")

merge_sort(books)

print("\nSorted books: ")

for author, title in books:
     print(f"{author}: {title}")