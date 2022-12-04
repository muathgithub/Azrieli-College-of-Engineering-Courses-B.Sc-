// Program Name: Mini Shell Version A
// Program Description: This program is a mini / simple shell that receives commands from the user as a normal shell and executes them if
// they are correct and belong to the commands that it explores their implementation (For example it doesn't support/execute the command "cd" Yet)

// Student name: Muath Abu Jamal


#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <pwd.h>
#include <stdlib.h>
#include <sys/wait.h>

// declaring preprocessor directives
#define MAX_PROMPT_LEN 512
#define MAX_COMMAND_LEN 512
#define MAX_DIR_PATH_LEN 412
#define TRUE 1
#define FALSE 0

// declaring the needed functions (explanation for each function bellow(where each function written))
void getPrompt(char *);
int strLenNoNewLine(char *);
int argsCount(const char *, int *, const int *);
char *getArgStr(const char *, const int *, const int *);
char **toArgsArray(const char *, int *, const int *, const int *);
void printStatistics(const int *, const int *);
void freeAll(char **, const int *);

int main() {
    // declaring the needed variables which i will ues a lot of them as global variables by passing their address to other functions

    // statistics variables
    int commandsNum = 0, totalCommandsLength = 0;
    // a variable to hold the arguments array length which is the number of the arguments/words (int the sentence) + NULL (at the end)
    int argsArrayLen = 0;

    // this variable used as a flag to know if we are in a word or not (in the sentence)
    int isInWord = FALSE;

    // a char array for holding the string of the prompt
    char promptStr[MAX_PROMPT_LEN];

    // a char array for holding the string of the user command
    char commandStr[MAX_COMMAND_LEN];
    // a double char pointer for pointing on the first address of the command arguments pointers (As Array of char*)
    char **commandArgs;

    // pid_t for holding the son pid (in father's process) and zero (in son's process) in order to differentiate between them
    pid_t pid;

    // an infinite loop that end when the user enters the word "done"
    for (;;) {
        // calling the method getPrompt() which will fill the array promptStr with the suitable prompt according to the current/active user and directory
        getPrompt(promptStr);

        // printing the prompt for the user
        printf("%s", promptStr);

        // scanning / receiving  the sentence from the user, if there is an error we print proper message for the user and we pass it and give the user the ability to enter a new sentence
        if (fgets(commandStr, MAX_COMMAND_LEN, stdin) == NULL) {
            fprintf(stderr,"Failed To Scan The Command\n");
            exit(EXIT_FAILURE);
        }

        // increasing the commands counter by one (statistics)
        commandsNum++;

        // calculating the length of the sentence after removing the '\n' character if it exists
        // by calling the function strLenNoNewLine()
        // this variable used as global variable by passing it's address to the other functions
        int commandStrLen = strLenNoNewLine(commandStr);

        // adding the commandStrLen to the totalCommandsLength (statistics)
        totalCommandsLength = totalCommandsLength + commandStrLen;

        // calculating the number of the arguments/words (int the sentence) + NULL (at the end)
        argsArrayLen = argsCount(commandStr, &isInWord, &commandStrLen) + 1;

        // checking if the command is an empty command ['\n' OR SPACES]
        // if true then continue to the next scan
        if(argsArrayLen == 1){
            continue;
        }

        // converting the sentence to array of arguments for the use of execvp()
        commandArgs = toArgsArray(commandStr, &isInWord, &commandStrLen, &argsArrayLen);

        // checking if the entered sentence is just the word "done" which is the word of exit/ending the program
        // if true we call printStatistics() to print the statistics then we free all the manual allocated memories then we break the loop
        if (argsArrayLen == 2 && strcmp(commandArgs[0], "done") == 0) {
            printStatistics(&commandsNum, &totalCommandsLength);
            freeAll(commandArgs, &argsArrayLen);
            break;
        }
            // checking if the entered sentence is the command "cd" which is not supported (Yet)
            // if true it prints a proper message and free all the manual allocated memories and continuing the for loop to scan again
        else if (argsArrayLen >= 2 && strcmp(commandArgs[0], "cd") == 0) {
            printf("command not supported (Yet)\n");
            freeAll(commandArgs, &argsArrayLen);
            continue;
        }

        // forking a new process to execute the entered user command
        pid = fork();

        // checking if the forking failed
        // if true we print a proper message, free the manual allocated memory end exit/end the program
        if (pid < 0) {
            perror("fork failed: ");
            freeAll(commandArgs, &argsArrayLen);
            exit(EXIT_FAILURE);
        }
            // checking if we are in the son process
            // if true we will call the function printSched() which will fork a new process to execute the command
            // cat on the sched file of the son process and it will wait till the process of cat finishes it's job
            // then it continue to execute the given user command
        else if (pid == 0) {

            int status = execvp(commandArgs[0], commandArgs);
            // checking if the execvp() failed
            // if true print a proper message, free all the manual allocated memories and exit/end the program
            if (status == -1) {
                perror("Command Not Found: ");
                freeAll(commandArgs, &argsArrayLen);
                exit(EXIT_FAILURE);
            }

        }
            // this else means that the process is the father(first/main process)
            // if true it will wait till his son finishes then it will continue to the next lines
        else
        {
            wait(NULL);
        }

        //freeing all the manual allocated memories
        freeAll(commandArgs, &argsArrayLen);
    }

    return 0;
}

