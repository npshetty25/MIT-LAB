def sort_words_alphabetically(text):
    """
    Sorts words in a text alphabetically.
    """
    # Split the text into words
    words = text.split()
    
    # Sort words alphabetically (case-insensitive)
    sorted_words = sorted(words, key=str.lower)
    
    return sorted_words

# Get user input
if __name__ == "__main__":
    print("=== Sort Words Alphabetically ===")
    
    text = input("Enter a sentence: ")
    sorted_words = sort_words_alphabetically(text)
    
    print("\nOriginal text:", text)
    print("Sorted words:", ' '.join(sorted_words))