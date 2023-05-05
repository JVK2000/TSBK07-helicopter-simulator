#include "common.h"
#include "helicopter_controller_shared.h"

static float yAngle = 0.0f;
static float zTilt = 0.0f;
static float xTilt = 0.0f;

static vec3 camPos = (vec3) {0, 0, 0};
static mat4 view; 

float getYAngle() 
{
    return yAngle; 
}

void setYAngle(float angle) 
{
    yAngle = angle;
}

float getZTilt() 
{
    return zTilt; 
}

void setZTilt(float tilt) 
{
    zTilt = tilt;
}

float getXTilt() 
{
    return xTilt; 
}

void setXTilt(float tilt) 
{
    xTilt = tilt;
}

vec3 getCamPos() 
{
    return camPos; 
}

void setCamPos(vec3 pos) 
{
    camPos = pos;
}

mat4 getViewMatrix()
{
    return view; 
}

void setViewMatrix(mat4 matrix)
{
    view = matrix;
}