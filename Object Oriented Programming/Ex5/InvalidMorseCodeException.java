//Declaring/Creating a new Exception InvalidMorseCodeException by extending the class Exception
public class InvalidMorseCodeException extends Exception {

	//This constructor receives String message and it sends it to the super constructor of the father Exception
	public InvalidMorseCodeException(String message) {
		super(message);
	}
	
}
