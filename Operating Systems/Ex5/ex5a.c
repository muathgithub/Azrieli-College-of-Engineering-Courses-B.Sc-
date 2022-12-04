// Program Name: Polynomial Evaluation Calculator [ex5a] threads version
// Program Description: This program receives a string of a polynomial and a value for x
// it evaluates that polynomial by using a suitable number of threads which make the calculations
// according to the given x value, at the end the main thread prints the result.

// Student name: Muath Abu Jamal

#include <stdio.h>

#include <string.h>

#include <stdlib.h>

#include <pthread.h>

// declaring preprocessor directives
#define TRUE 1
#define FALSE 0
#define SPLITERS_NUM 5
#define SPLITERS_MAX_STR_LEN 4
#define MAX_POLYNOMIAL_PARTS_NUM 4
#define MAX_PART_PARTS 2
#define MAX_SENTENCE_LEN 512

// declaring the needed functions (explanation for each function bellow(where each function written))
int strLenNoNewLine(char * );
int sectionsCount(char * , char * );
char ** toSectionsArray(char * , char * , char * );
char * getSectionStr(const char * ,
  const int * ,
    const int * );
void toCoefficientPowArray(char * );
void freeAll(char ** ,
  const int * );
int calcByThreads();
void * threadFunc(void * );
int powFunc(int, int);
void noSpaceStr(char * );
void resetGlobalStorage();

// declaring the needed Global variables
int coefficientPowArray[MAX_POLYNOMIAL_PARTS_NUM][MAX_PART_PARTS];
int currCoefficientPowArrayIndex = 0;
int threadsNum = 0;
int threadsResults[MAX_POLYNOMIAL_PARTS_NUM];
int xValue;

int main() {

  // a char array for holding the entered polynomial and x value from the user
  char sentence[MAX_SENTENCE_LEN];

  // an infinite loop that end when the user enters the word "done"
  for (;;) {
    // resetting the global storage for the next threads work/ parameter and results
    resetGlobalStorage();

    printf("Please Enter The Polynomial And X Value:\n");

    // scanning / receiving  the sentence from the user, if there is an error we print proper message for the user and we pass it and give the user the ability to enter a new sentence
    if (fgets(sentence, MAX_SENTENCE_LEN, stdin) == NULL) {
      fprintf(stderr, "Failed To Scan The Sentence\n");
      exit(EXIT_FAILURE);
    }

    // calling the function noSpaceStr() that removes all the spaces from the sentence to check if the sentence is the word "done" (exit)
    // and for more security while making string processing (splitting the polynomial and x value string)
    noSpaceStr(sentence);

    //checking if the word is "done" if true the break the loop and the program will exit
    if (strcmp(sentence, "done") == 0) {
      break;
    }

    // calling the method toCoefficientPowArray() that makes a string processing (preparing the values/work for the threads)
    toCoefficientPowArray(sentence);

    // calling the method calcByThreads() that calculates the evaluation by threads as asked in the task
    int result = calcByThreads();

    printf("%d\n", result);

    printf("---------------------------------------------\n");

  }

  return 0;
}

// This method restarts the Global variables to prepare/clean them for the next loop
void resetGlobalStorage() {

  currCoefficientPowArrayIndex = 0;
  threadsNum = 0;
  xValue = 0;

  int i;

  for (i = 0; i < MAX_POLYNOMIAL_PARTS_NUM; i++) {
    threadsResults[i] = 0;
  }
}

// This method removes all the spaces from the sentence for the check of "done" (exit) and for more security while splitting the entered user sentence
void noSpaceStr(char * sentence) {
  int sentenceLen = strLenNoNewLine(sentence);
  char tempStr[sentenceLen];

  int i, j;
  for (i = 0, j = 0; i <= sentenceLen; i++) {

    if (sentence[i] != ' ') {
      tempStr[j] = sentence[i];
      j++;
    }
  }

  strcpy(sentence, tempStr);
}

