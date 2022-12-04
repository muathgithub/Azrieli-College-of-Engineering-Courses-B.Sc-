// Program Name: HTTP Proxy - Phase 1 [Ex1]
// Program Description: This program is an HTTP Proxy server which receives http URLs from the user, it checks if the asked URL file
// is in the local filesystem [in the proxy server DB], if true it returns the file in the format of HTTP response, else it sends
// an HTTP request to the web server, then it receives a reply from the server it saves a copy of it then it sends it to the user.

// Student Name: Muath Abu Jamal


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <netinet/in.h>
#include <netdb.h>
#include <sys/socket.h>


// declaring preprocessor directives
#define TRUE 1
#define FALSE 0
#define EMPTY_STR ""
#define DEFAULT_PORT_STR "80"
#define DEFAULT_FILE_STR "index.html"
#define FORWARD_SLASH_STR "/"
#define MAX_SENTENCE_LEN 1000
#define CORRECT_ARGS_COUNT 2
#define HTTP_REQ_TEXT_LEN 100


// declaring the needed functions (explanation for each function bellow(where each function written))
int sectionsCount(char *, char *);

char **toSectionsArray(char *, char *, char *);

char *getSectionStr(const char *, const int *, const int *);

void freeAll(char **, int);

char *getHostName(char *);

char *getPortNumStr(char *);

int getPathItems(char *L, char ***);

char **cloneTwoDArray(char **, int, int);

int isFileExists(char *);

char *concatFilePath(char *, char **, int);

int createFilePath(char *);

char *createRequestStr(char *, char **, int);

int printFile(char *);

int sendServerRequest(int, char *);

int readServerResponse(int, char *);

int getBodyStartInd(const unsigned char *, int, int *, int *);

int main(int argc, char *argv[]) {

    // this if checks, if the number of argument is valid
    // if False it prints a proper message, and it exit()
    if (argc != CORRECT_ARGS_COUNT) {
        fprintf(stdout, "Usage: <URL>\n");
        exit(EXIT_FAILURE);
    }

    // declaring the needed variable for the URL parts
    char *hostName;
    char *portNumStr;

    int pathItemsNum;
    char **pathItems;
    char *filePath;

    // calling the private function getHostName to get the host name after making the needed string processing
    hostName = getHostName(argv[1]);

    // checking if there is no host name, if true print the proper message, free the manually allocated memory and exit from the program
    if (strcmp(hostName, EMPTY_STR) == 0) {
        fprintf(stdout, "Usage: Please Enter A Host Name\n");
        free(hostName);
        exit(EXIT_FAILURE);
    }

    // calling the private function getPortNumStr to get the string of the port number after making the needed string processing
    portNumStr = getPortNumStr(argv[1]);

    // checking if there is no port after the ":" of the port, if true print the proper message, free the manually allocated memory and exit from the program
    if (strcmp(portNumStr, EMPTY_STR) == 0) {
        fprintf(stdout, "Usage: Please Enter A Port Num Or Remove The ':' Of The Port\n");
        free(hostName);
        free(portNumStr);
        exit(EXIT_FAILURE);
    }

    // calling the private function getPathItems to get a 2D strings array of the path items after making the needed string processing
    pathItemsNum = getPathItems(argv[1], &pathItems);

    // calling the private function concatFilePath get the file path by making a concatenation of the host name and the path items
    filePath = concatFilePath(hostName, pathItems, pathItemsNum);


    // checking if the file exists in the local filesystem if true print a successful HTTP response and print the file content to the user
    if (isFileExists(filePath) == TRUE) {

        struct stat st;
        stat(filePath, &st);
        int fileSize = (int) st.st_size;

        char httpRequestText[HTTP_REQ_TEXT_LEN] = EMPTY_STR;

        int httpRequestFormatLen = sprintf(httpRequestText, "HTTP/1.0 200 OK\r\nContent-Length: %d\r\n\r\n", fileSize);

        printf("File is given from local filesystem\n");

        printf("%s", httpRequestText);

        // printing the file bay calling the private function printFile
        printFile(filePath);

        printf("\n Total response bytes: %d\n", fileSize + httpRequestFormatLen);

    }
        // if the file doesn't exist in the local file system, make a proper HTTP request for the file and write that request to the socket FD
    else {

        struct hostent *hostPointer;
        struct sockaddr_in destINAddress;

        // calling the method createRequestStr for building the HTTP request
        char *httpRequest = createRequestStr(hostName, pathItems, pathItemsNum);

        printf("HTTP request =\n%s\nLEN = %d\n", httpRequest, (int) strlen(httpRequest));

        // getting the host internet information
        hostPointer = (struct hostent *) gethostbyname(hostName);

        if (hostPointer == NULL) {
            herror("gethostbyname");
            free(hostName);
            free(portNumStr);
            free(httpRequest);
            freeAll(pathItems, pathItemsNum);
            free(filePath);
            exit(EXIT_FAILURE);
        }

        // creating the socket
        int sd;
        if ((sd = socket(PF_INET, SOCK_STREAM, 0)) < 0) {
            perror("socket");
            free(hostName);
            free(portNumStr);
            free(httpRequest);
            freeAll(pathItems, pathItemsNum);
            free(filePath);
            exit(EXIT_FAILURE);
        }

        // gill the information of the dest server in a socket internet style struct
        destINAddress.sin_family = AF_INET;
        destINAddress.sin_addr.s_addr = ((struct in_addr *) (hostPointer->h_addr_list[0]))->s_addr;
        destINAddress.sin_port = htons((int) strtol(portNumStr, (char **) NULL, 10));

        // connect to the det web server
        if (connect(sd, (struct sockaddr *) &destINAddress, sizeof(destINAddress)) < 0) {
            perror("connect");
            free(hostName);
            free(portNumStr);
            free(httpRequest);
            freeAll(pathItems, pathItemsNum);
            free(filePath);
            exit(EXIT_FAILURE);
        }

        // send the request to the server by calling the private function sendServerRequest
        sendServerRequest(sd, httpRequest);

        // reading the server response, printing it and saving a copy of it in the local file system f the response status code in 200
        int readBytesNum = readServerResponse(sd, filePath);

        printf("\n Total response bytes: %d\n", readBytesNum);

        close(sd);

        free(httpRequest);
    }


    free(hostName);

    free(portNumStr);

    freeAll(pathItems, pathItemsNum);

    free(filePath);

    return 0;
}

