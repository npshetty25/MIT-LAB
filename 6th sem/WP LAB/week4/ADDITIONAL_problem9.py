def bubble_sort(arr):
    """
    Sorts an array using bubble sort algorithm.
    
    Args:
        arr: List of numbers to sort
        
    Returns:
        Sorted list
    """
    n = len(arr)
    
    # Make a copy to avoid modifying original
    sorted_arr = arr.copy()
    
    # Traverse through all array elements
    for i in range(n):
        # Flag to optimize (stop if no swaps occur)
        swapped = False
        
        # Last i elements are already in place
        for j in range(0, n - i - 1):
            # Swap if element found is greater than next element
            if sorted_arr[j] > sorted_arr[j + 1]:
                sorted_arr[j], sorted_arr[j + 1] = sorted_arr[j + 1], sorted_arr[j]
                swapped = True
        
        # If no swaps occurred, array is sorted
        if not swapped:
            break
    
    return sorted_arr

# Get user input
if __name__ == "__main__":
    print("=== Bubble Sort ===\n")
    
    # Get list from user
    list_input = input("Enter list of numbers to sort (space-separated): ")
    numbers = list(map(float, list_input.split()))
    
    print(f"\nOriginal list: {numbers}")
    
    # Sort using bubble sort
    sorted_numbers = bubble_sort(numbers)
    
    print(f"Sorted list: {sorted_numbers}")