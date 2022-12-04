// Program Name: Virtual Memory Management Simulator [ex6]
// Program Description: This program is a Virtual Memory Management Simulator
// that simulates the steps and operations carried out by the memory management unit
//when the processor requests access to a specific address in the main memory.

// Student Name: Muath Abu Jamal 


#include "sim_mem.h"

char main_memory[MEMORY_SIZE];


int main() {

    sim_mem mem_sm((char*) "exec_file.txt", (char*) "swap_file.txt" ,10, 5, 5,10, 6, 5);

    mem_sm.store(10, 'Z');
    mem_sm.store(15, 'W');
    mem_sm.print_memory();
    mem_sm.print_swap();
    mem_sm.print_page_table();

    mem_sm.store(20, 'Y');
    mem_sm.store(25, 'X');
    mem_sm.print_memory();
    mem_sm.print_swap();
    mem_sm.print_page_table();

    mem_sm.load(0);
    mem_sm.load(5);
    mem_sm.print_memory();
    mem_sm.print_swap();
    mem_sm.print_page_table();

    mem_sm.store(13, 'T');
    mem_sm.store(18, 'J');
    mem_sm.print_memory();
    mem_sm.print_swap();
    mem_sm.print_page_table();

    mem_sm.store(23, 'L');
    mem_sm.store(28, '*');
    mem_sm.print_memory();
    mem_sm.print_swap();
    mem_sm.print_page_table();

    mem_sm.load(0);
    mem_sm.load(5);
    mem_sm.load(0);
    mem_sm.print_memory();
    mem_sm.print_swap();
    mem_sm.print_page_table();

    mem_sm.load(12);
    mem_sm.print_memory();
    mem_sm.print_swap();
    mem_sm.print_page_table();

    return 0;
}
