// Program Name: Virtual Memory Management Simulator [ex6]
// Program Description: This program is a Virtual Memory Management Simulator
// that simulates the steps and operations carried out by the memory management unit
//when the processor requests access to a specific address in the main memory.

// Student Name: Muath Abu Jamal


#ifndef OSEX6_SIM_MEM_H
#define OSEX6_SIM_MEM_H

#include <queue>


// defining the needed preprocessor directives
#define VALID 1
#define INVALID 0
#define DIRTY 1
#define NOT_DIRTY 0
#define NO_FRAME -1
#define NO_SWAP -1
#define READ 0
#define READ_WRITE 1
#define FAILED -1
#define SUCCEEDED 1
#define EXECUTABLE_FILE 0
#define SWAP_FILE 1
#define NEW_ZEROS_PAGE 2
#define TEXT 0
#define DATA 1
#define BSS 2
#define HEAP_STACK 3
#define NO_FILE -1

#define MEMORY_SIZE 10
extern char main_memory[MEMORY_SIZE];

const char zeroChar = '0';

typedef struct page_descriptor
{
    int V; // valid
    int D; // dirty
    int P; // permission
    int frame; //the number of a frame if in case it is page-mapped
    int swap_index; // where the page is located in the swap file.
} page_descriptor;

class sim_mem {
    int swapfile_fd; //swap file fd
    int program_fd; //executable file fd
    int text_size;
    int data_size;
    int bss_size;
    int heap_stack_size;
    int num_of_pages;
    int page_size;
    page_descriptor *page_table; //pointer to page table


public:
    sim_mem(char exe_file_name[], char swap_file_name[], int text_size, int data_size, int bss_size, int heap_stack_size, int num_of_pages, int page_size);
    ~sim_mem();
    char load(int address);
    void store(int address, char value);
    void print_memory();
    void print_swap ();
    void print_page_table();

// Declaring the needed private variables and functions for the purpose of modularity
// (explanation for each function writen in the file sim_mem.cpp (where each function written))
private:
    bool* isUsedFrame;
    bool* isUsedSwap;
    std::queue<int> activePagesQueue;

    void resetMemory();
    int getPhysicalAddress(int logicalAddress);
    int loadPage(int pageNum, int from);
    int getPageType(int pageNum);
    int getEmptyFrame();
    int swapOut(int pageNum);
    void modifyPagesQueue(int pageNum);
};


#endif //OSEX6_SIM_MEM_H
