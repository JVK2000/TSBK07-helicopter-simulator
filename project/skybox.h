#ifndef SKYBOX_H
#define SKYBOX_H

#include "common.h"

extern TextureData ttex;

void skyboxInit();
void update_time();
void drawSkybox(float cameraAngleY, float cameraAngleX) ;


#endif
