package Ex1_Question4;

//This class makes the serialization for the shape
public class TriangleSerialization {

	public String getJSONSerialization(double perpendicular, double base, Triangle triangle) {

		return String.format("{ \"perpendicular\": %f, \"base\": %f, \"area\": %f, \"perimeter\": %f }", perpendicular, base, triangle.getArea(),
				triangle.getPerimeter());
	}
}
