package Ex1_Question3;

// This class implements MathematicalShape interface which contains mathematical methods that we have to implement
public class Circle implements MathematicalShape{
	
	private double radius;
	private CircleMath circleMath;
	
	public Circle(double radius) {
		
		this.radius = radius;
		this.circleMath = new CircleMath();
	}
	
	// Implementation for the area method from the interface
	@Override
	public double getArea() {
		
		double area = this.circleMath.getArea(this.radius);
		
		return area;
	}
	
	// Implementation for the perimeter method from the interface
	@Override
	public double getPerimeter() {
		
		double perimeter = this.circleMath.getPerimeter(this.radius);

		return perimeter;
	}
	
}
