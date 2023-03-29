#ifndef HELICOPTER_H
#define HELICOPTER_H

#include "GL_utilities.h"
#include "VectorUtils3.h"
#include "LoadTGA.h"
#include "MicroGlut.h"
#include "LittleOBJLoader.h"


// void init(void);
void helicopter_init(void);
void drawHelicopter(GLuint program, mat4 cameraMatrix);

#endif