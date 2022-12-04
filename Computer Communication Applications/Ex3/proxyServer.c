// Program Name: HTTP Proxy - Phase 2 [Ex3]
// Program Description: This program is a multi threaded HTTP Proxy server which receives http requests from the clients, it checks if the asked file
// is in the local filesystem [in the proxy server DB], if true it returns the file in the format of HTTP response, else it sends
// an HTTP request to the web server, then it receives a reply from the server it saves a copy of it then it sends it to the user.

// Student Name: Muath Abu Jamal

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <fcntl.h>
#include <string.h>
#include <ctype.h>
#include <sys/stat.h>
#include <netdb.h>
#include <arpa/inet.h>
#include "threadpool.h"

// declaring preprocessor directives
#define TRUE 1
#define FALSE 0
#define EMPTY_STR ""
#define HTTP_REQ_FL_ITEMS_NUM 3
#define HTTP_REQ_V0_STR "HTTP/1.0"
#define HTTP_REQ_V1_STR "HTTP/1.1"
#define STATUS_CODE_STR_LEN 100
#define SPACE_CHAR ' '
#define CORRECT_ARGS_COUNT 5
#define MAX_LISTEN_QUEUE 10
#define FAILED (-1)
#define MAX_SENTENCE_LEN 1000
#define BODY_CONTENT_LEN 1000
#define HEADERS_CONTENT_LEN 1000
#define RESPONSE_CONTENT_LEN (BODY_CONTENT_LEN + HEADERS_CONTENT_LEN + 1)
#define BAD_REQUEST 400
#define NOT_FOUND 404
#define FORBIDDEN 403
#define INTERNAL_SERVER_ERROR 500
#define NOT_SUPPORTED 501
#define NO_ERRORS (-9)
#define IP_V4_LEN 32
#define DEFAULT_FILE_STR "index.html"
#define FORWARD_SLASH_STR "/"
#define SUCCEEDED 1
#define WEB_SERVER_PORT 80

// LinkList and Node data structures
typedef struct Node {
    char *data;
    struct Node *next;
} Node;

typedef struct LinkList {
    Node *head;
    Node *tail;
    int size;
} LinkList;

// declaring the needed functions (explanation for each function bellow(where each function written))
int sectionsCount(char *, char *);

char **toSectionsArray(char *, char *, char *);

char *getSectionStr(const char *, const int *, const int *);

void freeAll(char **, int);

char *getLowerCaseCopy(char *);

char *getOneSpaceCopy(char *);

void strTrim(char *);

int initializeFilterLists(int);

int add(LinkList *, char *);

int serverInitialize(int);

char *readClientRequest(int);

int checkClientReqErrors(char *);

int isBadRequest(char *);

int isGETMethod(char *);

int isInFilter(LinkList *, char *);

int statusErrorHandler(int, int);

char *getDomainStr(char *);

char *getFilePath(char *);

char *getFileName(char *);

char *get_mime_type(char *);

int isFileExists(char *);

int createFilePath(char *);

uint32_t strIpToUint32_t(char *);

int belongsToSub(char *, char *, int);

int sendLocalFileResponse(char *, int);

int connectToWebServer(char *);

char *buildWebServerRequest(char *);

int sendServerRequest(int, char *);

int readAndSendServerResponse(int, int, char *);

int getBodyStartInd(const unsigned char *, int, int *, int *);

int clientHandler(void *);

void freeList(LinkList *);

LinkList *filterNumDomains = NULL;
LinkList *filterStrDomains = NULL;

// The main function cares of making the threads pool and initializing the server and the filter lists
// by calling the proper private function to make these jobs, and it starts to accept the clients
// each client it sends to the threadPool jobs queue where the threads take care of when they are free(not busy)
// at the end it destroys the pool, and it makes the needed free for the manually allocated memories
int main(int argc, char *argv[]) {

    if (argc != CORRECT_ARGS_COUNT) {
        fprintf(stdout, "Usage: proxyServer <port> <pool-size> <max-number-of-request> <filter>\n");
        exit(EXIT_FAILURE);
    }

    int portNum = (int) strtol(argv[1], (char **) NULL, 10);
    int threadsNum = (int) strtol(argv[2], (char **) NULL, 10);
    int maxRequestsNumber = (int) strtol(argv[3], (char **) NULL, 10);

    if ((threadsNum <= 0 || threadsNum > MAXT_IN_POOL) || (portNum < 0) || (maxRequestsNumber < 0)) {
        fprintf(stdout, "Usage: proxyServer <port> <pool-size> <max-number-of-request> <filter>\n");
        exit(EXIT_FAILURE);
    }

    int filterFD = open(argv[4], O_RDONLY);
    if (filterFD == -1) {
        perror("error: open\n");
        exit(EXIT_FAILURE);
    }

    if (initializeFilterLists(filterFD) == FAILED) {
        close(filterFD);
        exit(EXIT_FAILURE);
    }

    close(filterFD);

    int *threadSDPointer = NULL;

    int welcomeSD = serverInitialize(portNum);

    if (welcomeSD == FAILED) {

        freeList(filterNumDomains);
        freeList(filterStrDomains);
        exit(EXIT_FAILURE);
    }

    threadpool *threadsPoolPointer = create_threadpool(threadsNum);

    if (threadsPoolPointer == NULL) {

        freeList(filterNumDomains);
        freeList(filterStrDomains);
        close(welcomeSD);
        exit(EXIT_FAILURE);
    }

    int requestsCounter;
    for (requestsCounter = 0; requestsCounter < maxRequestsNumber; requestsCounter++) {

        threadSDPointer = (int *) malloc(sizeof(int));

        if (threadSDPointer == NULL) {

            destroy_threadpool(threadsPoolPointer);
            freeList(filterNumDomains);
            freeList(filterStrDomains);
            exit(EXIT_FAILURE);
        }

        *(threadSDPointer) = accept(welcomeSD, NULL, NULL);

        if (*(threadSDPointer) < 0) {
            perror("error: accept\n");
            exit(EXIT_FAILURE);
        }

        dispatch(threadsPoolPointer, clientHandler, threadSDPointer);
    }

    destroy_threadpool(threadsPoolPointer);

    freeList(filterStrDomains);
    freeList(filterNumDomains);
    close(welcomeSD);

    return 0;
}


