
public class PrintingServer {
	
	PriorityQueue priorityQueue;
	
	//This empty constructor initializes the priorityQueue attribute to be a new PriorityQueue object   
	public PrintingServer() {
		priorityQueue = new PriorityQueue();
	}
	
	
	//This method receives a PrintJob object and it calls the priorityQueue.insert(p) method that insert the PrintJob
	//Object to the priority array in priorityQueue
	public void acceptJob(PrintJob p) {
		priorityQueue.insert(p);
	}
	
	//This method prints the first printJob object text and it calls the priorityQueue.remove() method
	//In order to remove it from the priorities array in priorityQueue
	public void print() {
		
		//This if checks if the priorities array in priorityQueue is empty or not
		//if true it dosen't do any thing (return;)
		if(priorityQueue.prioritiesList[0] == null) {
			return;
		}
		
		PrintJob currentPrintJob = (PrintJob) priorityQueue.remove();
		
		System.out.println(currentPrintJob.getText());
	}
	
}
