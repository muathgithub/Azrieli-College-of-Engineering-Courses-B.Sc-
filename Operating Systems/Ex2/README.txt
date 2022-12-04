Program Name: Mini Shell [ex2]

Student Name: Muath Abu Jamal

==Program Description ==
This program is a Mini/Simple shell that receives commands from the user as a normal shell and executes them if
they are correct and belong to the commands that it explores their implementation (For example it doesn't support/execute the command "cd" Yet)
it uses alot of the function that we have made in [ex1] to proccess the commands strings
there is a version B of that program which also prints the sched for each son process that made by fork.


== Variables ==
This program uses the variable (int isInWord) which the funnctions of processing the strings from [ex1] needs:
int isInWord: this variable used as a flag to know if we are in a word or not (in the sentence)

All the other variables are a clear name, type and purpose variables.


== functions ==
A-) functions from [ex1]:

I unsed all the functions from the previous exercise [ex1], however I have renamed most of these functions 
in order to make the names more meaningful for the purpose of this program
for each function I will write before it's name it's previous name between []
I will make the same for the returned type of these functions which i changed
these functions (functions from [ex1]) are:

1. int strLenNoNewLine(char*) - This function receives a pointer for the sentence that the user entered,
   	it removes the '\n' from the end of the sentence if it exists then it returns the length of the sentence.

2. int [wordsCount] > argsCount(const char*, int*, const int*) - This function receives three pointers: 
	a- for the sentence, 
	b- for the bool isInWord (Used As Global Variable), 
	c- for the sentenceLen (Used As Global Variable)

	the function goes through the hole sentence and it counts the number of words then it returns that number.

3. [void] > char** [printWordsList] > toArgsArray(const char*, int*, const int*) - This function receives three pointers: 
	a- for the sentence, 
	b- for the bool isInWord (Used As Global Variable), 
	c- for the sentenceLen (Used As Global Variable).

	the function finds the start and end indexes for each word in the sentence, and it sends 
    	them each time to the function getArgStr() that manually allocates memory for each word
	according to it's length and it returns a char* for it, at the end the function returns a char**
	which is manually allocated and holds the pointers for all the char* of the sentence words.

4. void [printWord] > getArgStr(const char*, const int*, const int*) - This function receives three pointers: 
	a- for the sentence, 
	b- for the first index in the word (according to the indexes of the sentence), 
	c- for the last index in the word (according to the indexes of the sentence).

	the function manually allocates memory for each word according to it's 
	length from the given wordStartInd to wordEndInd and returns a char* for the allocated memory.


B-) new functions for the purpose of this program [FOR VERSION A & B]:

1. void getPrompt(char *) - This function receives one pointer:
	a- for the char[] which Used As Global array that will hold the string of the prompt.
	
	the function fill the char[] with the suitable prompt according to the current/active user and directory
	by calling the two functions:
	1- getpwuid() which needs to pass the id of this process for it and that function returns a pointer to a struct which
 	can holds the information that we need (Username) and other unneeded information in our case.
	2- getcwd() which needs a buffer and it's maximum size and it fills the buffer with the current 
	directory path if it succeeded else it returns NULL.

2. void printStatistics(const int *, const int *) - This function receives two pointers:
	a- for the commands number.
	b- for the total commands strings length.
	
	the function calculates then prints the needed statistics in a formatted way 
	with "See you Next time !" message at the end as asked in the exercise.

3. void freeAll(char **, const int *) - This function receives two pointers:
	a- for the arguments/words array.
	b- for the number of the arguments/words.

	the function frees all the manually allocated memory by going through each pointer in the array of args pointers
	then it frees the array(char**) of pointers it self.


C-) new functions for the purpose of [VERSION B]:

1. void printSched(pid_t *, int *, char**, const int*) - This function receives three pointers:
	a- pid in order to save the sons pid.
	b- the flag isInWord which it passes to other needed functions to prepare the cat command.
	c- the command arguments/words array to free all the fathers manual allocated memories.
	d- the number of the arguments/words which the freeAll() needs to free the fathers manual allocated memories.

	the function prints the sched of the son process by making fork for a new process 
	that will execute the command cat with the id of the son.


== Program Files ==
1. ex2a.c
1. ex2b.c


== How To Compile? ==
compile [ex2a] : gcc ex2a.c -o ex2a
run: ./ex2a

compile [ex2b] : gcc ex2b.c -o ex2b
run: ./ex2b


== How To Exit/End? ==
the program exits/ends by typing the word "done"
	