#include "queue.h"
#include "stdlib.h"
#include "stdint.h"
#include "stdio.h"
#include "string.h"
#include "elevio.h"

//////////////////////////////////////////////////////////////////////////////////////////////////
// Queue variables
//////////////////////////////////////////////////////////////////////////////////////////////////

static struct order* head;                      // Linked list queue head
static int queue_order_length = 0;

static int global_elevator_current_floor = 0;
static struct order* global_elevator_current_order = NULL;
static struct order* global_elevator_next_order = NULL;

//////////////////////////////////////////////////////////////////////////////////////////////////
// Queue basic operations
//////////////////////////////////////////////////////////////////////////////////////////////////

int queue_initalize() {
   head = NULL;
   return 0;
}

int queue_insert(struct order* new_order) {
    new_order->next = head;
    if(head != NULL) {
        head->prev = new_order;
    }
    head = new_order;
    head->prev = NULL;
    return 0;
}

int queue_delete(struct order* order) {
    if(order == NULL) {
        return 1;
    }
    if (order->prev != NULL) {
        order->prev->next = order->next;
    }
    else {
        head = order->next;
    }
    if (order->next != NULL) {
        order->next->prev = order->prev;
    }
    if(order != NULL) {
        free(order);
        queue_order_length -= 1;
    }

    return 0;
}

struct order* queue_fetch_latest_order() {
    struct order* order = head;

    while (order != NULL && order->floor > 0 && order->dir > 0) {
        order = order->next;
    }
    return order;
}

struct order* queue_search(int floor) {
    struct order* order = head;

    while (order != NULL && order->floor != floor) {
        order = order->next;
    }
    return order;                               // return allocated structure (has to be freed)
}

int queue_length() {
    return queue_order_length;
}

int queue_create_new_order(int floor, ButtonType dir) {
    struct order* new_order = malloc(sizeof(struct order));
    
    new_order->dir = dir;
    new_order->floor = floor;

    queue_insert(new_order);
    queue_order_length += 1;

    return 0;
}

int queue_flush() {
    int queue_not_empty = 1;

    struct order* order = NULL;

    for(int floor = 0; floor < 5; floor++) {
        order = queue_search(floor);
        if(order != NULL) {
            queue_delete(order);
        }
    }
    return 0;
}

void queue_remove_floor_orders() {
    struct order* floor_order = queue_search(global_elevator_current_order->floor);
    while(floor_order != NULL) {
        floor_order = queue_search(global_elevator_current_order->floor);
        if(floor_order != NULL) {
            queue_delete(floor_order);
        }
    }
};

//////////////////////////////////////////////////////////////////////////////////////////////////
// Queue algorithms
//////////////////////////////////////////////////////////////////////////////////////////////////

int fetch_elevator_current_floor() {

    return 0;
}

int queue_update_next_order() {
    return 0;  
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// Queue debugging utils
//////////////////////////////////////////////////////////////////////////////////////////////////

int util_queue_print_current_order() {
    if(global_elevator_current_order == NULL) {
        return 1;
    } else {
        printf("test \n");
        //printf("Current order: Floor %i Direction %i \n", global_elevator_current_order->floor, global_elevator_current_order->dir);
        return 0;
    }
};

int util_print_order(struct order* order) {
    if(order == NULL) {
        return 1;
    } else {
        printf("Order: Floor %i Direction %i \n", order->floor, order->dir);
        return 0;
    }
}

int util_queue_print() {
    int queue_index = queue_order_length;
    struct order* order = head;

    while(order != NULL) {
        printf("Order: %i Floor: %i \n", queue_index, order->floor);
        order = order->next;
        queue_index -= 1;
    }
    return 0;
}