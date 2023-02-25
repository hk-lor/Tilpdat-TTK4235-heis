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
int update_queue();
int queue_insert(struct order* new_order);
int queue_delete(struct order* new_order);
int queue_create_new_order(int floor, enum direction dir);
int queue_initalize();
int queue_print();
int queue_flush();
struct order* queue_search(int floor);
struct order* queue_find_newest_order();