// This method calculates the polynomial parts as asked in the task
// by creating a suitable number of threads which each one of the calculates a certain part form the polynomial
// at the end it returns the hole result of the evaluation that the user entered
// which is the sum of all the threads results and the other values that doesn't need a thread (pow zero for x)
int calcByThreads() {

  // creating a pthread_t array to hold the threads ids in order to be able to wait them (pthread_join)
  pthread_t threadIdsArray[threadsNum];
  // this array is for holding for each thread which part of the polynomial to work in
  int threadsWorkIndexes[threadsNum];
  int threadsArrayCurrIndex = 0;

  // Looping through all of the polynomial parts and for each part chek if the power is zero
  // if true then just save the value of the coefficient
  // else make a thread which makes the calculation of the pow
  int i;
  for (i = 0; i < currCoefficientPowArrayIndex; i++) {
    if (coefficientPowArray[i][1] != 0) {
      threadsWorkIndexes[threadsArrayCurrIndex] = i;
      pthread_create( & threadIdsArray[threadsArrayCurrIndex], NULL, threadFunc, & threadsWorkIndexes[threadsArrayCurrIndex]);
      threadsArrayCurrIndex++;
    } else {
      threadsResults[i] = coefficientPowArray[i][0];
    }
  }

  // waiting for the threads that we have made till they finish there jobs using the method pthread_join()
  int j;
  for (j = 0; j < threadsNum; j++) {
    if (pthread_join(threadIdsArray[j], NULL) != 0) {
      perror("pthread join failed");
      exit(EXIT_FAILURE);
    }
  }

  // calculating the end result which is the sum of all the calculated parts values
  int result = 0;
  int k;
  for (k = 0; k < MAX_POLYNOMIAL_PARTS_NUM; k++) {
    result += threadsResults[k];
  }

  return result;
}

// this is the callback function for the threads which receives a pointer for the index
// where the values of the part that this thread has to work on are exists in the Global array coefficientPowArray
void * threadFunc(void * workIndex) {

  int arrayIndex = * (int * ) workIndex;

  threadsResults[arrayIndex] = coefficientPowArray[arrayIndex][0] * powFunc(xValue, coefficientPowArray[arrayIndex][1]);

  // adding the method pthread_exit() here made memory problems some times !!

  return NULL;
}

// a simple function that calculates the positive pow of the giver exponent and powValue
int powFunc(int exponent, int powValue) {

  int result = 1;

  int i;

  for (i = 0; i < powValue; i++) {
    result *= exponent;
  }

  return result;
}

// This method makes the needed string processing on the sentence that the user entered
// it splits the sentence in a certain way so at the end we have the integer values of each polynomial part
// (coefficient and pow) in the coefficientPowArray
void toCoefficientPowArray(char * sentence) {

  // declaring and initializing the needed strings for the splitting process
  char orderedSplitersStrArray[SPLITERS_NUM][SPLITERS_MAX_STR_LEN] = {
    "*x^",
    "*x",
    "x^",
    "x",
    ""
  };
  char commaStr[] = ",";
  char plusStr[] = "+";
  char defOne[] = "1";

  // counting the number of sections according to the comma string (according to the tasks sentence format the number of sections should be 2)
  int sectionsArrayLen = sectionsCount(sentence, commaStr);

  // splitting the sentence according to the comma string
  char ** sectionsArray = toSectionsArray(sentence, commaStr, NULL);

  // converting the value of the sectionsArray[1] which is the string of the x value into integer
  // then we save it in the global variable xValue
  xValue = (int) strtol(sectionsArray[1], (char ** ) NULL, 10);

  // counting the number of sections according to the plus string
  int plusSectionsArrayLen = sectionsCount(sectionsArray[0], plusStr);

  // splitting the sentence according to the plus string
  char ** plusSectionsArray = toSectionsArray(sectionsArray[0], plusStr, NULL);

  // freeing all the manually allocated memory that we don't need anymore
  freeAll(sectionsArray, & sectionsArrayLen);

  int i, j;

  char ** tempSectionsArray;
  int tempSectionsArrayLen;

  // looping through all of the splitting strings and for each polynomial part checking if we can cut it with that splitter
  // if true then we cut that part and we save it's values in the coefficientPowArray after converting it to an integer
  // else go to the next splitter
  // there is one special case which is when the pow is zero (the last splitter) in that case we manually put the value zero in the pow cell
  // in each loop free all the manually allocated memory that we don't need anymore
  for (i = 0; i < SPLITERS_NUM; i++) {
    for (j = 0; j < plusSectionsArrayLen; j++) {
      if (plusSectionsArray[j] != NULL) {
        tempSectionsArrayLen = sectionsCount(plusSectionsArray[j], orderedSplitersStrArray[i]);
        if (tempSectionsArrayLen == 2) {
          tempSectionsArray = toSectionsArray(plusSectionsArray[j], orderedSplitersStrArray[i], defOne);
          coefficientPowArray[currCoefficientPowArrayIndex][0] = (int) strtol(tempSectionsArray[0], (char ** ) NULL, 10);
          coefficientPowArray[currCoefficientPowArrayIndex][1] = (int) strtol(tempSectionsArray[1], (char ** ) NULL, 10);
          currCoefficientPowArrayIndex++;

          freeAll(tempSectionsArray, & tempSectionsArrayLen);
          free(plusSectionsArray[j]);
          plusSectionsArray[j] = NULL;

          threadsNum++;
        } else if (i == (SPLITERS_NUM - 1) && tempSectionsArrayLen == 1) {
          tempSectionsArray = toSectionsArray(plusSectionsArray[j], orderedSplitersStrArray[i], NULL);
          coefficientPowArray[currCoefficientPowArrayIndex][0] = (int) strtol(tempSectionsArray[0], (char ** ) NULL, 10);
          coefficientPowArray[currCoefficientPowArrayIndex][1] = 0;
          currCoefficientPowArrayIndex++;

          freeAll(tempSectionsArray, & tempSectionsArrayLen);
          free(plusSectionsArray[j]);
          plusSectionsArray[j] = NULL;
        }
      }
    }
  }

  // freeing all the manually allocated memory that we don't need anymore
  freeAll(plusSectionsArray, & plusSectionsArrayLen);

}

