#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "sjf.h"
#include "roundrobin.h"
#include "memory_allocation.h"



/*
allocate -f <filename> -s (SJF | RR) -m (infinite | best-fit) -q (1 | 2 | 3)

-f filename will specify a valid relative or absolute path to the input file describing the processes. 
-s scheduler where scheduler is one of {SJF, RR}.
-m memory-strategy where memory-strategy is one of {infinite, best-fit}.
-q quantum where quantum is one of {1, 2, 3}.

*/


// argc : number of arguments passed to the program
// argv : an array of strings representing the arguments
int main(int argc, char *argv[]){

    char *filename = NULL;
    char *scheduler = NULL;
    char *memory_strat = NULL;
    int quantum = 0;

    for (int i = 0; i < argc; i++) {
        if (strcmp(argv[i], "-f") == 0 && i + 1 < argc) {
            filename = argv[++i];
        } else if (strcmp(argv[i], "-s") == 0 && i + 1 < argc) {
            scheduler = argv[++i];
        } else if (strcmp(argv[i], "-m") == 0 && i + 1 < argc) {
            memory_strat = argv[++i];
        } else if (strcmp(argv[i], "-q") == 0 && i + 1 < argc) {
            quantum = atoi(argv[++i]);
        }
    }
    
    
    // printf("filename: %s, scheduler: %s, memory_strat: %s, quantum: %d\n", 
    //    filename, scheduler, memory_strat, quantum);

    
    if(strcmp(scheduler, "SJF") == 0){
        sjf_main(filename, quantum, memory_strat);
    } else if(strcmp(scheduler, "RR") == 0){
        rr_main(filename, quantum, memory_strat);
    } else {
        printf("ERROR: unkown schedule type");
        return 0;
    }
    
    // sjf_main(filename, quantum);
    
    
    return 0;
}