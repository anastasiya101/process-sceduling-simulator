#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "roundrobin.h"
#include "sjf.h"
#include "memory_allocation.h"


struct LinkedList* linkedlist_init() {
    struct LinkedList *list = malloc(sizeof(struct LinkedList));
    list->head = NULL;
    list->tail = NULL;
    list->size = 0;
    return list;
}


int already_in_ll(struct LLNode *head, struct LLProcess process){
    struct LLNode *current_node = head;
    while (current_node != NULL) {
        if (strcmp(current_node->my_process.name ,process.name) == 0) {
            // already present in ll
            return 1;
        }
        current_node = current_node->next;
    }

    return 0;


}

// 
void linkedlist_add(struct LinkedList *list, struct LLProcess process) {

    if(already_in_ll(list->head, process)== 1){
        return;
    }

    struct LLNode *new_node = malloc(sizeof(struct LLNode));
    new_node->my_process = process;
    new_node->next = NULL;
    if (list->head == NULL) {
        list->head = new_node;
        list->tail = new_node;
    } else {
        list->tail->next = new_node;
        list->tail = new_node;
    }
    list->size++;
}

const struct LLProcess EMPTY_LLPROCESS = { .arrival_time = -1, .name = "NULL", .cpu_time = -1, .memory_req = -1, .status = EMPTY };

// Get a Process at a specific index in the LinkedList
struct LLProcess linkedlist_get(struct LinkedList *list, int index) {
    if (index >= list->size) {
        // printf("ERROR: index out of bounds\n");
        return EMPTY_LLPROCESS;
    }

    // check ll is not empty
    if(list->head ==NULL){
        // printf("ERROR linked list is empty. \n");
        return EMPTY_LLPROCESS;
    }

    struct LLNode *current = list->head;
    int i = 0;
    while (i < index) {
        current = current->next;
        i++;
    }
    return current->my_process;
}

// Remove a Process at a specific index from the LinkedList
void linkedlist_remove(struct LinkedList *list, int index) {
    if (index >= list->size) {
        //printf("ERROR: index out of bounds\n");
        return;
    }

    // check ll is not empty
    if(list->head ==NULL){
        // printf("ERROR linked list is empty. \n");
        return;
    }
    
    if (index == 0) {
        struct LLNode *temp = list->head;
        list->head = list->head->next;
        free(temp);
    } else {
        struct LLNode *prev = list->head;
        struct LLNode *current = list->head->next;
        int i = 1;
        while (i < index) {
            prev = current;
            current = current->next;
            i++;
        }
        prev->next = current->next;
        free(current);
        if (index == list->size - 1) {
            list->tail = prev;
        }
    }

    list->size--;
}

void linkedlist_free(struct LinkedList *list) {
    struct LLNode *current = list->head;
    while (current != NULL) {
        struct LLNode *temp = current;
        current = current->next;
        free(temp);
    }
    free(list);
}

