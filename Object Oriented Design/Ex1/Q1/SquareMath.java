package Ex1_Question4;

//This class makes the calculations for the square area and perimeter
public class SquareMath {
	
	public double getArea(double length) {
		
		return Math.pow(length, 2);
	}
	
	public double getPerimeter(double length) {
		
		return (4 * length);
	}
}
