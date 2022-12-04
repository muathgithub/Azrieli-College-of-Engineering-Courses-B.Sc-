== Student Details ==

Student Name: Muath Abu Jamal


== Program Name & Discription ==

Program Name: HTTP Proxy - Phase 2 [Ex3]

Program Description: This program is a multi threaded HTTP Proxy server which receives http requests 
from the clients, it checks if the asked file is in the local filesystem [in the proxy server DB], 
if true it returns the file in the format of HTTP response, else it sends an HTTP request to the web server, 
then it receives a reply from the server it saves a copy of it then it sends it to the user.


== Program Files ==

1. proxyServer.c - This file contains the proxy server code which makes all the string processing, 
	      socket creation, sending and receving HTTP requests and response from the proxy and web servers.

2. threadpool.c - This file contains the implementstion of the threads pool functions which in declared in threadpool.h
		beside some other private function to modify the jobs queue.

3. README.txt - This file contains the student and program details.

== My/Private Functions ==

A-) The Functions Of [ proxyServer.c ]  

1. int sectionsCount(char*, char*) - This function receives two arguments:
	a- a pointer for the sentence, 
	b- apointer for a string (char*) in order to split according to it (spliter).

	the function goes through the hole sentence and it counts the number of sections according to the given spliter string
	then it returns that number.

2. char** toSectionsArray(char*, char*, char*) - This function receives three pointers:
	a- a pointer for the sentence, 
	b- a pointer for a string (char*) in order to split according to it (spliter)
	c- a pointer for the default string (to put if there is an empty split)
	   > for example if we split the string "x" according to "x" and the value of the default string = "1"
	   > then the result is the array ["1", "1"] before and after "x" there are an empty split.

	the function finds the start and end indexes for each section in the sentence,  
    	and it sends them each time to the function getSectionedStr() that manually allocates memory for each section
	according to it's length and it returns a char* for it, at the end the function returns a char**
	which is manually allocated and holds the pointers for all the char* of the sentence sections.

3. void getSectionStr(const char*, const int*, const int*) - This function receives three pointers: 
	a- for the sentence, 
	b- for the first index in the section(according to the indexes of the sentence), 
	c- for the last index in the section(according to the indexes of the sentence).

	the function manually allocates memory for each section according to it's 
	length from the given wordStartInd to wordEndInd and returns a char* for the allocated memory.

4. void freeAll(char**, int) - This function receives two pointers:
	a- for the arguments/sections array.
	b- for the number of the arguments/sections.

	the function frees all the manually allocated memory by going through each pointer in the array of args pointers
	then it frees the array(char**) of pointers it self.

5. char *getLowerCaseCopy(char *) - This function receives one argument: 
	1- a pointer to a string.

	the function returns a lower case copy of the given string.

6. char *getOneSpaceCopy(char *) - This function receives one argument: 
	1- a pointer to a string.

	the function returns a one space copy of that string which means where 
	there are a sequence of spaces it converts it to one space.

7. void strTrim(char *) - This function receives one argument: 
	1- a pointer to a string.
	
	the function removes the spaces at the beginning and at the end of the string, 
	and it stores it in its memory area that means that the function applies a 
	shift for the string to the left.

8. int initializeFilterLists(int) - This function receives one argument: 
	1- the filter file descriptor.

	the function reads the filter file, and it fills the Global filter linkLists
	with the read values after initializing these linkLists and making 
	the needed string processing on them.

9. int add(LinkList *, char *) - This function receives two argument: 
	1- a pointer to the linkList, 
	2- a pointer to the string that we want to add to the linkList.

	the function creates a new node and puts the data in it then 
	it adds it to the end of the link list.

10. int serverInitialize(int) - This function receives one argument: 
	1- the port number that we want to initialize the server with.

	the function creates the welcomeSocket for the server it binds it with the proper sockadd_in 
	as asked in the question and with the given port number at it makes it listen at maximum 
	for the value of the defined macro MAX_LISTEN_QUEUE at the end it returns the welcomeSD.

11. char *readClientRequest(int) - This function receives one argument: 
	1- the socket descriptor of the client.

	the function reads the client request till the "\r\n\r\n" it saves that request, 
	and it returns a pointer to it.