// This function receives four pointers: 1- for the sentence, 2- for the bool isInWord (Used As Global),
// 3- for the sentenceLen (Used As Global), 4- for the length of the arguments array
// the function finds the start and end indexes for each word in the sentence and it sends
// them each time to the function getArgStr() that allocates the needed memory for each argument/word
// and it fills it in that memory then it returns a pointer for it
char **toArgsArray(const char *sentence, int *isInWord, const int *sentenceLen, const int *argsArrayLen) {
    // manually allocating memories for the args pointers
    char **argsArray = (char **) malloc(sizeof(char *) * (*argsArrayLen));

    // checking if the memory allocation failed
    // if true print a proper message and exit/end the program
    if(argsArray == NULL){
        fprintf(stderr, "malloc failed\n");
        exit(EXIT_FAILURE);
    }

    int wordStartInd = 0, wordEndInd = 0;
    // resetting the variable/flag isInWord to the new check (because other function can use it as a global variable)
    *isInWord = FALSE;

    int i;
    // a pointer to the next pointer memory
    int argsArrCurrIndex = 0;
    // this for loop goes through the sentence and for each word it saves the start and end character indexes
    // after reaching the end index for each word it calls the function getArgStr() that allocates the needed memory for each argument/word
    // and it fills it in that memory then it returns a pointer for it which the argsArray will hold in the proper index (argsArrCurrIndex)
    for (i = 0; i < *(sentenceLen); i++) {
        if (!(*(isInWord)) && (sentence[i] != ' ')) {
            *isInWord = TRUE;
            wordStartInd = i;
        } else if (*(isInWord) && sentence[i] == ' ') {
            *isInWord = FALSE;
            wordEndInd = i - 1;
            argsArray[argsArrCurrIndex] = getArgStr(sentence, &wordStartInd, &wordEndInd);
            argsArrCurrIndex++;
        }
    }

    // checking the special case of finishing the for loop while we are still in a word that will not have a pointer
    // so here if we are still in word we make a pointer for it as before
    if (*(isInWord)) {
        wordEndInd = i - 1;
        argsArray[argsArrCurrIndex] = getArgStr(sentence, &wordStartInd, &wordEndInd);
        argsArrCurrIndex++;
    }

    // making the last pointer NULL for the use of execvp()
    argsArray[argsArrCurrIndex] = NULL;

    // returning the arguments array
    return argsArray;
}

// This function receives three pointers: 1- for the sentence, 2- for the first index in the word (according to the indexes of the sentence), 3- for the last index in the word (according to the indexes of the sentence)
// the function manually allocates the needed memory for the word according to the given indexes and it returns a pointer char* for it
char *getArgStr(const char *sentence, const int *wordStartInd, const int *wordEndInd) {
    int i, j;

    int wordLen = ((*(wordEndInd) - *(wordStartInd)) + 1);

    // allocating exactly needed memory for the characters of the word + 1 for the '\0' character
    char *currWordPointer = (char *) malloc(sizeof(char) * (wordLen + 1));

    // checking if the memory allocation goes wrong / failed
    // if true i print a message for the user then the program exits with EXIT_FAILURE code (1)
    if (currWordPointer == NULL) {
        fprintf(stderr, "malloc failed\n");
        exit(EXIT_FAILURE);
    }


    // going through the sentence according to the indexes of the word for copying in to the new allocated memory
    for (i = *(wordStartInd), j = 0; i <= *(wordEndInd); i++, j++) {
        currWordPointer[j] = sentence[i];
    }

    // finishing the word by putting the '\0' character at the end of the word
    currWordPointer[wordLen] = '\0';

    return currWordPointer;
}

