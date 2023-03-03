#include "stdio.h"
#include "stdlib.h"
#include "time.h"
#include "driver/queue.h"
#include "include/peripherals.h"


void peripherals_open_door_timer(int floor) {
    elevio_doorOpenLamp(floor);
    peripherals_timer_3seconds();
    elevio_doorOpenLamp(floor);
}

int peripherals_check_valid_floor() {
    if (elevio_floorSensor() != -1) {
        return 1;
    }
    else {
        return 0;
    }
}

void peripherals_button_polling() {
    for(int f = 0; f < N_FLOORS; f++){
            for(int b = 0; b < N_BUTTONS; b++){
                int btnPressed = elevio_callButton(f, b);
                if(btnPressed) {
                    elevio_buttonLamp(f, b, btnPressed);
                    queue_create_new_order(f, b);
                }
         }
    }
};

bool peripherals_check_obstruction() {
    return elevio_obstruction();
};

void peripherals_update_state() {
    
};

void peripherals_goto_floor_one() {
    if (elevio_floorSensor < 1) {
        while (elevio_floorSensor != 1) {
            elevio_motorDirection(DIRN_UP);
        }
        elevio_motorDirection(DIRN_STOP);
    }
    else {
        while (elevio_floorSensor != 1) {
            elevio_motorDirection(DIRN_DOWN);
        }
        elevio_motorDirection(DIRN_STOP);
    }
}

void peripherals_remove_current_order() {
    queue_remove_current_order();
}


void peripherals_check_queue() {
    return;
}

void peripherals_update_current_order() {
    return;
}

int peripherals_timer_3seconds() {
    int *count = malloc(sizeof(int));
    if (count == NULL) {
        return 1;
    }

    *count = 0;

    clock_t start_time = clock();
    double elapsed_time;

    while (*count < 3) {
        elapsed_time = (double)(clock() - start_time) / CLOCKS_PER_SEC;
        if (elapsed_time >= 1.0) {
            (*count)++;
            start_time = clock();
        }
    }

    free(count);
    count = NULL;

    return 0;
}