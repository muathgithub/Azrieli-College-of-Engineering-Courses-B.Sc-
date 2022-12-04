// This Class extends SpecialRobot Class that means that it has access to
// It's [ Public ] Constructors, Properties and Methods 
public class MoreSpecialRobot extends SpecialRobot{
	
	// ---------------- Attributes ---------------- //
	private int battery;
	
	
	// ---------------- Constructors ---------------- //
	//This constructor receives String name and it calls the super constructor 
	//with passing him the name(received attribute)
	//The explanation of the super constructor is written in it's class (The father class  of the father class [Robot])
	//More over it initializes the this.battery with 100 (%)
	public MoreSpecialRobot(String name) {
		super(name);
		this.battery = 100;
	}
	
	//This constructor receives (String name, int positionX, int positionY, int facing) 
	// and it calls the super constructor with passing to him the received attributes
	//The explanation of the super constructor is written in it's class (The father class  of the father class [Robot])
	//More over it initializes the this.battery with 100 (%)
	public MoreSpecialRobot(String name, int positionX, int positionY, int facing) {
		super(name, positionX, positionY, facing);
		this.battery = 100;
	}
	
	//This toString method prints the information about the robot in a formated way
	//By calling the toString() method of the super class then it adds to the result the battery percent
	public String toString() {
		
		//This line returns the String in a formated way it's the same as using +
		//but in a more organized way each data type has its symbol d for example is for int (this.battery) and % sign after it and s% for string
		return super.toString() + String.format(", Battery: %d%%", this.battery);
		
	}
	
	//This method calls the super move() method (the explanation of the method is in Robot Class)
	//Before that it checks if this.battery == 0 ||  this.getFacing() == SOUTH && this.getPositionY() == 0 
	// || (this.getFacing() == WEST && this.getPositionX() == 0) if true it dosen't make anything. else it reduces the battery with 1
	//Then it calls the move method
	public void move() {
		
		if(this.battery == 0 || (this.getFacing() == SOUTH && this.getPositionY() == 0) 
						     || (this.getFacing() == WEST && this.getPositionX() == 0))
			return;
		else
			this.battery--;
		
		super.move();
	}
	
	// ------------- Setters & Getters ------------- //
	public int getBattery() {
		return battery;
	}
}
