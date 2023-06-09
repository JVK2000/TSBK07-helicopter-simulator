#ifndef TERRAIN_H
#define TERRAIN_H

#include "common.h"

extern TextureData ttex;

void terrainInit(GLuint *tex1, GLuint *tex2);
void drawTerrain(mat4 cameraMatrix, vec3 p);
void detectCollision();

#endif
