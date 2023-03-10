#pragma once

#include "stdio.h"
#include "../source/driver/elevio.h"

struct order {
    int floor;
    ButtonType dir;
    struct order* next;
    struct order* prev;
};

struct fsm_packet {
    int current_order_dir;
    int current_order_floor;
    int next_order_dir;
    int next_order_floor;
    int elevator_current_floor;
};

// Basic operations 
int queue_update();
int queue_insert(struct order* new_order);
int queue_delete(struct order* new_order);
int queue_create_new_order(int floor, ButtonType dir);
int queue_initalize();
int queue_fetch_length();
int queue_flush();
void queue_remove_floor_orders(int floor);
struct order* queue_search(int floor);
struct order* queue_fetch_latest_order();
struct order* queue_fetch_order(int i);

// Queue Algorithm functions
void queue_assign_current_global_order(struct order* order);
void queue_assign_next_global_order(struct order* order);
void queue_set_global_orders_as_empty();

// Functions for external modules
int queue_update_elevator_current_floor();
struct fsm_packet queue_update_fsm();

// Debugging utils
int util_print_order(struct order* order);
int util_queue_print_current_order();
int util_queue_print();
int util_queue_print_next_order();

// Testing
void queue_simulate();