// This method receives three pointers: 1- for the sentence, 2- for the splitting string, 3- for the default string (to put if there is an empty split)
// the method splits the sentence according to the given splitStr and it puts the value of the given defStr if there is an empty split
// for example if we split the string "x" according to "x" and the value of defStr = "1"
// then the result is the array ["1", "1"] before and after "x" there are an empty split
// it uses the methods sectionsCount() and getSectionStr()
char ** toSectionsArray(char * sentence, char * splitStr, char * defStr) {

  // declaring and initializing the needed variables for the string processing
  // and allocating the needed memories for that
  int sectionsNum = sectionsCount(sentence, splitStr);

  char ** sectionsArray = (char ** ) malloc(sizeof(char * ) * (sectionsNum));
  int sectionsArrayCurrIndex = 0;

  if (sectionsArray == NULL) {
    fprintf(stderr, "malloc failed\n");
    exit(EXIT_FAILURE);
  }

  int splitStrCurrIndex = 0;
  int splitStrLastEndIndex = 0;
  int splitStrLen = strLenNoNewLine(splitStr);

  int sentenceLen = strLenNoNewLine(sentence);

  int isInSplit = FALSE;

  int sectionStartIndex, sectionEndIndex;

  int defStrStartIndex = 0, defStrLastIndex = 0;

  if (defStr != NULL) {
    defStrStartIndex = 0;
    defStrLastIndex = strLenNoNewLine(defStr) - 1;
  }

  int i;
  // going through the hole sentence and splitting it according to the splitStr
  for (i = 0; i < sentenceLen; i++) {
    if (sentence[i] == splitStr[splitStrCurrIndex]) {
      isInSplit = TRUE;
      splitStrCurrIndex++;

      if (splitStrCurrIndex == splitStrLen) {
        isInSplit = FALSE;
        splitStrCurrIndex = 0;

        sectionStartIndex = splitStrLastEndIndex;
        sectionEndIndex = i - splitStrLen;

        splitStrLastEndIndex = i + 1;
        // checking if the split is empty or not
        if (sectionEndIndex >= sectionStartIndex) {
          sectionsArray[sectionsArrayCurrIndex] = getSectionStr(sentence, & sectionStartIndex, & sectionEndIndex);
        } else {
          sectionsArray[sectionsArrayCurrIndex] = getSectionStr(defStr, & defStrStartIndex, & defStrLastIndex);
        }

        sectionsArrayCurrIndex++;
        continue;
      }

    } else if (sentence[i] != splitStr[splitStrCurrIndex] && isInSplit == TRUE) {
      isInSplit = FALSE;
      splitStrCurrIndex = 0;
    }
  }

  // dealing with the special case which is at the end of the sentence that the for loop finish before dealing with
  if (i == splitStrLastEndIndex) {
    sectionsArray[sectionsArrayCurrIndex] = getSectionStr(defStr, & defStrStartIndex, & defStrLastIndex);
  } else {
    sectionStartIndex = splitStrLastEndIndex;
    sectionEndIndex = i - 1;
    sectionsArray[sectionsArrayCurrIndex] = getSectionStr(sentence, & sectionStartIndex, & sectionEndIndex);
  }

  return sectionsArray;
}

