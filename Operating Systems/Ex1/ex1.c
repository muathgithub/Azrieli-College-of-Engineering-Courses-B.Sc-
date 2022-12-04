// Program Name: String Parser
// Program Description: this program receives a sentence from the user and it parses it,
// by counting the number of it's words and for each word it prints the word with its length beside.

// Student Name: Muath Abu Jamal

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

// declaring preprocessor directives
#define MAX_STR_LEN 511
#define TRUE 1
#define FALSE 0

// declaring the needed functions (explanation for each function bellow(where each function written))
int strLenNoNewLine(char*);
int wordsCount(const char*, int*, const int*);
void printWordsList(const char*, int*, const int*);
void printWord(const char*, const int*, const int*);

int main() {

    // declaring the needed variables which i will ues as global variables by passing their address to other functions
    char sentence [MAX_STR_LEN];
    // this variable used as a flag to know if we are in a word or not (in the sentence)
    int isInWord;

    // an infinite loop that end when the user enters the word "exit"
    for(;;){
        // initializing / resetting the variable isInWord
        isInWord = FALSE;

        printf("Please Enter A Sentence:\n");

        // scanning / receiving  the sentence from the user, if there is an error we pass it and give the user the ability to enter a new sentence
        if (fgets(sentence, MAX_STR_LEN, stdin) == NULL){
            printf("-----------------------------------\n");
            continue;
        }

        // calculating the length of the sentence after removing the '\n' character if it exists
        // by calling the function strLenNoNewLine()
        // this variable used as global variable by passing it's address to the other functions
        int sentenceLen = strLenNoNewLine(sentence);

        // checking if the entered sentence is the word "exit" which means the end of the program
        // if true we break the loop the the main returns 0 and ends the program successfully
        if(strcmp(sentence, "exit") == 0){
            printf("************** EXIT ***************\n");
            break;
        }

        // printing the number of words in the sentence by calling the function wordsCount()
        printf("%d Words\n", wordsCount(sentence, &isInWord, &sentenceLen));

        // calling the function printWordsList() that prints each word and its length beside it
        printWordsList(sentence, &isInWord, &sentenceLen);

        printf("-----------------------------------\n");
    }

    return 0;
}


// This function receives three pointers: 1- for the sentence, 2- for the bool isInWord (Used As Global), 3- for the sentenceLen (Used As Global)
// the function finds the start and end indexes for each word in the sentence and it sends
// them each time to the function printWord() that prints each word and its length beside
void printWordsList(const char* sentence, int* isInWord, const int* sentenceLen){
    int wordStartInd = 0, wordEndInd = 0;
    // resetting the variable/flag isInWord to the new check (because other function can use it as a global variable)
    *isInWord = FALSE;

    int i;
    // this for loop goes through the sentence and for each word it saves the start and end character indexes
    // after reaching the end index for each word it calls the function printWord() that allocate memory and copy the word to it then it prints it
    for (i = 0; i < *(sentenceLen); i++){
        if( !(*(isInWord)) && (sentence[i] != ' ')) {
            *isInWord = TRUE;
            wordStartInd = i;
        }
        else if (*(isInWord) && sentence[i] == ' '){
            *isInWord = FALSE;
            wordEndInd = i - 1;
            printWord(sentence, &wordStartInd, &wordEndInd);
        }
    }

    // checking the special case of finishing the for loop while we are still in a word that will not be printed
    // so here if we are still in word we print is as before
    if( *(isInWord) ){
        wordEndInd = i - 1;
        printWord(sentence, &wordStartInd, &wordEndInd);
    }
}

// This function receives three pointers: 1- for the sentence, 2- for the first index in the word (according to the indexes of the sentence), 3- for the last index in the word (according to the indexes of the sentence)
// the function prints the word from the given wordStartInd to wordEndInd after copying it to a new allocated memory then it frees the allocated memory
void printWord(const char* sentence, const int* wordStartInd, const int* wordEndInd){
    int i, j;

    int wordLen = (( *(wordEndInd) - *(wordStartInd) ) + 1);

    // allocating exactly needed memory for the characters of the word + 1 for the '\0' character
    char* currWordPointer = (char*) malloc(sizeof(char) * (wordLen + 1));

    // checking if the memory allocation goes wrong / failed
    // if true i print a message for the user then the program exits with EXIT_FAILURE code (1)
    if(currWordPointer == NULL){
        printf("*** FAILED TO ALLOCATE MEMORY ***");
        exit(EXIT_FAILURE);
    }


    // going through the sentence according to the indexes of the word for copying in to the new allocated memory
    for (i = *(wordStartInd), j = 0; i <= *(wordEndInd); i++, j++){
        currWordPointer[j] = sentence[i];
    }

    // finishing the word by putting the '\0' character at the end of the word
    currWordPointer[wordLen] = '\0';

    // printing the word and it's length beside it
    printf("%s: %d\n", currWordPointer, wordLen);

    //freeing the allocated memory
    free(currWordPointer);
}

// This function receives three pointers: 1- for the sentence, 2- for the bool isInWord (Used As Global), 3- for the sentenceLen (Used As Global)
// the function goes through the sentence and it counts the number of words in it using the variable isInWord
// which considered as a flag to know if we are in a word or not (in the sentence)
// at the end the function returns a value of type int which is the number of words in the sentence
int wordsCount(const char* sentence, int* isInWord, const int* sentenceLen){
    int wordsCounter = 0;
    // resetting the variable/flag isInWord to the new check (because other function can use it as a global variable)
    *isInWord = FALSE;

    int i;
    // this for that goes through the hole sentence and each time it finds character while
    // the variable isInWord is false it increases the wordsCounter by one
    for (i = 0; i < *(sentenceLen); i++){
        if( !(*(isInWord)) && (sentence[i] != ' ')) {
            *isInWord = TRUE;
            wordsCounter = wordsCounter + 1;
        }
        else if (*(isInWord) && sentence[i] == ' '){
            *isInWord = FALSE;
        }
    }

    return wordsCounter;
}


// This function receives a pointer for the sentence
// and it calculates the length of the sentence by calling the function strlen()
// then it checks it the last character is '\n', if true it changes it to '\0'
// and it decreases the length by one then it returns the length
int strLenNoNewLine(char* sentence){
    int sentenceLen = (int) strlen(sentence);

    if (sentence[sentenceLen - 1] == '\n'){
        sentence[sentenceLen - 1] = '\0';
        sentenceLen = sentenceLen - 1;
    }

    return sentenceLen;
}
