#include "include/peripherals.h"

void open_door_timer(int floor) {
    elevio_doorOpenLamp(floor);
    //time wait 3 secs??
    elevio_doorOpenLamp(floor);
}

bool check_valid_floor() {
    if (elevio_floorSensor() != -1) {
        return 1;
    }
    else {
        return 0;
    }
}

void button_polling() {

};

bool check_obstruction() {
    return elevio_obstruction();
};

void update_state() {
    
};

void goto_floor_one() {
    if (elevio_floorSensor < 1) {
        elevio_motorDirection(1);
        while (elevio_floorSensor != 1) {
            elevio_motorDirection(1);
        }
        elevio_motorDirection(0);
    }
    else {
        while (elevio_floorSensor != 1) {
            elevio_motorDirection(-1);
        }
        elevio_motorDirection(0);
    }
}