class PairFinder:
    """
    A class to find a pair of elements (indices) whose sum equals a target.
    """
    
    def find_pair_indices(self, numbers, target):
        """
        Finds indices of two numbers that add up to target.
        
        Args:
            numbers: List of integers
            target: Target sum
            
        Returns:
            Tuple of two indices or None if no pair found
        """
        # Dictionary to store number and its index
        seen = {}
        
        for i, num in enumerate(numbers):
            complement = target - num
            
            # Check if complement exists in seen dictionary
            if complement in seen:
                return seen[complement], i
            
            # Store current number and its index
            seen[num] = i
        
        return None
    
    def find_all_pairs(self, numbers, target):
        """
        Finds all pairs of indices whose sum equals target.
        """
        pairs = []
        seen = {}
        
        for i, num in enumerate(numbers):
            complement = target - num
            
            if complement in seen:
                # Add all indices where complement was found
                for j in seen[complement]:
                    pairs.append((j, i))
            
            # Store current number and its index
            if num not in seen:
                seen[num] = []
            seen[num].append(i)
        
        return pairs

# Get user input
if __name__ == "__main__":
    print("=== Find Pair with Target Sum ===\n")
    
    # Get array from user
    numbers_input = input("Enter array of numbers (space-separated): ")
    numbers = list(map(int, numbers_input.split()))
    
    # Get target sum
    target = int(input("Enter target sum: "))
    
    # Create PairFinder instance
    finder = PairFinder()
    
    print(f"\nInput: numbers = {numbers}, target = {target}")
    
    # Find first pair
    result = finder.find_pair_indices(numbers, target)
    
    if result:
        print(f"Output: {result[0]}, {result[1]}")
        print(f"\nVerification:")
        print(f"  numbers[{result[0]}] = {numbers[result[0]]}")
        print(f"  numbers[{result[1]}] = {numbers[result[1]]}")
        print(f"  Sum = {numbers[result[0]]} + {numbers[result[1]]} = {numbers[result[0]] + numbers[result[1]]}")
    else:
        print("Output: No pair found with the given target sum")
    
    # Optionally show all pairs
    print("\n" + "="*50)
    all_pairs = finder.find_all_pairs(numbers, target)
    if all_pairs:
        print(f"All possible pairs: {all_pairs}")
    else:
        print("No pairs found")