12. int checkClientReqErrors(char *) - This function receives one argument: 
	1- a pointer for the client http string request.

	the function checks if the request has any errors by calling the private function for errors checking
	if there is an error it returns the error status code else it returns defined macro NO_ERRORS.

13. int isBadRequest(char *) - This function receives one argument: 
	1- a pointer to the http request string.

	the function makes the needed string processing, and it checks if the request is a BadRequest
	BadRequest [in our case]: not contains a method or a path or a protocol or a host header
	the string processing done by the help of my private string processing function.

14. int isGETMethod(char *) - This function receives one argument: 
	1- a pointer to the http request string.

	the function makes the needed string processing, and it checks if the request is GET method request
	the string processing done by the help of my private string processing function.

15. int isInFilter(LinkList *, char *) - This function receives two arguments: 
	1- for the linkList of the filter items, 
	2- a string pointer to check if its exists in the filter linkList.

	the function loops through the filter linkList and checks if the domain string exists 
	in that linkList if the items linkList is subnets linkList it makes the needed 
	string processing to split the subnet ip from the subnet mask length.

16. int statusErrorHandler(int, int) - This function receives two arguments: 
	1- the error status code, 
	2- the socket descriptor to write the error in for the client.

	the method builds the error message template, and it fills it with the proper error information according 
	to the received error status code, and it writes that error to the SD of the client.

17. char *getDomainStr(char *) - This function receives one argument: 
	1- a pointer to the http request string.

	the function makes the needed string processing, and it returns the domain name from the request
	the string processing done by the help of my private string processing function.

18. char *getFilePath(char *) - This function receives one argument: 
	1- a pointer to the http request string.
	
	the function makes the needed string processing, and it returns the file path from the request
	the string processing done by the help of my private string processing function.

19. char *getFileName(char *) - This function receives one argument: 
	1- a pointer to the file path string.

	the function makes the needed string processing to get the file name 
	from the given file path, and it returns it.

20. char *get_mime_type(char *) - This function receives one argument: 
	1- a string pointer for the file name.

	the function is given from the teacher it returns the file type in the format 
	of the Content-Type http header format.

21. int isFileExists(char *) - This function receives one argument: 
	1- a pointer to the filePath string.
	
	The function checks if the file exists in the local filesystem 
	if true it returns TRUE, else it returns FALSE.

22. int createFilePath(char *) - This function receives one argument: 
	1- a pointer for the string of the file path.

	This function creates the all the not existing directories and the file of the path 
	if the file name is '/' it calls it "index.html" it returns SUCCEEDED 
	in success and FAILED in failure.

23. uint32_t strIpToUint32_t(char *) - This function receives one argument: 
	1- a pointer to the ip version 4 string.

	the function makes the needed string processing to split the string ip and convert its parts 
	to unsigned integers then it makes the needed bitwise operations to store the values 
	in one uint32_t, and it returns it.

24. int belongsToSub(char *, char *, int) - This function receives three arguments: 
	1- a string pointer to the ip that we want to check if it belongs to a specific subnet,
	2- a string pointer to the subnet ip, 3- the subnet mask length of the subnet.

	the converts the two ips to uint_32 using the private function strIpToUint32_t() and it applies the needed 
	bitwise operations on the uint32_t ips presentation to check if the ip belongs to the subnet.

25. int sendLocalFileResponse(char *, int) - This function receives two arguments: 
	1- a pointer for the filePath string, 
	2- the socket descriptor for the client.

	the function reads the file from the local file system, and it writes it to the client socket descriptor
	at the end it prints the asked message "File is given from local filesystem\n\n Total response bytes: %d\n".

26. int connectToWebServer(char *) - This function receives one argument: 
	1- a pointer to a string of the domainName that we want to connect to.

	the function creates a socket, then it connects to the server of the 
	given DomainName [domain name, ip] at the ind it returns the SD.

27. char *buildWebServerRequest(char *) - This method receives one argument: 
	1- a pointer to the client request string.

	it builds a new request with just a host and connection: close headers, so we don't stuck 
	in a connection with the web server that we want to send the request to.

28. int sendServerRequest(int, char *) - This function receives two arguments: 
	1- a socket descriptor for the server,
	2- a pointer to the request string which we want to send it to the server.
	
	the function writes the request to the server socket descriptor.