// This function receives two argument: 1- the socket File Descriptor, 2- the file path
// the function prints all the server request and, it checks if the web server response is successful "200 OK" if true the function
// calls the private function createFilePath to create the needed directories and the file to save the server response in
// the function saves only the body of the request and, it knows the start index of the body by calling the private function getBodyStartInd
// at the end the function returns the number of bytes that it read.
int readServerResponse(int sd, char *filePath) {

    unsigned char tempSentence[MAX_SENTENCE_LEN] = EMPTY_STR;
    memset(tempSentence, 0, MAX_SENTENCE_LEN);


    unsigned char splitStr[] = "200 OK\r\n";

    int fd = -1;
    int isSuccessfulResp = FALSE;
    int isInFirstLine = TRUE;
    int isInHeadersEndStr = FALSE;
    int headersEndStrIndex = 0;
    int bodyStartIndex = -1;

    int bytesCount = 0;
    int currBytesCount;

    // reading the server response
    while ((currBytesCount = (int) read(sd, tempSentence, MAX_SENTENCE_LEN)) != 0) {

        bytesCount += currBytesCount;

        write(STDOUT_FILENO, tempSentence, currBytesCount);

        // in this case it's a waste of time to continue to the below if conditions so continue to the next read
        // because the response is not "200 OK", so it's enough to write the response to the STDOUT FD
        if (isInFirstLine == FALSE && isSuccessfulResp == FALSE) {
            continue;
        }

        // checking if the response is "200 OK" which should by at the first line of the HTTP response
        if ((isInFirstLine == TRUE) && (isSuccessfulResp == FALSE) &&
            (sectionsCount((char *) tempSentence, (char *) splitStr) >= 2)) {

            isSuccessfulResp = TRUE;

            createFilePath(filePath);

            fd = open(filePath, O_WRONLY);

            if (fd == -1) {
                perror("open");
                exit(EXIT_FAILURE);
            }
        }

        if (isInFirstLine == TRUE) {

            isInFirstLine = FALSE;
        }

        if (bodyStartIndex == -1 && isSuccessfulResp == TRUE) {

            bodyStartIndex = getBodyStartInd(tempSentence, currBytesCount, &isInHeadersEndStr, &headersEndStrIndex);

            // that's mean that the end of headers split string is not in this read (buffer),
            // so we continue to the next read and so on till we find it
            if (bodyStartIndex == -1) {
                continue;
            }

            // that's mean that the start of the body is in the index 0 of the next read
            if (bodyStartIndex == MAX_SENTENCE_LEN) {
                bodyStartIndex = 0;
                continue;
            }

                // that's mean that the body start index is somewhere in the array
            else {

                int bodyContentLen = (currBytesCount - bodyStartIndex);

                if (write(fd, (tempSentence + bodyStartIndex), bodyContentLen) < 0) {
                    perror("write");
                    exit(EXIT_FAILURE);
                }

                continue;
            }
        }

        // writing the server response to the local file system
        if (isSuccessfulResp == TRUE && bodyStartIndex != -1) {
            if (write(fd, tempSentence, currBytesCount) < 0) {
                perror("write");
                exit(EXIT_FAILURE);
            }
        }
    }

    if (fd != -1) {
        close(fd);
    }

    return bytesCount;
}

