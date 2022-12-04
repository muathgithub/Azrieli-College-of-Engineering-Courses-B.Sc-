// Program Name: Mini Shell That Support Two Pipes, ctrl + z and fg
// Program Description: This program is a mini / simple shell that receives commands from the user as a normal shell and executes them if
// they are correct and belong to the commands that it explores their implementation (For example it doesn't support/execute the command "cd" Yet)
// in addition it supports piping till two pipes max, ctrl + z and fg

// Student name: Muath Abu Jamal

#include <stdio.h>

#include <string.h>

#include <unistd.h>

#include <pwd.h>

#include <stdlib.h>

#include <sys/wait.h>

#include <signal.h>

// declaring preprocessor directives
#define MAX_PROMPT_LEN 512
#define MAX_COMMAND_LEN 512
#define MAX_DIR_PATH_LEN 412
#define TRUE 1
#define FALSE 0
#define SPACE_CHAR ' '
#define PIPE_CHAR '|'
#define TAG_CHAR '"'
#define MAX_PIPES_NUM 2
#define CLOSED - 1
#define MAX_SONS_NUM 3
#define EOS - 3

// declaring the needed functions (explanation for each function bellow(where each function written))
void getPrompt(char * );

int strLenNoNewLine(char * );

int sectionsCount(char * , char);

char * getSectionedStr(const char * ,
  const int * ,
    const int * );

char ** toSectionsArray(char * , char);

void executeSection(char ** , int, int, int * , int * );

int pipesCount(char * );

void pipeConnection(char ** , int, int, int * , int * );

void pipeClose(int * , char ** , int * );

void printStatistics(const int * ,
  const int * );

void freeAll(char ** ,
  const int * );

void resetSonsPIDArray();

void sonsWait();

int sonsPIDArray[MAX_SONS_NUM];
int sonsPIDArrayCurrIndex;
int lastStoppedPID = EOS;

