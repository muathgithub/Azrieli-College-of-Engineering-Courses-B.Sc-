#include <stdio.h>

int main(void){

   //declaring the needed variables numOne for holding first/privious value and numTwo for holding the second needed value for applying the operation of diffrence
   int numOne, numTwo;

   printf("Please enter a series of positive numbers (ending with a negative number):\n");
   //scanning the first two int values from the user input 
   scanf("%d %d", &numOne, &numTwo);
   printf("\n");

   //This while loop will go through all the scaned values till it reach nigative value
   while(numOne >= 0 && numTwo >= 0){

        //This line prints the diffrence bettwen numOne and numTwo
        printf("%d ", (numTwo - numOne));
        //This line initializes numOne with the value of numTwo so we can get the next scaned value to apply the operation on the next couple of values
        numOne = numTwo;
        //scaning the next int value to numTwo (nextStep)
        scanf("%d", &numTwo);

   }


}


