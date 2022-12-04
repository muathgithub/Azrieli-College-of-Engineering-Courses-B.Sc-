package Ex1_Question4;

// This class makes the serialization for the shape
public class CircleSerialization {
	
	public String getJSONSerialization(double radius, Circle circle) {
		
		
		return String.format("{ \"radius\": %f, \"area\": %f, \"perimeter\": %f }", radius, circle.getArea(), circle.getPerimeter());
	}
}
