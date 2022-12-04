// This Class extends Robot Class that means that it has access to
// It's [ Public ] Constructors, Properties and Methods 
public class SpecialRobot extends Robot {

	// ---------------- Constructors ---------------- //
	//This constructor receives String name and it calls the super constructor 
	//with passing him the name(received attribute)
	//The explanation of the super constructor is written in it's class (The father class)  
	public SpecialRobot(String name) {
		super(name);
	}
	
	//This constructor receives (String name, int positionX, int positionY, int facing) 
	// and it calls the super constructor with passing to him the received attributes
	//The explanation of the super constructor is written in it's class (The father class)
	public SpecialRobot(String name, int positionX, int positionY, int facing) {
		super(name, positionX, positionY, facing);
	}
	
	//This method turns the face of the robot to the right (90 deg)
	//That made by calling the super.turnLeft() method 3 times = turning left 3 times (270 deg to left) = step to right
	//because we are not allowed to use set for facing property
	public void turnRight() {
		for(int i=0; i < 3; i++) {
			super.turnLeft();
		}
	}
	
	
}
