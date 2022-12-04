
public class InsertionSorter extends ArraySorter {
	
	//This method is an abstract method that this class extends from ArraySorter(super)
	//So it has to implement it according to it's need 
	//In this case it presents the InsertionSorte algorithm
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
		int temp, k;
		
		//This boolean attribute used for checking if the first comparison is counted so i don't count it one more time in the while loop
		//We need it because some time (arrayCopy[k] > temp) gives false so we don't go inside the while which increases the comarsionsCounter in it
		boolean isCounted = false;

		//This for runs from i = 1 to arrCopy.length - 1 that runs a while loop 
		//that keeps shifting the elements each one that are greater then the current element
		//To the Right one step till it reaches the correct index of the current element and it puts i there
		//So at the end of each loop the element in the index i - 1 will be the correct one in ascending order 
		for (int i = 1; i < arrayCopy.length; i++) {

			temp = arrayCopy[i];
			k = i - 1;

			//This line count the comparisons times
			this.comparisonsCounter++;
			isCounted = true;

			//Explained above
			while (k >= 0 && arrayCopy[k] > temp) {
				arrayCopy[k + 1] = arrayCopy[k];
				k--;

				if (!isCounted)
					this.comparisonsCounter++;

				//This line returns the isCounted boolean to false so the next while loops increase this.comparisonsCounter whith one
				isCounted = false;
			}
			
			//This if checks if k >= 0 && !isCounted so we count the last (arrayCopy[k] > temp) comparison that ended the while loop
			if(k >= 0 && !isCounted)
				this.comparisonsCounter++;
			
			arrayCopy[k + 1] = temp;

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
		return "InsertionSort" + super.getStatistics();
	}

}
