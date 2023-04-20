#ifndef ROUNDROBIN_H
#define ROUNDROBIN_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "sjf.h"
#include "memory_allocation.h"


enum LLstatus {
    RAWR,
    SCREAM,
    SWIM
};

typedef struct LLProcess {
    int arrival_time; // [0,2^32)
    char name[8]; // min length 1, max length 8
    int cpu_time; // [1,232) aka service_time
    int memory_req; // [1, 2048]
    int remaining_time;
    float turnaround_time;
    enum status status; // state of the process in one of the four possible states as specififed by the enum
} LLProcess;

// linked list node
 struct LLNode {
    struct LLProcess my_process;
    struct LLNode *next;
};

extern const struct LLProcess EMPTY_LLPROCESS;

struct LinkedList {
    struct LLNode *head;
    struct LLNode *tail;
    int size;
};

struct LinkedList *linkedlist_init();

void linkedlist_add(struct LinkedList *list, struct LLProcess process);

struct LLProcess linkedlist_get(struct LinkedList *list, int index);

void linkedlist_remove(struct LinkedList *list, int index);

void linkedlist_free(struct LinkedList *list);

int rr_main(char *filename,int quantum, char *memory_strat);




#endif /* ROUNDROBIN_H */