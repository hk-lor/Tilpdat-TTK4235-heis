#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <time.h>
#include "driver/elevio.h"
#include "../include/queue.h"
#include "../include/fsm.h"

int main(){   
    queue_initalize(); 
    fsm_entry();
    //queue_simulate();

    return 0;
}
