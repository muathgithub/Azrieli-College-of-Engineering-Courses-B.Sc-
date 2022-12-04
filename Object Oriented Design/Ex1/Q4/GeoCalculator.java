package Ex1_Question4;

import java.util.ArrayList;

//This class calculates the area and the perimeter for any MathematicalShape [one ,all]
public class GeoCalculator {

	public double getArea(MathematicalShape mathematicalShape) {

		return mathematicalShape.getArea();
	}

	public double getPerimeter(MathematicalShape mathematicalShape) {

		return mathematicalShape.getPerimeter();
	}

	public double getAreasSum(ArrayList<MathematicalShape> arrayList) {

		double areasSum = 0;

		for (MathematicalShape mathematicalShape : arrayList) {

			areasSum += mathematicalShape.getArea();
		}

		System.out.println(String.format("Areas Sum = %f", areasSum));

		return areasSum;
	}

	public double getPerimetersSum(ArrayList<MathematicalShape> arrayList) {

		double perimetersSum = 0;

		for (MathematicalShape mathematicalShape : arrayList) {

			perimetersSum += mathematicalShape.getPerimeter();
		}

		System.out.println(String.format("Perimeters Sum = %f", perimetersSum));

		return perimetersSum;
	}

}
