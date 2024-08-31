def find_max(L):
    maxValue = L[0]
    for n in L[1:]:
        if n > maxValue:
            maxValue = n
    return maxValue

def find_min(L):
    maxValue = L[0]
    for n in L[1:]:
        if n < maxValue:
            maxValue = n
    return maxValue
def factorial(n):
    result = 1
    for i in range(1,n+1):
        result *= i
    return result
        

user_input = input("Please enter your array with spaces in between numbers ")
L = list(map(int, user_input.split()))
method = input("Type 'max' to find highest number or 'min' to find the lowest number")
selected_num = None
if method == 'max':
    selected_num = find_max(L)
    print("the maxmimum number in the list is ", selected_num)
elif method == "min":
    selected_num = find_min(L)
    print("the minimum number in the list is ", selected_num)
else:
    print("Invalid choice ")

if selected_num is not None:
    factorial_choice = input("Do you want the factorial of {selected_num}? y/n ")
    if factorial_choice == 'y':
        print("The factorial of {selected_num} is ", factorial(selected_num))
    elif factorial_choice == 'n':
        print("factorial choice has been skipped")

