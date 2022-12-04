Program Name: Polynomial Evaluation Calculator [ex5] (version a & b)

Student Name: Muath Abu Jamal

==Program Description ==
This program receives a string of a polynomial and a value for x
it evaluates that polynomial by using a suitable number of Threads [version a] OR Sons [version b] 
which make the calculations according to the given x value. 
At the end the father process amkes a sum for all the Threads/Sons results 
and the other values that doesn't need a thread/Son (pow zero for x).


== functions ==
A-) functions that are used in both versions version a & b:
-NOTE: alot of the string processing functions have the same concepts from [ex1] and some of them are just the same.

1. int strLenNoNewLine(char*) - This function receives a pointer for the sentence that the user entered,
   	it removes the '\n' from the end of the sentence if it exists then it returns the length of the sentence.

2. int sectionsCount(char*, char*) - This function receives two arguments:
	a- a pointer for the sentence, 
	b- apointer for a string (char*) in order to split according to it (spliter).

	the function goes through the hole sentence and it counts the number of sections according to the given spliter string
	then it returns that number.

3. char** toSectionsArray(char*, char*, char*) - This function receives three pointers:
	a- a pointer for the sentence, 
	b- a pointer for a string (char*) in order to split according to it (spliter)
	c- a pointer for the default string (to put if there is an empty split)
	   > for example if we split the string "x" according to "x" and the value of the default string = "1"
	   > then the result is the array ["1", "1"] before and after "x" there are an empty split.

	the function finds the start and end indexes for each section in the sentence,  
    	and it sends them each time to the function getSectionedStr() that manually allocates memory for each section
	according to it's length and it returns a char* for it, at the end the function returns a char**
	which is manually allocated and holds the pointers for all the char* of the sentence sections.

4. void getSectionedStr(const char*, const int*, const int*) - This function receives three pointers: 
	a- for the sentence, 
	b- for the first index in the section(according to the indexes of the sentence), 
	c- for the last index in the section(according to the indexes of the sentence).

	the function manually allocates memory for each section according to it's 
	length from the given wordStartInd to wordEndInd and returns a char* for the allocated memory.

5. void noSpaceStr(char*) - This function receives one pointer:
	a- for the sentence.

	the function removes all the spaces from the sentence for the check of "done" (exit) 
	and for more security while splitting the entered user sentence.

6. void toCoefficientPowArray(char*) - This function receives one pointer:
	a- for the sentence.

	makes the needed string processing on the sentence that the user entered it splits the sentence 
	in a certain way so at the end we have the integer values of each polynomial part
	(coefficient and pow) in the coefficientPowArray.

7. int powFunc(int, int) - This function receives two arguments:
	1- for the exponent,
	2- for the power value.
	
	the function calculates the value of the exponent to the power of power value.

8. void resetGlobalStorage([int*: just for version b]) - This function receives one pointer [just for version b]:
	1- for the shared memory pointer.

	resets the all the Global variables and the shared memore [in version b] to prepare/clean them for the next loop.

9. void freeAll(char **, const int *) - This function receives two pointers:
	a- for the arguments/sections array.
	b- for the number of the arguments/sections.

	the function frees all the manually allocated memory by going through each pointer in the array of args pointers
	then it frees the array(char**) of pointers it self.

B-) function for the purpous of [version a]:

1. int calcByThreads() - This function receives no arguments:
	
	This method calculates the polynomial parts as asked in the task by creating a suitable 
	number of threads which each one of the calculates a certain part form the polynomial
	at the end it returns the hole result of the evaluation that the user entered
	which is the sum of all the threads results and the other values that doesn't need a thread (pow zero for x).

2. void* threadFunc(void*) - This function receives on pointer:
	a- a pointer for any kind of data [The programmer decides the type by making casting if he needs it].

	this is the callback function for the threads which receives a pointer for the index where the values
	of the part that this thread has to work on are exists in the Global array coefficientPowArray.

C-) function for the purpous of [version b]:

1. void shmAllocate(int*, int**) - This function receives two pointers:
	a- a pointer for the shm_id,
	b- a pointer for the shm pointer.
	
	this function allocates the needed shared memory and it puts the value of the shm_id in the int cell of the received 
	pointer shm_id and it puts the value of the pointer shm_ptr in the int* cell of the received pointer shm_ptr.

2. int calcBySons(int*) - This function receives one pointer:
	a- a pointer for the shm pointer.

	This method calculates the polynomial parts as asked in the task by creating a suitable number of sons which 
	each one of the calculates a certain part form the polynomial. At the end it returns the hole result of the 
	evaluation that the user entered which is the sum of all the sons results (int the shm) 
	and the other values that doesn't need a son process (pow zero for x).


== Program Files ==
1. ex5a.c
2. ex5b.c


== How To Compile? ==

version a [ex5a]:

compile: gcc ex5a.c -o ex5a -lpthread
run: ./ex5a

version b [ex5b]:

compile: gcc ex5b.c -o ex5b
run: ./ex5b


== How To Exit/End? ==
the program exits/ends by typing the word "done"

