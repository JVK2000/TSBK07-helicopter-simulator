#ifndef TERRAIN_H
#define TERRAIN_H

#include "common.h"

extern TextureData ttex;

void terrainInit(GLuint *tex1, GLuint *tex2);
Model* GenerateTerrain(TextureData *tex);
float find_height(float x, float z);
void draw_terrain_section(mat4 cameraMatrix, float x, float z);
void draw_terrain(mat4 cameraMatrix, vec3 p);
float texture_data_height();
void drawSkybox(GLuint texUnit, float cameraAngleZ, float cameraAngleX);

#endif
