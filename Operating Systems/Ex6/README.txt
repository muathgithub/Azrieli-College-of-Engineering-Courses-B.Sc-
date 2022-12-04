Program Name: Virtual Memory Management Simulator [ex6]

Student Name: Muath Abu Jamal

== Program Description ==
This program is a Virtual Memory Management Simulator that simulates 
the steps and operations carried out by the memory management unit 
when the processor requests access to a specific address in the main memory.


== functions ==
1. sim_mem(char, char, int, int, int, int, int, int) - This function is the constructor of the class sim_mem:
	the function receives all the needed arguments like the executeble file and swap sile paths, beside all the 
	needed sizes for the making the memory simulation. The function opens the files and initializes the 
	class attributes with the needed and ricived values of the arguments.

2. ~sim_mem() - This function is the destructure:
	the function frees all the manually allocated memories, and closes the opened files
	at the end of the program or in the case of exit(EXIT_FAILURE) [manually call before exit()].

3. char load(int) - This function recives one argument:
	1- the memory cell logical address.
	
	the function returns the char from the given address if it exists (the page exists or has been allocated)
	after converting the logical address to a physical address by calling the function getPhysicalAddress()
	which cares to bring the page to the main memory and to calculate the physical address.

4. void store(int, char) - This function receives two arguments: 
	1- the memory cell logical address, 
	2- char to be stored in the main memory.
	
	the function stores the given char in the main memory in the given address
	after converting the logical address to a physical address by calling the function getPhysicalAddress()
	which cares to bring the page to the main memory and to calculate the physical address
	if the page doesn't allocated yet it will be allocated also by the function getPhysicalAddress().
	
5. int getPhysicalAddress(int) - This function receives one argument: 
	1- the memory cell logical address.
	
	the function returns the physical address of the given logical address
	after making the needed calculations, allocations and swapping by the help
	of some other private functions that will be mentioned after/bellow.

6. int loadPage(int, int) - This function receives two arguments: 
	1- the num of the page that need to be loaded 
	2- the source which the page exists in.

	the function loads the specific page according to the given page number from the given source
	after making the needed resetting of the pointers in the case of executable and swap file
	in the case of new allocation of a bss, heap or stack it just fills a frame with zeros
	after getting an empty frame address by calling the function getEmptyFrame() that cares to get an empty frame.

7. int getEmptyFrame() - This function receives no arguments:

	the function cares to get an empty frame in the main memory
	if there is an empty one it returns its address if not it either swaps out a frame 
	or returns an address of a text or not_dirty data frame to be override on
	in the case of swapping it uses a LUR algorithm  to decide which page to be swapped
	that depends on a queue that holds all the valid pages numbers in an order which the most active page 
	are at the end of the queue and the leaset active page are at the start or the top of the queue.

8. int swapOut(int) - This function receives one argument: 
	1- the number of the page that need to be swapped out.

	the function swaps out the specific page according to the given page number (page_table[pageNum].frame)
	in the case of text or not_dirty data page the function will not make any swap it just
	will modify the needed indicators.

9. int getPageType(int) - This function receives one argument: 
	1- the number of the page that we want to know it's type.
	
	the functions return the type of the page according to simple calculations that depends on the
	arrangement and the sizes of the memory parts and the size of each page.

10. void modifyPagesQueue(int) - This function receives one argument:
	1- the number of the page that want to be added or moved in the queue.

	the function adds or moves the given page number in the queue so the most active page 
	allocate at the end of the queue and the least used page allocate at the top/start of the queue.

11. void resetMemory() - This function receives no arguments:

	the function just rests all the memories and indicators to the starting values.

12. void print_memory() - This function receives no arguments:

	the function prints the array of the main_memory.

13. void print_swap () - This function receives no arguments:

	the function prints the swapp file content.

14. void print_page_table() - This function receives no arguments:

	the function prints the pages table.


== Program Files ==
1. main.cpp
	this file contains a decleration/initialization to the main memory
	and the main function contains some testing commands.
	
2. sim_mem.h
	this file containes all the decleration for the class sim_mem [for the memory simulation class].

3. sim_mem.cpp
	this file contains all the initializations and implementaions for the attributes and the function 
	of the file sim_mem.h .	


== How To Compile? ==

compile: g++ main.cpp sim_mem.cpp -o ex6
run: ./ex6

== How To Exit/End? ==
the program will end after finishing the main function/thread.
