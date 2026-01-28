def binary_search_recursive(arr, target, left, right):
    """
    Implements binary search using recursion.
    Returns the index of target if found, otherwise -1.
    """
    # Base case: element not found
    if left > right:
        return -1
    
    # Calculate middle index
    mid = (left + right) // 2
    
    # Check if target is at mid
    if arr[mid] == target:
        return mid
    
    # If target is smaller, search in left half
    elif arr[mid] > target:
        return binary_search_recursive(arr, target, left, mid - 1)
    
    # If target is larger, search in right half
    else:
        return binary_search_recursive(arr, target, mid + 1, right)

def binary_search(arr, target):
    """
    Wrapper function for binary search.
    """
    return binary_search_recursive(arr, target, 0, len(arr) - 1)

# Get user input
if __name__ == "__main__":
    print("=== Binary Search with Recursion ===")
    
    # Get array from user
    arr_input = input("Enter sorted array elements (space-separated): ")
    arr = list(map(int, arr_input.split()))
    
    # Sort the array to ensure it's sorted for binary search
    arr.sort()
    print(f"Sorted array: {arr}")
    
    # Get target element
    target = int(input("Enter element to search: "))
    
    # Perform binary search
    result = binary_search(arr, target)
    
    if result != -1:
        print(f"\nElement {target} found at index {result}")
    else:
        print(f"\nElement {target} not found in array")