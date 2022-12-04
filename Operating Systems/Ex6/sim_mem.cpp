// Program Name: Virtual Memory Management Simulator [ex6]
// Program Description: This program is a Virtual Memory Management Simulator
// that simulates the steps and operations carried out by the memory management unit
//when the processor requests access to a specific address in the main memory.

// Student Name: Muath Abu Jamal


#include <unistd.h>
#include <malloc.h>
#include <fcntl.h>
#include <cstdlib>
#include <iostream>
#include "sim_mem.h"


// This constructor initializes all the attributes with the received values
// also it opens the needed files and it checks if the open function has been succeeded
// at the end it calls the function resetMemory() which resets the memories with a default/start values
sim_mem::sim_mem(char exe_file_name[], char swap_file_name[], int text_size, int data_size, int bss_size,
                 int heap_stack_size, int num_of_pages, int page_size) {

    this->program_fd = NO_FILE;
    this->swapfile_fd = NO_FILE;
    this->text_size = text_size;
    this->data_size = data_size;
    this->bss_size = bss_size;
    this->heap_stack_size = heap_stack_size;
    this->num_of_pages = num_of_pages;
    this->page_size = page_size;

    this->page_table = new page_descriptor[num_of_pages];
    this->isUsedFrame = new bool[MEMORY_SIZE / this->page_size];
    this->isUsedSwap = new bool[this->num_of_pages - (this->text_size / this->page_size)];

    this->program_fd = open(exe_file_name, O_RDONLY);
    if(this->program_fd == -1){
        perror("Failed To Open The Program File");
        this->~sim_mem();
        exit(EXIT_FAILURE);
    }

    this->swapfile_fd = open(swap_file_name, O_CREAT | O_RDWR | O_TRUNC, 0666);

    if(this->swapfile_fd == -1){
        perror("Failed To Open/Create The Swap File");
        this->~sim_mem();
        exit(EXIT_FAILURE);
    }

    resetMemory();
}

// This destructor frees all the manually allocated memory and closes the opened files
// and it checks if the closing operation has been succeeded
sim_mem::~sim_mem() {

    delete [] this->page_table;
    delete [] this->isUsedFrame;
    delete [] this-> isUsedSwap;

    if(this->program_fd != NO_FILE){
        if(close(this->program_fd) == -1){
            perror("Failed To Close Files");
            exit(EXIT_FAILURE);
        }
    }

    if(this->swapfile_fd != NO_FILE){
        if(close(this->swapfile_fd) == -1){
            perror("Failed To Close Files");
            exit(EXIT_FAILURE);
        }
    }

}

// This function receives one argument: 1- the logical address,
// the function returns the char from the given address if it exists (the page exists or has been allocated)
// after converting the logical address to a physical address by calling the function getPhysicalAddress()
// which cares to bring the page to the main memory and to calculate the physical address
char sim_mem::load(int address) {

    int pageNum = address / this->page_size;

    int pageType = getPageType(pageNum);

    // checking if the address is legal in the memory range/pages for this program
    if(pageType == FAILED){
        fprintf(stderr, "Invalid Address [Out Of Range]\n");
        return '\0';
    }

    // a special case which the memory doesn't allocated yet
    if((pageType == BSS || pageType == HEAP_STACK) && (this->page_table[pageNum].V == INVALID) && (this->page_table[pageNum].D == NOT_DIRTY)){
        fprintf(stderr, "No Memory Allocated [Load At The First Time Form Stack And Heap]\n");
        return '\0';
    }

    int physicalAddress = getPhysicalAddress(address);

    char resultChar = main_memory[physicalAddress];

    return resultChar;
}

