#ifndef HELICOPTER_H
#define HELICOPTER_H

#include "GL_utilities.h"
#include "VectorUtils3.h"
#include "LoadTGA.h"
#include "MicroGlut.h"
#include "LittleOBJLoader.h"

extern Model *helicopter_body;
extern Model *helicopter_components_1;
extern Model *helicopter_components_2;
extern Model *helicopter_blade_1;
extern Model *helicopter_blade_2;

void init(void);
void helicopter_init(void);
void drawHelicopter(GLuint program, mat4 cameraMatrix);

#endif