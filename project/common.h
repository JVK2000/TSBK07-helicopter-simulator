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

// extern GLint specularLightEnabledLoc = glGetUniformLocation(program, "specularLightEnabled");
// extern GLint ambientLightEnabledLoc = glGetUniformLocation(program, "ambientLightEnabled");
// extern GLint diffuseLightEnabledLoc = glGetUniformLocation(program, "diffuseLightEnabled");
// extern GLint textureEnabledLoc = glGetUniformLocation(program, "textureEnabled");
// extern GLint translationMatrixLoc = glGetUniformLocation(program, "translationMatrix");
// extern GLint mdlMatrixLoc = glGetUniformLocation(program, "mdlMatrix");
// extern GLint normalMatrixLoc = glGetUniformLocation(program, "normalMatrix");


#endif
