def find_smallest_element(lst):
    """
    Finds the smallest element from a list in linear time O(n).
    
    Args:
        lst: List of numbers
        
    Returns:
        The smallest element in the list
    """
    if not lst:
        return None
    
    smallest = lst[0]
    
    for element in lst:
        if element < smallest:
            smallest = element
    
    return smallest

# Get user input
if __name__ == "__main__":
    print("=== Find Smallest Element (Linear Time) ===\n")
    
    # Get list from user
    list_input = input("Enter list of numbers (space-separated): ")
    numbers = list(map(float, list_input.split()))
    
    print(f"\nInput list: {numbers}")
    
    # Find smallest element
    smallest = find_smallest_element(numbers)
    
    if smallest is not None:
        print(f"Smallest element: {smallest}")
    else:
        print("The list is empty!")