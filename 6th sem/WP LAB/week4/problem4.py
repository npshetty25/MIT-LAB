class SubsetsGenerator:
    def __init__(self, nums):
        """
        Initialize with a list of distinct integers
        """
        self.nums = nums

    def get_subsets(self):
        """
        Returns all possible unique subsets of the input list
        """
        result = [[]]  # Start with the empty subset

        for num in self.nums:
            # For each number, add it to all existing subsets
            new_subsets = [curr + [num] for curr in result]
            result.extend(new_subsets)

        return result


# ------------------ Usage ------------------
# Input from user
nums = list(map(int, input("Enter numbers separated by space: ").split()))

# Create object
generator = SubsetsGenerator(nums)

# Generate subsets
all_subsets = generator.get_subsets()

print("All unique subsets:")
print(all_subsets)
