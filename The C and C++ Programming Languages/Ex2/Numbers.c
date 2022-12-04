#include <stdio.h>
#include <string.h>
#include <math.h>

// declaring preprocessor directives
#define TRUE 1
#define FALSE 0


//declaring the needed const variables and initializing them with needed values
const int MAX_NUMS = 20;
const int MAX_DEC_NUMBER = 255;
const int OCT = 8;
const int BIN = 2;
const int DEC = 10;

// decalring private needed const variables
// currBasePointer which i use for pointing to the currbase varialbe in main so I can knoww the base and print it's name
// using the private function getBaseName() [For Making Things More Clear To The User When I Use printf()] 
// I use it in one other place which is the find method because there is a need for that in the way that i wrote the function
int *currBasePointer;
const int maxNumStrLen = 10;
const int maxOptionStrLen = 3;
// Sometimes i use this char for making the entered string invalid if for example if it's longer then eight (8) chars
// because mac chars is eight(8) in binay 11111111 (8) = 255 (8)
const char invalidChar = 'F';

// declaring the needed functions (explanation for each function bellow(where each function written))
void  printMenu(int);
int stringToInt(char[], int);
int enterNumber(int[], int, int);
int find(int[], int, int);
int howManyEven(int[], int);
int intToString(int, char[], int);
int printArray(int[], int);
int charToInt(char);

// declaring the needed (private functions)
// This function getBaseName(int) returns the name of the base according to it's number
const char* getBaseName(int);
// This function clearBuffer(void) keeps clearing/redaing from buffer till it's reads/clears a newLine charachter ('\n')
// So the next methods that wants to recive/ scan/ get inputs from the user like (fgets(), scanf()) didit't make an unexpexted/ weird behavior
void clearBuffer(void);
//This function makes bubble sort on the givven array and it uses swap function to swap the elements if needed
int bubbleSort(int[], int);
int swap(int[],int, int);
//This funciton checks if the entered string is above eight(8) digits [Invalid Input]
int isAboveEightDigits(char[]);


