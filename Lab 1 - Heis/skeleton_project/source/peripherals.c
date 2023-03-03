#include "stdio.h"
#include "driver/queue.h"
#include "include/peripherals.h"


void peripherals_open_door_timer(int floor) {
    elevio_doorOpenLamp(floor);
    //time wait 3 secs??
    elevio_doorOpenLamp(floor);
}

bool peripherals_check_valid_floor() {
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
    
}

// Updates FSM current_order and next_order
void peripherals_update_current_order();