#include "../include/fsm.h"
#include "../include/peripherals.h"
#include "../source/driver/elevio.h"
#include "../include/queue.h"
#include "../include/elevator_panels.h"

//////////////////////////////////////////////////////////////////////////////////////////////////
// State funcions and transistions
//////////////////////////////////////////////////////////////////////////////////////////////////

static stateFunctionRow_t stateFunctionA[] = {
    {"init", &fsm_init_enter, &fsm_init_exit, &fsm_init_update},
    {"idle", &fsm_idle_enter, &fsm_idle_exit, &fsm_idle_update},
    {"active_up", &fsm_active_up_enter, &fsm_active_up_exit, &fsm_active_down_update},
    {"active_down", &fsm_active_down_enter, &fsm_active_down_exit, &fsm_active_down_update},
    {"stop", &fsm_stop_enter, &fsm_stop_exit, &fsm_stop_update},
    {"valid_floor_check", &fsm_valid_floor_check_enter, &fsm_valid_floor_check_exit, &fsm_valid_floor_check_update}};

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

volatile event_t event;
stateMachine_t stateMachine;

static int started;

static int elevator_current_floor;
static struct order current_order;
static struct order next_order;
static struct fsm_packet current_packet;

//////////////////////////////////////////////////////////////////////////////////////////////////
// Enter functions
//////////////////////////////////////////////////////////////////////////////////////////////////

void fsm_init_enter()
{
    printf("Init enter! \n");
    event = no_event;
    init_lights();
    queue_initalize();
    if (started == 0) {
        peripherals_goto_floor_one();
    }
    started = 1;
    event = state_found;
};

void fsm_idle_enter()
{
    printf("Idle enter! \n");
    event = no_event;
    if (peripherals_check_valid_floor() == 0) {
        peripherals_open_door_timer();
    }
    queue_remove_floor_orders(elevator_current_floor);
};

void fsm_active_up_enter()
{
    printf("Active up enter! \n");
    event = no_event;
    elevio_motorDirection(DIRN_UP);
};
void fsm_active_down_enter()
{
    printf("Active down enter! \n");
    event = no_event;
    elevio_motorDirection(DIRN_DOWN);
};
void fsm_stop_enter()
{
    printf("Stop enter! \n");
    event = no_event;
    update_stop_button(1);
    elevio_motorDirection(DIRN_STOP);
    queue_flush();
    if (peripherals_check_valid_floor() == 0) {
        peripherals_open_door_timer();
    }
};
void fsm_valid_floor_check_enter()
{
    printf("Valid floor check enter! \n");
    event = no_event;
    queue_remove_floor_orders(elevator_current_floor);
    queue_update_fsm(&current_packet);
    fsm_update_state();

    if (peripherals_check_valid_floor())
    {
        fsm_init_enter();
    }
    else
    {
        fsm_idle_enter();
    }
};

//////////////////////////////////////////////////////////////////////////////////////////////////
// Exit functions
//////////////////////////////////////////////////////////////////////////////////////////////////

void fsm_init_exit()
{
    printf("Init exit \n");
    return;
}

void fsm_idle_exit()
{
    printf("Idle exit \n");
    close_door();
}

void fsm_active_up_exit()
{
    printf("Active up exit \n");
    queue_remove_floor_orders(elevator_current_floor);
    queue_update_fsm(&current_packet); // update current_order, next_order, current_floor
    fsm_update_state();
    elevio_motorDirection(DIRN_STOP);
}

void fsm_active_down_exit()
{
    printf("Active down exit \n");
    queue_remove_floor_orders(elevator_current_floor);
    queue_update_fsm(&current_packet);
    fsm_update_state();
    elevio_motorDirection(DIRN_STOP);
}

void fsm_stop_exit()
{
    printf("Stop exit \n");
    if (peripherals_check_valid_floor() == 1) {
        peripherals_timer(3);
    }
    update_stop_button(0);
}

