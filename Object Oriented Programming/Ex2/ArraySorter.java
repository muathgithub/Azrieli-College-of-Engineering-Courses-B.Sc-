
public abstract class ArraySorter {

	// ---------------- Protected Attributes ---------------- //
	// That can be used\reached in this class and in the classes that extends it
	protected int lastArrayLength = 0;
	protected long comparisonsCounter = 0;
	protected float ratio = 0;
	
	
	// This is an abstract method that every class whom extends this class has to implement it
	//(every class implements it \ override it according to it's job (sorting algorithm type))
	public abstract int[] sort(int[] a, boolean verbose);

	
	//This method returns a String that show the statistics of the of the processes that have been done on the elements of the last array
	//beside printing the sorting algorithm name and the length of the array
	//(it prints the comparisons time between the elements of the array and the relation between them and the length of the array (ratio)) 
	public String getStatistics() {
		
		//This line calculates the relation between the comparisonsCounter and the length of the array
		//And it casting it to be float
		this.ratio = ((float) this.comparisonsCounter) / (this.lastArrayLength == 0 ? 1 : this.lastArrayLength);
		
		
		//This line returns the String in a formated way it's the same as using +
		//but in a more organized way each data type has its symbol d for example is for int\long and % sign after it and s% for string
		// (%.1f) prints float with one digit after the decimal point
		return String.format(" - Array length %d, comparisons %d, ratio %.1f", 
							this.lastArrayLength, this.comparisonsCounter, this.ratio);
	}

	
	//This method returns a copy of the given array (int[] arr)
	//So the sorting algorithms dosen't change the original array(as asked in the question)
	protected int[] generateACopy(int[] arr) {
		int[] arrayCopy = new int[arr.length];

		for (int i = 0; i < arrayCopy.length; i++) {
			arrayCopy[i] = arr[i];
		}

		return arrayCopy;
	}

	//This method returns a String of the array elements 
	//In the known String\Printing format of the arrays
	protected String arrayToString(int[] arr) {
		String str = "{";
		
		for (int i = 0; i < arr.length - 1; i++)
			str = str.concat(arr[i] + ", ");

		return str.concat(arr[arr.length - 1] + "}");
	}
	
	//This method resets the statistics of the sorting algorithm that have been done in the last array
	//So we can use it in another array and we get correct statistics for the new array
	//Otherwise we have to make a new object of the sorting algorithm that we need to use 
	protected void resetStatistics() {
		this.lastArrayLength = 0;
		this.comparisonsCounter = 0;
		this.ratio = 0;
	}

}
