#ifndef HELICOPTER_H
#define HELICOPTER_H

#include "common.h"


void helicopterInit(void);
void drawHelicopter(mat4 cameraMatrix, float cameraAngleY);
void drawHelicopterBody(mat4 cameraMatrix);
void drawHelicopterMainRotor(mat4 cameraMatrix, float rotationMainRotor);
void drawHelicopterTailRotor(mat4 cameraMatrix, float rotationTailRotor);

#endif