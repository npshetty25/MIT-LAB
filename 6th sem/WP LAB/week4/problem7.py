class StringManipulator:
    """
    A class to accept a string from user and print it in upper case.
    """
    
    def __init__(self):
        """
        Initialize the class with an empty string.
        """
        self.string = ""
    
    def get_String(self):
        """
        Accepts a string from the user.
        """
        self.string = input("Enter a string: ")
    
    def print_String(self):
        """
        Prints the string in upper case.
        """
        print(self.string.upper())

# Use the class
if __name__ == "__main__":
    print("=== String Manipulator ===")
    
    # Create an instance of the class
    manipulator = StringManipulator()
    
    # Get string from user
    manipulator.get_String()
    
    # Print string in uppercase
    print("\nString in uppercase:")
    manipulator.print_String()