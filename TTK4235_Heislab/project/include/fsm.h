#pragma once

#include <stdint.h> // xintx_t types
#include <stdbool.h> // booleans
#include "../source/driver/elevio.h"//elevio driver
#include "../include/elevator_panels.h"

// States
typedef enum {
    init,
    idle,
    active_up,
    active_down,
    stop,
    valid_floor_check
} state_t;

// Transition events
typedef enum {
    no_event,
    order_up,
    order_down,
    valid_floor_in_queue,
    state_found,
    stop_button_pressed,
    stop_button_time_out
} event_t;

// Statemachine struct
typedef struct stateMachine_s {
    state_t currState;
} stateMachine_t;

typedef struct stateFunctionRow_s {
    const char * name;
    void (*enter_function)(void);
    void (*exit_function)(void);
    void (*update_function)(void);
} stateFunctionRow_t;

typedef struct transitions_s {
    state_t currState;
    event_t event;
    state_t nextState;
} transitions_t;

// Entry functions
void fsm_init_enter();
void fsm_idle_enter();
void fsm_active_up_enter();
void fsm_active_down_enter();
void fsm_stop_enter();
void fsm_valid_floor_check_enter();

// Exit functions
void fsm_init_exit();
void fsm_idle_exit();
void fsm_active_up_exit();
void fsm_active_down_exit();
void fsm_stop_exit();
void fsm_valid_floor_check_exit();

// Update functions
void fsm_update_state();
void fsm_init_update();
void fsm_idle_update();
void fsm_active_up_update();
void fsm_active_down_update();
void fsm_stop_update();
void fsm_valid_floor_check_update();
void fsm_entry();