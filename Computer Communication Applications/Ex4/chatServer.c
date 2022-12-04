// Program Name: Chat Server [Ex4]
// Program Description: This program is an Event-Driven Chat Server that the clients can
// connect to and start chatting with each other any message that the client sends will be sent by the
// chat server for all other clients that are connected to the server

// Student Name: Muath Abu Jamal

#include <sys/socket.h>
#include <stdio.h>
#include <netinet/in.h>
#include <unistd.h>
#include <signal.h>
#include <sys/ioctl.h>
#include <stdlib.h>
#include <string.h>
#include "chatServer.h"

// declaring preprocessor directives
#define TRUE 1
#define FALSE 0
#define FAILED (-1)
#define SUCCEEDED 1
#define MAX_LISTEN_QUEUE 10
#define CORRECT_ARGS_COUNT 2
#define EMPTY_STR ""

// declaring the needed functions (explanation for each function bellow(where each function written))
int serverInitialize(int);

char *readFromOrCloseIfClient(int SD, conn_pool_t *);

void freeMsgsList(conn_t *);

void updateMaxConnPoolFD(int, conn_pool_t *);

static int end_server = 0;

// This function receives one argument: 1- the number(id) of the signal
// the function applies the private scenario when the signal SIG_INT received (in our case the server need to be closed)
void intHandler(int SIG_INT) {

    if (SIG_INT == SIGINT){
        end_server = TRUE;
    }

    //end_server = TRUE;
}

// The main function initializes the chat server by calling the needed private functions,
// and it starts to make the needed actions when there is an event
int main(int argc, char *argv[]) {

    if (argc != CORRECT_ARGS_COUNT) {

        fprintf(stdout, "Usage: chatServer <port>\n");
        exit(EXIT_FAILURE);
    }

    int portNum = (int) strtol(argv[1], (char **) NULL, 10);

    if(portNum < 0){

        fprintf(stdout, "Usage: chatServer <port>\n");
        exit(EXIT_FAILURE);
    }

    signal(SIGINT, intHandler);

    conn_pool_t *pool = malloc(sizeof(conn_pool_t) * 1);

    if (pool == NULL) {

        perror("error: malloc\n");
        exit(EXIT_FAILURE);
    }

    init_pool(pool);

    int welcomeSD = serverInitialize(portNum);

    if (welcomeSD == FAILED) {

        free(pool);
        exit(EXIT_FAILURE);
    }

    FD_SET(welcomeSD, &(pool->read_set));

    int currSD;
    int newConnFD;
    char *tempClientMsg;
    do {

        updateMaxConnPoolFD(welcomeSD, pool);

        pool->ready_read_set = pool->read_set;
        pool->ready_write_set = pool->write_set;

        printf("Waiting on select()...\nMaxFd %d\n", pool->maxfd);
        if ((pool->nready = select(pool->maxfd + 1, &(pool->ready_read_set), &(pool->ready_write_set), NULL, NULL)) == -1) {

            break;
        }

        for (currSD = 0; (currSD < (pool->maxfd + 1)) && (pool->nready > 0); currSD++) {

            if (FD_ISSET(currSD, &(pool->ready_read_set))) {

                if (currSD == welcomeSD) {

                    newConnFD = accept(welcomeSD, NULL, NULL);

                    printf("New incoming connection on sd %d\n", newConnFD);

                    if (newConnFD >= 0) {

                        if (add_conn(newConnFD, pool) == 0) {

                            FD_SET(newConnFD, &(pool->read_set));
                        }
                    }

                    FD_CLR(welcomeSD, &(pool->ready_read_set));
                    pool->nready--;
                } else {

                    tempClientMsg = readFromOrCloseIfClient(currSD, pool);
                    pool->nready--;

                    if (tempClientMsg == NULL) {

                        continue;
                    }

                    add_msg(currSD, tempClientMsg, (int) strlen(tempClientMsg), pool);
                    free(tempClientMsg);
                }
            }

            /*******************************************************/
            /* Check to see if this descriptor is ready for write  */
            /*******************************************************/
            if (FD_ISSET(currSD, &(pool->ready_write_set))) {

                write_to_client(currSD, pool);
                pool->nready--;
            }
            /*******************************************************/


        } /* End of loop through selectable descriptors */

    } while (end_server == FALSE);


    conn_t *currConn;
    for (currConn = pool->conn_head; currConn != NULL; currConn = pool->conn_head) {

        printf("removing connection with sd %d \n", currConn->fd);
        remove_conn(currConn->fd, pool);
    }

    FD_ZERO(&(pool->read_set));
    FD_ZERO(&(pool->ready_read_set));
    FD_ZERO(&(pool->write_set));
    FD_ZERO(&(pool->ready_write_set));

    free(pool);

    close(welcomeSD);

    return 0;
}


