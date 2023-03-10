#pragma once

//Includes
#include "stdint.h"

#include "../source/driver/elevio.h"

// Heisdør
void open_door();
void close_door();

// Heispanel
void update_button_elevator_1(uint8_t button_value);
void update_button_elevator_2(uint8_t button_value);
void update_button_elevator_3(uint8_t button_value);
void update_button_elevator_4(uint8_t button_value);

void update_stop_button(uint8_t button_value);
void update_obstruction(uint8_t switch_value);

uint8_t fetch_stop_button_status();

// Etasjepanel
void update_floor_button_1_up(uint8_t button_value);
void update_floor_button_2_up(uint8_t button_value);
void update_floor_button_3_up(uint8_t button_value);

void update_floor_button_2_down (uint8_t button_value);
void update_floor_button_3_down (uint8_t button_value);
void update_floor_button_4_down (uint8_t button_value);

void update_floor_lamp_1(uint8_t lamp_value);
void update_floor_lamp_2(uint8_t lamp_value);
void update_floor_lamp_3(uint8_t lamp_value);
void update_floor_lamp_4(uint8_t lamp_value);