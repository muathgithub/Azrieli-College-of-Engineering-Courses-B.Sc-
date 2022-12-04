package Ex1_Question3;

//This class implements MathematicalShape interface which contains mathematical methods that we have to implement
public class Square implements MathematicalShape {

	private double length;
	private SquareMath squareMath;

	public Square(double length) {

		this.length = length;
		this.squareMath = new SquareMath();
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

}
