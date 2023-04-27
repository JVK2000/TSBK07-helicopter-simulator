#include "skybox.h"


// Model *tm, *skybox;
Model *skybox, *skyboxBottom;
GLint isSkyLoc;


void skyboxInit(GLuint *tex1, GLuint *tex2) {
    // Load terrain data
    glUniform1i(glGetUniformLocation(program, "tex"), 0); // Texture unit 0
    glUniform1i(glGetUniformLocation(program, "tex2"), 1); // Texture unit 1

    printError("init terrain");

    // skybox = LoadModel("cube.obj"); // 6 sides - not working
    skybox = LoadModel("labskybox.obj");
	isSkyLoc = glGetUniformLocation(program, "isSky");
}

void drawSkybox(GLuint texUnit, float cameraAngleY, float cameraAngleX) 
{
	glDisable(GL_DEPTH_TEST);
    glDisable(GL_CULL_FACE);

	glUniform1i(specularLightEnabledLoc, false);
    glUniform1i(ambientLightEnabledLoc, false);
    glUniform1i(diffuseLightEnabledLoc, false);
	glUniform1i(textureEnabledLoc, true);
	glUniform1i(isSkyLoc, true);

	mat4 translationMatrixSkybox = Mult(Rx(cameraAngleY), Ry(cameraAngleX));
	LoadTGATextureSimple("assets/skybox/labskybox-top.tga", &texUnit);			// Create texture object
	glBindTexture(GL_TEXTURE_2D, texUnit);						// Activate a texture object
	glUniform1i(glGetUniformLocation(program, "texUnit"), 0); 	// Texture unit 0
	glUniformMatrix4fv(glGetUniformLocation(program, "mdlMatrix"), 1, GL_TRUE, translationMatrixSkybox.m);
	DrawModel(skybox, program, "inPosition", "inNormal", "inTexCoord");

    // Draw the bottom skybox
    mat4 translationMatrixSkyboxBottom = Mult(Rx(cameraAngleY + M_PI), Ry(- cameraAngleX));
    LoadTGATextureSimple("assets/skybox/labskybox-bottom.tga", &texUnit);  // You may want to use a different texture for the bottom skybox
    glBindTexture(GL_TEXTURE_2D, texUnit);
    glUniform1i(glGetUniformLocation(program, "texUnit"), 0);
    glUniformMatrix4fv(glGetUniformLocation(program, "mdlMatrix"), 1, GL_TRUE, translationMatrixSkyboxBottom.m);
    DrawModel(skybox, program, "inPosition", "inNormal", "inTexCoord");


    glEnable(GL_CULL_FACE);
	glEnable(GL_DEPTH_TEST);

	glUniform1i(isSkyLoc, false);
} 