// This function receives two arguments: 1- a pointer to the buffer which holds what we read from the server, 2- the length of that read bytes,
// 3- a pointer to a boolean which holds the status that we are in "\r\n\r\n" string or not, 4- a pointer to the current index of the split string "\r\n\r\n"
// that we are pointing to.
// the function loops through the buffer at checks if till it reaches the end of headers split string and, it modifies the isInHeadersEnd and headersEndStrIndex
// which are saved in the calling function if there is a need for modification.
int getBodyStartInd(const unsigned char *tempSentence, int tempSentenceLen, int *isInHeadersEndStr,
                    int *headersEndStrIndex) {

    unsigned char headersEndStr[] = "\r\n\r\n";
    int headersEndStrLen = (int) strlen((char *) headersEndStr);

    int i;
    for (i = 0; i < tempSentenceLen; i++) {

        if (tempSentence[i] == headersEndStr[*headersEndStrIndex]) {
            *isInHeadersEndStr = TRUE;
            (*headersEndStrIndex)++;

            if (*headersEndStrIndex == headersEndStrLen) {
                return i + 1;
            }
        } else if ((tempSentence[i] != headersEndStr[*headersEndStrIndex]) && (*isInHeadersEndStr == TRUE)) {

            *isInHeadersEndStr = FALSE;
            *headersEndStrIndex = 0;
            i--;
        }
    }

    return -1;
}

// This function receives two arguments: 1- the file descriptor, 2- a pointer for the string of the request
// the function writes the request String to the server socket and, at the end it returns  the number of bytes that it writes.
int sendServerRequest(int sd, char *requestStr) {

    int bytesCount;
    if ((bytesCount = (int) write(sd, requestStr, strlen(requestStr))) < 0) {
        perror("write");
        exit(EXIT_FAILURE);
    }

    return bytesCount;
}


// This function receives one argument: 1- a pointer the string of the filePath
// The function opens the file which have to by in the local filesystem and, it writes it to the STDOUT FD
// at the end it returns the number of bytes that it wrote, which is the fileSize.
int printFile(char *filePath) {

    unsigned char tempSentence[MAX_SENTENCE_LEN] = EMPTY_STR;
    memset(tempSentence, 0, MAX_SENTENCE_LEN);

    int fd = open(filePath, O_RDONLY);

    if (fd == -1) {
        perror("open");
        exit(EXIT_FAILURE);
    }

    int bytesCount = 0;
    int currBytesCount;

    while ((currBytesCount = (int) read(fd, tempSentence, MAX_SENTENCE_LEN)) != 0) {

        write(STDOUT_FILENO, tempSentence, currBytesCount);

        bytesCount += currBytesCount;
    }

    close(fd);

    return bytesCount;
}