int main() {

  // statistics variables
  int commandsNum = 0, totalPipesNum = 0;
  // a variable to hold the arguments array length which is the number of the arguments/words (int the sentence) + NULL (at the end)
  int argsArrayLen;

  // two ints one to hold the number of the current sentence sections and the other to hold the number of pipes in the current sentence
  int sectionsNum;
  int currPipesNum;

  // a char array for holding the string of the prompt
  char promptStr[MAX_PROMPT_LEN];

  // a char array for holding the string of the user command
  char commandStr[MAX_COMMAND_LEN];

  // a double char pointer for pointing on the first address of the command arguments pointers (As Array of char*)
  char ** commandArgs;
  // a double char pointer for pointing to the first address of the sections strings(char*)
  char ** sectionsArray;

  // two int arrays to hold the FDs for the pipes
  int pipeOne[MAX_PIPES_NUM];
  pipeOne[0] = pipeOne[1] = CLOSED;
  int pipeTwo[MAX_PIPES_NUM];
  pipeTwo[0] = pipeTwo[1] = CLOSED;

  // Blocking/Ignoring the SIG_STP for the father process
  signal(SIGTSTP, SIG_IGN);

  // an infinite loop that end when the user enters the word "done"
  for (;;) {
    //Resetting the array of sons and it's curr index
    resetSonsPIDArray();

    // calling the method getPrompt() which will fill the array promptStr with the suitable prompt according to the current/active user and directory
    getPrompt(promptStr);

    // printing the prompt for the user
    printf("%s", promptStr);

    // scanning / receiving  the sentence from the user, if there is an error we print proper message for the user and we pass it and give the user the ability to enter a new sentence
    if (fgets(commandStr, MAX_COMMAND_LEN, stdin) == NULL) {
      fprintf(stderr, "Failed To Scan The Command\n");
      exit(EXIT_FAILURE);
    }

    if (strcmp(commandStr, "\n") == 0) {
      // checking if the command is an empty command ['\n']
      continue;
    }

    // checking if the number of pipes is illegal ( > 2 )
    // if true go to the next loop
    currPipesNum = pipesCount(commandStr);
    if (currPipesNum > MAX_PIPES_NUM) {
      fprintf(stderr, "** More Than Two Pipes **\n");
      continue;
    }

    // adding the legal number of pipes to the statistics variable which is totalPipesNum
    totalPipesNum = totalPipesNum + currPipesNum;

    // calculating the number of sections in the sentence
    sectionsNum = sectionsCount(commandStr, PIPE_CHAR);

    // checking if the number of sections in one
    // if true it means that there is an opportunity that the command is the exiting command which is the word "done"
    if (sectionsNum == 1) {
      // calculating the number of the arguments/words (int the sentence) + 1 for NULL for the use after ==> if the command is not the word "done"
      argsArrayLen = sectionsCount(commandStr, SPACE_CHAR) + 1;

      // checking if the command is an empty command ['\n' OR SPACES]
      // if true then continue to the next scan
      if (argsArrayLen == 1) {
        continue;
      }

      // converting the sentence to array of arguments for the check of the special cases ["done", "fg"]
      commandArgs = toSectionsArray(commandStr, SPACE_CHAR);

      // checking if the entered sentence is just the word "done" which is the word of exit/ending the program
      // or is just the word "fg" which means to bring the last stopped process to the foreground
      if (argsArrayLen == 2) {

        // in the case of "done"
        // we call printStatistics() to print the statistics then we free all the manual allocated memories then we break the loop
        if (strcmp(commandArgs[0], "done") == 0) {
          printStatistics( & commandsNum, & totalPipesNum);
          // freeing all the manually allocated memories
          freeAll(commandArgs, & argsArrayLen);
          break;
        }

        // in the case of "fg"
        // we check if there is a stopped process if true then we will send a SIGCONT for that process using it's saved pid
        // after that we add that id to the array of sons pid and we increase the value of the sonsPIDArrayCurrIndex by one
        // then we call sonsWait() to wait again for that process
        if (strcmp(commandArgs[0], "fg") == 0) {

          // increasing the commands counter by one (statistics)
          commandsNum++;

          if (lastStoppedPID == EOS) {
            fprintf(stderr, "NO JOB HAVE BEEN STOPPED\n");
            freeAll(commandArgs, & argsArrayLen);
            continue;
          }

          if (kill(lastStoppedPID, SIGCONT) == -1) {
            perror("fg failed: ");
            //freeing all the manual allocated memories
            freeAll(commandArgs, & argsArrayLen);
            exit(EXIT_FAILURE);
          }

          // freeing all the manually allocated memories
          freeAll(commandArgs, & argsArrayLen);

          sonsPIDArray[sonsPIDArrayCurrIndex] = lastStoppedPID;
          sonsPIDArrayCurrIndex = sonsPIDArrayCurrIndex + 1;

          sonsWait();

          continue;
        }

      }

      // freeing all the manually allocated memories
      freeAll(commandArgs, & argsArrayLen);
    }

    // increasing the commands counter by one (statistics)
    commandsNum++;

    // converting the sentence to array of sections to apply pipes between them
    sectionsArray = toSectionsArray(commandStr, PIPE_CHAR);

    // creating new two pipes if the previous ones were closed to connect between the new sons
    // and checking if one of them failed to be created
    if ((pipeOne[0] == CLOSED && pipe(pipeOne) == -1) || (pipeTwo[0] == CLOSED && pipe(pipeTwo) == -1)) {
      perror("Pipe Failed: ");
      // freeing all the manually allocated memories
      freeAll(sectionsArray, & sectionsNum);
      exit(EXIT_FAILURE);
    }

    int section;
    // a for loop that runs sectionsNum times to create the sons and pipe between them
    for (section = 0; section < sectionsNum; section++) {
      executeSection(sectionsArray, sectionsNum, section, pipeOne, pipeTwo);
      switch (section) {
      case 1:
        // closing the first pipe in the father's process to remain just with two processes connecting to the pipe
        pipeClose(pipeOne, sectionsArray, & sectionsNum);
        break;
      case 2:

        // closing the second pipe in the father's process to remain just with two processes connecting to the pipe
        pipeClose(pipeTwo, sectionsArray, & sectionsNum);
        break;
      default:
        break;
      }
    }

    sonsWait();

    //freeing all the manual allocated memories
    freeAll(sectionsArray, & sectionsNum);
  }

  return 0;
}

//This function waits for all the sons in the array sonsPIDArray
//using the method waitpid with the flag WUNTRACED which means that the wait will be broken
// in two cases:
// 1-) if the son terminated
// 2-) if the son stopped [WUNTRACED]
void sonsWait() {
  int i = 0;
  int sonStatus;

  while (sonsPIDArray[i] != EOS && i < sonsPIDArrayCurrIndex) {

    waitpid(sonsPIDArray[i], & sonStatus, WUNTRACED);

    // checking if the status of the son is stopped
    // if true we save the pid of that son which considered the last process that have been stopped
    if (WIFSTOPPED(sonStatus)) {
      printf("\n");
      lastStoppedPID = sonsPIDArray[i];
    }

    i = i + 1;
  }
}