29. int readAndSendServerResponse(int, int, char *) - This function receives two argument: 
	1- the socket File Descriptor, 
	2- the file path.

	the function prints all the server request and, it checks if the web server response is successful 
	"200 OK" if true the function calls the private function createFilePath to create the needed 
	directories and the file to save the server response in the function saves only the body of 
	the request and, it knows the start index of the body by calling the private function 
	getBodyStartInd at the end the function returns the number of bytes that it read.

30. int getBodyStartInd(const unsigned char *, int, int *, int *) - This function receives two arguments: 
	1- a pointer to the buffer which holds what we read from the server, 
	2- the length of that read bytes.
	3- a pointer to a boolean which holds the status that we are in "\r\n\r\n" string or not, 
	4- a pointer to the current index of the split string "\r\n\r\n" that we are pointing to.
	
	the function loops through the buffer at checks if till it reaches the end of headers split string and, 
	it modifies the isInHeadersEnd and headersEndStrIndex which are saved in the calling function 
	if there is a need for modification.

31. int clientHandler(void *) - This function receives one argument: 
	1- a pointer to an int which holds the socket descriptor of the client.
	
	the function executed by each thread (every thread handles a client), the function reads the client request
	it checks if the request is valid if false it calls the private statusErrorHandler() to handle the error
	by the error status code which given by the private function checkClientReqErrors()
	if the request is valid the function checks if the file is in the local file system if true it calls
	the function sendLocalFileResponse() to send to the client from the local file system
	else it connects to the web server of the domain that given in the client request
	it sends the client request to the web server if the response is "200 OK" it saves a copy of the response
	and at the same time it sends the response of the web server to the client
	of course the function makes some string processing where there is a need for that
	if there is a server error it free the manually allocated memories and it sends INTERNAL_SERVER_ERROR to the client
	then it ends, also if the domain is a name it gets it's ip and it searches for the asked file under the directory of that ip

32. void freeList(LinkList *) - This function receives one argument: 
	1- a pointer for the dictionary.

	the function frees all the manually allocated memories of the dictionary.


B-) The Functions Of [ threadpool.c ]

1. threadpool *create_threadpool(int) - This function receives one argument: 
	1- the number of the threads in the pool.

	the function creates the pool and initializes the threads pool variables in any case 
	of failure it frees all the manually allocated memories then it returns.

2. void *do_work(void *) - This function receives one argument: 
	1- a pointer to the thread pool.

	each thread applies this function, and it live there handling clients
	by taking each time a job from the jobs queue and making that job if there is no 
	job the thread waits till the dispatch function awakes him after adding a new job
	the thread lock and unlock the mutexes according to the need, so that the other 
	thread don't read or write to the queue which causes a problems [synchronization].

3. void dispatch(threadpool *, dispatch_fn, void *) - This function receives three arguments: 
	1- a pointer for the thread pool, 
	2- a pointer to the function that the thread going to apply.

	the function creates a new work_t struct which holds the clientHandler function and 
	the argument for that function which is the client SD the function adds that job to the 
	jobs queue then it wakes up a thread to make the job the function lock and unlock the 
	mutex according to the need so that the other thread don't read or write to the queue which 
	causes a problems [synchronization].

4. void destroy_threadpool(threadpool *) - This function receives one argument: 
	1- a pointer to the thread pool.

	the function destroys the thread pool starting by turning on the flag dont_accept to tell the dispatch
	function not to add new jobs to the jobs queue if the jobs queue is not empty it waits till the queue
	is empty (the thread that takes the last job awakes it) then it turns on the flag shut down, and it awakes 
	all the threads, so they see the shutdown flag on, and they exit after that it makes the needed frees and 
	destroys for the locks.

5. int enqueue(threadpool *, work_t *) - This function receives two arguments: 
	1- a pointer for the thread pool, 
	2- a pointer of work_t which is the job that we want to add to the queue.

	the function adds the new job to the jobs queue.

6. work_t *dequeue(threadpool *) - This function receives two arguments: 
	1- a pointer for the thread pool.

	the function pops and returns the first job in the queue.


== How To Compile? ==

compile: gcc proxyServer.c threadpool.c -lpthread -o proxyServer 
run: ./proxyServer <port> <pool-size> <max-number-of-request> <filter>


== How To Exit/End? ==

the program will end by itself after accepting the <max-number-of-request> which is given in the command line usage.
	
	