package Ex1_Question3;

//This class implements MathematicalShape interface which contains mathematical methods that we have to implement
public class Triangle implements MathematicalShape{
	
	private double perpendicular;
	private double base;
	private TriangleMath triangleMath;
	
	public Triangle(double perpendicular, double base) {
		
		this.perpendicular = perpendicular;
		this.base = base;
		this.triangleMath = new TriangleMath();
	}
	
	// Implementation for the area method from the interface
	@Override
	public double getArea() {
		
		double area = this.triangleMath.getArea(this.perpendicular, this.base);
		
		return area;
	}
	
	// Implementation for the perimeter method from the interface
	@Override
	public double getPerimeter() {

		double perimeter = this.triangleMath.getPerimeter(this.perpendicular, this.base);
		
		return perimeter;
	}
	
	
}