// This function receives five parameters: 1- pointer for the sections array to use and free in failure
// 2- the len of the sections array, 3- the curr section number to execute, (4,5)- pointers to the pipes
// the function connects the pipes if there is a need form them and and it executes the sons precesses
void executeSection(char ** sectionsArray, int sectionsArrayLen, int sectionNum, int * pipeOne, int * pipeTwo) {

  //printf("%s\n", sectionsArray[0]);

  // pid_t for holding the son pid (in father's process) and zero (in son's process) in order to differentiate between them
  pid_t pid;

  // forking a new process to execute the entered user command
  pid = fork();

  // checking if the forking failed
  // if true we print a proper message, free the manual allocated memory end exit/end the program
  if (pid < 0) {
    perror("fork failed: ");
    //freeing all the manual allocated memories
    freeAll(sectionsArray, & sectionsArrayLen);
    exit(EXIT_FAILURE);
  }

  // if you are the father then return to create the next son or to wait for you sons then to continue
  if (pid > 0) {
    sonsPIDArray[sonsPIDArrayCurrIndex] = pid;
    sonsPIDArrayCurrIndex = sonsPIDArrayCurrIndex + 1;
    return;
  }

  //UnBlocking the SIGTSTP for the son process
  signal(SIGTSTP, SIG_DFL);

  // closing the unneeded pipes for the current process pipes
  switch (sectionsArrayLen) {
  case 1:
    pipeClose(pipeOne, NULL, NULL);
    pipeClose(pipeTwo, NULL, NULL);
    break;
  case 2:
    pipeClose(pipeTwo, NULL, NULL);
    break;
  case 3:
    if (sectionNum == 0) {
      pipeClose(pipeTwo, NULL, NULL);
    } else if (sectionNum == 2) {
      pipeClose(pipeOne, NULL, NULL);
    }
    break;
  default:
    fprintf(stderr, "Illegal Number Of Pipes\n");
    exit(EXIT_FAILURE);
  }

  // checking if there is just one section which means that there is no need for pipes
  // ( just one process ) we dont enter to the if block
  if (sectionsArrayLen > 1) {
    pipeConnection(sectionsArray, sectionsArrayLen, sectionNum, pipeOne, pipeTwo);
  }

  // a double char pointer for pointing on the first address of the command arguments pointers (As Array of char*)
  char ** commandArgs;

  // calculating the number of the arguments/words (int the sentence) + NULL (at the end)
  int argsArrayLen = sectionsCount(sectionsArray[sectionNum], SPACE_CHAR) + 1;

  // converting the sentence/section to array of arguments for the use of execvp()
  commandArgs = toSectionsArray(sectionsArray[sectionNum], SPACE_CHAR);

  int status = 0;
  // checking if the entered sentence is not the command "cd" which is not supported (Yet)
  // if true then execute the command
  if (strcmp(commandArgs[0], "cd") != 0) {
    status = execvp(commandArgs[0], commandArgs);
  }

  // checking if the execvp() failed
  // if true print a proper message, free all the manual allocated memories and exit/end the program
  if (status == -1) {
    perror("Command Not Found: ");
  }
  // this else means that the command is "cd" which is not supported (Yet)
  else {
    printf("command not supported (Yet)\n");
  }

  // for both cases "command not found" and "cd"
  // we need to make: (the below code):

  //freeing all the manual allocated memories
  freeAll(sectionsArray, & sectionsArrayLen);
  freeAll(commandArgs, & argsArrayLen);

  // closing the pipes
  pipeClose(pipeOne, NULL, NULL);
  pipeClose(pipeTwo, NULL, NULL);

  exit(EXIT_FAILURE);
}

// This function receives three pointers: 1- for the pipe FDs, 2- for the sectionsArray, 3- for the sectionsNum( array Len )
// to close the pipe and to free the memories if there is a failure
void pipeClose(int * pipe, char ** sectionsStrings, int * sectionsNum) {
  if (pipe != NULL && ((pipe[0] != CLOSED && close(pipe[0]) == -1) || (pipe[1] != CLOSED && close(pipe[1]) == -1))) {
    perror("Pipe Closing Failed ");
    //freeing all the manual allocated memories
    freeAll(sectionsStrings, sectionsNum);
    exit(EXIT_FAILURE);
  }

  pipe[0] = pipe[1] = CLOSED;
}

