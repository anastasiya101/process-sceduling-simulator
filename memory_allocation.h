#ifndef MEMALLOC_H
#define MEMALLOC_H

enum state {
    HOLE,
    PROCESS
};

struct QNode {
    enum state state; // hole | process
    char name[8]; // EMPTY | process.name
    int space;
    int start;
    int end;
    struct QNode *next; // pointer to next node
};

struct QNode create_hole(struct QNode *current_node, int space, int start_pos);

struct MemoryQueue {
    struct QNode *head;
    struct QNode *tail;
    int size;
};
void print_mem_queue(struct QNode *node);
struct MemoryQueue* memory_queue_init();
int memory_queue_add(struct MemoryQueue *queue, char *name, int space);
void memory_queue_remove (struct MemoryQueue *queue, char *node_name);







#endif /* MEMALLOC_H */