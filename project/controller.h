#ifndef CONTROLLER_H
#define CONTROLLER_H

#include "common.h"

extern float cameraAngleZ;      // Camera rotation angle on z-axis
extern float cameraAngleX;      // Camera rotation angle on x-axis
extern vec3 cameraPosition;	    // Camera position 
extern vec3 lookAtPosition;	    // Position to look at
extern vec3 worldUpVector;		// Determines which axis is up
extern mat4 cameraMatrix;

void controllerInit();
void mouseMovement(int x, int y);
void keyboardMovement();
void moveRight(float *vel_x, float *vel_z, float fraction);
void moveLeft(float *vel_x, float *vel_z, float fraction);
void moveForward(float *vel_x, float *vel_z, float fraction);
void moveBackward(float *vel_x, float *vel_z, float fraction);

#endif