// This function receives three arguments: 1- a pointer for the string of the host name, 2- a pointer for the 2D array of the file items,
// 3- tne number of the file items
// The function creates an HTTP request string using the given argument and using the help of the private function concatFilePath.
char *createRequestStr(char *hostName, char **pathItems, int pathItemsNum) {

    char *requestStr;

    char tempRequestStr[MAX_SENTENCE_LEN] = EMPTY_STR;
    memset(tempRequestStr, 0, MAX_SENTENCE_LEN);

    char *filePathNoHostName = concatFilePath(NULL, pathItems, pathItemsNum);

    sprintf(tempRequestStr, "GET %s HTTP/1.0\r\nHost: %s\r\n\r\n", filePathNoHostName, hostName);

    requestStr = (char *) malloc((sizeof(char) * strlen(tempRequestStr)) + 1);

    if (requestStr == NULL) {
        fprintf(stderr, "malloc failed\n");
        exit(EXIT_FAILURE);
    }

    strcpy(requestStr, tempRequestStr);

    free(filePathNoHostName);

    return requestStr;
}

// This function receives one argument: 1- a pointer for the string of the file path
// This function creates the all the not existing directories and the file of the path
// if the file name is '/' it calls it "index.html"
// it returns TRUE in success and FALSE in failure.
int createFilePath(char *filePath) {

    char tempFilePath[MAX_SENTENCE_LEN] = EMPTY_STR;
    char splitStr[] = "/";

    int pathItemsNum = sectionsCount(filePath, splitStr);

    char **pathItemsArray = toSectionsArray(filePath, splitStr, EMPTY_STR);

    int i;
    for (i = 0; i < pathItemsNum; i++) {

        if ((i == (pathItemsNum - 1)) && (strcmp(pathItemsArray[i], FORWARD_SLASH_STR) == 0)) {
            strcat(tempFilePath, DEFAULT_FILE_STR);
        } else {
            strcat(tempFilePath, pathItemsArray[i]);
        }

        if (isFileExists(tempFilePath) == FALSE) {

            if (i != (pathItemsNum - 1)) {
                mkdir(tempFilePath, 0700);
            } else {

                FILE *fp = fopen(tempFilePath, (char *) "w");

                fclose(fp);
            }
        }

        if (i != (pathItemsNum - 1)) {
            strcat(tempFilePath, splitStr);
        }
    }

    freeAll(pathItemsArray, pathItemsNum);

    if (isFileExists(tempFilePath) == TRUE) {
        return TRUE;
    }

    return FALSE;
}

// This function receives three arguments: 1- a pointer for the hostName string, 2- a pointer to the 2D array of the path Items
// 3- the number of path items
// The function concat the host name and the path Items with each other (it makes a file path) and it returns it as one string
// in the case of hostName  == NULL that's means that we want just the file path without the hostName for the HTTP request
// so the function doesn't concat the host name to the string.
char *concatFilePath(char *hostName, char **pathItems, int pathItemsNum) {

    char *pathStr;

    char tempFilePath[MAX_SENTENCE_LEN] = EMPTY_STR;
    char splitStr[] = "/";

    if (hostName != NULL) {
        strcpy(tempFilePath, hostName);
    }

    int i;
    for (i = 0; i < pathItemsNum; i++) {

        strcat(tempFilePath, splitStr);

        if ((i == (pathItemsNum - 1)) && (strcmp(pathItems[i], FORWARD_SLASH_STR) == 0)) {

            if (hostName != NULL) {

                strcat(tempFilePath, DEFAULT_FILE_STR);
            }

        } else {
            strcat(tempFilePath, pathItems[i]);
        }

    }

    pathStr = (char *) malloc(sizeof(char) * ((int) strlen(tempFilePath) + 1));

    if (pathStr == NULL) {
        fprintf(stderr, "malloc failed\n");
        exit(EXIT_FAILURE);
    }

    strcpy(pathStr, tempFilePath);

    return pathStr;

}

