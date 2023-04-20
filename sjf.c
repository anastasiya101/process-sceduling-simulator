#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "sjf.h"
#include "memory_allocation.h"


// for testing purposes
char *status_str[] = {
    "STANDBY"
    "INPUT",
    "READY",
    "RUNNING",
    "FINISHED",
    "EMPTY"
};


const struct Process EMPTY_PROCESS = { .arrival_time = -1, .name = "NULL", .cpu_time = -1, .memory_req = -1, .status = EMPTY };

struct ReadyQueue* ready_queue_init(){
    struct ReadyQueue *list = malloc(sizeof(struct ReadyQueue));
    list->head = NULL;
    list->tail = NULL;
    list->size = 0;
    return list;
}

void ready_queue_add(struct ReadyQueue *queue, struct Process process){

    struct RNode *new_node = malloc(sizeof(struct RNode));
    new_node->this_process = process;
    new_node->next = NULL;
    if (queue->head == NULL) {
        queue->head = new_node;
        queue->tail = new_node;
    } else {
        queue->tail->next = new_node;
        queue->tail = new_node;
    }
    queue->size++;

}

const struct Process EMPTY_RPROCESS = { .arrival_time = -1, .name = "NULL", .cpu_time = -1, .memory_req = -1, .status = EMPTY };

struct Process ready_queue_get(struct ReadyQueue *list){
/*
    if (index >= list->size) {
        // printf("ERROR: index out of bounds\n");
        return EMPTY_RPROCESS;
    }
*/
    // if nothing in queue 
    if(list->head == NULL){
        return EMPTY_RPROCESS;
    }
    struct Process next_process = list->head->this_process;
    struct RNode *current = list->head;
    int i = 0;
    // get the smallest node

    //find the smallest time
    // int smallest_cpu_time = list->head->this_process.cpu_time;
    
    int index = 0;
    while (i < list->size) {
        if(current->this_process.cpu_time < next_process.cpu_time){
            next_process = current->this_process;
            index = i;
        } else if (current->this_process.cpu_time == next_process.cpu_time){
            if(current->this_process.arrival_time < next_process.arrival_time){
                next_process = current->this_process;
            } else if (current->this_process.arrival_time == next_process.arrival_time){
                if(strcmp(current->this_process.name, next_process.name) < 0){
                    next_process = current->this_process;
                }
            }
        }

        current = current->next;
        i++;
    }
    next_process.index = index;
    // ready_queue_remove(list->head, index);
    return next_process;

}

