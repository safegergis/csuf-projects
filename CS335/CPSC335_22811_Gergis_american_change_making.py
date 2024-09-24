def american_change_making(cents):
    coins = {"quarter": 0, "dime": 0, "nickel": 0, "penny": 0}
    coins["quarter"] = cents // 25
    cents = cents % 25
    coins["dime"] = cents // 10
    amount = amount % 10
    coins["nickel"] = cents // 5
    cents = cents % 5
    coins["penny"] = cents
    return coins
total_cents = int(input("Enter the total cents: "))
 
result = american_change_making(total_cents)
print(f"Quarters: {result["quarter"]}, Dimes: {result["dime"]}, Nickels: {result["nickel"]}, Pennies: {result["penny"]}")

