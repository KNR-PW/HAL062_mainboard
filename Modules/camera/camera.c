#include "camera.h"
#include "timers/timers.h"

extern int cam_bridge[3];


void handleCamera(uint8_t *data) {
    // Sprawdzenie czy dane spełniają warunki i włączenie odpowiedniej kamery

    switch(data[0]){

        case 1:
                 cam_bridge[0] = 0;
                 break;
        case 2:
                 cam_bridge[0] = 1;
                 break;
        case 3:
                 cam_bridge[0] = 2;
                 break;
        default:
        		break;


    }
    
     switch(data[1]){

        case 1:
                 cam_bridge[1] = 0;
                 break;
        case 2:
                 cam_bridge[1] = 1;
                 break;
        case 3:
                 cam_bridge[1] = 2;
                 break;
        default:
        		break;

    }

     switch(data[2]){

        case 1:
                 cam_bridge[2] = 0;
                 break;
        case 2:
                 cam_bridge[2] = 1;
                 break;
        case 3:
                 cam_bridge[2] = 2;
                 break;
        default:
        		break;

    }


    
}
