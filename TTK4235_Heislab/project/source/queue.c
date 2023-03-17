#include "stdlib.h"
#include "stdint.h"
#include "stdio.h"
#include "string.h"
#include <assert.h>
#include "../source/driver/elevio.h"
#include "../include/queue.h"
#include "../include/elevator_panels.h"
#include "../include/peripherals.h"

//////////////////////////////////////////////////////////////////////////////////////////////////
// Queue variables
//////////////////////////////////////////////////////////////////////////////////////////////////

static struct order* head;                      // Linked list queue head
static int queue_length = 0;

static int global_elevator_current_floor ;
static int global_previous_floor;

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

    switch(new_order->floor) {
            case 0:
                if(new_order->dir == BUTTON_HALL_UP) {
                    update_floor_button_1_up(1);
                }
                else if(new_order->dir == BUTTON_HALL_DOWN){
                    // Down button does not exist
                }
                else {
                    update_button_elevator_1(1);
                }
                break;
            case 1:
                if(new_order->dir == BUTTON_HALL_UP) {
                    update_floor_button_2_up(1);
                }
                else if(new_order->dir == BUTTON_HALL_DOWN){
                    update_floor_button_2_down(1);
                }
                else {
                    update_button_elevator_2(1);
                }
                break;
             case 2:
                if(new_order->dir == BUTTON_HALL_UP) {
                    update_floor_button_3_up(1);
                }
                else if(new_order->dir == BUTTON_HALL_DOWN){
                    update_floor_button_3_down(1);
                }
                else {
                    update_button_elevator_3(1);
                }
                break;
             case 3:
                if(new_order->dir == BUTTON_HALL_UP) {
                    // UP button does not exist
                }
                else if(new_order->dir == BUTTON_HALL_DOWN){
                    update_floor_button_4_down(1);
                }
                else {
                    update_button_elevator_4(1);
                }
                break;
            }
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
        switch(order->floor) {
            case 0:
                if(order->dir == BUTTON_HALL_UP) {
                    update_floor_button_1_up(0);
                }
                else if(order->dir == BUTTON_HALL_DOWN){
                    // Down button does not exist
                }
                else {
                    update_button_elevator_1(0);
                }
                break;
            case 1:
                if(order->dir == BUTTON_HALL_UP) {
                    update_floor_button_2_up(0);
                }
                else if(order->dir == BUTTON_HALL_DOWN){
                    update_floor_button_2_down(0);
                }
                else {
                    update_button_elevator_2(0);
                }
                break;
             case 2:
                if(order->dir == BUTTON_HALL_UP) {
                    update_floor_button_3_up(0);
                }
                else if(order->dir == BUTTON_HALL_DOWN){
                    update_floor_button_3_down(0);
                }
                else {
                    update_button_elevator_3(0);
                }
                break;
             case 3:
                if(order->dir == BUTTON_HALL_UP) {
                    // UP button does not exist
                }
                else if(order->dir == BUTTON_HALL_DOWN){
                    update_floor_button_4_down(0);
                }
                else {
                    update_button_elevator_4(0);
                }
                break;
        }

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

int queue_find_order(int floor, ButtonType dir) {    
    struct order* order = head;

    while (order != NULL && order->floor != floor && order->dir != dir) {
        order = order->next;
    }
    
    if(order == NULL) {
        return 0;
    } else {
        return 1; // return allocated structure (has to be freed)
    }                              
}

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

    // int order_exist = queue_find_order(floor, dir);
    // if(order_exist) {
    //     return 1;
    // } else {
    //     queue_insert(new_order);
    // }
    // 

    queue_insert(new_order);
    return 0;
}

int queue_flush() {
    struct order* order = NULL;
    for(int floor = 0; floor < N_FLOORS; floor++) {
        order = queue_search(floor);
        if(order != NULL) {
            util_print_order(order);
            queue_delete(order);
        }
    }

    queue_length = 0;

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
    queue_set_global_current_order_as_empty();
    queue_assign_current_global_order(&global_elevator_next_order);
    queue_set_global_next_order_as_empty();
};

// structs are statically allocated due to getting into trouble when using pointers
void queue_set_global_orders_as_empty() {
    global_elevator_current_order.floor = -1;
    global_elevator_next_order.floor = -1;
}

void queue_set_global_next_order_as_empty() {
    global_elevator_next_order.floor = -1;
}

void queue_set_global_current_order_as_empty() {
    global_elevator_current_order.floor = -1;
}


//////////////////////////////////////////////////////////////////////////////////////////////////
// Queue external module function calls
//////////////////////////////////////////////////////////////////////////////////////////////////

int queue_update_elevator_current_floor() {
    int elevator_current_floor = elevio_floorSensor(); 
    if(elevator_current_floor != -1) { // Updates only if floor is valid
         switch(global_previous_floor) {
            case 0:
                update_floor_lamp_1(0);
                break;
            case 1:
                update_floor_lamp_2(0);
                break;
            case 2:
                update_floor_lamp_3(0);
                break;
            case 3:
                update_floor_lamp_4(0);
                break;
        }

        global_previous_floor = global_elevator_current_floor;
        global_elevator_current_floor = elevator_current_floor;

        switch(elevator_current_floor) {
            case 0:
                update_floor_lamp_1(1);
                break;
            case 1:
                update_floor_lamp_2(1);
                break;
            case 2:
                update_floor_lamp_3(1);
                break;
            case 3:
                update_floor_lamp_4(1);
                break;
        }
    }
    return 0;
}

