package Ex1_Question4;

//This class implements MathematicalShape interface which contains mathematical methods that we have to implement
//and the interface MathematicalSerializable which contains the serilization methods
public class Square implements MathematicalShape, MathematicalSerializable {

	private double length;
	private SquareMath squareMath;
	private SquareSerialization squareSerialization;

	public Square(double length) {

		this.length = length;
		this.squareMath = new SquareMath();
		this.squareSerialization = new SquareSerialization();
	}

	// Implementation for the area method from the interface
	@Override
	public double getArea() {
		
		double area = this.squareMath.getArea(this.length);
		
		return area;
	}

	// Implementation for the perimeter method from the interface
	@Override
	public double getPerimeter() {
		
		double perimeter = this.squareMath.getPerimeter(this.length);
		
		return perimeter;
	}	
	
	// This is the implementation for the Serialization method from the implemented interface
	@Override
	public String getJSONSerialization() {
		
		return squareSerialization.getJSONSerialization(length, this);
	}

}
