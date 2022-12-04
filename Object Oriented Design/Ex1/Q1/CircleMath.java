package Ex1_Question4;

//This class makes the calculations for the circle area and perimeter
public class CircleMath {
	
	public double getArea(double radius) {
		
		return (Math.PI * Math.pow(radius, 2));
	}
	
	public double getPerimeter(double radius) {
		
		return (2 * Math.PI * radius);
	}
}
