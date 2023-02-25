#include "queue.h"
#include "stdlib.h"
#include "stdint.h"
#include "stdio.h"

static struct order* head;      // Linked list queue

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
    }
    
    return 0;
}

struct order* queue_find_newest_order() {
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
    return order;
}

int queue_length() {
    int length = 0;
    struct order* order = head;
    while (order != NULL && order->floor >= 0 && order->dir >= 0) {
        order = order->next;
        length += 1;
    }
    return length;
}

int queue_create_new_order(int floor, enum direction dir) {
    struct order* new_order = malloc(sizeof(struct order));
    
    new_order->dir = dir;
    new_order->floor = floor;

    queue_insert(new_order);
    return 0;
}

int queue_print() {
    int queue_index = 0;
    struct order* order = head;

    while(order != NULL) {
        printf("Order: %i Floor: %i \n", queue_index, order->floor);
        order = order->next;
        queue_index += 1;
    }

    return 0;
}

int queue_flush() {

    int queue_not_empty = 1;

    struct order* order = NULL;

    while (queue_not_empty)
    {
        order = queue_find_newest_order();

        if(order != NULL) {
            queue_delete(order);
        } else {
            queue_not_empty = 0;
        }
    }

    return 0;
}

int update_queue() {
    return 0;
};

int determine_next_floor() {
    return 0;
}

