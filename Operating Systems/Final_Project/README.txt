Program Name: Disk File System Simulation [Final Project]

Student Name: Muath Abu Jamal

== Program Description ==

This program is a Simulation of a Disk File System
which is managed by indexed allocation method, it simulates all the steps
that the Disk File System applies when an IO operation is required


== Data Structures ==

1. Main Directory ==> [ vector<FileDescriptor *> MainDir ]:

	a vector for the Main Directory which holds all the pointers for 
	the file descriptors of the files that are exists on the disk
	I used vector because it adjusts dynamically, so it's the most 
	sutable data structure for this job (In My Opinion/ Knowledge).

2. Open File Descriptor ==> [ map<int, FileDescriptor *> OpenFileDescriptors ]:

	a map for holding the file descriptor number and the file descriptor pointer for each open file
	I used map because it can hold two values (key [FDNum], value [FDPointer]) and it has sum functions that
	supplies for me what I need, and it makes the job more comfortable.


== classes & functions ==

A-) FsFile Class:

This class holds/saves the metadata for each file
like file size, how many blocks in use by this file (without the index block), index block address, block size
moreover it includes constructor, setters and getters.


B-) FileDescriptor Class:

This class is created for each file it's job is to connect between the FsFile(file metadata) and the file name
also it includes boolean value (inUse) for each file that saves if the file is open or close
beside the constructor, setters and getters.


C-) fsDisk Class:

This class is the simulation of the Disk Management System it simulates the disk and all the needed 
operations for maneging and editing the files by indexed allocation method
it includes constructor, public functions (as asked in the exercise) and private functions
to for the purpose of modularity

# fsDisk Class (private functions):

1. int findFileInMainDir(string) - This private function receives one argument: 
	1- file name.
	
	the function used for finding the index of a file in the Main Directory by it's name
	it uses iterator to navigate through the vector and the function distance to calculate
	how far is the item index form the beginning of the vector.

2. auto findFileInOpenFD(string) - This private function receives one argument: 
	1- file name.

	the function used for finding the iterator of the open file item <key, value> in the open file descriptor map by the given name (file name)
	it uses iterator to navigate through the map
	in the case of failure it returns OpenFileDescriptor.end().

3. void eraseFile(int) - This private function receives one argument: 
	1- fileIndex (In Main Directory).
	
	the function used for freeing/deleting all the manually allocated memories of the file
	and at the end it removes that file from the Main Directory vector
	it uses iterator to navigate through the vector.

4. int resetBlock(int) - This private function receives one argument: 
	1- block index.

	The function resets the block in the DISK_SIM_FILE to '\0's
	which means that the block now is free and others can use it
	the calling function responsible for making the BitVector[blockIndex] = NOT_IN_USE_BLOCK.

5. int getEmptyBlock() - This private function receives no arguments:

	the function returns the first empty block Index
	by looping through the BitVector.
	
6. int getFileDescriptorNumber() - This private function receives no arguments:

	the function returns the minimum possible file descriptor number
	according to the OpenFileDescriptor Map keys.

# fsDisk Class (public functions):

1. void fsFormat(int) - This function receives one argument: 
	1- block size.
	
	the function formats the disk and makes all the needed calculations and initializations.

2. int CreateFile(string) - This function receives one argument: 
	1- file name.

	the function creates a file if the disk is formated and the file not exists
	it makes a FsFile(metadata holder) and FileDescriptor for the file, and
	it saves that file descriptor pointer in the Main Directory vector and OpenFileDescriptor map
	the number of the open FD is supported by the function getFileDescriptorNumber() which returns the minimum
	possible number in the map <key>, at the end this function returns the FDNum.

3. int OpenFile(string) - This function receives one argument: 
	1- file name.

	the function opens the file by changing the bool value of InUse of the file to true
	and adding the file to the OpenFileDescriptor
	before that it checks if the file exists or if the file is already open.

4. string CloseFile(int) - This function receives one argument: 
	1- File Descriptor Number.

	The function closes the file with the file Descriptor number after checking if it is open
	closing means to change the bool value InUse to false and removing the item of the file from the OpenFileDescriptor map
	at the end it returns the nme of the file that have been close in case of failure it returns "-1".

5. int WriteToFile(int, char*, int) - This function receives three arguments : 
	1- file descriptor number, 
	2- a buffer that holds the string to be written, 
	3- the length of that string (in the buffer).

	the function makes the needed checks like if the file is open, if there is enough space in the file and the disk .....
	then the function seeks in the disk file and starts writing and allocating the needed blocks by the help of the private and non-private functions
	at the end it returns the number of chars that have been written in the file.

6. int ReadFromFile(int, char*, int) - This function receives three arguments: 
	1- the file descriptor number, 
	2- the buffer to write on what we read, 
	3- the length of characters that we want to read.

	the function makes the needed checks like if the file is open, if the disk is formatted ....
	then the function starts seeking and reading from the file according to the indexes of it's data blocks that are saved in the index block
	at the end it returns the length of the characters that have been read
	if the asked len is bigger than the file size it returns all the content of the file.

7. int DelFile(string) - This function receives one argument: 
	1- file name.

	the function deletes the file after making the needed checks like if the disk is formated, if the file exists ...
	also it cares about freeing/deleting all the manually allocating memories by the help of the private functions
	resetBlock(), eraseFile(), ... .

8. void listAll() - This function receives no arguments:

	the function prints [ indexes, names, isInUse ] for all the file that are exists in the Main Directory
	also it prints all the Disk Content including the indexes blocks binary values.


D-) Global/General Functions:

1. char decToBinary(int, char&) - This function receives two arguments: 
	1- the number which need to be converted to binary, 
	2- a pointer to char to store the binary result.

	the function resets the char by making & with zero then it converts the number 
	to binary using the help of bitwise operations.

2. int binaryToDec(char&) - This function receives one argument: 
	1- a pointer to the char that holds the binary value 
	that need to convert it to decimal(int) value.
	
	the function converts the binary to int using the help of bitwise operations.


== Program Files ==

1. ex7_final_proj.2021.cpp


== How To Compile? ==

compile: g++ ex7_final_proj.2021.cpp -o ex7_final_proj.2021
run: ./ex7_final_proj.2021


== How To Exit/End? ==

the program will end after pressing the number '0' (zero).
