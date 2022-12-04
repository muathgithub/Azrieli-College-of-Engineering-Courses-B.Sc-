#include <stdio.h>
#include <string.h>

#define TRUE 1
#define FALSE 0

void addBigNumbers(char[], char[], char[]);
int isValidDigit(char);
char intToChar(int);
int charToInt(char);

int main(void){

    char a[100], b[100], c[200];
    printf("Enter first number:\n");
    scanf("%s", a);
    printf("Enter second number:\n");
    scanf("%s", b);
    addBigNumbers(a, b, c);
    printf("The result is: %s", c);

}

void addBigNumbers(char first[], char second[], char result[]){

    int firstNumPointer = strlen(first) - 1;
    int secondNumPointer = strlen(second) - 1;
    int resultNumPointer = 0;
    int carry = 0, sumTwoDigits;
    int firstCharToInt, secondCharToInt;

    while(firstNumPointer != -1 && secondNumPointer != -1){

        firstCharToInt = charToInt(first[firstNumPointer]);
        secondCharToInt = charToInt(second[secondNumPointer]);

        if(firstCharToInt == -1 || secondCharToInt == -1){
            strcpy(result, "error");
            return;
        }

        sumTwoDigits = firstCharToInt + secondCharToInt + carry;

        if(sumTwoDigits > 9){
            carry = sumTwoDigits / 10;
            sumTwoDigits = sumTwoDigits % 10;
        } else {
           carry = 0; 
        }

        result[resultNumPointer] = intToChar(sumTwoDigits);

        firstNumPointer--;
        secondNumPointer--;
        resultNumPointer++;
    }

    if(firstNumPointer != -1 && secondNumPointer == -1){
        while(firstNumPointer != -1){
            firstCharToInt = charToInt(first[firstNumPointer]);

            if(firstCharToInt == -1){
                strcpy(result, "error");
                return;
            }

            sumTwoDigits = firstCharToInt + carry;

            if(sumTwoDigits > 9){
                carry = sumTwoDigits / 10;
                sumTwoDigits = sumTwoDigits % 10;
            } else {
                carry = 0; 
            }

            result[resultNumPointer] = intToChar(sumTwoDigits);

            firstNumPointer--;
            resultNumPointer++;
        }
    }

    if(firstNumPointer == -1 && secondNumPointer != -1){
        while(secondNumPointer != -1){
            secondCharToInt = charToInt(second[secondNumPointer]);

            if(secondCharToInt == -1){
                strcpy(result, "error");
                return;
            }

            sumTwoDigits = secondCharToInt + carry;

            if(sumTwoDigits > 9){
                carry = intToChar(sumTwoDigits / 10);
                sumTwoDigits = sumTwoDigits % 10;
            } else {
                carry = 0; 
            }

            result[resultNumPointer] = intToChar(sumTwoDigits);

            secondNumPointer--;
            resultNumPointer++;
        }
    }

    if(carry != 0){
        result[resultNumPointer] = intToChar(carry);
        resultNumPointer++;
    }

    
    char reverseArray[resultNumPointer];
    strcpy(reverseArray, result);
    int i;

    while(result[resultNumPointer - 1] == '0'){
        resultNumPointer--;
    }

    result[resultNumPointer] = '\0';

    for(i = 0; i < resultNumPointer; i++){

        result[i] = reverseArray[resultNumPointer - i - 1];
    }

}


char intToChar(int num){

    return num + 48;

}

int charToInt(char c){

    if(isValidDigit(c)){
        return c - 48;
    }

    return -1;
}

int isValidDigit(char c){

    if(c >= '0' && c <= '9'){
        return TRUE;
    }

    return FALSE;

}


