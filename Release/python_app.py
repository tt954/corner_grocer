import re
import string

def printsomething():
    print("Hello from python!")

def read_file(filename):
    with open(filename) as file:
        return [line.rstrip() for line in file]

# print a table of all the purchased items and their frequencies
def PrintSummary():
    products = read_file("input_file.txt") # read from file and create product list
    unique_products = sorted(set(products)) # sort products alphabetically and remove duplicates

    print("{:<15} {:<10}".format("Item", "Frequency")) # print header 
    print("-"*25)
    for item in unique_products: # for every product in the sorted, unique list 
        print("{:15} {:<10}".format(item, products.count(item))) # print item follow by how many items it appears in the list

# returns the amount of times an item was purchased
def SearchItem(userInput):
    products = [item.lower() for item in read_file("input_file.txt")] # read from file and create product list (convert strings to all lowercase)
    if userInput.lower() in products:
        return products.count(userInput.lower()) # return the item frequency if found in list 
    else:
        return 0 # return 0 for items not in list

# process input file to create output file 
def ProcessInput():
    products = read_file("input_file.txt") # read from file and create product list
    unique_products = sorted(set(products)) # sort products alphabetically and remove duplicates
    with open("frequency.dat", mode="w+") as file: # create new file or overwrite existing file
        for item in unique_products:
            file.write(f"{item} {products.count(item)}\n") # write to items and frequencies to file
    
