import math

class Circle:
    """
    A class to represent a circle with radius.
    """
    
    def __init__(self, radius):
        """
        Initialize circle with radius.
        
        Args:
            radius: Radius of the circle
        """
        self.radius = radius
    
    def area(self):
        """
        Calculates and returns the area of the circle.
        
        Returns:
            Area of the circle (π * r²)
        """
        return math.pi * (self.radius ** 2)
    
    def perimeter(self):
        """
        Calculates and returns the perimeter (circumference) of the circle.
        
        Returns:
            Perimeter of the circle (2 * π * r)
        """
        return 2 * math.pi * self.radius
    
    def __str__(self):
        """
        String representation of the circle.
        """
        return f"Circle(radius={self.radius})"

# Get user input
if __name__ == "__main__":
    print("=== Circle Area and Perimeter Calculator ===\n")
    
    # Get radius from user
    radius = float(input("Enter radius of the circle: "))
    
    # Create circle object
    circle = Circle(radius)
    
    # Calculate and display results
    print(f"\n{circle}")
    print(f"Radius: {circle.radius}")
    print(f"Area: {circle.area():.2f}")
    print(f"Perimeter (Circumference): {circle.perimeter():.2f}")
    
    # Additional information
    print(f"\nDetailed calculations:")
    print(f"  Area = π × r² = π × {radius}² = {circle.area():.6f}")
    print(f"  Perimeter = 2 × π × r = 2 × π × {radius} = {circle.perimeter():.6f}")