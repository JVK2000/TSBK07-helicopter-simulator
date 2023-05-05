#ifndef CONTROLLER_H
#define CONTROLLER_H

#include "common.h"

extern float cameraAngleY;      // Camera rotation angle on z-axis
extern float cameraAngleX;      // Camera rotation angle on x-axis
extern vec3 cameraPosition;	    // Camera position 
extern vec3 lookAtPosition;	    // Position to look at
extern vec3 worldUpVector;		// Determines which axis is up
extern mat4 cameraMatrix;

void controllerInit();
void controllerHandler();

#endif
