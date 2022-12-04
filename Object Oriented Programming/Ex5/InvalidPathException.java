//Declaring/Creating a new Exception InvalidPathException by extending the class Exception
public class InvalidPathException extends Exception {
	
	//This constructor receives String message and it sends it to the super constructor of the father Exception
	public InvalidPathException(String message) {
		super(message);
	}
}