// This function receives one argument: 1- a pointer to an int which holds the socket descriptor of the client
// the function executed by each thread (every thread handles a client), the function reads the client request
// it checks if the request is valid if false it calls the private statusErrorHandler() to handle the error
// by the error status code which given by the private function checkClientReqErrors()
// if the request is valid the function checks if the file is in the local file system if true it calls
// the function sendLocalFileResponse() to send to the client from the local file system
// else it connects to the web server of the domain that given in the client request
// it sends the client request to the web server if the response is "200 OK" it saves a copy of the response
// and at the same time it sends the response of the web server to the client
// of course the function makes some string processing where there is a need for that
// if there is a server error it free the manually allocated memories and it sends INTERNAL_SERVER_ERROR to the client
// then it ends, also if the domain is a name it gets it's ip and it searches for the asked file under the directory of that ip
int clientHandler(void *threadSDPointer) {

    int threadSD = *((int *) threadSDPointer);

    int statusErrorHandlerStatus;

    char *clientRequest = readClientRequest(threadSD);

    if (clientRequest == NULL) {

        statusErrorHandler(INTERNAL_SERVER_ERROR, threadSD);
        close(threadSD);
        free(threadSDPointer);
        return FAILED;
    }

    int errorStatus = checkClientReqErrors(clientRequest);

    if (errorStatus == FAILED) {

        statusErrorHandler(INTERNAL_SERVER_ERROR, threadSD);
        close(threadSD);
        free(threadSDPointer);
        free(clientRequest);
        return FAILED;
    }

    if (errorStatus != NO_ERRORS) {

        statusErrorHandlerStatus = statusErrorHandler(errorStatus, threadSD);
        close(threadSD);
        free(threadSDPointer);
        free(clientRequest);

        if (statusErrorHandlerStatus == FAILED) {
            return FAILED;
        }

        return SUCCEEDED;
    }

    char *domainStr = getDomainStr(clientRequest);

    if (domainStr == NULL) {
        statusErrorHandler(INTERNAL_SERVER_ERROR, threadSD);
        close(threadSD);
        free(threadSDPointer);
        free(clientRequest);
        return FAILED;
    }

    char *filePath = getFilePath(clientRequest);

    if (filePath == NULL) {
        statusErrorHandler(INTERNAL_SERVER_ERROR, threadSD);
        close(threadSD);
        free(threadSDPointer);
        free(clientRequest);
        free(domainStr);
        return FAILED;
    }

    if (strcmp(filePath, "/") == 0) {

        free(filePath);
        int indexFilePathStrLen = strlen(DEFAULT_FILE_STR) + 2;
        filePath = (char *) malloc(sizeof(char) * indexFilePathStrLen);

        if (filePath == NULL) {
            statusErrorHandler(INTERNAL_SERVER_ERROR, threadSD);
            close(threadSD);
            free(threadSDPointer);
            free(clientRequest);
            free(domainStr);
            return FAILED;
        }

        memset(filePath, '\0', indexFilePathStrLen);

        strcpy(filePath, "/");
        strcat(filePath, DEFAULT_FILE_STR);
    }

    int fullPathLen = (int) (strlen(domainStr) + strlen(filePath));

    char *fullPathStr = (char *) malloc(sizeof(char) * (fullPathLen + 1));
    memset(fullPathStr, '\0', fullPathLen);

    strcpy(fullPathStr, domainStr);
    strcat(fullPathStr, filePath);

    if (isFileExists(fullPathStr) == TRUE) {

        printf("HTTP request =\n%s\nLEN = %d\n", clientRequest, (int) strlen(clientRequest));

        if (sendLocalFileResponse(fullPathStr, threadSD) == FAILED) {

            statusErrorHandler(INTERNAL_SERVER_ERROR, threadSD);
            close(threadSD);
            free(threadSDPointer);
            free(clientRequest);
            free(domainStr);
            free(filePath);
            free(fullPathStr);
            return FAILED;
        }

    } else {

        printf("HTTP request =\n%s\nLEN = %d\n", clientRequest, (int) strlen(clientRequest));

        int webServerSD = connectToWebServer(domainStr);

        char *webServerRequest = buildWebServerRequest(clientRequest);

        if (webServerSD == FAILED || webServerRequest == NULL ||
            sendServerRequest(webServerSD, webServerRequest) == FAILED ||
            readAndSendServerResponse(webServerSD, threadSD, fullPathStr) == FAILED) {

            statusErrorHandler(INTERNAL_SERVER_ERROR, threadSD);
            close(threadSD);
            free(threadSDPointer);
            free(clientRequest);
            free(domainStr);
            free(filePath);
            free(fullPathStr);
            if (webServerRequest != NULL) {
                free(webServerRequest);
            }
            return FAILED;
        }

        close(webServerSD);
        free(webServerRequest);
    }

    close(threadSD);
    free(threadSDPointer);
    free(clientRequest);
    free(domainStr);
    free(filePath);
    free(fullPathStr);

    return SUCCEEDED;
}

// This method receives one argument: 1- a pointer to the client request string
// it builds a new request with just a host and connection: close headers,
// so we don't stuck in a connection with the web server that we want to send the request to
char *buildWebServerRequest(char *clientRequest) {

    char tempRequestStr[MAX_SENTENCE_LEN] = EMPTY_STR;
    memset(tempRequestStr, '\0', MAX_SENTENCE_LEN);

    char splitStr1[] = "\r\n";

    int sectionsNumSplit1 = sectionsCount(clientRequest, splitStr1);

    char **sectionsArraySplit1 = toSectionsArray(clientRequest, splitStr1, EMPTY_STR);

    if (sectionsArraySplit1 == NULL) {
        return NULL;
    }

    char *domainStr = getDomainStr(clientRequest);

    if (domainStr == NULL) {

        freeAll(sectionsArraySplit1, sectionsNumSplit1);
        return NULL;
    }

    sprintf(tempRequestStr, "%s\r\nHost: %s\r\nConnection: close\r\n\r\n", sectionsArraySplit1[0], domainStr);

    freeAll(sectionsArraySplit1, sectionsNumSplit1);
    free(domainStr);

    char *newRequest = (char *) malloc(sizeof(char) * (strlen(tempRequestStr) + 1));

    strcpy(newRequest, tempRequestStr);

    return newRequest;
}

// This function receives one argument: 1- a pointer for the dictionary
// the function frees all the manually allocated memories of the dictionary
void freeList(LinkList *list) {

    Node *prevNode = NULL;
    Node *currNode;

    for (currNode = list->head; currNode != NULL; prevNode = currNode, currNode = currNode->next) {

        if (prevNode != NULL) {
            free(prevNode);
        }

        free(currNode->data);
    }

    if (prevNode != NULL) {
        free(prevNode);
    }

    free(list);
}

