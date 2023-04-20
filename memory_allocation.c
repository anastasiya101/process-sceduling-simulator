#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "memory_allocation.h"

struct MemoryQueue* memory_queue_init() {
    struct MemoryQueue *queue = malloc(sizeof(struct MemoryQueue));
    struct QNode init_hole = {.state = HOLE, .name = "EMPTY", .space = 2048, .start = 0, .end = 2048, .next = NULL};
    queue->head = &init_hole;
    queue->tail = &init_hole;
    queue->size = 1;
    
    return queue;
}

void print_mem_queue(struct QNode *node){

    while(node != NULL){
        printf(" : %d", node->space);
        node = node->next;
    }

    return;
}

struct QNode create_hole(struct QNode *current_node, int space, int start_pos){
    struct QNode QUEUE_HOLE = {.state = HOLE, .name = "EMPTY", .space = space, .start = start_pos, .end = (start_pos + space), .next = NULL};
    current_node->next = &QUEUE_HOLE;
    //printf("SPACE : %d", QUEUE_HOLE.space);
    
     /*
    QUEUE_HOLE->state = HOLE;
    strcpy(QUEUE_HOLE->name,"EMPTY");
    QUEUE_HOLE->space = 2048 - space;
    QUEUE_HOLE->start = space;
    QUEUE_HOLE->end = 2048;
    QUEUE_HOLE->next = NULL;
    */

    return QUEUE_HOLE;
}

int memory_queue_add(struct MemoryQueue *queue, char *name, int space){

    // printf("CALLED");
    // create a node
    struct QNode *new_node = malloc(sizeof(struct QNode));
    strcpy(new_node->name, name);
    if(strcmp(name, "EMPTY") == 0){
        new_node->state = HOLE;
    } else {
        new_node->state = PROCESS;
    }
    new_node->space = space;

    // if empty add at the start 
    if(queue->head->space == 2048 && queue->head->state == HOLE){
        // printf("EXECUTED");

        // printf("first hole");
        // add process
        queue->head = new_node;
        new_node->start = 0;
        new_node->end = space;

        // create hole if there is space
        if(space < 2048){
            // printf("first space %d", space);
            ///////////////struct QNode QUEUE_HOLE = create_hole(new_node, 2048 - space, 2048 - space);
            // new_node->next = &QUEUE_HOLE;
            //QUEUE_HOLE.next = NULL;

            // printf("hole node :%d\n", QUEUE_HOLE.space );
            
            /*
            QUEUE_HOLE->state = HOLE;
            strcpy(QUEUE_HOLE->name,"EMPTY");
            QUEUE_HOLE->space = 2048 - space;
            QUEUE_HOLE->start = space;
            QUEUE_HOLE->end = 2048;
            QUEUE_HOLE->next = NULL;
            */
        }
        return 0;

    }

    // perform best fit algorithm
    // loop through all nodes
    // if hole check if space is compatable if it is
    struct QNode *current_node = queue->head;
    // printf("next node space : %d", current_node->next->space);
    // printf("queue space : %d\n", queue->head->space);
    int best_fit_space = 2048;
    int start_pos = 2048;
    int end_pos;
    int index = 0;
    int best_index;
    // printf("HEAD : \n");
    // printf("%s\n", queue->head->name);
    enum state hole = HOLE;
    while(current_node != NULL){
        
        // printf("checking all nodes : %d\n", current_node->state);
        if(current_node->state ==  hole){
            //check it fits
            // printf("current space  %d : new node space %d\n",current_node->space, new_node->space);
            if(current_node->space >= new_node->space){
                //check if its the smallest
                // if equal we want the earlier hole so no change
                
                if(current_node->space < best_fit_space){
                    // printf("current node space : %d\n", current_node->space);
                    best_fit_space = current_node->space;
                    start_pos = current_node->start;
                    end_pos = current_node->end;
                    best_index = index;
                }
            }
        }

        current_node = current_node->next;
        index++;
    }

    // remove the hole and replace with process and new hole
    // memory_queue_remove(queue, start_pos);

    // we have bext_index , start pos, end pos
    // insert the new node and the hole

    struct QNode *current = queue->head;
    struct QNode *previous = NULL;

    int count = 0;
    while(current != NULL&& count < best_index){
        if(count == best_index){
            struct QNode hole_node = create_hole(new_node, (end_pos - space), (space + start_pos));
            new_node->start = start_pos;
            new_node->end = (start_pos + space);
            new_node->next = &hole_node;
            hole_node.next = current->next;
            if (previous == NULL) {
                queue->head = new_node;
            } else {
                previous->next = new_node;
            }
            free(current);
        }
        count++;
        previous = current;
        current = current->next;


    }

    // return the position assigned at
    // if start_pos = 2048 no space is available
    // printf("start pos : %d", start_pos);
    return start_pos;


}