void queue_update_fsm(struct fsm_packet* packet) {
    queue_update();

    packet->current_order_dir = global_elevator_current_order.dir;
    packet->current_order_floor = global_elevator_current_order.floor;
    packet->next_order_dir = global_elevator_next_order.dir;
    packet->next_order_floor = global_elevator_next_order.floor;

    packet->elevator_current_floor = global_elevator_current_floor;

}

//////////////////////////////////////////////////////////////////////////////////////////////////
// Queue algorithm
//////////////////////////////////////////////////////////////////////////////////////////////////

int queue_update() {
    queue_update_elevator_current_floor();

    for (int i = 0; i < queue_length; ++i) {
        struct order* order = queue_fetch_order(i);
       
        if(order == NULL || queue_length == 0) {
            queue_set_global_orders_as_empty();
            return 1;
        } 
        
        else {
            if(global_elevator_current_order.floor == -1) {         // Check if current_order is empty
                queue_assign_current_global_order(order);
            } 
            else if (global_elevator_next_order.floor == -1) {     // Check if next_order is empty
                queue_assign_next_global_order(order);
            }
            else if(order->dir == BUTTON_CAB) {
                // printf("CABIN \n");

                int future_dir = global_elevator_current_order.floor - global_elevator_current_floor;
                int new_order_dir = order->floor - global_elevator_current_floor;

                // Check if order cab direction matches previous (Negative = down, positive = up)

                if((new_order_dir > 0 && future_dir > 0)) {
                    if(order->floor < global_elevator_current_order.floor) {
                        queue_assign_next_global_order(&global_elevator_current_order);
                        queue_assign_current_global_order(order);
                    }
                }
                    
                if(new_order_dir < 0 & future_dir < 0) { 
                     if(order->floor > global_elevator_current_order.floor) {
                        queue_assign_next_global_order(&global_elevator_current_order);
                        queue_assign_current_global_order(order);
                    }       
                }
            }
            else {        // Determines if we need to do stops along the way
                if(order->dir == BUTTON_HALL_UP) {
                    int future_dir = global_elevator_current_order.floor - global_elevator_current_floor;
                    int new_order_dir = 1;
                    
                    // Check if order cab direction matches previous (Negative = down, positive = up)

                    if((new_order_dir > 0 && future_dir > 0)) {
                        if(order->floor < global_elevator_current_order.floor) {
                            queue_assign_next_global_order(&global_elevator_current_order);
                            queue_assign_current_global_order(order);
                        }
                    }
                        
                    if(new_order_dir < 0 & future_dir < 0) { 
                        if(order->floor > global_elevator_current_order.floor) {
                            queue_assign_next_global_order(&global_elevator_current_order);
                            queue_assign_current_global_order(order);
                        }       
                    }
                }
        
                if(order->dir == BUTTON_HALL_DOWN) {
                    int future_dir = global_elevator_current_order.floor - global_elevator_current_floor;
                    int new_order_dir = -1;

                    // Check if order cab direction matches previous (Negative = down, positive = up)

                    if((new_order_dir > 0 && future_dir > 0)) {
                        if(order->floor < global_elevator_current_order.floor) {
                            queue_assign_next_global_order(&global_elevator_current_order);
                            queue_assign_current_global_order(order);
                        }
                    }
                        
                    if(new_order_dir < 0 & future_dir < 0) { 
                        if(order->floor > global_elevator_current_order.floor) {
                            queue_assign_next_global_order(&global_elevator_current_order);
                            queue_assign_current_global_order(order);
                        }       
                    }
                }
            }
        }
    }
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
    printf("Queue Current order: Floor %i dir %i \n", global_elevator_current_order.floor, global_elevator_current_order.dir);
    return 0;
};

int util_queue_print_next_order() {
    printf("Queue Next order: Floor %i dir %i \n", global_elevator_next_order.floor, global_elevator_next_order.dir);
    return 0;
};

int util_print_order(struct order* order) {
    if(order == NULL) {
        return 1;
    } else {
        printf("Queue Order: Floor %i Direction %i \n", order->floor, order->dir);
        return 0;
    }
}

void util_print_current_floor() {
    printf("QUEUE: Current floor: %i \n", global_elevator_current_floor);
}

int util_queue_print() {
    int queue_index = queue_length;
    struct order* order = head;

    while(order != NULL) {
        printf("Queue: Order: %i Floor: %i Direction: %i \n", queue_index, order->floor, order->dir);
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

    queue_initalize();

    queue_create_new_order(1, BUTTON_HALL_DOWN); // Enter idle
    queue_create_new_order(2, BUTTON_HALL_UP); // Enter idle
    queue_create_new_order(3, BUTTON_HALL_UP); // Enter idle

    util_queue_print();
    util_queue_print_current_order();
    util_queue_print_next_order();

    queue_update();

    util_queue_print();
    util_queue_print_current_order();
    util_queue_print_next_order();

}
