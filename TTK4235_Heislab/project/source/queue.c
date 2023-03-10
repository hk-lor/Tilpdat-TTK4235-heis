#include "stdlib.h"
#include "stdint.h"
#include "stdio.h"
#include "string.h"
#include <assert.h>
#include "../source/driver/elevio.h"
#include "../include/queue.h"

//////////////////////////////////////////////////////////////////////////////////////////////////
// Queue variables
//////////////////////////////////////////////////////////////////////////////////////////////////

static struct order* head;                      // Linked list queue head
static int queue_length = 0;

static int global_elevator_current_floor = 0;
static int global_previous_floor = 0;

static struct order global_elevator_current_order;
static struct order global_elevator_next_order;

//////////////////////////////////////////////////////////////////////////////////////////////////
// Queue basic operations
//////////////////////////////////////////////////////////////////////////////////////////////////

int queue_initalize() {
    head = NULL;
    global_elevator_current_order.floor = -1;
    global_elevator_next_order.floor = -1;

   return 0;
}

int queue_insert(struct order* new_order) {

    if(new_order == NULL) {
        return 1;
    } 

    new_order->next = head;
    if(head != NULL) {
        head->prev = new_order;
    }
    head = new_order;
    head->prev = NULL;

    queue_length += 1;

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
        queue_length -= 1;
    }

    return 0;
}

struct order* queue_fetch_latest_order() {
    struct order* order = head;

    while (order != NULL && order->floor > -1 && order->dir > -1) {
        order = order->next;
    }
    return order;
}

struct order* queue_fetch_order(int i) {
    struct order* order = head;

    for (int j = 0; j < i; ++j) {
        order = order->next;
    }
    return order;
};


struct order* queue_search(int floor) {
    struct order* order = head;

    while (order != NULL && order->floor != floor) {
        order = order->next;
    }
    return order;                               // return allocated structure (has to be freed)
}

int queue_fetch_length() {
    return queue_length;
}

int queue_create_new_order(int floor, ButtonType dir) {
    struct order* new_order = malloc(sizeof(struct order));
    
    new_order->dir = dir;
    new_order->floor = floor;

    queue_insert(new_order);

    return 0;
}

int queue_flush() {
    int queue_not_empty = 1;

    struct order* order = NULL;

    for(int floor = 0; floor < N_FLOORS; floor++) {
        order = queue_search(floor);
        if(order != NULL) {
            queue_delete(order);
        }
    }

    queue_set_global_orders_as_empty();

    return 0;
}

void queue_remove_floor_orders(int floor) {
    struct order* floor_order = queue_search(floor);

    while(floor_order != NULL) {
        floor_order = queue_search(floor);
        if(floor_order != NULL) {
            queue_delete(floor_order);
            
        }
    }
    queue_set_global_orders_as_empty();
};

