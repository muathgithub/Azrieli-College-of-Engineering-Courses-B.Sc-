
public class SortAnalyzer {
	
	private ArraySorter [] sorters;
	
	//This constructor initialize the ArraySorter [] sorters Array to be a new ArraySorter[3] 
	//It initializes the First index to be a new BubbleSorter() object (That extends ArraySorter)
	//It initializes the Second index to be a new InsertionSorter() object (That extends ArraySorter)
	//It initializes the first index to be a new MergeSorter() object (That extends ArraySorter)
	public SortAnalyzer() {
		sorters = new ArraySorter[3];
		
		sorters[0] = new BubbleSorter();
		sorters[1] = new InsertionSorter();
		sorters[2] = new MergeSorter();
	}
	
	//This method receives two arguments The first is numOfArrays which represents the number of the arrays that it has to generate
	//The second is maxSize that represents the length of these arrays
	public void analyze(int numOfArrays, int maxSize) {
		
		int [] tempArray;
		
		//This if condition checks if the maxSize is smaller than 10 if that is the case it will initialize it with 10
		//else it keeps him the same
		if(maxSize < 10) {
			maxSize = 10;
		}
		
		//This for runs numOfArrays times and each time it generates an array with the length maxSize
		//and it fills it with random values using the Math.random() method 
		for(int i = 0; i < numOfArrays; i++) {
			
			tempArray = new int[maxSize];
			
			for(int j = 0; j < tempArray.length; j++) {
				
				//This line fills the current index with a random value from 0 to 999
				tempArray[j] = (int) (Math.random() * 1000);
			}
			
			//The First line sorts the generated array by using BubbleSort algorithm
			//The Second line prints the statistics of the sorted that have been done on the generated array by calling getStatistics() method
			sorters[0].sort(tempArray, false);
			System.out.println(sorters[0].getStatistics());
			
			//The First line sorts the generated array by using InsertionSort algorithm
			//The Second line prints the statistics of the sorted that have been done on the generated array by calling getStatistics() method
			sorters[1].sort(tempArray, false);
			System.out.println(sorters[1].getStatistics());
			
			//The First line sorts the generated array by using MergeSort algorithm
			//The Second line prints the statistics of the sorted that have been done on the generated array by calling getStatistics() method
			sorters[2].sort(tempArray, false);
			System.out.println(sorters[2].getStatistics() + "\n");
		}
	}

}

