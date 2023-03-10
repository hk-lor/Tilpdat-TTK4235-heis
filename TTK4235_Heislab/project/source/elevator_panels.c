#include "stdio.h"
#include "stdint.h"

#include "../include/elevator_panels.h"
#include "../source/driver/elevio.h"

// HEISPANEL
static uint8_t button_elevator_1 = 0;
static uint8_t button_elevator_2 = 0;
static uint8_t button_elevator_3 = 0;
static uint8_t button_elevator_4 = 0;

static uint8_t stop_button = 0;
static uint8_t door_open = 0;
static uint8_t obstruction = 0;

// ETASJEPANEL
static uint8_t floor_button_1_up = 0;
static uint8_t floor_button_2_up = 0;
static uint8_t floor_button_3_up = 0;

static uint8_t floor_button_2_down = 0;
static uint8_t floor_button_3_down = 0;
static uint8_t floor_button_4_down = 0;

static uint8_t floor_lamp_1 = 0;
static uint8_t floor_lamp_2 = 0;
static uint8_t floor_lamp_3 = 0;
static uint8_t floor_lamp_4 = 0;

// Heisdør
void open_door() {
    if(door_open == 0) {
        door_open = 1;
        elevio_doorOpenLamp(door_open);
    } 
    
};
void close_door() {
    if(door_open == 1) {
        door_open = 0;
        elevio_doorOpenLamp(door_open);
    }
};

// Heispanel
void update_button_elevator_1(uint8_t button_value) {
    if(button_elevator_1 != button_value) {
        button_elevator_1 = button_value;
        elevio_callButton(0, BUTTON_CAB);
    }
};
void update_button_elevator_2(uint8_t button_value) {
    if(button_elevator_2 != button_value) {
        button_elevator_2 = button_value;
        elevio_callButton(1, BUTTON_CAB);

    }
};
void update_button_elevator_3(uint8_t button_value) {
    if(button_elevator_3 != button_value) {
        button_elevator_3 = button_value;
        elevio_callButton(2, BUTTON_CAB);
    }
};
void update_button_elevator_4(uint8_t button_value) {
    if(button_elevator_4 != button_value) {
        button_elevator_4 = button_value;
        elevio_callButton(3, BUTTON_CAB);

    }
};


void update_stop_button(uint8_t button_value) {
    if (stop_button != button_value) {
        stop_button = button_value;        
        elevio_stopLamp(button_value);
    }
};

void update_obstruction(uint8_t switch_value) {
     if (obstruction != switch_value) {
        obstruction = switch_value;
    }
};

uint8_t fetch_stop_button_status() {
    return stop_button;
}

// Etasjepanel
void update_floor_button_1_up(uint8_t button_value) {
    if(floor_button_1_up != button_value) {
        floor_button_1_up = button_value;
        elevio_buttonLamp(0, BUTTON_HALL_UP, button_value);
    }
};
void update_floor_button_2_up(uint8_t button_value) {
    if(floor_button_2_up != button_value) {
        floor_button_2_up = button_value;
        elevio_buttonLamp(1, BUTTON_HALL_UP, button_value);
    }
};
void update_floor_button_3_up(uint8_t button_value) {
     if(floor_button_3_up != button_value) {
        floor_button_3_up = button_value;
        elevio_buttonLamp(2, BUTTON_HALL_UP, button_value);
    }
};

void update_floor_button_2_down (uint8_t button_value) {
    if(floor_button_2_down != button_value) {
        floor_button_2_down  = button_value;
        elevio_buttonLamp(1, BUTTON_HALL_DOWN, button_value);
    }
};
void update_floor_button_3_down (uint8_t button_value) {
    if(floor_button_3_down != button_value) {
        floor_button_3_down  = button_value;
        elevio_buttonLamp(2, BUTTON_HALL_DOWN, button_value);
    }
};
void update_floor_button_4_down (uint8_t button_value) {
    if(floor_button_4_down != button_value) {
        floor_button_4_down  = button_value;
        elevio_buttonLamp(3, BUTTON_HALL_DOWN, button_value);
    }
};

void update_floor_lamp_1(uint8_t lamp_value) {
    if(floor_lamp_1 != lamp_value) {
        floor_lamp_1  = lamp_value;
        elevio_floorIndicator(0);
    }
};
void update_floor_lamp_2(uint8_t lamp_value) {
 if(floor_lamp_2 != lamp_value) {
        floor_lamp_2  = lamp_value;
        elevio_floorIndicator(1);
    }
};
void update_floor_lamp_3(uint8_t lamp_value) {
    if(floor_lamp_3 != lamp_value) {
        floor_lamp_3  = lamp_value;
        elevio_floorIndicator(2);
    }
};
void update_floor_lamp_4(uint8_t lamp_value) {
    if(floor_lamp_4 != lamp_value) {
        floor_lamp_4  = lamp_value;
        elevio_floorIndicator(3);
    }
};

// Init
void init_lights() {
    close_door();

// Heispanel
    update_button_elevator_1(0);
    update_button_elevator_2(0);
    update_button_elevator_3(0);
    update_button_elevator_4(0);

    update_stop_button(0);

// Etasjepanel
    update_floor_button_1_up(0);
    update_floor_button_2_up(0);
    update_floor_button_3_up(0);

    update_floor_button_2_down (0);
    update_floor_button_3_down (0);
    update_floor_button_4_down (0);

    update_floor_lamp_1(0);
    update_floor_lamp_2(0);
    update_floor_lamp_3(0);
    update_floor_lamp_4(0);
}