int main(void){

    // declaring and initializing the needed variables
    int numsArr[MAX_NUMS];
    int totalNums = 0;
    int currBase = DEC;
    int optionNum;

    int chosenBaseNum;

    char numStr[maxNumStrLen];
    //This array i use for holding the scanned entered option from the user so i can check first if it's valid or not
    char optionCharHolder[maxOptionStrLen];

    // infinit loop ends when the user choses the number 6 from the main menu
    for(;;){

        // printing the main menu for the user
        printMenu(currBase);
        // scanning the input(chosen option) from the user
        fgets(optionCharHolder, maxOptionStrLen, stdin);

        // This if checks if the chosen option is valid or not by cheking the char in position [1] 
        // if it's not equal to '\n' that means that the entered option is invalid like (ex. 12, 1a, 123) the entered inbuts
        // are out of range which is (1 - 3) and the array lingth is 3 so we scan two cahrs and the last position is for '\0'
        // so if the input is with valid length the char in postioin 2 will be '\n'
        // is it's not '\n' I call clearBuffer function so get red from all not needed characters and i get red from '\n' that stuck in the buffer
        // the i make the string invalid by making the character in position [0] 'F' (invalidChar)
        // so intToString method returns (-1) which means the string is invalid
        if(optionCharHolder[maxOptionStrLen - 2] != '\n'){
            clearBuffer();
            optionCharHolder[0] = invalidChar;
        }

        // if the string is with valid length i replace '\n' in the second position[1] with '\0' so for the computer/compiler the string ends there 
        optionCharHolder[maxOptionStrLen - 2] = '\0';

        // after I checked that the input is with valid length I conver the char to a real num (int) so i can continue with the code
        optionNum = stringToInt(optionCharHolder, DEC);

        // if optionNum == 6 that means (Exite) so i break the infinit for loop
        if(optionNum == 6){
            break;
        }

        //This switch is for the options from (1-5)
        switch(optionNum){

            // option (1) is for changeing the base
            case 1:
                //this infinit for loop keeps going till the user enters a valid option number
                for(;;){
                    
                    printf("%s\n%s\n%s\n%s", "1. Decimal", "2. Binary", "3. Octal", "Enter The Option Number:\n");

                    // scanning the input(chosen option) from the user
                    fgets(optionCharHolder, 3, stdin);
                    
                    // This if checks if the chosen option is valid or not by cheking the char in position [1] 
                    // if it's not equal to '\n' that means that the entered option is invalid like (ex. 12, 1a, 123) the entered inbuts
                    // are out of range which is (1 - 3) and the array lingth is 3 so we scan two cahrs and the last position is for '\0'
                    // so if the input is with valid length the char in postioin 2 will be '\n'
                    // is it's not '\n' I call clearBuffer function so get red from all not needed characters and i get red from '\n' that stuck in the buffer
                    // the i make the string invalid by making the character in position [0] 'F' (invalidChar)
                    // so intToString method returns (-1) which means the string is invalid
                    if(optionCharHolder[maxOptionStrLen - 2] != '\n'){
                        clearBuffer();
                        optionCharHolder[0] = invalidChar;
                    }
                    
                    // if the string is with valid length i replace '\n' in the second position[1] with '\0' so for the computer/compiler the string ends there 
                    optionCharHolder[maxOptionStrLen - 2] = '\0';

                    // after I checked that the input is with valid length I conver the char to a real num (int) so i can continue with the code
                    chosenBaseNum = stringToInt(optionCharHolder, DEC);

                    // This if checks if the enterred option in the valid range (1 -3)
                    // if true it enters to a switch whitch makes the currBase = chosen base and it breaks the infinit for loop
                    // else it prints a warning message to the user and makes the whole process again
                    if(chosenBaseNum >= 1 && chosenBaseNum <= 3){
                        switch (chosenBaseNum) {
                        
                            case 1:
                                currBase = DEC;
                                break;

                            case 2:
                                currBase = BIN;
                                break;

                            case 3:
                                currBase = OCT;;
                                break;

                        }

                        break;

                    } else {
                        printf("**Please chose a valid number from the given list:**\n");
                    }
                }
            break;

            // option (2) is enter a number which calls the function enterNumber() that asks the user to enter a number and it adds it the the array numsArr
            // and if the process succeeded it returns TRUE and I increase the totoalNums with one 
            case 2:
                if(enterNumber(numsArr, totalNums, currBase) == TRUE){
                    totalNums++;
                } 
            break;

            // option (3) is find a number this option asks the user to enter the number that he wants to find and it checks if it's length is valid 
            // if true it calls the function find that gets the job done also if the user enters an invalid number the function find deals with that status and it asks the user to enter the number againg 
            case 3:
                printf("Please enter a number of %s base that you want to find:\n", getBaseName(currBase));
                fgets(numStr, maxNumStrLen, stdin);

                // This if checks if the chosen option is valid or not by cheking if the number of digits is <= 8 by calling the function isAboveEightDigits(numStr)
                // if the length is above eight digits the I call clearBuffer function so get red from all not needed characters and i get red from '\n' that stuck in the buffer
                // the i make the string invalid by making the character in position [0] 'F' (invalidChar)
                // so intToString method returns (-1) which means the string is invalid
                if(isAboveEightDigits(numStr)){
                    numStr[0] = invalidChar;
                    clearBuffer();
                } else {
                    numStr[strlen(numStr) - 1] = '\0';
                }

                int convertedInt = stringToInt(numStr, currBase);
                // initializing the pointer currBasePointer with the address of &(currBase)
                currBasePointer = &currBase;
                
                find(numsArr, convertedInt, totalNums);
            break;

            // option (4) calls howManyEven() function that returns the number of the even numbers in the array numsArr
            case 4:
                printf("==> Even Numbers: %d\n", howManyEven(numsArr, totalNums));
            break;

            // option (5) prints the numbers in the array numsArr as asked in 3 formats DEC, BIN, OCT in ascending order
            case 5:
                printArray(numsArr, totalNums);
                break;

            // if the option is not in the range (1 - 6) i print a warning message to the user and i asks him to chose an option aggain 
            // so the big/main for loop will be executed
            default:
                printf("**Please chose a valid number from the given list:**\n");
                break;
        }
    }

    return 0;
}

// This function printMenu(int base) prints the main menu with the curr base name which i get from the method  getBaseName(base)
void printMenu(int base){

    printf("%s%s%s\n%s\n%s\n%s\n%s\n%s\n%s\n%s\n",
    "You are in ", getBaseName(base), " Mode", "1. Change Base.", "2. Enter a number.",
    "3. Find a number.", "4. Get the amount of even numbers.", "5. Print.",
    "6. Exit.", "Please Chose An Option Number:");

}

