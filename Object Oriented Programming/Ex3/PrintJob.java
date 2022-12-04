
public class PrintJob implements Prioritizeable{
	
	//This attribute holds the text of the printJob
	private String text;
	///This attribute holds the priority of the printJob
	private int priority;
	
	
	//This constructor receives the text of the printJob And the priority of it
	//Then it initializes the attributes with these values
	public PrintJob(String text, int priority) {
		
		this.text =text;
		
		//This if checks if the priority value is legal
		//if false it initializes 1
		//else it initializes the priority given value
		if(priority > MAX_PRIORITY || priority < (MIN_PRIORITY + 1)) {
			this.priority = 1;
		} else {
			this.priority = priority;
		}
	}
	
	//This get method returns the text of the printJob
	public String getText() {
		return this.text;
	}

	//This compareTo method compares this with o after casting it to Prioritizeable
	//And it returns value > 0 if this.getPriority() > o.getPriority()
	//				 value < 0 if this.getPriority() < o.getPriority()
	//				 value = 0 if this.getPriority() = o.getPriority()
	//So the priorityQueue can sort the objects in the priority array
	public int compareTo(Object o) {
		
		return this.getPriority() - ((Prioritizeable) o).getPriority();
	}

	//This method returns the priority of this object
	public int getPriority() {
		
		return this.priority;
	}

	//This method sets the priority of this object
	public void setPriority(int p) {
		
		
		//This if checks if the priority value is legal
		//if false it initializes 1
		//else it initializes the priority given value
		if(p > MAX_PRIORITY || p < (MIN_PRIORITY + 1)) {
			this.priority = 1;
		} else {
			this.priority = p;
		}
	}
	
	//This toStirng method returns the information of the printJob in a formated way
	public String toString() {
		
		//This line returns the String in a formated way it's the same as using +
		//but in a more organized way each data type has its symbol d for example is for int\long and % sign after it and s% for string
		return String.format("PrintJob --> Text: %s | Priority: %d", this.text, this.priority);		
	}

}