// This function receives five parameters: 1- pointer for the sections array to use and free in failure
// 2- the len of the sections array, 3- the curr section number to execute, (4,5)- pointers to the pipes
// the function connect the sons with the suitable pipes according to there number and free the manually allocated memories in the case of failure
void pipeConnection(char ** sectionsArray, int sectionsArrayLen, int sectionNum, int * pipeOne, int * pipeTwo) {
  int failed = FALSE;
  switch (sectionNum) {
    // if the son is the first son then
    // sons OUTPUT ==> pipeOne INPUT
    // closing the pipeOne OUTPUT
  case 0:
    if (close(pipeOne[0]) == -1) {
      perror("Pipe Closing Failed: ");
      failed = TRUE;
    } else {
      pipeOne[0] = CLOSED;
    }
    if (dup2(pipeOne[1], STDOUT_FILENO) == -1) {
      perror("Dup2 Failed: ");
      failed = TRUE;
    }
    break;
  case 1:
    // if the son is the second son then
    // sons INPUT ==> pipeOne OUTPUT
    // closing the pipeOne INPUT
    if (close(pipeOne[1]) == -1) {
      perror("Pipe Closing Failed: ");
      failed = TRUE;
    } else {
      pipeOne[1] = CLOSED;
    }
    if (dup2(pipeOne[0], STDIN_FILENO) == -1) {
      perror("Dup2 Failed: ");
      failed = TRUE;
    }
    // sons OUTPUT ==> pipeTwo INPUT [ if pipe Two != NULL]
    // closing the pipeTwo OUTPUT
    if (sectionsArrayLen == 3) {
      if (close(pipeTwo[0]) == -1) {
        perror("Pipe Closing Failed: ");
        failed = TRUE;
      } else {
        pipeTwo[0] = CLOSED;
      }
      if (dup2(pipeTwo[1], STDOUT_FILENO) == -1) {
        perror("Dup2 Failed: ");
        failed = TRUE;
      }
    }
    break;
  case 2:
    // if the son is the third son then
    // sons INPUT ==> pipeTwo OUTPUT
    // closing the pipeTwo INPUT
    if (close(pipeTwo[1]) == -1) {
      perror("Pipe Closing Failed: ");
      failed = TRUE;
    } else {
      pipeTwo[1] = CLOSED;
    }
    if (dup2(pipeTwo[0], STDIN_FILENO) == -1) {
      perror("Dup2 Failed: ");
      failed = TRUE;
    }
    break;

  default:
    fprintf(stderr, "INVALID PIPE NUM");
    failed = TRUE;
    break;
  }

  if (failed) {
    // free all the manually allocated memory
    freeAll(sectionsArray, & sectionsArrayLen);
    exit(EXIT_FAILURE);
  }
}

// This function receives one pointer to a string which is the sentence
// it counts the number of pipes in the sentence without the pipes which appear between a tags ( " )
int pipesCount(char * sentence) {
  int isInTag = FALSE;
  int pipesCounter = 0;

  int sentenceLen = strLenNoNewLine(sentence);

  int i;
  for (i = 0; i < sentenceLen; i++) {
    if (isInTag == FALSE && sentence[i] == TAG_CHAR) {
      isInTag = TRUE;
      continue;
    }

    if (isInTag == TRUE && sentence[i] == TAG_CHAR) {
      isInTag = FALSE;
      continue;
    }

    if (isInTag == TRUE) {
      continue;
    }

    if (sentence[i] == PIPE_CHAR) {
      pipesCounter = pipesCounter + 1;
    }
  }

  return pipesCounter;
}

// This function resets the sons array pid by substituting the value EOS (End Of Sons)
// in all the cells of the array sonsPIDArray and it substitutes the value 0 in the sonsPIDArrayCurrIndex
void resetSonsPIDArray() {
  int i;

  for (i = 0; i < MAX_SONS_NUM; i++) {
    sonsPIDArray[i] = EOS;
  }

  sonsPIDArrayCurrIndex = 0;
}

