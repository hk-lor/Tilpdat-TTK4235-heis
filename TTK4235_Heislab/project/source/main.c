#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <time.h>
#include "driver/elevio.h"
#include "../include/queue.h"
#include "../include/fsm.h"
#include "../include/peripherals.h"

int main(){   
    elevio_init();

    fsm_entry();

    return 0;
}
