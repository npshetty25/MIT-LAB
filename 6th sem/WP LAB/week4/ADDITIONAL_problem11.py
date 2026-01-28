class ParenthesesValidator:
    def __init__(self, s):
        """
        Initialize with the input string of parentheses/brackets
        """
        self.s = s

    def is_valid(self):
        """
        Returns True if the string is valid, False otherwise
        """
        stack = []
        # Dictionary to map closing brackets to opening brackets
        mapping = {')': '(', '}': '{', ']': '['}

        for char in self.s:
            if char in mapping.values():  # If opening bracket
                stack.append(char)
            elif char in mapping:  # If closing bracket
                if not stack or stack[-1] != mapping[char]:
                    return False  # Mismatch or stack empty
                stack.pop()  # Correct match, pop from stack
            else:
                # Ignore any other characters (optional)
                continue

        # If stack is empty at the end, all brackets matched
        return len(stack) == 0


# ------------------ Usage ------------------
# Input from user
string = input("Enter a string of parentheses/brackets: ")

validator = ParenthesesValidator(string)

if validator.is_valid():
    print("The string is valid ✅")
else:
    print("The string is invalid ❌")
