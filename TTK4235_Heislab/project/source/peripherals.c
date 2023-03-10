#include "stdio.h"
#include "stdlib.h"
#include "time.h"
#include "../source/driver/elevio.h"
#include "../include/peripherals.h"
#include "../include/elevator_panels.h"
#include "../include/queue.h"

// timer with seconds
int peripherals_timer(int seconds) {
    int *count = malloc(sizeof(int));
    if (count == NULL) {
        return 1;
    }

    *count = 0;

    clock_t start_time = clock();
    double elapsed_time;

    while (*count < seconds) {
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

int peripherals_check_obstruction() {
    return elevio_obstruction();
};

// Open door
void peripherals_open_door_timer() {
    open_door();
    peripherals_timer(3);
    if (!elevio_obstruction()) {
        close_door();
    }
}

int peripherals_check_valid_floor() {
    if (elevio_floorSensor() != -1) {
        return 0;
    }
    else {
        return 1;
    }
}

void peripherals_button_polling() {
    for(int f = 0; f < N_FLOORS; f++){
        for(int b = 0; b < N_BUTTONS; b++){
            int btnPressed = elevio_callButton(f, b);
            
            if(btnPressed == 1) {
                queue_create_new_order(f, b);
            }
        }
    }
};

void peripherals_goto_floor_one() {
    if (elevio_floorSensor() != 0) {
        while (elevio_floorSensor() != 0) {
            elevio_motorDirection(DIRN_DOWN);
        }
        elevio_motorDirection(DIRN_STOP);
    }
}

void peripherals_update_floor_lamp(int current_floor, uint8_t on_off) {
    switch(current_floor) {
        case 0: 
            update_floor_lamp_1(on_off);
            break;
        case 1:
            update_floor_lamp_2(on_off);
            break;
        case 2:
            update_floor_lamp_3(on_off);
            break;
        case 3:
            update_floor_lamp_4(on_off);
    }
};