// This method prints the array numsArr as asked in 3 formats DEC, BIN, OCT in ascending order
int printArray(int numbers[], int totalNums){
    int i;
    char numStr[maxNumStrLen];

    //sorting the array by calling the method bubbleSort() on it
    bubbleSort(numbers, totalNums);

    // printing the array in formated way
    printf("List Of Numbers\n==============================\nDEC\tBIN\t\tOCT\n");

    for(i = 0; i < totalNums; i++){
        intToString(numbers[i], numStr, DEC);
        printf("%s\t", numStr);
        intToString(numbers[i], numStr, BIN);
        printf("%s\t", numStr);
        intToString(numbers[i], numStr, OCT);
        printf("%s\n", numStr);
    }
}


// This function enterNumber receives/ scanns number from the user and if it's a valid number and the array are not full it adds the number to the array
int enterNumber(int numbers[], int totalNums, int base){
    char enteredStrNumber[maxNumStrLen];
    int convertedInt;

    // This for loop keeps going till the user enters a valid number the is breaks and the function return TRUE
    for(;;){
        printf("Please enter a number of %s base:\n", getBaseName(base));
        fgets(enteredStrNumber, maxNumStrLen, stdin);

        // This if checks if the chosen option is valid or not by cheking if the number of digits is <= 8 by calling the function isAboveEightDigits(numStr)
        // if the length is above eight digits the I call clearBuffer function so get red from all not needed characters and i get red from '\n' that stuck in the buffer
        // the i make the string invalid by making the character in position [0] 'F' (invalidChar)
        // so intToString method returns (-1) which means the string is invalid
        if(isAboveEightDigits(enteredStrNumber)){
            enteredStrNumber[0] = invalidChar;
            clearBuffer();
        } else {
            enteredStrNumber[strlen(enteredStrNumber) - 1] = '\0';
        }

        //if the array is full i print a message for the user and the function returns FALSE
        if(totalNums == MAX_NUMS){
            printf("**The Array Is Full**\n");
            return FALSE;
        }


        convertedInt = stringToInt(enteredStrNumber, base);
       
        // this if checks if the number is invalid according to the currBase so it prints a warning message and continue 
        // so we ask the user to enter a number again
        if(convertedInt == -1 || convertedInt > 255){
            printf("The Base Is ****(%s [Positive]) Between (0 - %d)****\n", getBaseName(base), MAX_DEC_NUMBER);
            continue;
        }

        // if everything is good/ correct the function adds the nubmer to the array and it breks the infinit for the the function reutrns TRUE
        numbers[totalNums] = convertedInt;
        break;
    }

    return TRUE;
}


// This funciotn chks if the digit is valid according to the currBase
// if it's valid return TRUE if not returns FALSE
int isValidDigit(int digit, int base){
    if(digit >= 0 && digit < base){
        return TRUE;
    }

    return FALSE;
}

// This function returns a string (char array) of the base name according to the given base number
const char* getBaseName(int base){

    char *currBaseName;

    switch (base) {

        case 10:
            currBaseName = "Decimal";
            break;

        case 2:
            currBaseName = "Binary";
            break;

        case 8:
            currBaseName = "Octal";
            break;
    }

    return currBaseName;

}


//This function returns the int value of a char (ASCII value) if it's valid (0 - 9)
// if it's invalid it return (-1)
int charToInt(char c){
    
    if(c >= '0' && c <= '9'){
        return c - 48;
    }
    
    return -1;
}


//This function converts the string (from the bases BIN, DEC, OCT) to a DEC value and it returns the result like int 
int stringToInt(char number[], int base){
    

    //This if checks if the string length equals to zero if true it returns (-1)
    if(strlen(number) == 0){
        return -1;
    }
    
    int result, currCharInt;
    int i;

    // This for loop goes through all the digits in the given string and it converts each digit according to the given base to the correct 
    // number in DEC and it adds the result to the variable result which we return at the end of the function
    // [according to the explained algorithm in the question]
    for(i = 0; i < strlen(number); i++){
        currCharInt = charToInt(number[i]);

        if(currCharInt == -1 || currCharInt >= base){
            return -1;
        }

        result += currCharInt * pow(base, (strlen(number) - i - 1));
    }

    return result;
}