// This function receives three pointers: 1- for the sentence, 2- for the first index in the section (according to the indexes of the sentence), 3- for the last index in the section (according to the indexes of the sentence)
// the function manually allocates the needed memory for the word according to the given indexes and it returns a pointer char* for it
char * getSectionStr(const char * sentence,
  const int * sectionStartIndex,
    const int * sectionEndIndex) {
  int i, j;

  int sectionLen = (( * (sectionEndIndex) - * (sectionStartIndex)) + 1);

  // allocating exactly needed memory for the characters of the section + 1 for the '\0' character
  char * currSectionPointer = (char * ) malloc(sizeof(char) * (sectionLen + 1));

  // checking if the memory allocation goes wrong / failed
  // if true i print a message for the user then the program exits with EXIT_FAILURE code (1)
  if (currSectionPointer == NULL) {
    fprintf(stderr, "malloc failed\n");
    exit(EXIT_FAILURE);
  }

  // going through the sentence according to the indexes of the word for copying in to the new allocated memory
  for (i = * (sectionStartIndex), j = 0; i <= * (sectionEndIndex); i++, j++) {
    currSectionPointer[j] = sentence[i];
  }

  // finishing the word by putting the '\0' character at the end of the word
  currSectionPointer[sectionLen] = '\0';

  return currSectionPointer;
}

// This method receives two pointers: 1- for the sentence, 2- for the splitStr
// the method goes through the hole sentence and it counts the number of sections according to the given splitStr
int sectionsCount(char * sentence, char * splitStr) {

  int sectionsCount = 1;

  int splitStrCurrIndex = 0;
  int splitStrLen = strLenNoNewLine(splitStr);

  int sentenceLen = strLenNoNewLine(sentence);

  int isInSplit = FALSE;

  int i;
  // going through the hole sentence and counting the sections number
  for (i = 0; i < sentenceLen; i++) {

    if (sentence[i] == splitStr[splitStrCurrIndex]) {
      isInSplit = TRUE;
      splitStrCurrIndex++;

      if (splitStrCurrIndex == splitStrLen) {
        sectionsCount++;
        isInSplit = FALSE;
        splitStrCurrIndex = 0;
        continue;
      }

    } else if (sentence[i] != splitStr[splitStrCurrIndex] && isInSplit == TRUE) {
      isInSplit = FALSE;
      splitStrCurrIndex = 0;
    }
  }

  return sectionsCount;
}

// This function receives a pointer for the sentence
// and it calculates the length of the sentence by calling the function strlen()
// then it checks it the last character is '\n', if true it changes it to '\0'
// and it decreases the length by one then it returns the length
int strLenNoNewLine(char * sentence) {
  int sentenceLen = (int) strlen(sentence);

  if (sentenceLen != 0 && sentence[sentenceLen - 1] == '\n') {
    sentence[sentenceLen - 1] = '\0';
    sentenceLen = sentenceLen - 1;
  }

  return sentenceLen;
}

// This method receives two pointers: 1- for the arguments array, 2- for the number of the arguments
// it frees all the manually allocated memory by going through each pointer in the array of args pointers
// then it frees the array of pointers it self
void freeAll(char ** sectionsArrayPointer,
  const int * sectionsArrayLen) {

  if (sectionsArrayPointer == NULL || sectionsArrayLen == NULL) {
    return;
  }

  int i;
  for (i = 0; i < ( * sectionsArrayLen); i++) {
    if (sectionsArrayPointer[i] != NULL) {
      free(sectionsArrayPointer[i]);
    }
  }

  free(sectionsArrayPointer);
}