int rr_main(char *filename,int quantum, char *memory_strat) {

    int best_fit_mem = 0;
    if(strcmp(memory_strat, "best-fit") == 0){
        best_fit_mem = 1;
    }

    // int quantum = 3;
    int current_time = 0;
    int cycle_no = 0;

    FILE* input_file = fopen(filename, "r");

    if(input_file == NULL){
        perror("ERROR: File could not be opened. \n");
        exit(EXIT_FAILURE);
    }

    int no_processes = count_lines(input_file) - 1;
    
    struct LLProcess process[no_processes];

    // inserts the process into an array of structs
    for(int i = 0; i < no_processes; i++){

        int at,ct,mr;
        char name[8];
        fscanf(input_file, "%d %s %d %d\n", &at, name, &ct, &mr);

        process[i].arrival_time = at;
        process[i].cpu_time = ct;
        process[i].memory_req = mr;
        strncpy(process[i].name, name, sizeof(process[i].name) - 1);
        process[i].name[sizeof(process[i].name) - 1] = '\0';
        process[i].remaining_time = ct;
        process[i].status = STANDBY;
    }


    struct LinkedList *ll = linkedlist_init();

    // initialize my memory queue
    struct MemoryQueue *mem_queue = memory_queue_init();
    // printf("mem queue : %d", mem_queue->head->state);
    //add my first process which always has arrival time = 0


    process[0].status = READY;

    if(best_fit_mem == 1){
        int mem_pos = memory_queue_add(mem_queue, process[0].name, process[0].memory_req);
        // mem_pos++;
        // <time>,READY,process_name=<process-name>,assigned_at=<mem_addr>\n
        printf("%d,READY,process_name=%s,assigned_at=%d\n", current_time,process[0].name, mem_pos);
        //printf("%d,READY,process_name=%s,assigned_at=%d\n", current_time,process[0].name, 0);
    }

    //print_mem_queue(mem_queue->head);
    // printf("MEM: %d\n",mem_queue->head->space );
    // printf("first process in mem : %s : %d", mem_queue->head->next->name, mem_queue->head->next->end);
    linkedlist_add(ll, process[0]);

    int finished_processes = 0;
    struct LLProcess prev_process = EMPTY_LLPROCESS;

    // print_mem_queue(mem_queue->head);
    
    while(finished_processes != no_processes){
    
        for (int j = 0; j < no_processes; j++ ){

            if(process[j].status == STANDBY && process[j].arrival_time <= current_time){
                    process[j].status = INPUT;
            }
            // printf("at [%d] : %d \n", j, process[j].arrival_time);
            if(process[j].status == INPUT && process[j].arrival_time <= current_time){
                
                

                if(best_fit_mem == 1){
                    
                    int mem_pos = memory_queue_add(mem_queue, process[j].name, process[j].memory_req);
                    if( mem_pos != 2048){
                        process[j].status = READY;

                        // <time>,READY,process_name=<process-name>,assigned_at=<mem_addr>\n
                        printf("%d,READY,process_name=%s,assigned_at=%d\n", current_time,process[j].name, mem_pos);
                    }
                    
                    // printf("%d,READY,process_name=%s,assigned_at=%d\n", current_time,process[j].name, 0);
                } else {
                process[j].status = READY;
                // ad to the ready linked list
                linkedlist_add(ll, process[j]);
                
                }

                
            } 
        }

        // get the next process in the linked list and run for the amount of quantum

        // next_process : a varaible with the information we need copied into it
        // still need to acces the array via name to modify it
        struct LLProcess next_process = linkedlist_get(ll, 0);
        
        linkedlist_remove(ll, 0);

        if(next_process.status == EMPTY){
            // find next arrival time assuming they are sorted in order of arrival time
            int next_at = process[finished_processes].arrival_time;
            // printf("WAITING : %d seconds\n", next_at - current_time);
            current_time = next_at;
        } else {
            // if same process is running again don't execute the RUNNING line

            // run the process
            // if(current_process != previous_process)
            // printf(" prev proc %s : current proc %s\n", prev_process.name,next_process.name);
            if(strcmp(prev_process.name,next_process.name) != 0){
                printf("%d,RUNNING,process_name=%s,remaining_time=%d \n", current_time, next_process.name, next_process.remaining_time);
            }
            current_time += quantum;
            cycle_no++;


            // check if finished
            int proc_remaining = 0;
            int next_process_index;
            for(int i = 0; i < no_processes; i++){

                // place the process into the input queue because they are ready to be considered to execute
                if(process[i].status == STANDBY && process[i].arrival_time <= current_time){
                    process[i].status = INPUT;
                    // printf("put in input\n");
                }
                

                // get the process we are working on
                if(strcmp(next_process.name,process[i].name) == 0){
                    process[i].remaining_time -= quantum;
                    next_process_index = i;
                    // printf("rem time: %d", process[i].remaining_time );
                    // hecck if it's finished
                    if(process[i].remaining_time <= 0){
                        // memory_queue_remove(mem_queue, process[i].name);
                        process[i].status = FINISHED;
                        next_process.status = FINISHED;
                        finished_processes++;
                        process[i].turnaround_time = current_time - process[i].arrival_time;

                
                    } 
                    
                    
                    // -----if it's not finished add back to the end of the queue
                    /*else{
                        linkedlist_add(ll, process[i]);
                    }  ----- */

                } // end of the process[i] which is our current running process

                

                // calculate proc_remaining
                else if(process[i].status == INPUT || process[i].status == READY){
                    proc_remaining++;
                }

            } // end of for loop


            if(process[next_process_index].status == FINISHED){
                // <time>,FINISHED,process_name=<process-name>,proc_remaining=<num-proc-left>\n
                printf("%d,FINISHED,process_name=%s,proc_remaining=%d\n", current_time, next_process.name, proc_remaining);
            } else{
                for(int i = 0; i < no_processes; i++){
                    // printf("check list %s : %d | ct = %d\n",process[i].name, process[i].arrival_time, current_time );
                    // printf("\n status index : %d", process[i].status);
                    

                    if(process[i].status == INPUT && process[i].arrival_time <= current_time){
                        //printf("EXECUTED");
                        
        
                        if(best_fit_mem == 1){
                            int mem_pos = memory_queue_add(mem_queue, process[i].name, process[i].memory_req);
                            // <time>,READY,process_name=<process-name>,assigned_at=<mem_addr>\n
                            if( mem_pos != 2048){
                                process[i].status = READY;
                                printf("%d,READY,process_name=%s,assigned_at=%d\n", current_time,process[i].name, mem_pos);
                                linkedlist_add(ll, process[i]);
                            }
                            // printf("%d,READY,process_name=%s,assigned_at=%d\n", current_time,process[i].name, 0);
                        } else {
                        // ad to the ready linked list
                        // printf("added to ll : %s\n", process[i].name);
                            process[i].status = READY;
                            linkedlist_add(ll, process[i]);
                        }
                    }
                }

                linkedlist_add(ll, process[next_process_index]);
            } 

        }

        prev_process = next_process;

/*
        struct LLNode *current_node = ll->head;
        printf("ll ");
        while (current_node != NULL) {
            printf(" : %s", current_node->my_process.name);
            current_node = current_node->next;
        }
        printf("\n");
*/
    }


    


    free(ll);

    /*
   -------------------------
     PERFORMANCE STATISTICS  
   -------------------------

    Turnaround Time
    - current_time after execution - arrival_time
    - sum of all
    - divided by no_processes


    Time Overhead
    - turnaround_time / cpu_time
    - loop through to get maximum 
    - while suming them up to get an average


    Makespan
    - current_time

    */

    float avg_tt = 0.0;
    float max_to = 0.0;
    float avg_to = 0.0;
    // printf("finished process : %d\n", finished_processes);
    for(int i = 0; i < finished_processes; i++){

        // sum of all turnaround times
        avg_tt += process[i].turnaround_time;
        // printf("turnaround time : %d\n", process[i].turnaround_time);

        float temp_to = (float) process[i].turnaround_time / (float) process[i].cpu_time;
        if(temp_to > max_to){
            max_to = temp_to;
        }

        avg_to += temp_to;
        // printf("avg to so far : %f\n", avg_to);

    }
    // printf("avg :%f\n", avg_to);
    avg_tt = avg_tt / finished_processes;
    avg_to = avg_to / finished_processes;

    //printf("Turnaround Time %.0f\nTime overhead %.2f %.2f\nMakespan %d\n", ceil(avg_tt), max_to, avg_to, current_time);
    printf("Turnaround time %.0f\nTime overhead %.2f %.2f\nMakespan %d\n", ceil(avg_tt), max_to, avg_to, current_time);
    
    // terminating all processes
    //exit(0);

    return 0;

}
