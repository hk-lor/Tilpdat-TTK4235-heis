#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <time.h>
#include "driver/elevio.h"
#include "driver/queue.h"

int main(){    
    int length = 0;
    
    queue_initalize();
   
    queue_create_new_order(1, BUTTON_HALL_UP);
    queue_create_new_order(3, BUTTON_HALL_DOWN);
    queue_create_new_order(4, BUTTON_CAB);

    queue_print();
    
    queue_update_next_order();

    queue_print_current_order();

    update_elevator_current_floor();

    queue_order_finished_signal();

    queue_update_next_order();

    queue_print();

    queue_print_current_order();

    
    return 0;

}
