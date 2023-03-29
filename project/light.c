#include "light.h"


void lightInit() 
{
    vec3 lightSourcesColorsArr[] = 
    { 
        {1.0f, 0.0f, 0.0f}, // Red light
        {0.0f, 1.0f, 0.0f}, // Green light
        // {0.0f, 0.0f, 1.0f}, // Blue light
        {1.0f, 1.0f, 1.0f},  // White light
        {1.0f, 1.0f, 1.0f}  // White light
    }; 

    GLint isDirectional[] = 
    {
        0,0,1,1
    };

    vec3 lightSourcesDirectionsPositions[] = 
    { 
        {10.0f, 5.0f, 0.0f}, // Red light, positional
        {0.0f, 5.0f, 10.0f}, // Green light, positional
        {-1.0f, 0.0f, 0.0f}, // Blue light along X
        {0.0f, 0.0f, -1.0f}  // White light along Z
    };

    GLfloat specularExponent[] = 
    {
        100.0, 200.0, 60.0
    };

    glUniform3fv(glGetUniformLocation(program, "lightSourcesDirPosArr"), 4, &lightSourcesDirectionsPositions[0].x);
    glUniform3fv(glGetUniformLocation(program, "lightSourcesColorArr"), 4, &lightSourcesColorsArr[0].x);
    glUniform1iv(glGetUniformLocation(program, "isDirectional"), 4, isDirectional);
    glUniform1f(glGetUniformLocation(program, "specularExponent"), specularExponent[1]);
}