// This function receives one argument: 1- a pointer to a connections pool
// the method initializes all the conn_pool_t attributes, and it zeros all the fd_sets
int init_pool(conn_pool_t *pool) {

    pool->maxfd = -1;
    pool->nready = 0;
    FD_ZERO(&(pool->read_set));
    FD_ZERO(&(pool->ready_read_set));
    FD_ZERO(&(pool->write_set));
    FD_ZERO(&(pool->ready_write_set));
    pool->conn_head = NULL;
    pool->nr_conns = 0;

    return 0;
}

// This function receives two arguments: 1- the new connection socket descriptor, 2- a pointer ot the connections pool
// the function builds a new connection, and it adds it to the connections pool duple linkedList
int add_conn(int sd, conn_pool_t *pool) {
    /*
     * 1. allocate connection and init fields
     * 2. add connection to pool
     * */

    conn_t *newConn = (conn_t *) malloc(sizeof(conn_t) * 1);

    if (newConn == NULL) {

        return FAILED;
    }

    newConn->fd = sd;
    newConn->write_msg_head = NULL;
    newConn->write_msg_tail = NULL;
    newConn->prev = NULL;
    newConn->next = NULL;

    if (pool->conn_head != NULL) {

        pool->conn_head->prev = newConn;
    }

    newConn->next = pool->conn_head;
    pool->conn_head = newConn;
    pool->nr_conns++;

    return 0;
}

// This function receives two arguments: 1- the connection socket descriptor, 2- a pointer to the pool connection
// the function removes the connection of the given socket descriptor from the duple linkedList of the connections
int remove_conn(int sd, conn_pool_t *pool) {
    /*
    * 1. remove connection from pool
    * 2. deallocate connection
    * 3. remove from sets
    * 4. update max_fd if needed
    */

    conn_t *currConn;

    for (currConn = pool->conn_head; currConn != NULL; currConn = currConn->next) {

        if (currConn->fd == sd) {

            if (currConn->prev == NULL) {

                pool->conn_head = currConn->next;
                if (pool->conn_head != NULL) {
                    pool->conn_head->prev = NULL;
                }
            } else {

                currConn->prev->next = currConn->next;
                if (currConn->next != NULL) {
                    currConn->next->prev = currConn->prev;
                }
            }

            freeMsgsList(currConn);
            free(currConn);
            break;
        }
    }

    FD_CLR(sd, &(pool->read_set));
    FD_CLR(sd, &(pool->write_set));
    FD_CLR(sd, &(pool->ready_read_set));
    FD_CLR(sd, &(pool->ready_write_set));
    pool->nr_conns--;

    close(sd);

    return 0;
}

// This function receives two arguments: 1- the welcome socket descriptor, 2- a pointer to the connections pool
// the function updates the pool->maxFD by looping through the hole connections linkedList and getting the max sd
// then comparing it with the welcome socket descriptor
void updateMaxConnPoolFD(int welcomeSD, conn_pool_t *pool) {

    int maxFD = -1;

    conn_t *currConn;

    for (currConn = pool->conn_head; currConn != NULL; currConn = currConn->next) {

        if (currConn->fd > maxFD) {
            maxFD = currConn->fd;
        }
    }

    if (welcomeSD > maxFD) {

        maxFD = welcomeSD;
    }

    pool->maxfd = maxFD;
}

// This function receives one argument: 1- a pointer to the connection that we want to free all of its messages queue
// the function goes through the hole queue, and it frees all the messages and there structs
void freeMsgsList(conn_t *currConn) {

    msg_t *currMsg;
    for (currMsg = currConn->write_msg_head; currMsg != NULL; currMsg = currConn->write_msg_head) {

        currConn->write_msg_head = currConn->write_msg_head->next;

        if(currConn->write_msg_head != NULL){
            currConn->write_msg_head->prev = NULL;
        }

        free(currMsg->message);
        free(currMsg);
    }

    currConn->write_msg_head = NULL;
    currConn->write_msg_tail = NULL;
}