// This function receives two arguments: 1- logical address, 2- char to be stored in the main memory
// the function stores the given char in the main memory in the given address
// after converting the logical address to a physical address by calling the function getPhysicalAddress()
// which cares to bring the page to the main memory and to calculate the physical address
// if the page doesn't allocated yet it will be allocated also by the function getPhysicalAddress() this time
// because the function store will not return as the function load before calling the function getPhysicalAddress()
void sim_mem::store(int address, char value) {

    int pageNum = address / this->page_size;

    int pageType = getPageType(pageNum);

    // checking if the address is legal in the memory range/pages for this program
    if(pageType == FAILED){
        fprintf(stderr, "Invalid Address [Out Of Range]\n");
        return;
    }

    // checking if the pageType is TEXT if true print a proper message and return (continue the program)
    if(pageType == TEXT){
        fprintf(stderr, "No Write Permission [Text Page]\n");
        return;
    }

    int physicalAddress = getPhysicalAddress(address);

    main_memory[physicalAddress] = value;

    // making the page dirty if it's not, because we have edited it's content
    if(this->page_table[pageNum].D == NOT_DIRTY) {
        this->page_table[pageNum].D = DIRTY;
    }

}

// This function receives one argument: 1- logical address
// the function returns the physical address of the given logical address
// after making the needed calculations, allocations and swapping
int sim_mem::getPhysicalAddress(int logicalAddress) {


    int pageNum = logicalAddress / this->page_size;
    int cellOffset = logicalAddress % this->page_size;
    int newFrame = 0;
    int physicalAddress;

    int pageType;

    // checking if the page is out of range
    if((pageType = getPageType(pageNum)) == FAILED){
        return FAILED;
    }

    // checking if the page is valid if true the return it's physical address
    // no need for swapping or allocation
    if(this->page_table[pageNum].V == VALID){
        physicalAddress = (page_table[pageNum].frame * this->page_size) + cellOffset;
        modifyPagesQueue(pageNum);
        return physicalAddress;
    }

    // if the page is invalid then there is two cases both of them call the function loadPage
    // that loads the page from the given source
    if(this->page_table[pageNum].V == INVALID){

        // if the page is dirty bring that page from the swap file
        if(page_table[pageNum].D == DIRTY){
            newFrame = loadPage(pageNum, SWAP_FILE);
        }
        // if the page is not dirty there is two cases according to the page type
        else if(page_table[pageNum].D == NOT_DIRTY){

            // if the page is a text or data page then bring that page from the executable file
            if(pageType == TEXT || pageType == DATA){
                newFrame = loadPage(pageNum, EXECUTABLE_FILE);
            }
            // if the page is BSS or HEAP_STACK page then allocate a new zeros page
            else if (pageType == BSS || pageType == HEAP_STACK){
                newFrame = loadPage(pageNum, NEW_ZEROS_PAGE);
            }
        }
    }

    physicalAddress = (newFrame * this->page_size) + cellOffset;

    return physicalAddress;

}

// This function receives two arguments: 1- the num of the page that need to be loaded, 2- the source which the page exists in
// the function loads the specific page according to the given page number from the given source
// after making the needed resetting of the pointers in the case of executable and swap file
// in the case of NEW_ZEROS_PAGE [ new allocation of a bss, heap or stack ] it just fills a frame with zeros
// after getting an empty frame address by calling the function getEmptyFrame() that cares to get an empty frame
// if there is an empty frame it returns its address if not it swaps a frame or return an text ord not_dirty data frame to be override
int sim_mem::loadPage(int pageNum, int src) {

    int frameNum = getEmptyFrame();
    int fileDescriptor;

    switch (src) {
        case EXECUTABLE_FILE:
            lseek(this->program_fd, pageNum * page_size, SEEK_SET);
            fileDescriptor = this->program_fd;
            break;

        case SWAP_FILE:
            lseek(this->swapfile_fd, page_table[pageNum].swap_index * page_size, SEEK_SET);
            fileDescriptor = this->swapfile_fd;
            break;

        default:
            break;
    }

    int i;
    char tempChar;
    size_t bytesNum = 1;

    for(i = 0; i < (this->page_size); i++){
        if(src == EXECUTABLE_FILE || src == SWAP_FILE) {

            if(read(fileDescriptor, &tempChar, bytesNum) == -1){
                perror("Failed To Read From Exe/Swap File");
                return FAILED;
            }

            main_memory[frameNum*this->page_size + i] = tempChar;
        }
        else {
            main_memory[frameNum * this->page_size + i] = zeroChar;
        }
    }

    // in the case of swap it cares to reset the swapped page in the swap file to zeros
    if(src == SWAP_FILE){
        lseek(this->swapfile_fd, page_table[pageNum].swap_index * page_size, SEEK_SET);

        for(i = 0; i < this->page_size; i++){
            if(write(this->swapfile_fd, &zeroChar, bytesNum) == -1){
                perror("Failed To Write To Swap File");
                return FAILED;
            }
        }

        isUsedSwap[page_table[pageNum].swap_index] = false;
    }

    modifyPagesQueue(pageNum);

    // making all the needed changes to the flags and indicators
    isUsedFrame[frameNum] = true;

    page_table[pageNum].swap_index = NO_SWAP;
    page_table[pageNum].frame = frameNum;
    page_table[pageNum].V = VALID;

    return frameNum;
}