void fsm_valid_floor_check_exit()
{
    printf("Valid floor check exit \n");
    return;
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// Update functions
//////////////////////////////////////////////////////////////////////////////////////////////////

void fsm_update_state()
{

    current_order.dir = current_packet.current_order_dir;
    current_order.floor = current_packet.current_order_floor;

    next_order.dir = current_packet.next_order_dir;
    next_order.floor = current_packet.next_order_floor;

    elevator_current_floor = current_packet.elevator_current_floor;
}

void fsm_init_update()
{
    return;
}

void fsm_idle_update()
{
    queue_update_fsm(&current_packet);
    fsm_update_state();

    peripherals_button_polling();

    // event for stop button
    int stop_button_status = elevio_stopButton();
    if (stop_button_status != 0)
    {
        update_stop_button(1);
        event = stop_button_pressed;
    }

    // event for either going up or down
    if (current_order.floor != -1 && elevio_obstruction() != 1)
    {
        if ((current_order.floor > elevator_current_floor)) 
        {
            event = order_up;
        }
        if ( (current_order.floor < elevator_current_floor))
        {
            event = order_down;
        }
        if(current_order.floor == elevator_current_floor) {
            queue_remove_floor_orders(elevator_current_floor);
            if (peripherals_check_valid_floor() == 0) {
                peripherals_open_door_timer();
            }
        }
    }
}

void fsm_active_up_update()
{
    queue_update_fsm(&current_packet);
    fsm_update_state();

    peripherals_button_polling();

    if (elevio_stopButton())
    {
        update_stop_button(1);
        event = stop_button_pressed;
    }

    if(current_order.floor == elevator_current_floor && peripherals_check_valid_floor() == 0)
    {
            elevio_motorDirection(DIRN_STOP);
            event = valid_floor_in_queue;
    }
}

void fsm_active_down_update()
{
    queue_update_fsm(&current_packet);
    fsm_update_state();

    peripherals_button_polling();

    if (elevio_stopButton())
    {
        update_stop_button(1);
        event = stop_button_pressed;
    }

    if(current_order.floor == elevator_current_floor && peripherals_check_valid_floor() == 0)
    {
            elevio_motorDirection(DIRN_STOP);
            event = valid_floor_in_queue;
    }
}

void fsm_stop_update()
{
    // if not stop button is pressed timeout and then event = stop_button_time_out
    queue_flush(); // Delete all new orders

    peripherals_button_polling();
    if (!elevio_stopButton())
    {
        update_stop_button(0);
        event = stop_button_time_out;
    }
    if (peripherals_check_valid_floor() == 0)
    {
        open_door();
    }
    else
    {
        if (elevio_obstruction() != 0) {
            close_door();
        }
    }
}

void fsm_valid_floor_check_update()
{
    //printf("VALID FLOOR UPDATE \n");
    if (peripherals_check_valid_floor() == 0)
    {
        event = state_found;
    }
    
    return;
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// FSM debugging
//////////////////////////////////////////////////////////////////////////////////////////////////

int util_fsm_print_current_order()
{
    printf("FSM: Current order: Floor %i dir %i \n", current_order.floor, current_order.dir);
    return 0;
};

int util_fsm_print_next_order()
{
    printf("FSM: Next order: Floor %i dir %i \n", next_order.floor, next_order.dir);
    return 0;
};

int util_fsm_values()
{
    printf("FSM: Current state: %i \n", stateMachine.currState);
    printf("FSM: Current Floor: %i \n", elevator_current_floor);
    return 0;
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// FSM entry
//////////////////////////////////////////////////////////////////////////////////////////////////

void fsm_entry()
{
    
    fsm_init_enter();
    while (1)
    {
        volatile int state_transitions_array_len = sizeof(state_transitions) / sizeof(state_transitions[0]);

        // Transitions (ENTER AND EXIT)
        for (int i = 0; i < state_transitions_array_len; i++)
        {
            if (stateMachine.currState == state_transitions[i].currState)
            {
                if (event == state_transitions[i].event)
                {
                    // Transition to the next state
                    // Call the function associated with the exit transition
                    (stateFunctionA[stateMachine.currState].exit_function)();
                    // Changing state
                    stateMachine.currState = state_transitions[i].nextState;

                    // Call the function associated with enter transition
                    (stateFunctionA[stateMachine.currState].enter_function)();
                    // event = no_event;
                    break;
                }
            }
        }

        // UPDATE LOOP, RUNS THE STATE UPDATES
        for (int i = 0; i < state_transitions_array_len; i++)
        {
            if (stateMachine.currState == state_transitions[i].currState)
            {
                (stateFunctionA[stateMachine.currState].update_function)();
                // printf("update function commence \n");
                break;
            }
        }

        //util_fsm_print_current_order();
        // util_fsm_print_next_order();
        // util_fsm_values();
        // util_queue_print();
    }
}

// Dør skal ikke åpnes mellom etasjer
// stopp knapp holdes inne --> ikke legg inn nye queues.