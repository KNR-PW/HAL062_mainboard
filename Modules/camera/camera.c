void handleCamera(uint8_t *data) {
    // Sprawdzenie czy dane spełniają warunki i włączenie odpowiedniej kamery
    if (data[0] == 1) 
    {
        cam_bridge[0] = 1;
    }
    else
    {
        cam_bridge[0] = 4;	
    }
    
    if (data[0] == 2) 
    {
        cam_bridge[0] = 2;
    }
    else
    {
        cam_bridge[0] = 4;
    }
    
    if (data[0] == 3) 
    {
        cam_bridge[0] = 3;
    }
    else
    {
        cam_bridge[0] = 4;
    }

    if (data[1] == 1) 
    {
        cam_bridge[1] = 1;
    }
    else
    {
        cam_bridge[1] = 4;	
    }
    
    if (data[1] == 2) 
    {
        cam_bridge[1] = 2;
    }
    else
    {
        cam_bridge[1] = 4;
    }
    
    if (data[1] == 3) 
    {
        cam_bridge[1] = 3;
    }
    else
    {
        cam_bridge[1] = 4;
    }

    if (data[2] == 1) 
    {
        cam_bridge[2] = 1;
    }
    else
    {
        cam_bridge[2] = 4;	
    }
    
    if (data[2] == 2) 
    {
        cam_bridge[2] = 2;
    }
    else
    {
        cam_bridge[2] = 4;
    }
    
    if (data[2] == 3) 
    {
        cam_bridge[2] = 3;
    }
    else
    {
        cam_bridge[2] = 4;
    }
}
