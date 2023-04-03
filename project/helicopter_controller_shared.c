#include "helicopter_controller_shared.h"

static float yAngle = 0.0f;
static float zTilt = 0.0f;
static float xTilt = 0.0f;

float getYAngle() {
    return yAngle; 
}

void setYAngle(float angle) {
    yAngle = angle;
}

float getZTilt() {
    return zTilt; 
}

void setZTilt(float tilt) {
    zTilt = tilt;
}

float getXTilt() {
    return xTilt; 
}

void setXTilt(float tilt) {
    xTilt = tilt;
}