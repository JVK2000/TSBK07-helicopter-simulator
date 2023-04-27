#include "skybox.h"


// Model *tm, *skybox;
Model *skybox, *skyboxBottom;
GLint isSkyLoc;
GLuint texUnit1, texUnit2;


void skyboxInit(GLuint *skytex1, GLuint *skytex2) {
    skybox = LoadModel("labskybox.obj");
	isSkyLoc = glGetUniformLocation(program, "isSky");
    printError("init terrain");

    LoadTGATextureSimple("assets/skybox/labskybox-top.tga", skytex1); // update skytex1 and skytex2 values
    LoadTGATextureSimple("assets/skybox/labskybox-bottom.tga", skytex2);

    // Note: No need to set the uniform here, as the texture unit doesn't change.
}

void drawSkybox(GLuint skytex1, GLuint skytex2, float cameraAngleY, float cameraAngleX) 
{
	glDisable(GL_DEPTH_TEST);
    glDisable(GL_CULL_FACE);

	glUniform1i(specularLightEnabledLoc, false);
    glUniform1i(ambientLightEnabledLoc, false);
    glUniform1i(diffuseLightEnabledLoc, false);
	glUniform1i(textureEnabledLoc, true);
	glUniform1i(isSkyLoc, true);

    // Bind the texture for the top skybox and tell the shader to use texture unit 0
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, skytex1);
	glUniform1i(glGetUniformLocation(program, "tex"), 0);  // Texture unit 0

	mat4 translationMatrixSkybox = Mult(Rx(cameraAngleY), Ry(cameraAngleX));
	glUniformMatrix4fv(glGetUniformLocation(program, "mdlMatrix"), 1, GL_TRUE, translationMatrixSkybox.m);
	DrawModel(skybox, program, "inPosition", "inNormal", "inTexCoord");

	// Bind the texture for the bottom skybox and tell the shader to use texture unit 0
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, skytex2);
	glUniform1i(glGetUniformLocation(program, "tex"), 0);  // Texture unit 0

	// Draw the bottom skybox
	mat4 translationMatrixSkyboxBottom = Mult(Rx(cameraAngleY + M_PI), Ry(- cameraAngleX));
	glUniformMatrix4fv(glGetUniformLocation(program, "mdlMatrix"), 1, GL_TRUE, translationMatrixSkyboxBottom.m);
	DrawModel(skybox, program, "inPosition", "inNormal", "inTexCoord");

	glEnable(GL_CULL_FACE);
	glEnable(GL_DEPTH_TEST);

	glUniform1i(isSkyLoc, false);
}
