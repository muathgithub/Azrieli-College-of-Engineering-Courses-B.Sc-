
public class PriorityQueue {

	Prioritizeable[] prioritiesList;

	//This constructor initializes the Prioritizeable[] prioritiesList array with new empty array with the length of 100
	public PriorityQueue() {

		prioritiesList = new Prioritizeable[100];
	}

	//This method inserts the given Prioritizeable object to the array in the suitable index according to it's priority
	public void insert(Prioritizeable element) {
		
		//This if checks if there is a place in the array or it is full
		//if it's full it returns (dosen't do any thing)
		if(prioritiesList[prioritiesList.length - 1] != null) {
			return;
		}
		
		//This for goes through the array till it finds the suitable index for the given Prioritizeable object according to it's priority
		for (int i = 0; i < prioritiesList.length; i++) {

			//This if checks if the array in the current index is empty
			//So it adds the element to the index and it returns
			if (prioritiesList[i] == null) {

				prioritiesList[i] = element;
				return;
			}

			//This if cheks if the priority of the current object in the i index is lower the the priority of the element
			//So if true it puts the element in the current i index and it shifts the rest of the element one step to the right and it breaks
			if (prioritiesList[i].compareTo(element) < 0) {

				//These to temporary Prioritizeable pointers for making the shift(swap)
				Prioritizeable tempOne = element;
				Prioritizeable tempTwo;
				
				//This for goes through the array from index i to the end of the array or to the first null element if there is one
				//And it keeps swapping the elements so at the end the element will be shifted
				for (int j = i; tempOne != null && j < prioritiesList.length; j++) {
					tempTwo = prioritiesList[j];
					prioritiesList[j] = tempOne;
					tempOne = tempTwo;
				}

				break;
			}

		}

	}

	
	//This method removes the first element from the array and it returns it
	public Prioritizeable remove() {
		//This temporary Prioritizeable pointer holds the first element in order to return it at the end of the method
		Prioritizeable firstElement = this.prioritiesList[0];
		
		//This line removes the first element from the array
		this.prioritiesList[0] = null;
		int i;

		//This for goes through the array and it shifts all the elements from index 1 to the end of the array or to the first null
		//one step to the left
		for (i = 1; i < prioritiesList.length && prioritiesList[i] != null; i++) {
			prioritiesList[i - 1] = prioritiesList[i];
		}
		
		//This line initializes the element in the i - 1 index with null because the element have been shifted to the index i - 2
		prioritiesList[i - 1] = null;

		return firstElement;
	}

	
	//This toString method returns a String of all the Prioritizeable objects in the array that are not equals to null
	public String toString() {

		String str = "";

		for (int i = 0; i < this.prioritiesList.length && prioritiesList[i] != null; i++) {
			
			str = str.concat(prioritiesList[i].toString() + "\n");
		}

		return str;

	}

}
