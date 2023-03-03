#include "skeleton_project\include\fsm.h"
#include "skeleton_project\include\peripherals.h"
#include "driver/elevio.h"
#include "driver/queue.h"

//////////////////////////////////////////////////////////////////////////////////////////////////
// State funcions and transistions
//////////////////////////////////////////////////////////////////////////////////////////////////

static stateFunctionRow_t stateFunctionA[] = {
    {"init",              &fsm_init_enter,          &fsm_init_exit,         &fsm_init_update},
    {"idle",              &fsm_idle_enter,          &fsm_idle_exit,         &fsm_idle_update},
    {"active_up",         &fsm_active_up_enter,     &fsm_active_up_exit,    &fsm_active_down_update},
    {"active_down",       &fsm_active_down_enter,   &fsm_active_down_exit,  &fsm_active_down_update},
    {"stop",              &fsm_stop_enter,          &fsm_stop_exit,         &fsm_stop_update},
    {"valid_floor_check", &fsm_valid_floor_check_enter, &fsm_valid_floor_check_exit, &fsm_valid_floor_check_update}
};

static transitions_t state_transitions[] = {
    {init, state_found, idle},
    {idle, order_up, active_up},
    {idle, order_down, active_down},
    {idle, stop_button_pressed, stop},
    {active_up, valid_floor_in_queue, idle},
    {active_up, stop_button_pressed, stop},
    {active_down, valid_floor_in_queue, idle},
    {active_down, stop_button_pressed, stop},
    {stop, stop_button_time_out, valid_floor_check},
    {valid_floor_check, state_found, idle},
};

//////////////////////////////////////////////////////////////////////////////////////////////////
// Variables
//////////////////////////////////////////////////////////////////////////////////////////////////

static volatile event_t event;
static stateMachine_t *stateMachine;
static int elevator_current_floor;

static struct order current_order;
static struct order next_order;

//////////////////////////////////////////////////////////////////////////////////////////////////
// Enter functions
//////////////////////////////////////////////////////////////////////////////////////////////////

void fsm_init_enter() {
    queue_initalize();
    goto_floor_one();
};

void fsm_idle_enter() {
    update_state();
    open_door_timer();
};
void fsm_active_up_enter() {
    elevio_motorDirection(DIRN_UP);
};
void fsm_active_down_enter() {
    elevio_motorDirection(DIRN_DOWN);
};
void fsm_stop_enter() {
    elevio_motorDirection(DIRN_STOP);
    start_stop_timer();
    queue_flush();
    update_state();
};
void fsm_valid_floor_check_enter() {
    if(update_state() == NULL) {
        init_enter();
    } else {
        idle_enter();
    }
};

//////////////////////////////////////////////////////////////////////////////////////////////////
// Exit functions
//////////////////////////////////////////////////////////////////////////////////////////////////

void fsm_init_exit() {
    return;
}

void fsm_idle_exit() {
    close_door();
}

void fsm_active_up_exit() {
    peripherals_remove_current_order();
    elevio_floorIndicator(elevator_current_floor);
}

void fsm_active_down_exit() {
    peripherals_remove_current_order();
    elevio_floorIndicator(elevator_current_floor);
}

void fsm_stop_exit() {
    return;
}

void fsm_valid_floor_check_exit() {
    return;
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// Update functions
//////////////////////////////////////////////////////////////////////////////////////////////////

void fsm_init_update() {
    return;
}

void fsm_idle_update() {
    //check_queue
    fsm_button_polling();
    check_obstruction();
}

void fsm_active_up_update() {
    //check_queue
    fsm_button_polling();
    //update_state
    //update_destination_floor
}

void fsm_active_down_update() {
    //check_queue
    peripherals_button_polling();
    //update_state
    //update_destination_floor
    peripherals_check_valid_floor();
}
void fsm_stop_update() {
    peripherals_button_polling();
    if (peripheralscheck_valid_floor() != -1 && check_obstruction() == 0) {
        open_door_timer();
    }
    else {
        return;
    }
}
void fsm_valid_floor_check_update() {
    //check_valid_floor
    //if statement
}
