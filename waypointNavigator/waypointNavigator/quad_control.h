/*
 * quad_control.h
 *
 * Created: 7/10/2014 5:48:20 PM
 *  Author: Chris
 */ 

#include "main.h"
#include "timing.h"
#include "altimeter.h"
#include "quad_output.h"
#include "rx.h"
#include "debug.h"


#include <util/delay.h>



//------FUNCTION DECLARATIONS------//

//reach and maintain the given height
float quad_control_regulate_height(float targetHeight);

//send the arming pattern to the flight controller board to enable motors
void quad_control_arm();

void quad_control_test_regulate_height();