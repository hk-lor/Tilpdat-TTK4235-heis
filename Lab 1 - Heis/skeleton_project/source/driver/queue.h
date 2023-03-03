#pragma once

#include "stdio.h"
#include "elevio.h"

#define MAX_QUEUE_SIZE 10

struct order {
    int floor;
    ButtonType dir;
    struct order* next;
    struct order* prev;
};

int queue_update();
int queue_insert(struct order* new_order);
int queue_delete(struct order* new_order);
int queue_create_new_order(int floor, ButtonType dir);
int queue_initalize();
int queue_length();
int queue_flush();
int queue_update_next_order();
void queue_remove_current_order();

int update_elevator_current_floor();
int determine_next_floor();

struct order* queue_search(int floor);
struct order* queue_fetch_latest_order();

int util_print_order(struct order* order);
int util_queue_print_current_order();
int util_queue_print();

