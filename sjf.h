#ifndef SJF_H
#define SJF_H


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

enum status {
    STANDBY,
    INPUT,
    READY,
    RUNNING,
    FINISHED,
    EMPTY
};

typedef struct Process {
    int arrival_time; // [0,2^32)
    char name[8]; // min length 1, max length 8
    int cpu_time; // [1,232) aka service_time
    int remaining_time;
    int index;
    int memory_req; // [1, 2048]
    int turnaround_time; // current_time @ completion - arrival_time
    enum status status; // state of the process in one of the four possible states as specififed by the enum
} Process;

struct Node {
    struct Process ready_process;
    struct Node *left_child;
    struct Node *right_child;
};

extern const struct Process EMPTY_RPROCESS;

struct PQ{
    struct Node *root;
    int size;
};

struct RNode {
    struct Process this_process;
    struct RNode *next;
};

struct ReadyQueue {
    struct RNode *head;
    struct RNode *tail;
    int size;

};

struct ReadyQueue* ready_queue_init();
void ready_queue_add(struct ReadyQueue *queue, struct Process process);
struct Process ready_queue_get(struct ReadyQueue *list);
void ready_queue_remove(struct ReadyQueue *list, int index);

int count_lines(FILE* input);

extern const struct Process EMPTY_PROCESS;

struct Node* create_node(struct Process process);

int is_empty(struct PQ *pq);

struct Node* get_root(struct PQ *pq);

void print_my_pq(struct Node* node);

void insert_node(struct Node *node, struct Process process, struct PQ *pq);

//void insert_node(struct Node *root, struct Process process, struct PQ *pq);

//void insert_node_helper(struct Node *node, struct Node *new_node, struct PQ *pq);

void enqueue(struct PQ *pq, struct Process process);

void heapify_down(struct Node *node);

struct Node* get_last_node(struct Node* node);

struct Node* get_parent(struct Node* root, struct Node* node);

struct Process dequeue(struct PQ *pq);

int sjf_main(char *filename,int quantum, char *memory_strat);


#endif /* SJF_H */