// This function receives one argument: 1- a pointer to the filePath string
// The function checks if the file exists in the local filesystem if true it returns TRUE, else it returns FALSE.
int isFileExists(char *filePath) {

    if (access(filePath, F_OK) == 0) {
        return TRUE;
    }

    return FALSE;
}

// This function receives one argument: 1- a pointer to the string of the URL
// The function makes the proper string parsing/splitting using the private functions of string processing and, it returns the string of the hostName.
char *getHostName(char *URL) {

    char *hostName;

    char tempSentence[MAX_SENTENCE_LEN] = EMPTY_STR;
    char **sectionsArray;
    int sectionsNum;

    char splitStr1[] = "://";
    char splitStr2[] = ":";
    char splitStr3[] = "/";

    sectionsNum = sectionsCount(URL, splitStr1);

    sectionsArray = toSectionsArray(URL, splitStr1, EMPTY_STR);

    strcpy(tempSentence, sectionsArray[1]);

    freeAll(sectionsArray, sectionsNum);

    sectionsNum = sectionsCount(tempSentence, splitStr2);

    if (sectionsNum == 2) {

        sectionsArray = toSectionsArray(tempSentence, splitStr2, EMPTY_STR);

        hostName = (char *) malloc(sizeof(char) * ((int) strlen(sectionsArray[0]) + 1));

        if (hostName == NULL) {
            fprintf(stderr, "malloc failed\n");
            exit(EXIT_FAILURE);
        }

        strcpy(hostName, sectionsArray[0]);

        freeAll(sectionsArray, sectionsNum);

        return hostName;
    }

    sectionsNum = sectionsCount(tempSentence, splitStr3);

    if (sectionsNum > 1) {

        sectionsArray = toSectionsArray(tempSentence, splitStr3, EMPTY_STR);

        hostName = (char *) malloc(sizeof(char) * ((int) strlen(sectionsArray[0]) + 1));

        if (hostName == NULL) {
            fprintf(stderr, "malloc failed\n");
            exit(EXIT_FAILURE);
        }

        strcpy(hostName, sectionsArray[0]);

        freeAll(sectionsArray, sectionsNum);

        return hostName;
    }

    hostName = (char *) malloc(sizeof(char) * ((int) strlen(tempSentence) + 1));

    if (hostName == NULL) {
        fprintf(stderr, "malloc failed\n");
        exit(EXIT_FAILURE);
    }

    strcpy(hostName, tempSentence);

    return hostName;
}

// This function receives one argument: 1- a pointer to the string of the URL
// The function makes the proper string parsing/splitting using the private functions of string processing and, it returns the string of the portNumber
// if there is no port number the function returns a string with the value of the default port "80"
char *getPortNumStr(char *URL) {

    char *portNumStr;

    char tempSentence[MAX_SENTENCE_LEN] = EMPTY_STR;
    char **sectionsArray;
    int sectionsNum;

    char splitStr1[] = ":";
    char splitStr2[] = "/";

    sectionsNum = sectionsCount(URL, splitStr1);

    if (sectionsNum == 2) {

        portNumStr = (char *) malloc(sizeof(char) * ((int) strlen(DEFAULT_PORT_STR) + 1));

        if (portNumStr == NULL) {
            fprintf(stderr, "malloc failed\n");
            exit(EXIT_FAILURE);
        }

        strcpy(portNumStr, DEFAULT_PORT_STR);

        return portNumStr;
    }

    sectionsArray = toSectionsArray(URL, splitStr1, EMPTY_STR);

    strcpy(tempSentence, sectionsArray[2]);

    freeAll(sectionsArray, sectionsNum);

    sectionsNum = sectionsCount(tempSentence, splitStr2);

    if (sectionsNum == 1) {

        portNumStr = (char *) malloc(sizeof(char) * ((int) strlen(tempSentence) + 1));

        if (portNumStr == NULL) {
            fprintf(stderr, "malloc failed\n");
            exit(EXIT_FAILURE);
        }

        strcpy(portNumStr, tempSentence);

        return portNumStr;
    }

    sectionsArray = toSectionsArray(tempSentence, splitStr2, EMPTY_STR);

    portNumStr = (char *) malloc(sizeof(char) * ((int) strlen(sectionsArray[0]) + 1));

    if (portNumStr == NULL) {
        fprintf(stderr, "malloc failed\n");
        exit(EXIT_FAILURE);
    }

    strcpy(portNumStr, sectionsArray[0]);

    freeAll(sectionsArray, sectionsNum);

    return portNumStr;
}

