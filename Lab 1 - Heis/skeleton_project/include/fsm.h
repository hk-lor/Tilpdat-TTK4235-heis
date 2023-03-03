#pragma once

#include <stdint.h> // xintx_t types
#include <stdbool.h> // booleans
#include "driver/elevio.h" //elevio driver

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
    void (*update_function)(void);
} stateFunctionRow_t;

typedef struct transitions_s {
    state_t currState;
    event_t event;
    state_t nextState;
} transitions_t;

// Entry functions
void init_enter();
void idle_enter();
void active_up_enter();
void active_down_enter();
void stop_enter();
void valid_floor_check_enter();

// Update functions
void init_update();
void idle_update();
void active_up_update();
void active_down_update();
void stop_update();
void valid_floor_check_update();