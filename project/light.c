#include "light.h"


void lightInit() 
{
    vec3 lightSourcesColorsArr[] = 
    { 
        {1.0f, 1.0f, 1.0f}  // White light
    }; 

    vec3 lightSourcesDirectionsPositions[] = 
    { 
        {0.0f, 1.0f, -1.0f}  // light along Y and Z
    };


    glUniform3fv(glGetUniformLocation(program, "lightSourcesDirPosArr"), 4, &lightSourcesDirectionsPositions[0].x);
    glUniform3fv(glGetUniformLocation(program, "lightSourcesColorArr"), 4, &lightSourcesColorsArr[0].x);
    glUniform1f(glGetUniformLocation(program, "specularExponent"), 60.0);
}