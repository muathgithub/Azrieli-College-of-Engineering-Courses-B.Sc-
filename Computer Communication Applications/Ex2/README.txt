== Student Details ==

Student Name: Muath Abu Jamal

== Program Name & Discription ==

Program Name: HTTP Proxy - Phase 1 [Ex1]

Program Description: This program is an HTTP Proxy server which receives http URLs from the user, it checks 
if the asked URL file is in the local filesystem [in the proxy server DB], if true it returns the file in the format 
of HTTP response, else it sends an HTTP request to the web server, then it receives a reply from the server it saves 
a copy of it then it sends it to the user.


== Program Files ==

1. proxy1.c - This file contains the hole program code which makes all the string processing, 
	      socket creation, sending and receving HTTP requests and response from the proxy and web servers.

2. README.txt - This file contains the student and program details.

== My/Private Functions ==

A-) I used some functions for string processing from a previous c code that we wrote in the Operating Systems Course,
these functions are:

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


B-) I also write some private function for the purpose of making the code more modular and easy to modify,
these functions are:

1. char *getHostName(char*) - This function receives one argument: 
	1- a pointer to the string of the URL.

	the function makes the proper string parsing/splitting using the private functions 
	of string processing and, it returns the string of the hostName.
	
2. char *getPortNumStr(char*) - This function receives one argument: 
	1- a pointer to the string of the URL.

	the function makes the proper string parsing/splitting using the private functions of 
	string processing and, it returns the string of the portNumber if there is no port number the function 
	returns a string with the value of the default port "80".

3. int getPathItems(char*, char***) - This function receives two argument: 
	1- a pointer to the string of the URL, 
	2- a pointer to the 2D array pointer from the calling function.

	the function makes the proper string parsing/splitting using the private functions of string processing 
	it buts the result in 2D array and it puts the address value of the 2D array in the ***pathItems, 
	at the end it returns the number of path Items.

4. char **cloneTwoDArray(char**, int, int) - This function receives three argument: 
	1- a pointer to 2D strings array, 
	2- the start index of the copy, 
	3- the end index of the copy.

	the function creates a new strings array and copies the values from the given array according to the given indexes
	into the new array and at the end it returns the new array.

	I use it in the string parsing process for the path items to get a new 2D array without the host name.

5. int isFileExists(char*) - This function receives one argument: 
	1- a pointer to the filePath string.

	The function checks if the file exists in the local filesystem if true it 
	returns TRUE, else it returns FALSE.

6. int createFilePath(char*) - This function receives one argument: 
	1- a pointer for the string of the file path.

	This function creates the all the not existing directories and the file of the path if the 
	file name is '/' it calls it "index.html" it returns TRUE in success and FALSE in failure.

7. char *concatFilePath(char*, char**, int) - This function receives three arguments: 
	1- a pointer for the hostName string, 
	2- a pointer to the 2D array of the path Items,
	3- the number of path items.
	
	the function concat the host name and the path Items with each other (it makes a file path) 
	and it returns it as one string in the case of hostName  == NULL that's means that we 
	want just the file path without the hostName for the HTTP request so the function 
	doesn't concat the host name to the string.

8. char *createRequestStr(char*, char**, int) - This function receives three arguments: 
	1- a pointer for the string of the host name, 
	2- a pointer for the 2D array of the file items,
	3- tne number of the file items.

	the function creates an HTTP request string using the given argument and using 
	the help of the private function concatFilePath.

9. int sendServerRequest(int, char*) - This function receives two arguments: 
	1- the file descriptor, 
	2- a pointer for the string of the request.

	the function writes the request String to the server socket and, 
	at the end it returns  the number of bytes that it writes.

10. int readServerResponse(int, char*) - This function receives two argument: 
	1- the socket File Descriptor, 
	2- the file path.

	the function prints all the server request and, it checks if the web server response is successful "200 OK" 
	if true the function calls the private function createFilePath to create the needed directories and the file 
	to save the server response in the function saves only the body of the request and, it knows the start index 
	of the body by calling the private function getBodyStartInd at the end the function 
	returns the number of bytes that it read.

11. int getBodyStartInd(const unsigned char*, int, int*, int*) - This function receives two arguments: 
	1- a pointer to the buffer which holds what we read from the server, 
	2- the length of that read bytes,
	3- a pointer to a boolean which holds the status that we are in "\r\n\r\n" string or not, 
	4- a pointer to the current index of the split string "\r\n\r\n" that we are pointing to.

	the function loops through the buffer at checks if till it reaches the end of headers split string and, 
	it modifies the isInHeadersEnd and headersEndStrIndex which are saved in the calling function if 
	there is a need for modification.
	

12. int printFile(char*) - This function receives one argument: 
	1- a pointer the string of the filePath.
	
	The function opens the file which have to by in the local filesystem and, it writes it to the STDOUT FD
	at the end it returns the number of bytes that it wrote, which is the fileSize.


== How To Compile? ==

compile: gcc -Wall -o proxy1 proxy1.c
run: ./proxy1


== How To Exit/End? ==

the program will end by itself after responding to the user HTTP request.