void memory_queue_remove(struct MemoryQueue *queue, char *node_name){

    // remove from list 
    
    struct QNode *tmp = queue->head; // c
    struct QNode *current = NULL; // p
    struct QNode *previous = NULL; // pp
    struct QNode *p_p = NULL; // ppp

// current becomes previous
// pp becomes current
    int count = 0;
    int index = 2048;
    // locate the nodes index and current and previous nodes
    //f("WHILE\n");
    while(current != NULL && count < index){

        if(strcmp(current->name, node_name) == 0){
            index = count;
            //printf("INDEXX : %d", index);
        }
        p_p = previous;
        previous = current;
        current = tmp;
        tmp = tmp->next;
        count++;
    }

    // printf("prev stat %d : current stat %d : next state %d\n",previous->state, current->state, current->next->state);
    //printf("EXITTEDD \n");
    if(current == NULL){
        // printf("ERROR: index out of bounds");
        return;
    }

    //printf("kill");
    // if the next element is NULL or a PROCESSi want make the node into a hole
    if(previous->state == PROCESS){
        //printf("HEADDDDD\n");
    }
    if((previous == NULL && (current->next == NULL  || current->next->state == PROCESS)) || (previous->state == PROCESS && (current->next == NULL  || current->next->state == PROCESS))){
        //printf("THIS111\n\n");
        struct QNode new_current = create_hole(previous, current->space, current->start);
        if(previous == NULL){
            queue->head = &new_current;
        }
        previous->next = &new_current;
        new_current.next = current->next;
        // free(current);

    } 
    // we want to merge with the hole behind us
    else if (previous->state == HOLE && (current->next->state == PROCESS || current->next == NULL)){ 
        //printf("THIS>\n\n");
        struct QNode new_current = create_hole(previous, (previous->space + current->space), previous->start );
        p_p->next = &new_current;
        if(p_p == NULL){
            queue->head = &new_current;
        }
        new_current.next = current->next;
        // free(current);
        // free(previous);


    } 
    // merge with the hole in front of us
    else if(current->next->state == HOLE && (previous->state == PROCESS || previous == NULL)){
        //printf("THIS3\n\n");
        struct QNode new_current = create_hole(current, (current->space + current->next->space), current->start );
        previous->next = &new_current;
        if(previous == NULL){
            queue->head = &new_current;
        }
        new_current.next = current->next->next;
        // current->next  = &new_current;
        // current = &new_current;
        // free(current);
        // free(current->next);
    } 
    // merge the hole in front of us and behind us
    else if(previous->state == HOLE && current->next->state == HOLE){
        //printf("THIS555\n\n");
        struct QNode new_current = create_hole(current, (current->space + current->next->space + previous->space), previous->start );
        p_p->next = &new_current;
        new_current.next = current->next->next;
        // free(previous);
        // free(current);
        // free(current->next);

    } else{
        printf("ERROR: unkown location to place node");
    }

    return;

}







