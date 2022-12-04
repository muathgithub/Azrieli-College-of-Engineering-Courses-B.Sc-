
public class BubbleSorter extends ArraySorter {

	
	//This method is an abstract method that this class extends from ArraySorter(super)
	//So it has to implement it according to it's need 
	//In this case it presents the BubbleSorte algorithm
	public int[] sort(int[] a, boolean verbose) {
		
		//This line calls the extended resetStatistics() method from the super
		//So it reset the statistics in order to prepare for the new array
		super.resetStatistics();
		
		//This if checks the special case if(a == null) it returns null
		if(a == null)
			return null;
		
		//This if checks the special case if(a.length == 0) it returns new array of int with the length of (0)
		if(a.length == 0)
			return new int[0];
		
		//This line initialize the lastArrayLength with the length of the new array (For Statistics)
		this.lastArrayLength = a.length;
		
		//This line generates a new copy of the array (a), so we don't change the original one
		int[] arrayCopy = super.generateACopy(a);
		int temp;

		//This for runs from i = 0 to arrayCopy.length - 2 and each time it runs the inner 
		//for from  j = 0 to arrayCopy.length - (i + 2) and it keeps swapping the element in index j with the element in index j + 1 if its grater than them
		//So at the end of each inner for the last number in the segment from j = 0 to arrayCopy.length - (i + 2) will be the maximum 
		for (int i = 0; i < arrayCopy.length - 1; i++) {
			
			for (int j = 0; j < arrayCopy.length - (i + 1); j++) {
				
				//This line count the comparisons times
				this.comparisonsCounter++;
				
				if (arrayCopy[j] > arrayCopy[j + 1]) {
					//These three line are for swapping the element in j with the element in j + 1
					temp = arrayCopy[j + 1];
					arrayCopy[j + 1] = arrayCopy[j];
					arrayCopy[j] = temp;
				}
			}

			
			//This line prints the array after each outer for if verbose  == true by calling super.arrayToString() on the arrayCopy
			if (verbose) {
				System.out.println(super.arrayToString(arrayCopy));
			}
		}

		return arrayCopy;
	}
	
	//This getStatistics method returns a String of the statistics 
	//By calling the getStatistics() method from the super class and adding it to the name of the sorting algorithm
	public String getStatistics() {
		return "BubbleSort" + super.getStatistics();
	}
}
