package Ex1_Question4;

//This class implements MathematicalShape interface which contains mathematical methods that we have to implement
// and the interface MathematicalSerializable which contains the serilization methods
public class Circle implements MathematicalShape, MathematicalSerializable{
	
	private double radius;
	private CircleMath circleMath;
	private CircleSerialization circleSerialization;
	
	public Circle(double radius) {
		
		this.radius = radius;
		this.circleMath = new CircleMath();
		this.circleSerialization = new CircleSerialization();
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
	
	// This is the implementation for the Serialization method from the implemented interface
	@Override
	public String getJSONSerialization() {
		
		return circleSerialization.getJSONSerialization(radius, this);
	}
	
}