void ready_queue_remove(struct ReadyQueue *list, int index) {
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
        struct RNode *temp = list->head;
        list->head = list->head->next;
        free(temp);
    } else {
        struct RNode *prev = list->head;
        struct RNode *current = list->head->next;
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

// space complexity O(1)
// time complexity O(N)
// counts the number of process in the file to determine the size of the array to store the process
int count_lines(FILE* input){
    
    int count = 0;
    char ch;

    while((ch = fgetc(input)) != EOF) {
        if (ch == '\n') {
            count++;
        }
    }

    // adds last line
    if(ch != '\n' && count != 0) {
        count++;
    }

    rewind(input);

    return count;

}


// create a new node for the priority queue
struct Node* create_node(struct Process process) {
    struct Node *new_node = (struct Node*) malloc(sizeof(struct Node));
    new_node->ready_process = process;
    new_node->left_child = NULL;
    new_node->right_child = NULL;
    return new_node;
}


int is_empty(struct PQ *pq){
    return pq->root == NULL;
}

struct Node* get_root(struct PQ *pq){

    if (pq == NULL || pq->root == NULL) {
        return NULL;
    }

    return pq->root;
}

void print_my_pq(struct Node* node) {
    if (node == NULL) {
        return;
    }
    print_my_pq(node->left_child);
    printf("Process name: %s, Arrival Time: %d, CPU Time: %d, Memory Required: %d\n", node->ready_process.name, node->ready_process.arrival_time, node->ready_process.cpu_time, node->ready_process.memory_req);
    print_my_pq(node->right_child);
}

/*
// insert a process into the priority queue
void insert_node(struct Node *node, struct Process process, struct PQ *pq) {
    if (node->left_child == NULL) {
        node->left_child = create_node(process);
    } else if (node->right_child == NULL) {
        node->right_child = create_node(process);
    } else {
        if (node->left_child->ready_process.cpu_time < node->right_child->ready_process.cpu_time) {
            insert_node(node->left_child, process, pq);
        } else if (node->left_child->ready_process.cpu_time == node->right_child->ready_process.cpu_time){
            if(node->left_child->ready_process.arrival_time < node->right_child->ready_process.arrival_time){
                insert_node(node->left_child, process, pq);
            } else if (node->left_child->ready_process.arrival_time == node->right_child->ready_process.arrival_time){
                if (strcmp(node->left_child->ready_process.name, node->right_child->ready_process.name) < 0) {
                    insert_node(node->left_child, process, pq);
                } else {
                    insert_node(node->right_child, process, pq);
                }
            } else{
                insert_node(node->right_child, process, pq);
            }
        } else {
            insert_node(node->right_child, process, pq);
        }
    }

    heapify_down(pq->root);
}

*/
// insert a process into the priority queue
void insert_node(struct Node *node, struct Process process, struct PQ *pq) {
    if (node->left_child == NULL) {
        node->left_child = create_node(process);
    } else if (node->right_child == NULL) {
        node->right_child = create_node(process);
    } else {
        insert_node(node->left_child, process, pq);
        /*
        if (node->left_child->ready_process.cpu_time <= node->right_child->ready_process.cpu_time) {
            insert_node(node->left_child, process, pq);
        } else {
            insert_node(node->right_child, process, pq);
        }
        */
    }
        heapify_down(pq->root);
}

// enqueue a new process into the priority queue
void enqueue(struct PQ *pq, struct Process process) {
    if (pq->root == NULL) {
        pq->root = create_node(process);
    } else {
        insert_node(pq->root, process, pq);
    }
    pq->size++;
}

// takes in the root initially 

// Restructure the tree to maintain the min-heap property
void heapify_down(struct Node *node) {
    
    // Find the minimum child node
    struct Node *min_child = NULL;

    int restructure = 0;
    if (node->left_child != NULL && node->right_child != NULL) {
        // printf("pq root %s -> %s : %s ", node->ready_process.name, node->left_child->ready_process.name, node->right_child->ready_process.name);

        if (node->left_child->ready_process.cpu_time < node->right_child->ready_process.cpu_time) {
            min_child = node->left_child;
          // if their cpu_times are equal  
        } else if (node->left_child->ready_process.cpu_time == node->right_child->ready_process.cpu_time){
            if (node->left_child->ready_process.arrival_time < node->right_child->ready_process.arrival_time){
                min_child = node->left_child;
            } else if (node->left_child->ready_process.arrival_time == node->right_child->ready_process.arrival_time){
                // equal cpu_time and arrival_time
                if(strcmp(node->left_child->ready_process.name, node->right_child->ready_process.name) < 0){
                    min_child = node->left_child;
                } else {
                    min_child = node->right_child;
                }
            } else {
                min_child = node->right_child;
            }
        } else {
            min_child = node->right_child;
        }
    } else if (node->left_child != NULL) {
        min_child = node->left_child;
    } else if (node->right_child != NULL) {
        min_child = node->right_child;
    }

    // Swap the node with the minimum child node conditions
    if (min_child != NULL && min_child->ready_process.cpu_time == node->ready_process.cpu_time 
            && min_child->ready_process.arrival_time < node->ready_process.arrival_time) {
        restructure = 1;
    } else if (min_child != NULL && min_child->ready_process.cpu_time < node->ready_process.cpu_time) {
        restructure = 1;
    } else if (min_child != NULL && min_child->ready_process.cpu_time == node->ready_process.cpu_time 
                && min_child->ready_process.arrival_time == node->ready_process.arrival_time 
                && strcmp(min_child->ready_process.name, node->ready_process.name) < 0) {
        restructure = 1;
    }

    if (restructure == 1){
        // printf("restructure: %d\n", restructure);
        struct Process temp_process = node->ready_process;
        node->ready_process = min_child->ready_process;
        min_child->ready_process = temp_process;
        heapify_down(min_child);
    }

}

struct Node* get_last_node(struct Node* node){
    
    // empty tree if no nodes
    if (node == NULL){
        return NULL;
    }

    // last node is the one with no children
    if(node->left_child == NULL && node-> right_child == NULL){
        return node;
    }

    // keep moving right in the tree unless it is empty check left
    if (node->right_child != NULL){
        return get_last_node(node->right_child);
    } else {
        return get_last_node(node->left_child);
    }


}

struct Node* get_parent(struct Node* root, struct Node* node){
    if (root == NULL || root == node) {
        return NULL;
    }

    if (root->left_child == node || root->right_child == node) {
        return root;
    }
    
    struct Node* left = get_parent(root->left_child, node);
    if (left != NULL) {
        return left;
    }

    return get_parent(root->right_child, node);

}

// dequeue removes the highest priority node
struct Process dequeue(struct PQ *pq){
    // hceck pq is not empty
    if (pq->root == NULL) {
        // printf("ERROR priority queue is empty. \n");
        return EMPTY_PROCESS;
    }

    // getting the highest priority process
    struct Process sjf_process = pq->root->ready_process;

    // replacing the root node with the last node
    struct Node *last_node = get_last_node(pq->root);
    // printf("last node :%s\n", last_node->ready_process.name);

    // remove the last node
    if (last_node == pq->root) {
        pq->root = NULL;
    } else {
        
        pq->root->ready_process = last_node->ready_process;
        struct Node *parent = get_parent(pq->root, last_node);
        if (parent->left_child == last_node) {
            parent->left_child = NULL;
        } else {
            parent->right_child = NULL;
        }
        free(last_node);
    }

    // Restructure the tree to maintain the min-heap property
    if (pq->root != NULL) {
        heapify_down(pq->root);
    }
    // printf("the root: %s\n", pq->root->ready_process.name);
    pq->size--;
    return sjf_process;
}


int sjf_main(char *filename,int quant_time, char *memory_strat){

       // initializing simulation time and setting it to 0
    int current_time = 0;

    int best_fit_mem = 0;
    if(strcmp(memory_strat, "best-fit") == 0){
        best_fit_mem = 1;
    }

    // initializing number of cycles completed
    // int cycle_no = 0;

    //int no_processes = 0;

    // take in the input
    // input.txt The file is space delimited,
    //  and each line (including the last) will be terminated 
    // with an LF (ASCII 0x0a) control character.
    FILE* input_file = fopen(filename, "r");

    if(input_file == NULL){
        perror("ERROR: File could not be opened. \n");
        exit(EXIT_FAILURE);
    }

    int no_processes = count_lines(input_file) - 1;
    // printf("no process  :  %d\n", no_processes);

    struct Process process[no_processes];

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

        //fscanf(input_file, "%d %s %d %d\n", &process[i].arrival_time, process[i].name, &process[i].cpu_time, &process[i].memory_req);
        process[i].status = STANDBY;
        // printf("process at = %u \n",(unsigned int) process[i].status);
    }

 //making sure im reading in the input correctly
/*
    for (int i = 0; i < 4; i++) {
        printf("Process %d: arrival time = %d, name = %s, CPU time = %d, memory requirement = %d, status = %s \n", i + 1, process[i].arrival_time, process[i].name, process[i].cpu_time, process[i].memory_req, status_str[process[i].status]);
    }
*/

    fclose(input_file);

   
    // initalize ready queue
    // -------------------------- struct PQ pq = {NULL, 0};

    // initialize my ready queue list
    struct ReadyQueue *ready_queue = ready_queue_init();

    /* ------------
    struct Process ready_queue[no_processes];

    // initialize queue with empty processes
    for(int i = 0; i < no_processes; i++){
        ready_queue[i] = EMPTY_PROCESS;
        // printf(" %d ready _queue : %s", i, ready_queue[i].name);
    }
    -------------- */

    int finished_processes = 0;
    // printf("finished process : %d\n", finished_processes);
    while(finished_processes != no_processes){

        // change process status to INPUT


        // compare arrival time of the process to the current time
        // if AT <= CT ; add to ready queue
        for (int j = 0; j < no_processes; j++ ){

            if(process[j].status == STANDBY && process[j].arrival_time <= current_time){
                    process[j].status = INPUT;
            }

            // printf("at [%d] : %d \n", j, process[j].arrival_time);
            if(process[j].status == INPUT && process[j].arrival_time <= current_time){
                process[j].status = READY;

                if(best_fit_mem == 1){
                    // <time>,READY,process_name=<process-name>,assigned_at=<mem_addr>\n
                    printf("%d,READY,process_name=%s,assigned_at=%d\n", current_time,process[j].name,0);
                }

                // add to pq
                // printf("process : %s \n", process[j].name);

                // ---------------- enqueue(&pq, process[j]);

                ready_queue_add(ready_queue, process[j]);

                //----------------ready_queue[j] = process[j];-----------------
            }

        }

        // struct Node *root = get_root(&pq);
        //print_my_pq(root);



        // pick process witht he shortest cpu_time

        struct Process sjf_process = ready_queue_get(ready_queue);
        ready_queue_remove(ready_queue, sjf_process.index);
        // --------------------struct Process sjf_process = dequeue(&pq); // --------------------------

        
        // if status is empty means ready queue is empty
        // we need to complete a wait period until the next arrival time
        if(sjf_process.status == EMPTY){
            // find next arrival time assuming they are sorted in order of arrival time
            int next_at = process[finished_processes].arrival_time;
            // printf("WAITING : %d seconds\n", next_at - current_time);
            current_time = next_at;
            
        } else {
            // run the process
            sjf_process.status = RUNNING;

            // <time>,RUNNING,process_name=<process-name>,remaining_time=<T>\n
            printf("%d,RUNNING,process_name=%s,remaining_time=%d \n", current_time, sjf_process.name, sjf_process.cpu_time);
            
            current_time += (int) ceil((float) sjf_process.cpu_time / (float) quant_time) * quant_time;

            // add to finished state
            sjf_process.status = FINISHED;
            finished_processes++;
            
            int proc_remaining = 0;
            for (int j = 0; j < no_processes; j++ ){

                // place the process into the input queue because they are ready to be considered to execute
                if (process[j].status == STANDBY && process[j].arrival_time <= (current_time - quant_time)){
                    process[j].status = INPUT;
                    // printf("put in input\n");
                }

                if (strcmp(sjf_process.name,process[j].name) == 0){
                    process[j].status = FINISHED;
                    
                    // printf("FINISHED %d :%s \n", j, process[j].name);
                    process[j].turnaround_time = current_time - process[j].arrival_time;
                }
                

                // calculate proc_remaining
                if (process[j].status == INPUT || process[j].status == READY){
                    proc_remaining++;
                } 


            }


            
            // <time>,FINISHED,process_name=<process-name>,proc_remaining=<num-proc-left>\n
            printf("%d,FINISHED,process_name=%s,proc_remaining=%d\n", current_time, sjf_process.name, proc_remaining);

            // Turnaround Time = current_time after execution - arrival_time
            //sjf_process.turnaround_time = current_time - sjf_process.arrival_time;
        }

        // deallocate memory
        // sjf_process = EMPTY_PROCESS;
    }

/*
        // check if any process left in ready state
        if(sjf_process.status == EMPTY){
            // check INPUT status
            // if there is still input get the one with the next arrival time
            int shortest_at = -1;
            for(int i = 0; i < no_processes; i++){
                if(process[i].status == INPUT){
                    if(process[i].arrival_time < shortest_at){
                        shortest_at = process[i].arrival_time;
                    }
                }
            }
        }
*/






    // de-allocating memory - linked list
    /*
    while (head) {
        Process* next = head->next;
        free(head);
        head = next;
    }
    */


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

    return 0;
}