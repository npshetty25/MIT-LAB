def multiply_matrices(matrix1, matrix2):
    """
    Multiplies two matrices.
    
    Args:
        matrix1: First matrix (list of lists)
        matrix2: Second matrix (list of lists)
        
    Returns:
        Result matrix or None if multiplication not possible
    """
    # Get dimensions
    rows1 = len(matrix1)
    cols1 = len(matrix1[0])
    rows2 = len(matrix2)
    cols2 = len(matrix2[0])
    
    # Check if multiplication is possible
    if cols1 != rows2:
        return None
    
    # Initialize result matrix with zeros
    result = [[0 for _ in range(cols2)] for _ in range(rows1)]
    
    # Perform multiplication
    for i in range(rows1):
        for j in range(cols2):
            for k in range(cols1):
                result[i][j] += matrix1[i][k] * matrix2[k][j]
    
    return result

def print_matrix(matrix, name="Matrix"):
    """Helper function to print a matrix nicely."""
    print(f"\n{name}:")
    for row in matrix:
        print(row)

# Get user input
if __name__ == "__main__":
    print("=== Matrix Multiplication ===\n")
    
    # Get first matrix
    print("Enter First Matrix:")
    rows1 = int(input("Enter number of rows: "))
    cols1 = int(input("Enter number of columns: "))
    
    matrix1 = []
    print(f"\nEnter {rows1} rows (each with {cols1} space-separated numbers):")
    for i in range(rows1):
        row = list(map(float, input(f"Row {i+1}: ").split()))
        if len(row) != cols1:
            print(f"Error: Expected {cols1} numbers, got {len(row)}")
            exit()
        matrix1.append(row)
    
    # Get second matrix
    print("\nEnter Second Matrix:")
    rows2 = int(input("Enter number of rows: "))
    cols2 = int(input("Enter number of columns: "))
    
    matrix2 = []
    print(f"\nEnter {rows2} rows (each with {cols2} space-separated numbers):")
    for i in range(rows2):
        row = list(map(float, input(f"Row {i+1}: ").split()))
        if len(row) != cols2:
            print(f"Error: Expected {cols2} numbers, got {len(row)}")
            exit()
        matrix2.append(row)
    
    # Print input matrices
    print_matrix(matrix1, "Matrix 1")
    print_matrix(matrix2, "Matrix 2")
    
    # Multiply matrices
    result = multiply_matrices(matrix1, matrix2)
    
    if result:
        print_matrix(result, "Result (Matrix 1 × Matrix 2)")
    else:
        print(f"\nError: Matrix multiplication not possible!")
        print(f"Matrix 1 dimensions: {rows1}×{cols1}")
        print(f"Matrix 2 dimensions: {rows2}×{cols2}")
        print(f"For multiplication, columns of Matrix 1 ({cols1}) must equal rows of Matrix 2 ({rows2})")