// This function receives no arguments
// the function cares to get an empty frame in the main memory
// if there is an empty one it returns its address if not it either swaps out a frame or returns
// an address of a text or not_dirty data frame to be override on
// in the case of swapping it uses the LUR Second_Chance algorithm to decide which page to be swapped
int sim_mem::getEmptyFrame() {

    int emptyFrameNum;
    int swapPageNum;

    int i;
    int framesNum = MEMORY_SIZE / this->page_size;

    for(i = 0; i < framesNum; i++){
        if(!isUsedFrame[i]){
            emptyFrameNum = i;
            return emptyFrameNum;
        }
    }

    swapPageNum = this->activePagesQueue.front();

    emptyFrameNum = this->page_table[swapPageNum].frame;

    // swapping out the chosen page by the LUR Second_Chance algorithm
    // in the case of text or not_dirty data page the swapOut() will not make any swap
    if(swapOut(swapPageNum) == FAILED){
        fprintf(stderr, "Failed To Find Empty Page In Swap File\n");
        return FAILED;
    }

    this->activePagesQueue.pop();

    return emptyFrameNum;
}

// This function receives one argument: 1- the number of the page that need to be swapped out
// the function swaps out the specific page according to the given page number (page_table[pageNum].frame)
// in the case of text or not_dirty data page the function will not make any swap it just
// will modify the needed indicators
int sim_mem::swapOut(int pageNum) {

    if(getPageType(pageNum) == TEXT || (getPageType(pageNum) == DATA && page_table[pageNum].D == NOT_DIRTY)){
        page_table[pageNum].frame = NO_FRAME;
        page_table[pageNum].V = INVALID;
        return SUCCEEDED;
    }

    int swapFilePagesNum = this->num_of_pages - (this->text_size / this->page_size);
    int emptySwapPageNum = -1;

    int i;
    for(i = 0; i < swapFilePagesNum; i++){
        if(!(this->isUsedSwap[i])){
            emptySwapPageNum = i;
            break;
        }
    }

    if(emptySwapPageNum == -1){
        return FAILED;
    }

    lseek(swapfile_fd, emptySwapPageNum * this->page_size, SEEK_SET);
    size_t bytesNum = 1;

    for(i = 0; i < this->page_size; i++){
        if(write(this->swapfile_fd, &main_memory[(page_table[pageNum].frame * this->page_size) + i], bytesNum) == -1){
            perror("Failed To Write To Swap File");
            return FAILED;
        }
    }

    isUsedSwap[emptySwapPageNum] = true;
    isUsedFrame[page_table[pageNum].frame] = false;

    page_table[pageNum].swap_index = emptySwapPageNum;
    page_table[pageNum].frame = NO_FRAME;
    page_table[pageNum].V = INVALID;

    return SUCCEEDED;
}

