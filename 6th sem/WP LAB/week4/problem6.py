class Power:
    """
    A class to implement pow(x, n) - calculates x raised to power n.
    """
    
    def pow(self, x, n):
        """
        Calculates x^n using iterative approach.
        
        Args:
            x: Base number
            n: Exponent (can be positive, negative, or zero)
            
        Returns:
            Result of x^n
        """
        if n == 0:
            return 1
        
        # Handle negative exponent
        if n < 0:
            x = 1 / x
            n = -n
        
        result = 1
        current_product = x
        
        # Binary exponentiation (fast power algorithm)
        while n > 0:
            if n % 2 == 1:
                result *= current_product
            current_product *= current_product
            n //= 2
        
        return result

# Get user input
if __name__ == "__main__":
    print("=== Implement pow(x, n) ===")
    
    x = float(input("Enter base (x): "))
    n = int(input("Enter exponent (n): "))
    
    power = Power()
    result = power.pow(x, n)
    
    print(f"\nResult: pow({x}, {n}) = {result}")
    print(f"Verification with built-in: {x ** n}")