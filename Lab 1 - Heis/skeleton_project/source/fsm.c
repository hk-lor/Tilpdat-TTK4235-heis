#include "skeleton_project\include\fsm.h"
#include "driver/elevio.h"

// Entry functions
void init_enter() {
    update_state();
};
void idle_enter() {
    update_state();
    open_door_timer();
};
void active_up_enter() {
    elevio_motorDirection(DIRN_UP);
};
void active_down_enter() {
    elevio_motorDirection(DIRN_DOWN);
};
void stop_enter() {
    elevio_motorDirection(DIRN_STOP);
    start_stop_timer();
    queue_flush();
    update_state();
};
void valid_floor_check_enter() {
    if(update_state() == NULL) {
        init_enter();
    } else {
        idle_enter();
    }
};

// Update functions
void init_update() {
    //check_valid_floor
    return;
}

void idle_update() {
    //check_queue
    //button_polling
    //check_obstruction
}

void active_up_update() {
    //check_queue
    //button_polling
    //update_state
    //update_destination_floor
    //check_valid_floor
}

void active_down_update() {
    //check_queue
    //button_polling
    //update_state
    //update_destination_floor
    //check_valid_floor
}
void stop_update() {
    //button_polling
    //check_obstruction
    //check_valid_floor
    //if valid_floor --> open door
    // if !valid_floor --> do nothing
}
void valid_floor_check_update() {
    //check_valid_floor
    //if statement
}
