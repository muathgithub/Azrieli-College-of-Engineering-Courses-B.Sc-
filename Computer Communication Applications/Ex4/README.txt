== Student Details ==

Student Name: Muath Abu Jamal


== Program Name & Discription ==

Program Name: Chat Server [Ex4]

Program Description: This program is an Event-Driven Chat Server that the clients can
connect to and start chatting with each other any message that the client sends will be sent by the
chat server for all other clients that are connected to the server.


== Program Files ==

1. chatServer.c - This file contains the chat server code which includes the implementations of the functions
		that are given in the file chatServer.h beside all the needed initializations like initializing the 
		server socket and the connections pool. then it satrts to accept the clients and dealing with them.

3. README.txt - This file contains the student and program details.


== Program Functions ==

1. int init_pool(conn_pool_t *) - This function receives one argument: 
	1- a pointer to a connections pool.

	the method initializes all the conn_pool_t attributes, and it zeros all the fd_sets.

2. int serverInitialize(int) - This function receives one argument: 
	1- the port number that we want to initialize the server with.

	the function creates the welcomeSocket for the server it makes the socket non-blocking as 
	asked in the question then it binds the welcome socket with the proper sockadd_in as asked in 
	the question and with the given port number at it makes it listen at maximum for the value of the 
	defined macro MAX_LISTEN_QUEUE at the end it returns the welcomeSD.

3. char *readFromOrCloseIfClient(int, conn_pool_t *) - This function receives two  argument: 
	1- the socket descriptor of the client, 
	2- a pointer to the connections pool.

	the function reads the client request till the "\r\n" it saves that request, 
	and it returns a pointer to it also it prints the needed messages 
	as asked in the question.

4. int add_conn(int, conn_pool_t *) - This function receives two arguments: 
	1- the new connection socket descriptor, 
	2- a pointer ot the connections pool.

	the function builds a new connection, and it adds it to the connections pool duple linkedList.

5. int remove_conn(int, conn_pool_t *) - This function receives two arguments: 
	1- the connection socket descriptor, 
	2- a pointer to the pool connection.

	the function removes the connection of the given socket descriptor from the 
	duple linkedList of the connections.

6. void updateMaxConnPoolFD(int, conn_pool_t *) - This function receives two arguments: 
	1- the welcome socket descriptor, 
	2- a pointer to the connections pool.

	the function updates the pool->maxFD by looping through the hole connections 
	linkedList and getting the max sd then comparing it with the welcome socket descriptor.

7. void freeMsgsList(conn_t *) - This function receives one argument: 
	1- a pointer to the connection that we want to free all of its messages queue.

	the function goes through the hole queue, and it frees all the messages and there structs.

8. int add_msg(int, char *, int, conn_pool_t *) - This function receives four arguments: 
	1- the connection socket descriptor,
	2- the buffer of the message that we want to add it to the connection messages queue,
	3- the length of the buffer that we want to send to the client,
	4- a pointer to the connections pool.

	the function makes a copy of the message, and it adds it to the head of the messages queue of the connection.

9. int write_to_client(int, conn_pool_t *) - This function receives two arguments: 
	1- the socket descriptor of the connection that we want to write its messages, 
	2- a pointer to the connections pool.

	the function tries to write all the messages in the messages queue to the client.

10. void intHandler(int) - This function receives one argument: 
	1- the number(id) of the signal.

	the function applies the private scenario when the signal SIG_INT received 
	(in our case the server need to be closed).


== How To Compile? ==

compile: gcc chatServer.c -o chatServer 
run: ./chatServer <port>


== How To Exit/End? ==

the program will end if we send the signal SIG_INT to the server [ctrl + c] in linux.
	