// This function receives two argument: 1- a pointer to the string of the URL, 2- a pointer to the 2D array pointer from the calling function
// The function makes the proper string parsing/splitting using the private functions of string processing it buts the result in 2D array and
// it puts the address value of the 2D array in the ***pathItems, at the end it returns the number of path Items
int getPathItems(char *URL, char ***pathItems) {

    char tempSentence[MAX_SENTENCE_LEN] = EMPTY_STR;
    char **sectionsArray;
    int sectionsNum;

    char splitStr1[] = "://";
    char splitStr2[] = "/";

    sectionsNum = sectionsCount(URL, splitStr1);

    sectionsArray = toSectionsArray(URL, splitStr1, EMPTY_STR);

    strcpy(tempSentence, sectionsArray[1]);

    freeAll(sectionsArray, sectionsNum);

    sectionsNum = sectionsCount(tempSentence, splitStr2);

    if (sectionsNum == 1) {
        *pathItems = (char **) malloc(sizeof(char *) * 1);

        if (*pathItems == NULL) {
            fprintf(stderr, "malloc failed\n");
            exit(EXIT_FAILURE);
        }

        **pathItems = (char *) malloc(sizeof(char) * ((int) strlen(FORWARD_SLASH_STR) + 1));

        if (**pathItems == NULL) {
            fprintf(stderr, "malloc failed\n");
            exit(EXIT_FAILURE);
        }

        strcpy(**pathItems, FORWARD_SLASH_STR);

        return 1;
    }

    sectionsArray = toSectionsArray(tempSentence, splitStr2, FORWARD_SLASH_STR);

    char **pathItemsArray = cloneTwoDArray(sectionsArray, 1, sectionsNum);

    freeAll(sectionsArray, sectionsNum);

    *pathItems = pathItemsArray;

    return sectionsNum - 1;
}

// This function receives three argument: 1- a pointer to 2D strings array, 2- the start index of the copy, 3- the end index of the copy
// The function creates a new strings array and copies the values from the given array according to the given indexes
// into the new array and at the end it returns the new array.
// I use it in the string parsing process for the path items to get a new 2D array without the host name.
char **cloneTwoDArray(char **twoDArray, int from, int to) {

    int cloneArrayLen = to - from;

    char **cloneArray = (char **) malloc(sizeof(char *) * cloneArrayLen);

    if (cloneArray == NULL) {
        fprintf(stderr, "malloc failed\n");
        exit(EXIT_FAILURE);
    }

    int i;
    for (i = 0; i < cloneArrayLen; i++) {

        cloneArray[i] = (char *) malloc(sizeof(char) * ((int) strlen(twoDArray[from + i]) + 1));

        if (cloneArray[i] == NULL) {
            fprintf(stderr, "malloc failed\n");
            exit(EXIT_FAILURE);
        }

        strcpy(cloneArray[i], twoDArray[from + i]);
    }

    return cloneArray;
}