// This function receives four arguments: 1- the connection socket descriptor,
// 2- the buffer of the message that we want to add it to the connection messages queue
// 3- the length of the buffer that we want to send to the client
// 4- a pointer to the connections pool
// the function makes a copy of the message, and it adds it to the head of the messages queue of the connection
int add_msg(int sd, char *buffer, int len, conn_pool_t *pool) {

    /*
     * 1. add msg_t to write queue of all other connections
     * 2. set each fd to check if ready to write
     */

    msg_t *tempMsg_t;

    conn_t *currConn;
    for (currConn = pool->conn_head; currConn != NULL; currConn = currConn->next) {

        if (currConn->fd != sd) {

            tempMsg_t = (msg_t *) malloc(sizeof(msg_t) * 1);

            if (tempMsg_t == NULL) {

                return FAILED;
            }

            tempMsg_t->prev = NULL;
            tempMsg_t->next = NULL;
            tempMsg_t->size = len;

            tempMsg_t->message = (char *) malloc(sizeof(char) * (len + 1));

            if (tempMsg_t->message == NULL) {

                free(tempMsg_t);
                return FAILED;
            }

            memset(tempMsg_t->message, '\0', (len + 1));

            strcpy(tempMsg_t->message, buffer);

            if (currConn->write_msg_head == NULL) {

                currConn->write_msg_head = tempMsg_t;
                currConn->write_msg_tail = tempMsg_t;
            } else {

                tempMsg_t->next = currConn->write_msg_head;
                currConn->write_msg_head = tempMsg_t;
            }

            FD_SET(currConn->fd, &(pool->write_set));
        }
    }

    return 0;
}

// This function receives two arguments: 1- the socket descriptor of the connection that we want to write its messages, 2- a pointer to the connections pool
// the function tries to write all the messages in the messages queue to the client
int write_to_client(int sd, conn_pool_t *pool) {

    /*
     * 1. write all msgs in queue
     * 2. deallocate each writen msg
     * 3. if all msgs were writen successfully, there is nothing else to write to this fd... */

    int writeStatus = SUCCEEDED;

    msg_t *currMsg;
    conn_t *currConn;
    for (currConn = pool->conn_head; currConn != NULL; currConn = currConn->next) {

        if (currConn->fd == sd) {

            for (currMsg = currConn->write_msg_tail; currMsg != NULL; currMsg = currConn->write_msg_tail) {

                if (write(sd, currMsg->message, currMsg->size) == -1) {

                    writeStatus = FAILED;
                    break;
                }

                currConn->write_msg_tail = currConn->write_msg_tail->prev;

                if (currConn->write_msg_tail == NULL) {

                    currConn->write_msg_head = NULL;
                } else {

                    currConn->write_msg_tail->next =NULL;
                }

                free(currMsg->message);
                free(currMsg);
            }

            if (writeStatus == SUCCEEDED) {

                FD_CLR(currConn->fd, &(pool->ready_write_set));
                FD_CLR(currConn->fd, &(pool->write_set));
            }

            break;
        }
    }

    return 0;
}

// This function receives one argument: 1- the port number that we want to initialize the server with
// the function creates the welcomeSocket for the server it makes the socket non-blocking as asked in the question
// then it binds the welcome socket with the proper sockadd_in as asked in the question and with the given port number at it makes it listen
// at maximum for the value of the defined macro MAX_LISTEN_QUEUE at the end it returns the welcomeSD
int serverInitialize(int portNum) {

    int welcomeSD;
    if ((welcomeSD = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("error: socket\n");
        return FAILED;
    }

    int on = 1;

    if (ioctl(welcomeSD, (int) FIONBIO, (char *) &on) == -1) {

        perror("error: ioctl\n");
        close(welcomeSD);
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

// This function receives two  argument: 1- the socket descriptor of the client, 2- a pointer to the connections pool
// the function reads the client request till the "\r\n" it saves that request, and it returns a pointer to it
// also it prints the needed messages as asked in the question
char *readFromOrCloseIfClient(int SD, conn_pool_t *pool) {

    int bytesNum;

    char clientMsgStr[BUFFER_SIZE + 1] = EMPTY_STR;
    memset(clientMsgStr, '\0', BUFFER_SIZE + 1);

    printf("Descriptor %d is readable\n", SD);

    bytesNum = (int) read(SD, clientMsgStr, BUFFER_SIZE);

    printf("%d bytes received from sd %d\n", bytesNum, SD);

    if (bytesNum <= 0) {

        if (bytesNum == 0) {
            printf("Connection closed for sd %d\n", SD);
            printf("removing connection with sd %d \n", SD);
        } else {

            printf("removing connection with sd %d \n", SD);
        }

        remove_conn(SD, pool);
        return NULL;
    }

    char *finalClientMsgStr = (char *) malloc(sizeof(char) * (strlen(clientMsgStr) + 1));

    if (finalClientMsgStr == NULL) {

        return NULL;
    }

    strcpy(finalClientMsgStr, clientMsgStr);

    return finalClientMsgStr;
}