// This function receives three pointers: 1- for the sentence, 2- for the bool isInWord (Used As Global), 3- for the sentenceLen (Used As Global)
// the function goes through the sentence and it counts the number of words in it using the variable isInWord
// which considered as a flag to know if we are in a word or not (in the sentence)
// at the end the function returns a value of type int which is the number of words in the sentence
int argsCount(const char *sentence, int *isInWord, const int *sentenceLen) {
    int argsCounter = 0;
    // resetting the variable/flag isInWord to the new check (because other function can use it as a global variable)
    *isInWord = FALSE;

    int i;
    // this for that goes through the hole sentence and each time it finds character while
    // the variable isInWord is false it increases the argsCounter by one
    for (i = 0; i < *(sentenceLen); i++) {
        if (!(*(isInWord)) && (sentence[i] != ' ')) {
            *isInWord = TRUE;
            argsCounter = argsCounter + 1;
        } else if (*(isInWord) && sentence[i] == ' ') {
            *isInWord = FALSE;
        }
    }

    return argsCounter;
}

// This function receives a pointer for the sentence
// and it calculates the length of the sentence by calling the function strlen()
// then it checks it the last character is '\n', if true it changes it to '\0'
// and it decreases the length by one then it returns the length
int strLenNoNewLine(char *sentence) {
    int sentenceLen = (int) strlen(sentence);

    if (sentence[sentenceLen - 1] == '\n') {
        sentence[sentenceLen - 1] = '\0';
        sentenceLen = sentenceLen - 1;
    }

    return sentenceLen;
}

// This function receives a pointer for the promptStr (Used As Global)
// and it fill the array promptStr with the suitable prompt according to the current/active user and directory
// by calling the two functions:
// 1- getpwuid() which needs to pass the id of this process for it and that function returns a pointer to a struct which
// can holds the information that we need (Username) and other unneeded information in our case
// 2- getcwd() which needs a buffer and it's maximum size and it fills the buffer with the current directory path if it succeeded else it returns NULL
void getPrompt(char *promptStr) {

    struct passwd *structPasswdPointer;
    char currDirPath[MAX_DIR_PATH_LEN];

    structPasswdPointer = getpwuid(getuid());

    // checking if the pointer equals to NULL which means that the function getpwuid() failed
    // or if the char* pw_name == NULL which means that the struct created successfully but it failed to get the Username
    // if true it substitutes null in the username place
    if (structPasswdPointer == NULL || structPasswdPointer->pw_name == NULL) {
        strcpy(promptStr, "null@");
    }
        // else substitute the username from the struct by using strcpy() and strcat() methods to get the desired format
    else
    {
        strcpy(promptStr, structPasswdPointer->pw_name);
        strcat(promptStr, "@");
    }

    // this if calls the function getcwd to get the current directory in currDirPath and checks if the operation failed
    // if true it substitutes null in the current directory place
    if (getcwd(currDirPath, (size_t) MAX_DIR_PATH_LEN) == NULL) {
        strcat(promptStr, "null>");
    }
        // else substitute the directoryPath from the array currDirPath
    else
    {
        strcat(promptStr, currDirPath);
        strcat(promptStr, ">");
    }

}

// this function receives two pointers: 1- commandsNum for the commands number, 2- totalCommandsLength for the total commands length
// and it calculates then prints the needed statistics in a formatted way with "See you Next time !" message
void printStatistics(const int *commandsNum, const int *totalCommandsLength) {
    float averageCommandsLength = ((float) (*totalCommandsLength)) / ((float) (*commandsNum));

    printf("Num of commands: %d\nTotal length of all commands: %d\nAverage length of all commands: %f\nSee you Next time !\n",
           *commandsNum, *totalCommandsLength, averageCommandsLength);
}

// This method receives two pointers: 1- for the arguments array, 2- for the number of the arguments
// it frees all the manually allocated memory by going through each pointer in the array of args pointers
// then it frees the array of pointers it self
void freeAll(char **argsArrayPointer, const int *argsArrayLen) {

    int i;
    for (i = 0; i < (*argsArrayLen); i++) {
        free(argsArrayPointer[i]);
    }

    free(argsArrayPointer);
}