// This function receives two parameters: 1- a pointer for the sentence, 2- the dividing char
// the function finds the start and end indexes for each section in the sentence and it sends
// them each time to the function getSectionedStr() that allocates the needed memory for each argument/word
// and it fills it in that memory then it returns a pointer for it
char ** toSectionsArray(char * sentence, char splitChar) {
  // this variable used as a flag to know if we are in a word or not (in the sentence)
  int isInWord = FALSE;

  // this variable used as a flag to know if we are in a tag ( " ) or not (in the sentence)
  int isInTag = FALSE;

  // calculating the length of the sentence after removing the '\n' character if it exists
  // by calling the function strLenNoNewLine()
  int sentenceLen = strLenNoNewLine(sentence);

  int sectionsArrayLen;

  if (splitChar == SPACE_CHAR) {
    // calculating the number of the arguments/words (int the sentence) + NULL (at the end)
    sectionsArrayLen = sectionsCount(sentence, SPACE_CHAR) + 1;
  } else if (splitChar == PIPE_CHAR) {
    sectionsArrayLen = sectionsCount(sentence, PIPE_CHAR);
  } else {
    fprintf(stderr, "WRONG DIVIDING CHAR");
    exit(EXIT_FAILURE);
  }

  // manually allocating memories for the args pointers
  char ** sectionsArray = (char ** ) malloc(sizeof(char * ) * (sectionsArrayLen));

  // checking if the memory allocation failed
  // if true print a proper message and exit/end the program
  if (sectionsArray == NULL) {
    fprintf(stderr, "malloc failed\n");
    exit(EXIT_FAILURE);
  }

  int wordStartInd = 0, wordEndInd = 0;

  int i;
  // a pointer to the next pointer memory
  int argsArrCurrIndex = 0;
  // this for loop goes through the sentence and for each section it saves the start and end character indexes
  // after reaching the end index for each word it calls the function getSectionedStr() that allocates the needed memory for each argument/word
  // and it fills it in that memory then it returns a pointer for it which the sectionsArray will hold in the proper index (argsArrCurrIndex)
  // the start and end tags not counted in SPACE_CHAR cases
  for (i = 0; i < sentenceLen; i++) {

    // checking if we are in a tag and still we didn't reach the last tag (closing tag)
    if (isInTag && sentence[i] != TAG_CHAR) {
      continue;
    }
    // checking if we are in a tag at we reached a closing tag
    else if (isInTag && sentence[i] == TAG_CHAR) {
      isInTag = FALSE;
      continue;
    }

    if (!(isInWord) && (sentence[i] != splitChar)) {
      isInWord = TRUE;
      if (splitChar == SPACE_CHAR && sentence[i] == TAG_CHAR) {
        wordStartInd = i + 1;
      } else {
        wordStartInd = i;
      }
    } else if (isInWord && sentence[i] == splitChar) {
      isInWord = FALSE;
      if (splitChar == SPACE_CHAR && sentence[i - 1] == TAG_CHAR) {
        wordEndInd = i - 2;
      } else {
        wordEndInd = i - 1;
      }
      sectionsArray[argsArrCurrIndex] = getSectionedStr(sentence, & wordStartInd, & wordEndInd);
      argsArrCurrIndex++;
    }

    if (!(isInTag) && sentence[i] == TAG_CHAR) {
      isInTag = TRUE;
    }
  }

  // checking the special case of finishing the for loop while we are still in a section that will not have a pointer
  // so here if we are still in section we make a pointer for it as before
  // if the sentence finishes with tag we don't take it
  if (isInWord) {
    if (splitChar == SPACE_CHAR && sentence[i - 1] == TAG_CHAR) {
      wordEndInd = i - 2;
    } else {
      wordEndInd = i - 1;
    }
    sectionsArray[argsArrCurrIndex] = getSectionedStr(sentence, & wordStartInd, & wordEndInd);
    argsArrCurrIndex++;
  }

  // checking if the spilt is for arguments array for the execvp() function
  // if true make the last pointer NULL for the use of execvp()
  if (splitChar == SPACE_CHAR) {
    sectionsArray[argsArrCurrIndex] = NULL;
  }

  // returning the arguments array
  return sectionsArray;
}

// This function receives three pointers: 1- for the sentence, 2- for the first index in the section (according to the indexes of the sentence), 3- for the last index in the section (according to the indexes of the sentence)
// the function manually allocates the needed memory for the word according to the given indexes and it returns a pointer char* for it
char * getSectionedStr(const char * sentence,
  const int * wordStartInd,
    const int * wordEndInd) {
  int i, j;

  int wordLen = (( * (wordEndInd) - * (wordStartInd)) + 1);

  // allocating exactly needed memory for the characters of the word + 1 for the '\0' character
  char * currWordPointer = (char * ) malloc(sizeof(char) * (wordLen + 1));

  // checking if the memory allocation goes wrong / failed
  // if true i print a message for the user then the program exits with EXIT_FAILURE code (1)
  if (currWordPointer == NULL) {
    fprintf(stderr, "malloc failed\n");
    exit(EXIT_FAILURE);
  }

  // going through the sentence according to the indexes of the word for copying in to the new allocated memory
  for (i = * (wordStartInd), j = 0; i <= * (wordEndInd); i++, j++) {
    currWordPointer[j] = sentence[i];
  }

  // finishing the word by putting the '\0' character at the end of the word
  currWordPointer[wordLen] = '\0';

  return currWordPointer;
}