// This function receives one argument: 1- the number of the page that want to be added or moved in the queue
// the function adds or moves the given page number in the queue
// so the most active page allocate at the end of the queue
// and the least used page allocate at the top/start of the queue
void sim_mem::modifyPagesQueue(int pageNum) {

    // if the page is invalid that means that the page is not in the queue
    // so add the page to the end of the queue
    if(this->page_table[pageNum].V == INVALID){
        this->activePagesQueue.push(pageNum);
        return;
    }

    // else move the existing page to the end of the queue and
    // keep the other pages number at there order
    if (this->page_table[pageNum].V == VALID){
        int tempPageNum;
        int queueSize = (int) this->activePagesQueue.size();

        int i;
        for(i = 0; i < queueSize; i++){
            tempPageNum = this->activePagesQueue.front();
            this->activePagesQueue.pop();

            if(tempPageNum != pageNum){
                this->activePagesQueue.push(tempPageNum);
            }
        }

        this->activePagesQueue.push(pageNum);
    }

}

// This function receives no arguments
// the function just rests all the memories and indicators to the starting values
void sim_mem::resetMemory() {

    int i;
    for(i = 0; i < MEMORY_SIZE; i++){
        main_memory[i] = zeroChar;
    }

    int textPagesNum = this->text_size / this->page_size;

    for(i = 0; i < this->num_of_pages; i++){
        page_table[i].V = INVALID;
        page_table[i].D = NOT_DIRTY;
        page_table[i].frame = NO_FRAME;
        page_table[i].swap_index = NO_SWAP;

        if(i < textPagesNum){
            page_table[i].P = READ;
        } else {
            page_table[i].P = READ_WRITE;
        }
    }

    lseek(swapfile_fd, 0, SEEK_SET);

    int sizeWithoutText = ((this->num_of_pages - textPagesNum) * this->page_size);
    size_t bytesNum = 1;

    for(i = 0; i < sizeWithoutText; i++){
        if(write(this->swapfile_fd, &zeroChar, bytesNum) == -1){
            perror("Failed To Write To Swap File");
            return;
        }
    }


    int framesNum = MEMORY_SIZE / this->page_size;

    for(i = 0; i < framesNum; i++){
        isUsedFrame[i] = false;
    }

    int swapArrayLen = this->num_of_pages - (this->text_size / this->page_size );

    for(i = 0; i < swapArrayLen; i++){
        isUsedSwap[i] = false;
    }

}

// This function receives one argument: 1- the number of the page that we want to know it's type
// the functions return the type of the page according to simple calculations that depends on the
// arrangement and the sizes of the memory parts and the size of each page
int sim_mem::getPageType(int pageNum) {

    if(pageNum >= 0 && pageNum < text_size / page_size){
        return TEXT;
    }
    else if(pageNum >= text_size / page_size && pageNum < (text_size + data_size) / page_size){
        return DATA;
    }
    else if (pageNum >= (text_size + data_size) / page_size && pageNum < (text_size + data_size + bss_size) / page_size){
        return BSS;
    }
    else if(pageNum >= (text_size + data_size + bss_size) / page_size && pageNum < (text_size + data_size + bss_size + heap_stack_size) / page_size){
        return HEAP_STACK;
    } else {
        return FAILED;
    }
}

/**************************************************************************************/
void sim_mem::print_memory() {
    int i;
    printf("\n Physical memory\n");
    for (i = 0; i < MEMORY_SIZE; i++) { printf("[%c]\n", main_memory[i]); }
}

/************************************************************************************/
void sim_mem::print_swap() {
    char *str = (char*) malloc(this->page_size * sizeof(char));
    int i;
    printf("\n Swap memory\n");
    lseek(swapfile_fd, 0, SEEK_SET); // go to the start of the file
    while (read(swapfile_fd, str, this->page_size) == this->page_size) {
        for (i = 0; i < page_size; i++) {
            printf("%d - [%c]\t", i, str[i]);
        }
        printf("\n");
    }

    free(str);
}

/***************************************************************************************/
void sim_mem::print_page_table(){
    int i;
    printf("\n page table \n");
    printf("Valid\t Dirty\t Permission \t Frame\t Swap index\n");
    for (i = 0; i < num_of_pages; i++) {
        printf("[%d]\t[%d]\t[%d]\t[%d]\t[%d]\n", page_table[i].V, page_table[i].D, page_table[i].P,
               page_table[i].frame, page_table[i].swap_index);
    }
}