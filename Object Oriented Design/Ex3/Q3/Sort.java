package Ex3_Question4;

import java.util.Random;

public class Sort { // Strategy class have a sort fumction
	
	public void fillsArrayNumbersRandom(int arr[] , int len) {
		Random objGenerator = new Random();
        for (int iCount = 0; iCount< len; iCount++){
          arr[iCount] = objGenerator.nextInt(100)+1;
         }
	}
	public void printArray(int [] arr) {
		for (int i  = 0; i < arr.length; i++) {
			System.out.print(arr[i]);
			if(i!=arr.length-1) System.out.print(", ");
		}
		System.out.println("\n");
	}
	public void Sort(int arr[] , int len , SortStrategy SortMethod){ // this function sort an array, depends what receive parameter SortArrayStrategy are 
		fillsArrayNumbersRandom(arr , len);
		System.out.println("Array Before The Sort:\n" );
		printArray(arr);
		SortMethod.Sort(arr);
		System.out.println("Array After The Sort:\n" );
		printArray(arr);
	}
}
