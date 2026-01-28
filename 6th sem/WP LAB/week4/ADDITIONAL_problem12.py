class StringReverser:
    """
    A class to reverse a string word by word.
    """
    
    def reverse_words(self, s):
        """
        Reverses the words in a string.
        
        Args:
            s: Input string
            
        Returns:
            String with words reversed
        """
        # Split string into words (handles multiple spaces)
        words = s.split()
        
        # Reverse the list of words
        reversed_words = words[::-1]
        
        # Join with single space
        return ' '.join(reversed_words)
    
 

# Get user input
if __name__ == "__main__":
    print("=== Reverse String Word by Word ===\n")
    
    reverser = StringReverser()
    
    # Get string from user
    s = input("Enter a string: ")
    
    # Reverse words
    reversed_string = reverser.reverse_words(s)
    
    print(f"\nOriginal string: '{s}'")
    print(f"Reversed string: '{reversed_string}'")
   