// This method receives three pointers: 1- for the sentence, 2- for the splitting string, 3- for the default string (to put if there is an empty split)
// the method splits the sentence according to the given splitStr, and it puts the value of the given defStr if there is an empty split
// for example if we split the string "x" according to "x" and the value of defStr = "1"
// then the result is the array ["1", "1"] before and after "x" there are an empty split
// it uses the methods sectionsCount() and getSectionStr()
char **toSectionsArray(char *sentence, char *splitStr, char *defStr) {

    // declaring and initializing the needed variables for the string processing
    // and allocating the needed memories for that
    int sectionsNum = sectionsCount(sentence, splitStr);

    char **sectionsArray = (char **) malloc(sizeof(char *) * (sectionsNum));
    int sectionsArrayCurrIndex = 0;

    if (sectionsArray == NULL) {
        fprintf(stderr, "malloc failed\n");
        exit(EXIT_FAILURE);
    }

    int splitStrCurrIndex = 0;
    int splitStrLastEndIndex = 0;
    int splitStrLen = (int) strlen(splitStr);

    int sentenceLen = (int) strlen(sentence);

    int isInSplit = FALSE;

    int sectionStartIndex, sectionEndIndex;

    int defStrStartIndex = 0, defStrLastIndex = 0;

    if (defStr != NULL) {
        defStrStartIndex = 0;
        defStrLastIndex = (int) strlen(defStr) - 1;
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
                    sectionsArray[sectionsArrayCurrIndex] = getSectionStr(sentence, &sectionStartIndex,
                                                                          &sectionEndIndex);
                } else {
                    sectionsArray[sectionsArrayCurrIndex] = getSectionStr(defStr, &defStrStartIndex, &defStrLastIndex);
                }

                sectionsArrayCurrIndex++;
                continue;
            }

        } else if (sentence[i] != splitStr[splitStrCurrIndex] && isInSplit == TRUE) {
            isInSplit = FALSE;
            splitStrCurrIndex = 0;
            i--;
        }
    }

    // dealing with the special case which is at the end of the sentence that the for loop finish before dealing with
    if (i == splitStrLastEndIndex) {
        sectionsArray[sectionsArrayCurrIndex] = getSectionStr(defStr, &defStrStartIndex, &defStrLastIndex);
    } else {
        sectionStartIndex = splitStrLastEndIndex;
        sectionEndIndex = i - 1;
        sectionsArray[sectionsArrayCurrIndex] = getSectionStr(sentence, &sectionStartIndex, &sectionEndIndex);
    }

    return sectionsArray;
}

// This function receives three pointers: 1- for the sentence, 2- for the first index in the section (according to the indexes of the sentence), 3- for the last index in the section (according to the indexes of the sentence)
// the function manually allocates the needed memory for the word according to the given indexes, and it returns a pointer char* for it
char *getSectionStr(const char *sentence,
                    const int *sectionStartIndex,
                    const int *sectionEndIndex) {
    int i, j;

    int sectionLen = ((*(sectionEndIndex) - *(sectionStartIndex)) + 1);

    // allocating exactly needed memory for the characters of the section + 1 for the '\0' character
    char *currSectionPointer = (char *) malloc(sizeof(char) * (sectionLen + 1));

    // checking if the memory allocation goes wrong / failed
    // if true I print a message for the user then the program exits with EXIT_FAILURE code (1)
    if (currSectionPointer == NULL) {
        fprintf(stderr, "malloc failed\n");
        exit(EXIT_FAILURE);
    }

    // going through the sentence according to the indexes of the word for copying in to the new allocated memory
    for (i = *(sectionStartIndex), j = 0; i <= *(sectionEndIndex); i++, j++) {
        currSectionPointer[j] = sentence[i];
    }

    // finishing the word by putting the '\0' character at the end of the word
    currSectionPointer[sectionLen] = '\0';

    return currSectionPointer;
}

// This method receives two pointers: 1- for the sentence, 2- for the splitStr
// the method goes through the hole sentence, and it counts the number of sections according to the given splitStr
int sectionsCount(char *sentence, char *splitStr) {

    int sectionsCount = 1;

    int splitStrCurrIndex = 0;
    int splitStrLen = (int) strlen(splitStr);

    int sentenceLen = (int) strlen(sentence);

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
            i--;
        }
    }

    return sectionsCount;
}

// This method receives two pointers: 1- for the arguments array, 2- for the number of the arguments
// it frees all the manually allocated memory by going through each pointer in the array of args pointers
// then it frees the array of pointers itself
void freeAll(char **sectionsArrayPointer, int sectionsArrayLen) {

    if (sectionsArrayPointer == NULL) {
        return;
    }

    int i;
    for (i = 0; i < sectionsArrayLen; i++) {
        if (sectionsArrayPointer[i] != NULL) {
            free(sectionsArrayPointer[i]);
        }
    }

    free(sectionsArrayPointer);
}