//This method converts the given int to a string in the format of the given base according to the explained algorithm in the question
int intToString(int num, char numStr[], int base){
    double tempNum = num;
    //This char array i use for holding/ saving the carry, because making casting some time will give an incorrect result
    //which is a problem i faced if i print the number it prints 2.0 but if i make casting to int it prints 1
    // so i chose to save the carry as chars the i reconvert the to a int using the method charToInt()
    char carryArr[maxNumStrLen];
    int i = 0, j;

    while(i < maxNumStrLen - 2){
        carryArr[i] =(char) (((((double) tempNum / base) - ((int) tempNum / base)) * base) + 48);
        tempNum = (int) (tempNum / base);
        i++;

        //if the base is binary i need to keep going and putting zeros till the eight postion[7]
        //so i will get the assked foramt 00000000 eight digits
        if(base != BIN && tempNum == 0){
            break;
        }
    }


    for(j = 0; j < i; j++){
        numStr[j] = carryArr[i - j - 1];
    }

    // changing the end of the string to the postion after the last digit of the case
    numStr[j] = '\0';

    return TRUE;
}

// This method recivec a number and it searches if this number exists in the array 
//if the number exists it returns TRUE else it returns FALSE
//but befor the it earches it checks if the number is valid or nut according to the cuurBase
//if it's valid it executes the search proccess else it asks the user to enter the number again
int find(int numbers[], int numKey, int totalNums){
    char enteredStrNumber[maxNumStrLen];
    int convertedInt = numKey, i;
    int isFirstTime = 1;
    

    for(;;){
        // first time i don't need to ask the user to enter the number and to check if it's valid because this job have been done in the main
        if(isFirstTime == 0){

            printf("Please enter a number of %s base that you want to find:\n", getBaseName(*currBasePointer));
            fgets(enteredStrNumber, maxNumStrLen, stdin);

            // This if checks if the chosen option is valid or not by cheking if the number of digits is <= 8 by calling the function isAboveEightDigits(numStr)
            // if the length is above eight digits the I call clearBuffer function so get red from all not needed characters and i get red from '\n' that stuck in the buffer
            // the i make the string invalid by making the character in position [0] 'F' (invalidChar)
            // so intToString method returns (-1) which means the string is invalid
            if(isAboveEightDigits(enteredStrNumber)){
                enteredStrNumber[0] = invalidChar;
                clearBuffer();
            } else {
            enteredStrNumber[strlen(enteredStrNumber) - 1] = '\0';
            }

            convertedInt = stringToInt(enteredStrNumber, *currBasePointer);
        }

        if(isFirstTime == 1){
            isFirstTime--;
        }

        if(convertedInt == -1 || convertedInt > 255){
             printf("The Base Is ****(%s [Positive]) Between (0 - %d)****\n", getBaseName(*currBasePointer), MAX_DEC_NUMBER);
            continue;
        }

        //This for searches for the number and if the number is found it prints a message that the number is found then it returns TRUE
        for(i = 0; i < totalNums; i++){
            if(numbers[i] == convertedInt){
                printf("The Number Is ( Found )\n");
                return TRUE;
            } 
        }

        //if the number is found we break the infinit for
        break;  

    }

    // printing a message that the number in not Found and return FALSE
    printf("The Number Is ( Not Found )\n");
    return FALSE;

}


//This function count how many even numbers are in the array and it returns the result of the counting
//it makes that job using the bitwise operator & bettwen the number[i] and the number One (1) [number[i] & 1]
//if the result is 1 that means that the number[i] is odd so the operation ( 1 - [number[i] & 1]) adds 0 to the result
//else if the result is 0 the operation ( 1 - [number[i] & 1]) adds 0 to the result
int howManyEven(int numsArr[], int totalNums){
    int i;
    int counter = 0;

    for(i=0; i < totalNums; i++){
        
        counter += 1 - (numsArr[i]&1);
    }

    return counter;
}


//This method executes bubble sort on the given array which is numsArr
int bubbleSort(int numsArr[], int totalNums){

    int i,j;

    for(i = 0; i < totalNums; i++){
        for(j = 0; j < totalNums - i - 1; j++){
            if(numsArr[j] > numsArr[j+1]){
                swap(numsArr, j, j+1);
            }
        }
    }

}

//This methods swaps two elements in the array according to the given indexes
int swap(int arr[],int indOne, int indTwo){

    int temp = arr[indOne];
    arr[indOne] = arr[indTwo];
    arr[indTwo] = temp;
}

// This function clearBuffer(void) keeps clearing/redaing from buffer till it's reads/clears a newLine charachter ('\n')
// So the next methods that wants to recive/ scan/ get inputs from the user like (fgets(), scanf()) didit't make an unexpexted/ weird behavior
void clearBuffer(void){
    while(getchar() != '\n');
}

//This funciton checks if the entered string is above eight(8) digits [Invalid Input]
int isAboveEightDigits(char number[]){

    if(strlen(number) == 9 && number[8] != '\n'){
        return TRUE;
    }

    return FALSE;

}