#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <time.h>
#include "driver/elevio.h"
#include "driver/queue.h"

int main(){
    
    queue_initalize();

    queue_create_new_order(0, UP);
    queue_create_new_order(1, UP);
    queue_create_new_order(2, UP);
    queue_create_new_order(3, UP);
    queue_create_new_order(0, UP);
    queue_create_new_order(1, UP);
    queue_create_new_order(2, UP);
    queue_create_new_order(3, UP);
    queue_create_new_order(0, UP);
    queue_create_new_order(1, UP);
    queue_create_new_order(2, UP);
    queue_create_new_order(3, UP);

    queue_print();
    queue_flush();

    queue_create_new_order(0, UP);
    queue_create_new_order(1, UP);
    queue_create_new_order(2, UP);

    queue_print();

    struct order* test = queue_search(3);

    if(test != NULL) {
        printf("Found floor %i \n", test->floor);
        queue_delete(test);
    }

    queue_create_new_order(2, UP);
    
    struct order* test1 = queue_search(2);

    if(test1 != NULL) {
        printf("Found floor %i \n", test1->floor);
    } else {
        printf("NULL \n");
    }

    queue_delete(test1);

    struct order* test2 = queue_search(1);

    if(test2 != NULL) {
        printf("Found floor %i \n", test2->floor);
    }

    queue_delete(test2);
    
    queue_print();

    return 0;

}