// This function receives two parameters: 1- a pointer for the sentence, 2- the dividing char
// the function goes through the sentence and it counts the number of sections in it using the variable isInWord
// which considered as a flag to know if we are in a section or not
// at the end the function returns a value of type int which is the number of sections in the sentence
int sectionsCount(char * sentence, char splitChar) {
  // this variable used as a flag to know if we are in a word or not (in the sentence)
  int isInWord = FALSE;

  // this variable used as a flag to know if we are in a tag ( " ) or not (in the sentence)
  int isInTag = FALSE;

  // calculating the length of the sentence after removing the '\n' character if it exists
  // by calling the function strLenNoNewLine()
  int sentenceLen = strLenNoNewLine(sentence);

  int sectionsCounter = 0;

  int i;
  // this for that goes through the hole sentence and each time it finds character while
  // the variable isInWord is false it increases the sectionsCounter by one
  // all what between the tags not processed (will not be counted)
  for (i = 0; i < sentenceLen; i++) {
    // checking if we are in a tag and still we didn't reach the last tag (closing tag)
    if (isInTag && sentence[i] != TAG_CHAR) {
      continue;
    }
    // checking if we are in a tag at we reached a closing tag
    else if (isInTag && sentence[i] == TAG_CHAR) {
      isInTag = FALSE;
      continue;
    }

    if (!(isInWord) && (sentence[i] != splitChar)) {
      isInWord = TRUE;
      sectionsCounter = sectionsCounter + 1;
    } else if (isInWord && sentence[i] == splitChar) {
      isInWord = FALSE;
    }

    if (!(isInTag) && sentence[i] == TAG_CHAR) {
      isInTag = TRUE;
    }
  }

  return sectionsCounter;
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

// This function receives a pointer for the promptStr (Used As Global)
// and it fill the array promptStr with the suitable prompt according to the current/active user and directory
// by calling the two functions:
// 1- getpwuid() which needs to pass the id of this process for it and that function returns a pointer to a struct which
// can holds the information that we need (Username) and other unneeded information in our case
// 2- getcwd() which needs a buffer and it's maximum size and it fills the buffer with the current directory path if it succeeded else it returns NULL
void getPrompt(char * promptStr) {

  struct passwd * structPasswdPointer;
  char currDirPath[MAX_DIR_PATH_LEN];

  structPasswdPointer = getpwuid(getuid());

  // checking if the pointer equals to NULL which means that the function getpwuid() failed
  // or if the char* pw_name == NULL which means that the struct created successfully but it failed to get the Username
  // if true it substitutes null in the username place
  if (structPasswdPointer == NULL || structPasswdPointer -> pw_name == NULL) {
    strcpy(promptStr, "null@");
  }
  // else substitute the username from the struct by using strcpy() and strcat() methods to get the desired format
  else {
    strcpy(promptStr, structPasswdPointer -> pw_name);
    strcat(promptStr, "@");
  }

  // this if calls the function getcwd to get the current directory in currDirPath and checks if the operation failed
  // if true it substitutes null in the current directory place
  if (getcwd(currDirPath, (size_t) MAX_DIR_PATH_LEN) == NULL) {
    strcat(promptStr, "null>");
  }
  // else substitute the directoryPath from the array currDirPath
  else {
    strcat(promptStr, currDirPath);
    strcat(promptStr, ">");
  }

}

// this function receives two pointers: 1- commandsNum for the commands number, 2- totalCommandsLength for the total commands length
// and it calculates then prints the needed statistics in a formatted way with "See you Next time !" message
void printStatistics(const int * commandsNum,
  const int * totalPipesNum) {

  printf("Num of commands: %d\nNumber of pipes: %d\nSee you Next time !\n", * commandsNum, * totalPipesNum);
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
    free(sectionsArrayPointer[i]);
  }

  free(sectionsArrayPointer);
}