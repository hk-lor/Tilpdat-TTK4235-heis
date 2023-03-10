#pragma once

#include "../source/driver/elevio.h"
#include "stdint.h"

void peripherals_open_door_timer();
int peripherals_check_valid_floor();
void peripherals_button_polling();
int peripherals_check_obstruction();
void peripherals_update_state();
void peripherals_goto_floor_one();
void peripherals_remove_floor_orders(int floor);
int peripherals_timer(int seconds);
void peripherals_button_polling();
void peripherals_update_floor_lamp(int current_floor, uint8_t on_off);