// This function receives two argument: 1- the socket File Descriptor, 2- the file path
// the function prints all the server request and, it checks if the web server response is successful "200 OK" if true the function
// calls the private function createFilePath to create the needed directories and the file to save the server response in
// the function saves only the body of the request and, it knows the start index of the body by calling the private function getBodyStartInd
// at the end the function returns the number of bytes that it read.
int readAndSendServerResponse(int webServerSD, int clientSD, char *filePath) {

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
    while ((currBytesCount = (int) read(webServerSD, tempSentence, MAX_SENTENCE_LEN)) > 0) {

        bytesCount += currBytesCount;

        if (write(clientSD, tempSentence, currBytesCount) == -1) {

            perror("error: write\n");
            return FAILED;
        }

        // in this case it's a waste of time to continue to the below if conditions so continue to the next read
        // because the response is not "200 OK", so it's enough to write the response to the STDOUT FD
        if (isInFirstLine == FALSE && isSuccessfulResp == FALSE) {
            continue;
        }

        // checking if the response is "200 OK" which should by at the first line of the HTTP response
        if ((isInFirstLine == TRUE) && (isSuccessfulResp == FALSE) &&
            (sectionsCount((char *) tempSentence, (char *) splitStr) >= 2)) {

            isSuccessfulResp = TRUE;

            if (createFilePath(filePath) == FAILED) {

                return FAILED;
            }

            fd = open(filePath, O_WRONLY);

            if (fd == -1) {

                perror("error: open\n");
                return FAILED;
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

                if (write(fd, (tempSentence + bodyStartIndex), bodyContentLen) == -1) {
                    perror("error: write\n");
                    return FAILED;
                }

                continue;
            }
        }

        // writing the server response to the local file system
        if (isSuccessfulResp == TRUE && bodyStartIndex != -1) {
            if (write(fd, tempSentence, currBytesCount) == -1) {
                perror("error: write\n");
                return FAILED;
            }
        }
    }

    if (currBytesCount == -1) {
        perror("error: read\n");
        return FAILED;
    }

    memset(tempSentence, 0, MAX_SENTENCE_LEN);

    sprintf((char *) tempSentence, "File is given from origin server\n\n Total response bytes: %d\n", bytesCount);

    printf("%s", tempSentence);

    if (fd != -1) {
        close(fd);
    }

    return SUCCEEDED;
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

// This function receives one argument: 1- a pointer to a string of the domainName that we want to connect to
// the function creates a socket, then it connects to the server of the given DomainName [domain name, ip] at the ind it returns the SD
int connectToWebServer(char *domainName) {

    struct hostent *hostPointer;
    struct sockaddr_in destINAddress;

    hostPointer = (struct hostent *) gethostbyname(domainName);

    if (hostPointer == NULL) {

        return FAILED;
    }

    int sd;
    if ((sd = socket(PF_INET, SOCK_STREAM, 0)) < 0) {
        perror("error: socket\n");
        return FAILED;
    }

    destINAddress.sin_family = AF_INET;
    destINAddress.sin_addr.s_addr = ((struct in_addr *) (hostPointer->h_addr_list[0]))->s_addr;
    destINAddress.sin_port = htons(WEB_SERVER_PORT);

    // connect to the det web server
    if (connect(sd, (struct sockaddr *) &destINAddress, sizeof(destINAddress)) < 0) {
        perror("error: connect\n");
        return FAILED;
    }

    return sd;
}

// This function receives two arguments: 1- a socket descriptor for the server,
// 2- a pointer to the request string which we want to send it to the server
// the function writes the request to the server socket descriptor
int sendServerRequest(int sd, char *requestStr) {

    int bytesCount;
    if ((bytesCount = (int) write(sd, requestStr, strlen(requestStr))) == -1) {
        perror("error: write\n");
        return FAILED;
    }

    return SUCCEEDED;
}

// This function receives two arguments: 1- a pointer for the filePath string, 2- the socket descriptor for the client
// the function reads the file from the local file system, and it writes it to the client socket descriptor
// at the end it prints the asked message "File is given from local filesystem\n\n Total response bytes: %d\n"
int sendLocalFileResponse(char *filePath, int SD) {

    struct stat st;
    int statFileStatus = stat(filePath, &st);

    if (statFileStatus == -1) {
        fprintf(stdout, "stat method failed\n");
        return FAILED;
    }

    int fileSize = (int) st.st_size;

    char *fileName = getFileName(filePath);

    if (fileName == NULL) {

        return FAILED;
    }

    unsigned char tempSentence[MAX_SENTENCE_LEN] = EMPTY_STR;
    memset(tempSentence, 0, MAX_SENTENCE_LEN);

    int httpResponseFormatLen;
    char *mime_type = get_mime_type(fileName);
    free(fileName);

    if (mime_type == NULL) {

        httpResponseFormatLen = sprintf((char *) tempSentence,
                                        "HTTP/1.0 200 OK\r\nContent-Length: %d\r\nConnection: close\r\n\r\n",
                                        fileSize);
    } else {

        httpResponseFormatLen = sprintf((char *) tempSentence,
                                        "HTTP/1.0 200 OK\r\nContent-Length: %d\r\nContent-Type: %s\r\nConnection: close\r\n\r\n",
                                        fileSize,
                                        mime_type);
    }

    int fd = open(filePath, O_RDONLY);

    if (fd == -1) {
        perror("error: open\n");
        return FAILED;
    }

    if (write(SD, tempSentence, httpResponseFormatLen) == -1) {

        perror("error: write\n");
        return FAILED;
    }

    int currBytesCount;

    while ((currBytesCount = (int) read(fd, tempSentence, MAX_SENTENCE_LEN)) > 0) {

        if (write(SD, tempSentence, currBytesCount) == -1) {

            perror("error: write\n");
            close(fd);
            return FAILED;
        }
    }

    if (currBytesCount == -1) {
        perror("error: read\n");
        close(fd);
        return FAILED;
    }

    close(fd);

    memset(tempSentence, 0, MAX_SENTENCE_LEN);

    sprintf((char *) tempSentence, "File is given from local filesystem\n\n Total response bytes: %d\n",
            fileSize + httpResponseFormatLen);

    printf("%s", tempSentence);

    return SUCCEEDED;
}

// This function receives one argument: 1- a string pointer for the file name
// the function is given from the teacher it returns the file type in the format of the Content-Type http header format
char *get_mime_type(char *name) {

    char *ext = strrchr(name, '.');

    if (!ext) return NULL;

    if (strcmp(ext, ".html") == 0 || strcmp(ext, ".htm") == 0) return "text/html";

    if (strcmp(ext, ".jpg") == 0 || strcmp(ext, ".jpeg") == 0) return "image/jpeg";

    if (strcmp(ext, ".gif") == 0) return "image/gif";

    if (strcmp(ext, ".png") == 0) return "image/png";

    if (strcmp(ext, ".css") == 0) return "text/css";

    if (strcmp(ext, ".au") == 0) return "audio/basic";

    if (strcmp(ext, ".wav") == 0) return "audio/wav";

    if (strcmp(ext, ".avi") == 0) return "video/x-msvideo";

    if (strcmp(ext, ".mpeg") == 0 || strcmp(ext, ".mpg") == 0) return "video/mpeg";

    if (strcmp(ext, ".mp3") == 0) return "audio/mpeg";

    return NULL;
}

// This function receives one argument: 1- a pointer to the file path string
// the function makes the needed string processing to get the file name from the given file path, and it returns it
char *getFileName(char *filePath) {

    char splitStr[] = "/";

    int sectionsNum = sectionsCount(filePath, splitStr);

    char **sectionsArray = toSectionsArray(filePath, splitStr, EMPTY_STR);

    if (sectionsArray == NULL) {

        return NULL;
    }

    char *fileName = (char *) malloc(sizeof(char) * (strlen(sectionsArray[sectionsNum - 1]) + 1));

    if (filePath == NULL) {

        fprintf(stdout, "Allocation failure: malloc failed\n");
        freeAll(sectionsArray, sectionsNum);
        return NULL;
    }

    memset(fileName, '\0', (strlen(sectionsArray[sectionsNum - 1]) + 1));

    strcpy(fileName, sectionsArray[sectionsNum - 1]);

    freeAll(sectionsArray, sectionsNum);

    return fileName;
}

// This function receives one argument: 1- the port number that we want to initialize the server with
// the function creates the welcomeSocket for the server it binds it with the proper sockadd_in as asked in the question
// and with the given port number at it makes it listen at maximum for the value of the defined macro MAX_LISTEN_QUEUE
// at the end it returns the welcomeSD
int serverInitialize(int portNum) {

    int welcomeSD;
    if ((welcomeSD = socket(PF_INET, SOCK_STREAM, 0)) < 0) {
        perror("error: socket\n");
        return FAILED;
    }

    struct sockaddr_in serverNAddress;

    serverNAddress.sin_family = AF_INET;

    serverNAddress.sin_port = htons(portNum);

    serverNAddress.sin_addr.s_addr = htonl(INADDR_ANY);

    if (bind(welcomeSD, (struct sockaddr *) &serverNAddress, sizeof(serverNAddress)) < 0) {
        perror("error: bind\n");
        close(welcomeSD);
        return FAILED;
    }

    if (listen(welcomeSD, MAX_LISTEN_QUEUE) == -1) {
        perror("error: listen\n");
        close(welcomeSD);
        return FAILED;
    }

    return welcomeSD;
}


// This function receives one argument: 1- the filter file descriptor
// the function reads the filter file, and it fills the Global filter linkLists
// with the read values after initializing these linkLists and making the needed string processing on them
int initializeFilterLists(int filterFD) {

    char tempSentence[MAX_SENTENCE_LEN] = EMPTY_STR;
    memset(tempSentence, '\0', MAX_SENTENCE_LEN);

    int sectionsNum;
    char **sectionsArray;

    char splitStr[] = "\r\n";

    filterNumDomains = (LinkList *) malloc(sizeof(LinkList) * 1);
    if (filterNumDomains == NULL) {

        fprintf(stdout, "Allocation failure: malloc failed\n");
        return FAILED;
    }
    filterNumDomains->head = NULL;
    filterNumDomains->tail = NULL;
    filterNumDomains->size = 0;

    filterStrDomains = (LinkList *) malloc(sizeof(LinkList) * 1);
    if (filterStrDomains == NULL) {

        fprintf(stdout, "Allocation failure: malloc failed\n");
        free(filterNumDomains);
        return FAILED;
    }
    filterStrDomains->head = NULL;
    filterStrDomains->tail = NULL;
    filterStrDomains->size = 0;

    int currBytesNum;

    int i, j;
    while ((currBytesNum = (int) read(filterFD, tempSentence, MAX_SENTENCE_LEN - 1)) > 0) {

        tempSentence[MAX_SENTENCE_LEN - 1] = '\0';

        if (currBytesNum == MAX_SENTENCE_LEN - 1) {

            for (i = currBytesNum - 1; i >= 0; i--) {

                if (tempSentence[i] == '\n') {

                    tempSentence[i - 1] = '\0';

                    lseek(filterFD, (-1) * (currBytesNum - i - 1), SEEK_CUR);

                    break;
                }
            }
        }

        sectionsNum = sectionsCount(tempSentence, splitStr);

        if (sectionsNum <= 1) {

            continue;
        }

        sectionsArray = toSectionsArray(tempSentence, splitStr, EMPTY_STR);

        if (sectionsArray == NULL) {

            freeList(filterNumDomains);
            freeList(filterStrDomains);
            return FAILED;
        }

        for (j = 0; j < sectionsNum; j++) {

            if (strcmp(sectionsArray[j], EMPTY_STR) == 0) {
                free(sectionsArray[j]);
                continue;
            }

            if (isdigit(sectionsArray[j][0])) {

                add(filterNumDomains, sectionsArray[j]);
            } else {

                add(filterStrDomains, sectionsArray[j]);
            }
        }

        free(sectionsArray);
    }

    if (currBytesNum == -1) {

        perror("error: read\n");
        freeList(filterNumDomains);
        freeList(filterStrDomains);
        return FAILED;
    }

    return SUCCEEDED;
}

// This function receives one argument: 1- the socket descriptor of the client
// the function reads the client request till the "\r\n\r\n" it saves that request, and it returns a pointer to it
char *readClientRequest(int SD) {

    int sizeTimes = 1;
    char *clientRequest = (char *) malloc(sizeof(char) * ((MAX_SENTENCE_LEN * sizeTimes) + 1));

    if (clientRequest == NULL) {
        fprintf(stdout, "Allocation failure: malloc failed\n");
        return NULL;
    }

    memset(clientRequest, '\0', ((MAX_SENTENCE_LEN * sizeTimes) + 1));

    char splitStr[] = "\r\n\r\n";

    int bytesCount = 0;
    int currBytesNum;

    while (TRUE) {

        currBytesNum = (int) read(SD, (char *) (clientRequest + bytesCount),
                                  ((MAX_SENTENCE_LEN * sizeTimes) - bytesCount));

        if (currBytesNum == -1) {

            perror("error: read\n");
            free(clientRequest);
            return NULL;
        }

        bytesCount += currBytesNum;

        if (currBytesNum == MAX_SENTENCE_LEN) {

            sizeTimes++;
            clientRequest = (char *) realloc(clientRequest, sizeof(char) * ((MAX_SENTENCE_LEN * sizeTimes) + 1));

            if (clientRequest == NULL) {

                fprintf(stdout, "Allocation failure: malloc failed\n");
                free(clientRequest);
                return NULL;
            }

            memset((clientRequest + (MAX_SENTENCE_LEN * (sizeTimes - 1))), '\0',
                   (MAX_SENTENCE_LEN + 1));
        }

        if (sectionsCount(clientRequest, splitStr) >= 2) {
            break;
        }
    }

    clientRequest[bytesCount] = '\0';

    char *finalRequestStr = (char *) malloc(sizeof(char) * (bytesCount + 1));

    if (finalRequestStr == NULL) {

        fprintf(stdout, "Allocation failure: malloc failed\n");
        free(clientRequest);
        return NULL;
    }

    strcpy(finalRequestStr, clientRequest);

    free(clientRequest);

    return finalRequestStr;
}

// This function receives one argument: 1- a pointer for the client http string request
// the function checks if the request has any errors by calling the private function for errors checking
// if there is an error it returns the error status code else it returns defined macro NO_ERRORS
int checkClientReqErrors(char *clientRequest) {

    if (isBadRequest(clientRequest) == TRUE) {
        return BAD_REQUEST;
    }

    if (isGETMethod(clientRequest) == FALSE) {
        return NOT_SUPPORTED;
    }

    char *domainStr = getDomainStr(clientRequest);

    if (domainStr == NULL) {

        return FAILED;
    }

    int isInFilterStatus;

    if (isdigit(domainStr[0])) {

        isInFilterStatus = isInFilter(filterNumDomains, domainStr);

        if (isInFilterStatus == FAILED) {

            free(domainStr);
            return FAILED;
        }

        if (isInFilterStatus == TRUE) {

            free(domainStr);
            return FORBIDDEN;
        }

    } else {

        isInFilterStatus = isInFilter(filterStrDomains, domainStr);

        if (isInFilterStatus == FAILED) {

            free(domainStr);
            return FAILED;
        }

        if (isInFilterStatus == TRUE) {

            free(domainStr);
            return FORBIDDEN;
        }

        struct hostent *hostPointer = NULL;
        struct in_addr host_in_address;

        hostPointer = (struct hostent *) gethostbyname(domainStr);

        if (hostPointer == NULL) {

            free(domainStr);
            return NOT_FOUND;
        }

        host_in_address.s_addr = ((struct in_addr *) (hostPointer->h_addr_list[0]))->s_addr;
        char *hostStrIp = inet_ntoa(host_in_address);

        isInFilterStatus = isInFilter(filterNumDomains, hostStrIp);

        if (isInFilterStatus == FAILED) {

            free(domainStr);
            return FAILED;
        }

        if (isInFilterStatus == TRUE) {

            free(domainStr);
            return FORBIDDEN;
        }

    }

    free(domainStr);

    return NO_ERRORS;
}

// This function receives two arguments: 1- the error status code, 2- the socket descriptor to write the error in for the client
// the method builds the error message template, and it fills it with the proper error information according to the received error status code,
// and it writes that error to the SD of the client
int statusErrorHandler(int statusCode, int SD) {

    char errorStr[STATUS_CODE_STR_LEN] = EMPTY_STR;
    memset(errorStr, '\0', STATUS_CODE_STR_LEN);

    char errorDescription[STATUS_CODE_STR_LEN] = EMPTY_STR;
    memset(errorDescription, '\0', STATUS_CODE_STR_LEN);

    char bodyContent[BODY_CONTENT_LEN] = EMPTY_STR;
    memset(bodyContent, '\0', BODY_CONTENT_LEN);

    char headersContent[HEADERS_CONTENT_LEN] = EMPTY_STR;
    memset(bodyContent, '\0', HEADERS_CONTENT_LEN);

    char responseContent[RESPONSE_CONTENT_LEN] = EMPTY_STR;
    memset(responseContent, '\0', RESPONSE_CONTENT_LEN);

    switch (statusCode) {

        case BAD_REQUEST:
            strcpy(errorStr, "400 Bad Request");
            strcpy(errorDescription, "Bad Request.");
            break;

        case FORBIDDEN:
            strcpy(errorStr, "403 Forbidden");
            strcpy(errorDescription, "Access denied.");
            break;

        case NOT_FOUND:
            strcpy(errorStr, "404 Not Found");
            strcpy(errorDescription, "File not found.");
            break;

        case INTERNAL_SERVER_ERROR:
            strcpy(errorStr, "500 Internal Server Error");
            strcpy(errorDescription, "Some server side error.");
            break;

        case NOT_SUPPORTED:
            strcpy(errorStr, "501 Not supported");
            strcpy(errorDescription, "Method is not supported.");
            break;

        default:
            return FAILED;
    }


    sprintf(bodyContent, "<HTML><HEAD><TITLE>%s</TITLE></HEAD>\r\n<BODY><H4>%s</H4>\r\n%s\r\n</BODY></HTML>\r\n",
            errorStr, errorStr, errorDescription);

    sprintf(headersContent, "HTTP/1.0 %s\r\nContent-Type: text/html\r\nContent-Length: %d\r\nConnection: close\r\n\r\n",
            errorStr, (int) strlen(bodyContent));

    strcpy(responseContent, headersContent);
    strcat(responseContent, bodyContent);

    if (write(SD, responseContent, (int) strlen(responseContent)) == -1) {
        perror("error: write\n");
        return FAILED;
    }

    return SUCCEEDED;
}


// This function receives two arguments: 1- for the linkList of the filter items, 2- a string pointer to check if its exists in the filter linkList
// the function loops through the filter linkList and checks if the domain string exists in that linkList
// if the items linkList is subnets linkList it makes the needed string processing to split the subnet ip from the subnet mask length
int isInFilter(LinkList *filterDomainsList, char *domainStr) {

    int isIP = FALSE;
    int maskLen;
    int belongsToSubStatus;

    int sectionsNum;
    char **sectionsArray;
    char splitStr[] = "/";

    if (isdigit(domainStr[0])) {
        isIP = TRUE;
    }

    Node *currNode;
    for (currNode = filterDomainsList->head; currNode != NULL; currNode = currNode->next) {

        if (isIP == TRUE) {

            sectionsNum = sectionsCount(currNode->data, splitStr);

            sectionsArray = toSectionsArray(currNode->data, splitStr, EMPTY_STR);

            if (sectionsArray == NULL) {

                return FAILED;
            }

            maskLen = ((int) strtoul(sectionsArray[1], (char **) NULL, 10));

            belongsToSubStatus = belongsToSub(domainStr, sectionsArray[0], maskLen);

            if (belongsToSubStatus == FAILED) {
                freeAll(sectionsArray, sectionsNum);
                return FAILED;
            }

            if (belongsToSubStatus == TRUE) {
                freeAll(sectionsArray, sectionsNum);
                return TRUE;
            }

            freeAll(sectionsArray, sectionsNum);

        } else if (strcmp(currNode->data, domainStr) == 0) {

            return TRUE;
        }

    }

    return FALSE;
}

// This function receives one argument: 1- a pointer to the ip version 4 string
// the function makes the needed string processing to split the string ip and convert its parts to unsigned integers
// then it makes the needed bitwise operations to store the values in one uint32_t, and it returns it
uint32_t strIpToUint32_t(char *strIP) {

    uint32_t uint32_IP = 0;
    const int ipV4PartsNum = 4;

    char splitStr1[] = ".";

    int sectionsNum = sectionsCount(strIP, splitStr1);

    char **sectionsArray = toSectionsArray(strIP, splitStr1, EMPTY_STR);

    if (sectionsArray == NULL) {

        return FAILED;
    }

    int i;
    for (i = 0; i < sectionsNum; i++) {

        uint32_IP =
                uint32_IP | (((int) strtoul(sectionsArray[i], (char **) NULL, 10)) << (8 * (ipV4PartsNum - (i + 1))));
    }

    freeAll(sectionsArray, sectionsNum);

    return uint32_IP;
}

// This function receives three arguments: 1- a string pointer to the ip that we want to check if it belongs to a specific subnet,
// 2- a string pointer to the subnet ip, 3- the subnet mask length of the subnet
// the converts the two ips to uint_32 using the private function strIpToUint32_t() and it applies the needed bitwise operations
// on the uint32_t ips presentation to check if the ip belongs to the subnet
int belongsToSub(char *ip, char *subIP, int subMaskLen) {

    uint32_t uint32_ip = strIpToUint32_t(ip);

    if ((int) uint32_ip == FAILED) {
        return FAILED;
    }

    uint32_t uint32_subIP = strIpToUint32_t(subIP);

    if ((int) uint32_subIP == FAILED) {
        return FAILED;
    }

    int shift = IP_V4_LEN - subMaskLen;

    uint32_ip = uint32_ip >> shift;
    uint32_subIP = uint32_subIP >> shift;

    if ((uint32_ip ^ uint32_subIP) == 0) {
        return TRUE;
    }

    return FALSE;
}

// This function receives one argument: 1- a pointer to the http request string
// the function makes the needed string processing, and it checks if the request is GET method request
// the string processing done by the help of my private string processing function
int isGETMethod(char *requestStr) {

    char splitStr1[] = "\r\n";
    char splitStr2[] = " ";
    char GETMethodStr[] = "GET";

    char *requestStrOneSpace = getOneSpaceCopy(requestStr);

    if (requestStrOneSpace == NULL) {
        return FAILED;
    }

    strTrim(requestStrOneSpace);

    int reqSectionsNum = sectionsCount(requestStrOneSpace, splitStr1);

    char **reqSectionsArray = toSectionsArray(requestStrOneSpace, splitStr1, EMPTY_STR);

    if (reqSectionsArray == NULL) {

        free(requestStrOneSpace);
        return FAILED;
    }

    strTrim(reqSectionsArray[0]);

    int firstLineSectionsNum = sectionsCount(reqSectionsArray[0], splitStr2);

    char **firstLineSectionsArray = toSectionsArray(reqSectionsArray[0], splitStr2, EMPTY_STR);

    if (firstLineSectionsArray == NULL) {

        freeAll(reqSectionsArray, reqSectionsNum);
        free(requestStrOneSpace);
        return FAILED;
    }

    if (strcmp(firstLineSectionsArray[0], GETMethodStr) == 0) {

        freeAll(reqSectionsArray, reqSectionsNum);
        freeAll(firstLineSectionsArray, firstLineSectionsNum);
        free(requestStrOneSpace);
        return TRUE;
    }

    freeAll(reqSectionsArray, reqSectionsNum);
    freeAll(firstLineSectionsArray, firstLineSectionsNum);
    free(requestStrOneSpace);

    return FALSE;
}

// This function receives one argument: 1- a pointer to the http request string
// the function makes the needed string processing, and it checks if the request is a BadRequest
// BadRequest [in our case]: not contains a method or a path or a protocol or a host header
// the string processing done by the help of my private string processing function
int isBadRequest(char *requestStr) {

    int isFirstLineCorrect = FALSE;
    int isHostHeaderExists = FALSE;

    char splitStr1[] = "\r\n";
    char splitStr2[] = " ";
    char splitStr3[] = "host:";

    char *requestStrOneSpace = getOneSpaceCopy(requestStr);

    if (requestStrOneSpace == NULL) {
        return FAILED;
    }

    strTrim(requestStrOneSpace);

    int reqSectionsNum = sectionsCount(requestStrOneSpace, splitStr1);

    char **reqSectionsArray = toSectionsArray(requestStrOneSpace, splitStr1, EMPTY_STR);

    if (reqSectionsArray == NULL) {

        free(requestStrOneSpace);
        return FAILED;
    }

    strTrim(reqSectionsArray[0]);

    int firstLineSectionsNum = sectionsCount(reqSectionsArray[0], splitStr2);

    char **firstLineSectionsArray = toSectionsArray(reqSectionsArray[0], splitStr2, EMPTY_STR);

    if (firstLineSectionsArray == NULL) {

        freeAll(reqSectionsArray, reqSectionsNum);
        free(requestStrOneSpace);
        return FAILED;
    }

    if (firstLineSectionsNum == HTTP_REQ_FL_ITEMS_NUM) {

        int isFirstLineItemsNotEmpty = TRUE;

        int i;
        for (i = 0; i < firstLineSectionsNum; i++) {
            if (strcmp(firstLineSectionsArray[i], EMPTY_STR) == 0) {
                isFirstLineItemsNotEmpty = FALSE;
            }
        }

        if (isFirstLineItemsNotEmpty == TRUE) {
            if ((firstLineSectionsArray[1][0] == '/') && (strcmp(firstLineSectionsArray[2], HTTP_REQ_V0_STR) == 0 ||
                                                          strcmp(firstLineSectionsArray[2], HTTP_REQ_V1_STR) == 0)) {

                isFirstLineCorrect = TRUE;
            }
        }
    }

    freeAll(reqSectionsArray, reqSectionsNum);
    freeAll(firstLineSectionsArray, firstLineSectionsNum);

    if (isFirstLineCorrect == TRUE) {

        char *lowerCaseStr = getLowerCaseCopy(requestStrOneSpace);

        if (lowerCaseStr == NULL) {

            free(requestStrOneSpace);
            return FAILED;
        }

        if (sectionsCount(lowerCaseStr, splitStr3) >= 2) {

            isHostHeaderExists = TRUE;
        }

        free(lowerCaseStr);
    }

    free(requestStrOneSpace);

    if ((isFirstLineCorrect == TRUE) && (isHostHeaderExists == TRUE)) {
        return FALSE;
    }

    return TRUE;
}

// This function receives two argument: 1- a pointer to the linkList, 2- a pointer to the string that we want to add to the linkList
// the function creates a new node and puts the data in it then it adds it to the end of the link list
int add(LinkList *link_list, char *data) {

    Node *new_node = malloc(sizeof(Node));
    if (new_node == NULL) {
        fprintf(stdout, "Allocation failure: malloc failed\n");
        return FAILED;
    }

    *new_node = (Node) {data, NULL};

    if (link_list->head == NULL) {
        link_list->head = new_node;
        link_list->tail = new_node;
    } else {
        link_list->tail->next = new_node;
        link_list->tail = new_node;
    }

    link_list->size++;
    return SUCCEEDED;
}

// This function receives one argument: 1- a pointer to the http request string
// the function makes the needed string processing, and it returns the domain name from the request
// the string processing done by the help of my private string processing function
char *getDomainStr(char *requestStr) {

    char *requestStrOneSpace = getOneSpaceCopy(requestStr);

    if (requestStrOneSpace == NULL) {
        return NULL;
    }

    strTrim(requestStrOneSpace);

    char splitStr1[] = "\r\n";
    char splitStr2[] = " ";
    char splitStr3[] = ":";
    char hostHeaderStr[] = "host:";

    int tempSectionsNum;
    char **tempSectionsArray;

    int sectionsNumSplit1 = sectionsCount(requestStrOneSpace, splitStr1);
    char **sectionsArraySplit1 = toSectionsArray(requestStrOneSpace, splitStr1, EMPTY_STR);

    int sectionsNumSplit3;
    char **sectionsArraySplit3;

    if (sectionsArraySplit1 == NULL) {

        free(requestStrOneSpace);
        return NULL;
    }

    int i;
    for (i = 0; i < sectionsNumSplit1; i++) {

        strTrim(sectionsArraySplit1[i]);

        tempSectionsNum = sectionsCount(sectionsArraySplit1[i], splitStr2);
        tempSectionsArray = toSectionsArray(sectionsArraySplit1[i], splitStr2, EMPTY_STR);

        if (tempSectionsArray == NULL) {

            freeAll(sectionsArraySplit1, sectionsNumSplit1);
            free(requestStrOneSpace);
            return NULL;
        }

        if (tempSectionsNum >= 2) {

            if (strcasecmp(tempSectionsArray[0], hostHeaderStr) == 0) {

                char *domainNameStr;

                sectionsNumSplit3 = sectionsCount(tempSectionsArray[1], splitStr3);

                if (sectionsNumSplit3 == 2) {

                    sectionsArraySplit3 = toSectionsArray(tempSectionsArray[1], splitStr3, EMPTY_STR);
                    domainNameStr = (char *) malloc(sizeof(char) * (strlen(sectionsArraySplit3[0]) + 1));
                    strcpy(domainNameStr, sectionsArraySplit3[0]);

                    freeAll(sectionsArraySplit3, sectionsNumSplit3);
                } else {

                    domainNameStr = (char *) malloc(sizeof(char) * (strlen(tempSectionsArray[1]) + 1));
                    strcpy(domainNameStr, tempSectionsArray[1]);
                }

                if (domainNameStr == NULL) {
                    fprintf(stdout, "Allocation failure: malloc failed\n");
                    freeAll(sectionsArraySplit1, sectionsNumSplit1);
                    freeAll(tempSectionsArray, tempSectionsNum);
                    free(requestStrOneSpace);
                    return NULL;
                }

                freeAll(sectionsArraySplit1, sectionsNumSplit1);
                freeAll(tempSectionsArray, tempSectionsNum);
                free(requestStrOneSpace);

                return domainNameStr;
            }
        }

        freeAll(tempSectionsArray, tempSectionsNum);
    }

    freeAll(sectionsArraySplit1, sectionsNumSplit1);
    free(requestStrOneSpace);

    return NULL;
}

// This function receives one argument: 1- a pointer to the http request string
// the function makes the needed string processing, and it returns the file path from the request
// the string processing done by the help of my private string processing function
char *getFilePath(char *requestStr) {

    char *requestStrOneSpace = getOneSpaceCopy(requestStr);

    if (requestStrOneSpace == NULL) {
        return NULL;
    }

    char splitStr1[] = "\r\n";
    char splitStr2[] = " ";

    int sectionsNumSplit1 = sectionsCount(requestStrOneSpace, splitStr1);
    char **sectionsArraySplit1 = toSectionsArray(requestStrOneSpace, splitStr1, EMPTY_STR);

    if (sectionsArraySplit1 == NULL) {

        free(requestStrOneSpace);
        return NULL;
    }

    strTrim(sectionsArraySplit1[0]);

    int sectionsNumSplit2 = sectionsCount(sectionsArraySplit1[0], splitStr2);
    char **sectionsArraySplit2 = toSectionsArray(sectionsArraySplit1[0], splitStr2, EMPTY_STR);

    if (sectionsArraySplit2 == NULL) {

        freeAll(sectionsArraySplit1, sectionsNumSplit1);
        free(requestStrOneSpace);
        return NULL;
    }

    char *filePath = (char *) malloc(sizeof(char) * (strlen(sectionsArraySplit2[1]) + 1));

    if (filePath == NULL) {

        fprintf(stdout, "Allocation failure: malloc failed\n");
        freeAll(sectionsArraySplit1, sectionsNumSplit1);
        freeAll(sectionsArraySplit2, sectionsNumSplit2);
        free(requestStrOneSpace);
        return NULL;
    }

    strcpy(filePath, sectionsArraySplit2[1]);

    freeAll(sectionsArraySplit1, sectionsNumSplit1);
    freeAll(sectionsArraySplit2, sectionsNumSplit2);
    free(requestStrOneSpace);

    return filePath;
}

// This function receives one argument: 1- a pointer for the string of the file path
// This function creates the all the not existing directories and the file of the path
// if the file name is '/' it calls it "index.html"
// it returns SUCCEEDED in success and FAILED in failure.
int createFilePath(char *filePath) {

    char tempFilePath[MAX_SENTENCE_LEN] = EMPTY_STR;
    char splitStr[] = "/";

    int pathItemsNum = sectionsCount(filePath, splitStr);

    char **pathItemsArray = toSectionsArray(filePath, splitStr, EMPTY_STR);

    if (pathItemsArray == NULL) {
        return FAILED;
    }

    int i;
    for (i = 0; i < pathItemsNum; i++) {

        if ((i == (pathItemsNum - 1)) && (strcmp(pathItemsArray[i], FORWARD_SLASH_STR) == 0)) {
            strcat(tempFilePath, DEFAULT_FILE_STR);
        } else {
            strcat(tempFilePath, pathItemsArray[i]);
        }

        if (isFileExists(tempFilePath) == FALSE) {

            if (i != (pathItemsNum - 1)) {
                if (mkdir(tempFilePath, 0700) == -1) {
                    fprintf(stdout, "Error: Failed To Create Directory\n");
                    freeAll(pathItemsArray, pathItemsNum);
                    return FAILED;
                }
            } else {

                FILE *fp = fopen(tempFilePath, (char *) "w");

                if (fp == NULL) {
                    fprintf(stdout, "Error: Failed To Create The File\n");
                    freeAll(pathItemsArray, pathItemsNum);
                    return FAILED;
                }

                fclose(fp);
            }
        }

        if (i != (pathItemsNum - 1)) {
            strcat(tempFilePath, splitStr);
        }
    }

    freeAll(pathItemsArray, pathItemsNum);

    int isFileExistsStatus = isFileExists(tempFilePath);

    if (isFileExistsStatus == TRUE) {

        return SUCCEEDED;
    }

    return FAILED;
}

// This function receives one argument: 1- a pointer to the filePath string
// The function checks if the file exists in the local filesystem if true it returns TRUE, else it returns FALSE.
int isFileExists(char *filePath) {

    if (access(filePath, F_OK) == 0) {
        return TRUE;
    }

    return FALSE;
}

///////////////////////////////////////   String Processing Methods   ///////////////////////////////////////

// This function receives one argument: 1- a pointer to a string
// the function returns a one space copy of that string which means where there are a sequence of spaces it converts it to one space
char *getOneSpaceCopy(char *str) {

    int strLen = (int) strlen(str);

    char *tempLowerCaseStr = (char *) malloc(sizeof(char) * (strLen + 1));

    if (tempLowerCaseStr == NULL) {
        fprintf(stdout, "Allocation failure: malloc failed\n");
        return NULL;
    }

    memset(tempLowerCaseStr, '\0', (strLen + 1));

    int inSpace = FALSE;

    int i, j;
    for (i = 0, j = 0; i < strLen; i++) {

        if (inSpace == TRUE && str[i] == SPACE_CHAR) {
            continue;
        }

        if (inSpace == FALSE && str[i] == SPACE_CHAR) {
            tempLowerCaseStr[j] = str[i];
            inSpace = TRUE;
            j++;
            continue;
        }

        if ((inSpace == TRUE && str[i] != SPACE_CHAR) || (inSpace == FALSE && str[i] != SPACE_CHAR)) {
            tempLowerCaseStr[j] = str[i];
            inSpace = FALSE;
            j++;
            continue;
        }
    }

    char *oneSpaceStr = (char *) malloc(sizeof(char) * (strlen(tempLowerCaseStr) + 1));

    if (oneSpaceStr == NULL) {
        fprintf(stdout, "Allocation failure: malloc failed\n");
        free(tempLowerCaseStr);
        return NULL;
    }

    strcpy(oneSpaceStr, tempLowerCaseStr);

    free(tempLowerCaseStr);

    return oneSpaceStr;
}

// This function receives one argument: 1- a pointer to a string
// the function returns a lower case copy of the given string
char *getLowerCaseCopy(char *str) {

    int strLen = (int) strlen(str);

    char *lowerCaseStr = (char *) malloc(sizeof(char) * (strLen + 1));

    if (lowerCaseStr == NULL) {
        fprintf(stdout, "Allocation failure: malloc failed\n");
        return NULL;
    }

    int i;
    for (i = 0; i < strLen; i++) {

        if (str[i] >= 'A' && str[i] <= 'Z') {
            lowerCaseStr[i] = (char) (str[i] + 32);
        } else {
            lowerCaseStr[i] = str[i];
        }
    }

    lowerCaseStr[strLen] = '\0';

    return lowerCaseStr;
}

// This function receives one argument: 1- a pointer to a string
// the function removes the spaces at the beginning and at the end of the string, and it stores it in its memory area
// that means that the function applies a shift for the string to the left
void strTrim(char *sentence) {

    int sentenceLen = (int) strlen(sentence);

    if (sentenceLen == 0) {
        return;
    }

    int firstCharIndex = 0;
    int lastCharIndex = 0;
    int sentenceStarted = FALSE;

    int i, j;
    for (i = 0, j = 0; i < sentenceLen; i++) {

        if (sentenceStarted == FALSE && sentence[i] != SPACE_CHAR) {

            sentenceStarted = TRUE;
            firstCharIndex = i;
        }

        if (sentenceStarted == TRUE) {

            if (sentence[i] != SPACE_CHAR) {
                lastCharIndex = i;
            }

            sentence[j] = sentence[i];
            j++;
        }
    }

    int newSentenceLen = lastCharIndex - firstCharIndex + 1;

    sentence[newSentenceLen] = '\0';

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

    if (sectionsArray == NULL) {
        fprintf(stdout, "Allocation failure: malloc failed\n");
        return NULL;
    }

    int sectionsArrayCurrIndex = 0;

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

                    if (sectionsArray[sectionsArrayCurrIndex] == NULL) {
                        freeAll(sectionsArray, sectionsArrayCurrIndex);
                        return NULL;
                    }
                } else {
                    sectionsArray[sectionsArrayCurrIndex] = getSectionStr(defStr, &defStrStartIndex, &defStrLastIndex);

                    if (sectionsArray[sectionsArrayCurrIndex] == NULL) {
                        freeAll(sectionsArray, sectionsArrayCurrIndex);
                        return NULL;
                    }
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

    if (sectionsArray[sectionsArrayCurrIndex] == NULL) {
        freeAll(sectionsArray, sectionsArrayCurrIndex);
        return NULL;
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
        fprintf(stdout, "Allocation failure: malloc failed\n");
        return NULL;
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

    if (sectionsArrayPointer == NULL || sectionsArrayLen <= 0) {
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
