#include <stdio.h>
#include <math.h>

//This is a macro definition for the array length
#define ARRLEN 3

int order3(int);
int bubbleSort(int[]);
int swap(int[],int, int);
int numLength(int);


int main(void){
    //declaring the needed variables
    int num, tempOrdNum;
    //scanning the value from the user input for the first time
    printf("Please enter a number with three digits:\n");
    scanf("%d", &num);

    //This while keeps going / runs till the user inters a nigative value/number
    while(num >= 0){
        //This if condition checks if the number length equals to 3(digits) / correct input
        if(numLength(num) == 3){
            //This line calls the method order3() which it's job to order the digits of the scanned number in an Ascending from left to right
            tempOrdNum = order3(num);
            //THis line prints the orderd number
            printf("%d\n", tempOrdNum);
        }

        //These to lines asks the user to enter the next value and the scanf() reads it for ordring it in the next while if its a valid input (positive number whith three digits)
        printf("Please enter a number with three digits:\n");
        scanf("%d", &num);
    }

    printf("End\n");

}

//This method orders the digits of the number and it cinverts them into a real number and it returns it
int order3(int num){

    int arr[ARRLEN];
    int i;
    int result = 0;

    //splitting the digits of the number and saving each digit in the arr(array)
    for(i = 0; i < ARRLEN; i++){
        arr[i]=num%10;
        num /= 10;
    }

    //calling the private bubbleSort() method for sorting the array (ordring the numbers in a Ascending order)
    bubbleSort(arr);

    //converting the orderd array into a real/one int by multiplying each digit with 10 pow (position)
    for(i = 0; i < ARRLEN; i++){
        result += arr[i] * pow(10, ARRLEN - i - 1);
    }

    //returning the result (the number with the ordered digits)
    return result;
}


//This method applayes bubbleSort on the received array
int bubbleSort(int arr[]){

    int i,j;

    for(i = 0; i < ARRLEN; i++){
        for(j = 0; j < ARRLEN - i - 1; j++){
            if(arr[j] > arr[j+1]){
                swap(arr, j, j+1);
            }
        }
    }

}

//This method waps the emelents of the two gevin indexes
int swap(int arr[],int indOne, int indTwo){

    int temp = arr[indOne];
    arr[indOne] = arr[indTwo];
    arr[indTwo] = temp;
}


//This method returns the length of the received number by keep deviding it with 10 till it becomes zero
int numLength(int num){

    int lenCounter = 0;

    while(num != 0){
        lenCounter++;
        num /= 10;
    }

    return lenCounter;
}



