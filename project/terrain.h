#include "GL_utilities.h"
#include "VectorUtils3.h"
#include "LoadTGA.h"
#include "MicroGlut.h"
#include "LittleOBJLoader.h"


extern TextureData ttex;

void terrain_init(GLuint program, GLuint tex1, GLuint tex2);
Model* GenerateTerrain(TextureData *tex);
float find_height(float x, float z);
void draw_terrain_section(GLuint program, mat4 cameraMatrix, float x, float z);
void draw_terrain(GLuint program, mat4 cameraMatrix, vec3 p);
float texture_data_height();
void drawSkybox(GLuint program, GLuint texUnit, float angle_z, float angle_x);