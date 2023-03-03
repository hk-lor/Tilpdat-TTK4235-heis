#pragma once

#include "stdio.h"

#define str(x)
#define xstr(x) str(x)

#define MAX_QUEUE_SIZE 10

enum direction {UP=0, DOWN};

struct order {
    int floor;
    enum direction dir;
    struct order* next;
    struct order* prev;
};

int determine_next_floor();
int queue_update();
int queue_insert(struct order* new_order);
int queue_delete(struct order* new_order);
int queue_create_new_order(int floor, enum direction dir);
int queue_initalize();
int queue_print();
int queue_length();
int queue_flush();

struct order* queue_search(int floor);
struct order* queue_find_latest_order();
int queue_update_next_order();
int queue_print_current_order();
int util_print_order(struct order* order);
void queue_order_finished_signal();
int update_elevator_current_floor();