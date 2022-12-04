package Ex1_Question4;

//This class makes the serialization for the shape
public class SquareSerialization {
	
	public String getJSONSerialization(double length, Square square) {
		
		
		return String.format("{ \"length\": %f, \"area\": %f, \"perimeter\": %f }", length, square.getArea(), square.getPerimeter());
	}
	
}
