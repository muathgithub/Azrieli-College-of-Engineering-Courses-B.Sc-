
public class MergeSorter extends ArraySorter {

	// This attribute used to save the value of the verbose that sort() receives
	// So I can use it in merge()
	private boolean verbose;

	// This method is an abstract method that this class extends from
	// ArraySorter(super)
	// So it has to implement it according to it's need
	// In this case it presents the InsertionSorte algorithm
	public int[] sort(int[] a, boolean verbose) {

		// This line calls the extended resetStatistics() method from the super
		// So it reset the statistics in order to prepare for the new array
		super.resetStatistics();

		// This if checks the special case if(a == null) it returns null
		if (a == null)
			return null;

		// This if checks the special case if(a == null) it returns null
		if (a.length == 0)
			return new int[0];

		// Explained above
		this.verbose = verbose;

		// This line initialize the lastArrayLength with the length of the new array
		// (For Statistics)
		this.lastArrayLength = a.length;

		// This line generates a new copy of the array (a), so we don't change the
		// original one
		int[] arrayCopy = super.generateACopy(a);

		// This line calls mergeSort() method by sending to it the arrayCopy and the
		// start index which is 0 and the end index which is arrayCopy.length - 1
		mergeSort(arrayCopy, 0, arrayCopy.length - 1);

		return arrayCopy;
	}

	// This is a recursive method that keeps dividing the array to 2 parts each time
	// till the length of the last part equals to 1
	// Then it start calling the merge() on these small parts to sort them by mixing
	// the according to the ascending order
	// And it keeps go up till it sorts the hole array by the same method
	private void mergeSort(int[] arr, int start, int end) {

		if (start >= end)
			return;

		int mid = (start + end) / 2;

		mergeSort(arr, start, mid);
		mergeSort(arr, mid + 1, end);
		merge(arr, start, end);
	}

	// This method sorts the two giving parts that mergeSort() gives her
	// by mixing the according to the ascending order
	private void merge(int[] arr, int start, int end) {

		// This is a temporary array that the sorted mix saved at
		int[] newArr = new int[end - start + 1];
		int mid = (start + end) / 2;

		// These two counter used to point on the current position in each part
		int arr1Counter = start;
		int arr2Counter = mid + 1;

		// This for runs on the newArr array from start to end
		for (int i = 0; i < newArr.length; i++) {

			// This if check if the first part has finished so we fill the rest with the
			// rest second part element
			// by calling the fillWithTheRest() method that makes the job then it break/end
			// the for
			if (arr1Counter > mid) {

				fillWithTheRest(arr, newArr, arr2Counter, i);
				break;

			}
			// This if check if the second part has finished so we fill the rest with the
			// rest first part elements
			// by calling the fillWithTheRest() method that makes the job then it break/end
			// the for
			else if (arr2Counter > end) {

				fillWithTheRest(arr, newArr, arr1Counter, i);
				break;

			}
			// This if condition checks if the number in the first part is bigger than the
			// current number of the second part
			// So it adds it to the new array and it increases the arr1Counter with one
			else if (arr[arr1Counter] < arr[arr2Counter]) {
				// This line count the comparisons times
				this.comparisonsCounter++;
				newArr[i] = arr[arr1Counter];
				arr1Counter++;

			}
			// This else means that the number in the second part is bigger or equals
			// than/to the current number of the second part
			// So it adds it to the new array and it increases the arr2Counter with one
			else {
				// This line count the comparisons times
				this.comparisonsCounter++;
				newArr[i] = arr[arr2Counter];
				arr2Counter++;

			}

		}

		// This for refills the array (arr) with the same sorted elements from the
		// newArr in the correct indexes
		for (int i = 0; i < newArr.length; i++) {
			arr[start] = newArr[i];
			start++;
		}

		//This line prints the array after each outer for if verbose  == true by calling super.arrayToString() on the arrayCopy
		if (this.verbose) {
			System.out.println(super.arrayToString(arr));
		}

	}

	// This method fills the given newArr with the rest elements of the given array
	// arr according to the given indexes from merge()
	private void fillWithTheRest(int[] arr, int[] newArr, int currentIndex, int i) {
		while (i < newArr.length) {

			newArr[i] = arr[currentIndex];
			currentIndex++;
			i++;

		}
	}

	// This getStatistics method returns a String of the statistics
	// By calling the getStatistics() method from the super class and adding it to
	// the name of the sorting algorithm
	public String getStatistics() {
		return "MergeSort" + super.getStatistics();
	}

}
