#ifndef HELICOPTER_CONTROLLER_SHARED_H
#define HELICOPTER_CONTROLLER_SHARED_H

void setYAngle(float angle);
float getYAngle();

void setZTilt(float tilt);
float getZTilt();

void setXTilt(float tilt);
float getXTilt();

vec3 getCamPos();
void setCamPos(vec3 pos);

mat4 getViewMatrix();
void setViewMatrix(mat4 matrix);

#endif /* HELICOPTER_CONTROLLER_SHARED_H */