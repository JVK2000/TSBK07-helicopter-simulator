#ifndef COMMON_H
#define COMMON_H

#ifdef __APPLE__
#include <OpenGL/gl3.h>
#endif

#include "MicroGlut.h"
#include "GL_utilities.h"
#include "VectorUtils3.h"
#include "LittleOBJLoader.h"
#include "LoadTGA.h"

extern GLuint program;

extern GLint specularLightEnabledLoc;
extern GLint ambientLightEnabledLoc;
extern GLint diffuseLightEnabledLoc;
extern GLint textureEnabledLoc;
extern GLint translationMatrixLoc;
extern GLint mdlMatrixLoc;
extern GLint normalMatrixLoc;


#endif
