#pragma once

#include "skeleton_project\source\driver\elevio.h"

void peripherals_open_door_timer();
bool peripherals_check_valid_floor();
void peripherals_button_polling();
bool peripherals_check_obstruction();
void peripherals_update_state();
void peripherals_goto_floor_one();
void peripherals_remove_current_order();
void peripherals_check_queue();
void peripherals_timer_3seconds();