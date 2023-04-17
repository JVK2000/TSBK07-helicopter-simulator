#ifndef TERRAIN_H
#define TERRAIN_H

#include "common.h"

extern TextureData ttex;

void terrainInit(GLuint *tex1, GLuint *tex2);
Model* GenerateTerrain(TextureData *tex, int x_offset, int z_offset);
float find_height(float x, float z);
void draw_terrain_section(mat4 cameraMatrix, Model *terrainModel, float x, float z);
void draw_terrain(mat4 cameraMatrix, vec3 p);
float texture_data_height();
void drawSkybox(GLuint texUnit, float cameraAngleY, float cameraAngleX);

float wrappedNoise2D(float x, float x_offset, float z, float z_offset, int terrainScale, TextureData *tex);
int check_collision(float x, float y, float z, float radius);

void detect_collision();

#endif
