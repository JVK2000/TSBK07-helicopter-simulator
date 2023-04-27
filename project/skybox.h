#ifndef SKYBOX_H
#define SKYBOX_H

#include "common.h"

extern TextureData ttex;

void skyboxInit(GLuint *tex1, GLuint *tex2);
void drawSkybox(GLuint texUnit, float cameraAngleY, float cameraAngleX);


#endif
