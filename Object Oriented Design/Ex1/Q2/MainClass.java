package Ex1_Question3;

import java.util.ArrayList;

public class MainClass {
	
	public static void main(String[] args) {
		
		Square square = new Square(5.5);
		Circle circle = new Circle(3.5);
		Triangle triangle = new Triangle(6.5, 4.5);
		GeoCalculator geoCalculator = new GeoCalculator();
		
		System.out.println(String.format("Square Area = %.4f", geoCalculator.getArea(square)));
		System.out.println(String.format("Square Perimeter = %.4f", geoCalculator.getPerimeter(square)));
		System.out.println();
		
		System.out.println(String.format("Circle Area = %.4f", geoCalculator.getArea(circle)));
		System.out.println(String.format("Circle Perimeter = %.4f", geoCalculator.getPerimeter(circle)));
		System.out.println();
		
		System.out.println(String.format("Triangle Area = %.4f", geoCalculator.getArea(triangle)));
		System.out.println(String.format("Triangle Perimeter = %.4f", geoCalculator.getPerimeter(triangle)));
		System.out.println();
		
	}
}
