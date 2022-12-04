package Ex1_Question4;

//This class implements MathematicalShape interface which contains mathematical methods that we have to implement
//and the interface MathematicalSerializable which contains the serilization methods
public class Triangle implements MathematicalShape, MathematicalSerializable{
	
	private double perpendicular;
	private double base;
	private TriangleMath triangleMath;
	private TriangleSerialization triangleSerialization;
	
	public Triangle(double perpendicular, double base) {
		
		this.perpendicular = perpendicular;
		this.base = base;
		this.triangleMath = new TriangleMath();
		this.triangleSerialization = new TriangleSerialization();
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
	
	// This is the implementation for the Serialization method from the implemented interface
	@Override
	public String getJSONSerialization() {
		
		return triangleSerialization.getJSONSerialization(perpendicular, base, this);
	}
	
	
	
}
