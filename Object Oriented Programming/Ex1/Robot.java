
public class Robot {

	// ---------------- Static Attributes ---------------- //
	public final static int NORTH = 1, EAST = 2, SOUTH = 3, WEST = 4;

	// ---------------- Attributes ---------------- //
	private String name;
	private int positionX, positionY;
	private int facing;
	
	
	// ---------------- Constructors ---------------- //
	//This constructor receives the name and it initializes this.name = name
	//And the other properties with the default this.positionX = this.positionY = 0 && this.facing = NORTH
	public Robot(String name) {
		this.name = name;
		this.positionX = this.positionY = 0;
		this.facing = NORTH;
	}

	
	//This constructor receives (String name, int positionX, int positionY, int facing)
	//And it initializes the received attributes in the properties after making a the needed checks
	public Robot(String name, int positionX, int positionY, int facing) {
		
		this.name = name;
		
		//These to line if conditions check if the value of the positions is >= 0
		//if true it initializes the properties with the received values
		//else it initializes the properties with 0
		this.positionX = positionX >= 0 ? positionX : 0;
		this.positionY = positionY >= 0 ? positionY : 0;

		
		//This switch initializes the facing property according to the received facing value
		// if the value is valid else it initializes the facing property with 1 (NORTH)
		// also it can be made with if( facing <= 4 && facing  >= 1) else condition but switch is more readable and obvious
		switch (facing) {

		case 1:
			this.facing = NORTH;
			break;

		case 2:
			this.facing = EAST;
			break;

		case 3:
			this.facing = SOUTH;
			break;

		case 4:
			this.facing = WEST;
			break;

		default:
			this.facing = NORTH;
			break;

		}
	}
	
	//This toString method prints the information about the robot in a formated way
	public String toString() {
		//This array for saving and presenting (printing) the directions names
		//The direction name is saved in directionsArray[this.facing(it's int value) - 1]
		String [] directionsArray = {"NORTH", "EAST", "SOUTH", "WEST"};
		
		//This line returns the String in a formated way it's the same as using +
		//but in a more organized way each data type has its symbol d for example is for int (this.positionX) and % sign after it and s% for string
		return String.format("Name: %s, Position: (%d, %d), Facing: %d (%s)", this.name, this.positionX, 
				this.positionY, this.facing, directionsArray[this.facing -1]);
	}
	
	
	//This method changes the position of the robot each time it called/work/executed
	//with one step and thats according to sum conditions
	public void move() {
		//The moving step/ position changing depends on this.facing[the direction of the robot] 
		//If the direction is X+ Or Y+ it will add 1 to the attribute of the direction
		//Else it will reduce the direction with 1 if the direction is > 0 else it dosen't make any thing
		switch(this.facing) {
		case 1:
			this.positionY++;
			break;
			
		case 2:
			this.positionX++;
			break;
			
		case 3:
			if(this.positionY > 0)
				this.positionY--;
			break;
			
		case 4:
			if(this.positionX > 0)
				this.positionX--;
			break;
		}
	}
	
	//This method turns the face of the robot to the left (90 deg)
	public void turnLeft() {
		
		//This if checks if the direction of the robot is 1 (NORTH) and it makes it WEST (4)
		//So we stay at the range of the static defined directions
		//else it reduce this.faing with 1 (step to the left)
		if(this.facing == 1)
			this.facing = 4;
		else
			this.facing--;
	}
	
	
	// ------------- Setters & Getters ------------- //
	public String getName() {
		return this.name;
	}
	
	public int getPositionX() {
		return this.positionX;
	}
	
	public int getPositionY() {
		return this.positionY;
	}
	
	public int getFacing() {
		return this.facing;
	}

}
