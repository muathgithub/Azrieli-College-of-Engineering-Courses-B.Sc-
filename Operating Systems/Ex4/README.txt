Program Name: Mini Shell That Support Two Pipes, ctrl + z and fg [ex4]

Student Name: Muath Abu Jamal

==Program Description ==
This program is a Mini/Simple shell that receives commands from the user as a normal shell and executes them if
they are correct and belong to the commands that it explores their implementation (For example it doesn't support/execute the command "cd" Yet)
in addition it supports piping till two pipes max, ctrl + z and fg
it uses all the functions that we have made in [ex1], [ex2] and [ex3] to proccess the commands strings and to execute them.

== functions ==
A-) functions from [ex1], [ex2] and [ex3]:

I used all the functions from the previous exercises [ex1], [ex2] and [ex3] these functions are:

1. int strLenNoNewLine(char*) - This function receives a pointer for the sentence that the user entered,
   	it removes the '\n' from the end of the sentence if it exists then it returns the length of the sentence.

2. int sectionsCount(const char*, char) - This function receives two arguments:
	a- a pointer for the sentence, 
	b- for the split/dividing char (that char that we divide according to in our case SPACE_CHAR and PIPE_CHAR)

	the function goes through the hole sentence and it counts the number of sections according to the split cahr
	then it returns that number [Note: any split cahr between quotation marks ( " ) dosen't count].

3. char** toSectionsArray(const char*, char) - This function receives two arguments: 
	a- a pointer for the sentence, 
	b- for the split/dividing char (that char that we divide according to in our case SPACE_CHAR and PIPE_CHAR)

	the function finds the start and end indexes for each section in the sentence (taking care care about the start and end quotation marks),  
    	and it sends them each time to the function getSectionedStr() that manually allocates memory for each section
	according to it's length and it returns a char* for it, at the end the function returns a char**
	which is manually allocated and holds the pointers for all the char* of the sentence sections.

4. void getSectionedStr(const char*, const int*, const int*) - This function receives three pointers: 
	a- for the sentence, 
	b- for the first index in the section(according to the indexes of the sentence), 
	c- for the last index in the section(according to the indexes of the sentence).

	the function manually allocates memory for each section according to it's 
	length from the given wordStartInd to wordEndInd and returns a char* for the allocated memory.

5. void getPrompt(char *) - This function receives one pointer:
	a- for the char[] which Used As Global array that will hold the string of the prompt.
	
	the function fill the char[] with the suitable prompt according to the current/active user and directory
	by calling the two functions:
	1- getpwuid() which needs to pass the id of this process for it and that function returns a pointer to a struct which
 	can holds the information that we need (Username) and other unneeded information in our case.
	2- getcwd() which needs a buffer and it's maximum size and it fills the buffer with the current 
	directory path if it succeeded else it returns NULL.

6. void printStatistics(const int *, const int *) - This function receives two pointers:
	a- for the commands number.
	b- for the total pipes number.
	
	the function prints the statistics in a formatted way 
	with "See you Next time !" message at the end as asked in the exercise.

7. void freeAll(char **, const int *) - This function receives two pointers:
	a- for the arguments/sections array.
	b- for the number of the arguments/sections.

	the function frees all the manually allocated memory by going through each pointer in the array of args pointers
	then it frees the array(char**) of pointers it self.

8. void executeSection(char **, int, int, int *, int *) - This function receives five arguments:
	a- a pointer (char**) for the sections of the sentence that the user entered which have been divided by PIPE_CHAR using the function to SectionsArray(.., PIPE_CHAR).
	b- the length of the sections array (how many sections).
	c- the current section (which it's turn to be executed).
	d- a pointer for the first pipe.
	e- a pointer for the second pipe.

	the function forkes a new son process and in the son process it closes the unneeded pipes
	and it connects the son to the needed pipes by calling the function pipeConnection()
	then it makes the needed processing on the section/command string and it executes it.

9. void pipeConnection(char **, int, int, int *, int *) - This function receives five arguments:
	a- a pointer (char**) for the sections of the sentence that the user entered which have been divided by PIPE_CHAR using the function to SectionsArray(.., PIPE_CHAR).
	b- the length of the sections array (how many sections).
	c- the current section (which it's turn to be executed).
	d- a pointer for the first pipe.
	e- a pointer for the second pipe.

	the function connects the current process with the needed pipes and colses the FDs for the unneeded ones
	and it changes the STDIN and STDOUT according to the needed.
	
10. void pipeClose(int *, char **, int *) - This function receives three pointers:
	a- a pointer of (int*) to the array of pipes FDs numbers.
	b- a pointer to the sections array to free all the memories in case of failure (exit).
	c- a pointer to the length of the sections array.
	
	this function closes the received pipe if it not closed and it puts the value of [CLOSED = -1] preprocessor directives in the two cells of the pipe FDs numbers array.

11. int pipesCount(char *) - This function receives one pointer:
	a- a pointer to the sentence that the user entered.
	
	the function counts the number of pipes in the sentence without the pipes that appear between quotation marks
	and it returns that number, which used to decied if the number of pipes is legal or not.

B-) new functions for the purpose of this program:

1. void resetSonsPIDArray():
	
	this function resets the global array that I use to store the PIDs for the current command sons in order to prepare it
	to hold the new sons PIDs so we don't get unexpected/weird behavior, also it resets the current element index for that array.

2. void sonsWait():

	this function applies the function waitpid() to all the PIDs (sons) in the global array [that saves all the sons PIDs 
	for the current command], also it checks if the returned status of the sun is STOP status if true it saves that process id as last stopped 
	process id in order to allow us to send a SIGCONT signal to that process when the user enters the command "fg".

== Program Files ==
1. ex4.c


== How To Compile? ==
compile: gcc ex4.c -o ex4
run: ./ex4


== How To Exit/End? ==
the program exits/ends by typing the word "done"