// structs are statically allocated due to getting into trouble when using pointers
void queue_set_global_orders_as_empty() {
    global_elevator_current_order.floor = -1;
    global_elevator_next_order.floor = -1;
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// Queue external module function calls
//////////////////////////////////////////////////////////////////////////////////////////////////

int queue_update_elevator_current_floor() {
    int elevator_current_floor = elevio_floorSensor();
    if(elevator_current_floor != -1) {
        global_previous_floor = global_elevator_current_floor;
        global_elevator_current_floor = elevator_current_floor;
    }
    return 0;
}

struct order queue_update_fsm() {
    queue_update();

    struct fsm_packet packet; 

    packet.current_order_dir = global_elevator_current_order.dir;
    packet.current_order_floor = global_elevator_current_order.floor;

    packet.next_order_dir = global_elevator_next_order.dir;
    packet.next_order_floor = global_elevator_next_order.floor;

    packet.current_floor = global_elevator_current_floor;
    
    return fsm_packet;
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// Queue algorithm
//////////////////////////////////////////////////////////////////////////////////////////////////

int queue_update() {
    queue_update_elevator_current_floor();

    for (int i = 0; i < queue_length; ++i) {

        struct order* order = queue_fetch_order(i);

        if(order == NULL && queue_length == 0) {
            queue_set_global_orders_as_empty();
            return 1;
        } 
        else {
            if(global_elevator_current_order.floor == -1) {         // Check if current_order is empty
                queue_assign_current_global_order(order);

            } else if(global_elevator_next_order.floor == -1) {     // Check if next_order is empty
                queue_assign_next_global_order(order);
            } else if(order->dir == BUTTON_CAB) {
                int previous_dir = global_elevator_current_floor - global_previous_floor;
                int future_dir = order->floor - global_elevator_current_floor;
                if( (previous_dir > 0 && future_dir > 0) || (previous_dir < 0 & future_dir < 0)) { // Check if order cab direction matches previous (Negative = down, positive = up)
                    queue_assign_next_global_order(&global_elevator_current_order);
                    queue_assign_current_global_order(order);
                }

            } else if(order->dir == global_elevator_current_order.dir) {        // Determines if we need to do stops along the way
                if(order->dir == DIRN_UP) {
                    if(order->floor < global_elevator_current_order.floor) {
                        queue_assign_next_global_order(&global_elevator_current_order);
                        queue_assign_current_global_order(order);
                    }
                else if(order->dir == DIRN_DOWN) {
                    if(order->floor > global_elevator_current_order.floor) {
                        queue_assign_next_global_order(&global_elevator_current_order);
                        queue_assign_current_global_order(order);
                        }
                    }
                }
            }
        }

    }
    queue_update_fsm();
    return 0; 
}

void queue_assign_current_global_order(struct order* order) {
    global_elevator_current_order.dir = order->dir;
    global_elevator_current_order.floor = order->floor;
}

void queue_assign_next_global_order(struct order* order) {
    global_elevator_next_order.dir = order->dir;
    global_elevator_next_order.floor = order->floor;
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// Queue debugging utils
//////////////////////////////////////////////////////////////////////////////////////////////////

int util_queue_print_current_order() {
    printf("Current order: Floor %i dir %i \n", global_elevator_current_order.floor, global_elevator_current_order.dir);
    return 0;
};

int util_queue_print_next_order() {
    printf("Next order: Floor %i dir %i \n", global_elevator_next_order.floor, global_elevator_next_order.dir);
    return 0;
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
    int queue_index = queue_length;
    struct order* order = head;

    while(order != NULL) {
        printf("Order: %i Floor: %i \n", queue_index, order->floor);
        order = order->next;
        queue_index -= 1;
    }
    printf("--------------------- \n");
    return 0;
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// Queue unit tests / simulation
//////////////////////////////////////////////////////////////////////////////////////////////////

void unit_test_queue_initalize() {
    queue_initalize();
    assert(head == NULL);
}

void unit_test_queue_insert() {
    unit_test_queue_initalize();
    struct order* order = NULL;
    order->dir = 1;
    order->floor = 1;
    order->next = NULL;
    order->prev = NULL;
    queue_insert(order);
    struct order* order_queue = queue_fetch_latest_order();

    assert(order->dir == order_queue->dir && order->floor == order_queue->floor);
}

void unit_test_queue_delete() {
    queue_initalize();

    struct order* order = malloc(sizeof(struct order));
    order->dir = 1;
    order->floor = 1;
    order->next = NULL;
    order->prev = NULL;

    queue_insert(order);
    struct order* order_queue = queue_fetch_latest_order();

    assert(order->dir == order_queue->dir && order->floor == order_queue->floor);

    queue_delete(order);

    struct order* deleted_queue = queue_fetch_latest_order();

    assert(deleted_queue == NULL);
}

void unit_test_queue_flush() {
    queue_initalize();

    struct order* order1 = malloc(sizeof(struct order));
    order1->dir = 1;
    order1->floor = 1;
    order1->next = NULL;
    order1->prev = NULL;

    struct order* order2 = malloc(sizeof(struct order));
    order2->dir = 1;
    order2->floor = 2;
    order2->next = NULL;
    order2->prev = NULL;

    struct order* order3 = malloc(sizeof(struct order));
    order3->dir = 1;
    order3->floor = 3;
    order3->next = NULL;
    order3->prev = NULL;

    queue_insert(order1);
    queue_insert(order2);
    queue_insert(order3);

    util_queue_print();

    queue_flush();

    util_queue_print();
}

void unit_test_queue_remove_floor_orders() {
    global_elevator_current_order.dir = 1;
    global_elevator_next_order.floor = 1;

    struct order* order1 = malloc(sizeof(struct order));
    order1->dir = 1;
    order1->floor = 1;
    order1->next = NULL;
    order1->prev = NULL;

    struct order* order2 = malloc(sizeof(struct order));
    order2->dir = 2;
    order2->floor = 1;
    order2->next = NULL;
    order2->prev = NULL;

    struct order* order3 = malloc(sizeof(struct order));
    order3->dir = 3;
    order3->floor = 1;
    order3->next = NULL;
    order3->prev = NULL;

    struct order* order4 = malloc(sizeof(struct order));
    order4->dir = 3;
    order4->floor = 2;
    order4->next = NULL;
    order4->prev = NULL;

    queue_insert(order1);
    queue_insert(order2);
    queue_insert(order3);
    queue_insert(order4);

    util_queue_print();
    queue_remove_floor_orders(1);
    util_queue_print();

}


void queue_scenario() {

    // Initalize FSM 

    int elevator_current_floor = 0;

    queue_create_new_order(2, BUTTON_HALL_DOWN); // Enter idle

    elevator_current_floor += 1; // Exit idle, Enter ACTIVE_UP

    util_queue_print();

    queue_create_new_order(0, BUTTON_HALL_UP); 

    elevator_current_floor += 1;

    queue_remove_floor_orders(elevator_current_floor); // Enter idle, exit ACTIVE_UP

    util_queue_print();

    queue_create_new_order(0, BUTTON_CAB);

    elevator_current_floor -= 1; // Exit idle, enter ACTIVE_DOWN

    queue_create_new_order(1, BUTTON_HALL_DOWN);

    queue_remove_floor_orders(elevator_current_floor); // Exit ACTIVE_DOWN, enter idle

    queue_create_new_order(0, BUTTON_CAB);

    elevator_current_floor -= 1; // Exit idle, enter ACTIVE_DOWN

    queue_remove_floor_orders(elevator_current_floor); // Exit ACTIVE_DOWN, enter idle

    queue_create_new_order(3, BUTTON_HALL_UP);

    util_queue_print();

    queue_flush();

    util_queue_print();
}

void queue_simulate() {

   unit_test_queue_remove_floor_orders();
}
