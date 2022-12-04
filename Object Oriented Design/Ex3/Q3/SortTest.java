package Ex3_Question4;

import java.util.Scanner;

public class SortTest {
// main function for testing the code
	public static void main(String[] args) {
		System.out.print("Please Enter The Array Size: ");
		Scanner scan = new Scanner(System.in);
		int len = scan.nextInt();
		System.out.println();
		int array[] = new int[len];
		Sort strategysort = new Sort();
		if (len <= 20) { 
			strategysort.Sort(array, len, new InsertionSort());
		} else if (20 < len && len <= 100) { 
			strategysort.Sort(array, len, new QuickSort());
		} else if (len > 100) {
			strategysort.Sort(array, len, new MergeSort());
		} else {
			System.out.println("put a size bigger than 0");
			return;
		}
	}

}