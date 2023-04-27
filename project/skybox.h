#ifndef SKYBOX_H
#define SKYBOX_H

#include "common.h"

extern TextureData ttex;

void skyboxInit(GLuint *skytex1, GLuint *skytex2);
void drawSkybox(GLuint skytex1, GLuint skytex2, float cameraAngleY, float cameraAngleX) ;


#endif
