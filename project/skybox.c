#include "skybox.h"
#include <time.h>

const float TIME_SCALE = 0.1f;
const int USE_REAL_TIMER = 0; // When true, bug occurs the frames becomes low during terrain generation

Model *skybox, *skyboxBottom;
GLint isSkyLoc;
GLuint texUnit1, texUnit2, skyTexTop, skyTexBottom, skyTexNightTop, skyTexNightBottom;

float skyTimer;
float lastTime = 0.0f;
float transitionPeriod = 0.2; // Transition period between day and night is 20% of the cycle
int frameCount = 0;

void skyboxInit() 
{
    skybox = LoadModel("assets/skybox/labskybox.obj");
	isSkyLoc = glGetUniformLocation(program, "isSky");
    printError("init terrain");

    LoadTGATextureSimple("assets/skybox/labskybox-top.tga", &skyTexTop); 
    LoadTGATextureSimple("assets/skybox/labskybox-bottom.tga", &skyTexBottom);
    LoadTGATextureSimple("assets/skybox/labskybox-night.tga", &skyTexNightTop);
    LoadTGATextureSimple("assets/skybox/labskybox-night.tga", &skyTexNightBottom);
}


float calculateValue(float input, float minVal, float maxVal) 
{
    return maxVal - (input * (maxVal - minVal));
}


void updateTime() 
{
    float deltaTime;
    
    if (USE_REAL_TIMER) {
        float currentTime = clock() / (float) CLOCKS_PER_SEC;
        if (lastTime == 0.0f) {
            deltaTime = 0.0f;
        } else {
            deltaTime = currentTime - lastTime; 
        }
        lastTime = currentTime;
    } else {
        deltaTime = 1 / 60.0f; // Assuming 60 FPS
    }

    // Ensure the skyTimer stays within the range of 0 to 2π
    skyTimer += deltaTime * TIME_SCALE;
    if (skyTimer > 2.0f * M_PI)
        skyTimer -= 2.0f * M_PI;


    if (skyTimer >= 1.0 + transitionPeriod / 2) {
        skyTimer = 0.0; // Reset timer after a full day-night cycle
    }

    float dayTimeBlender;
    if (skyTimer < 0.5 - transitionPeriod / 2) {
        dayTimeBlender = 0.0; // Full day
    } else if (skyTimer < 0.5 + transitionPeriod / 2) {
        dayTimeBlender = (skyTimer - (0.5 - transitionPeriod / 2)) / transitionPeriod; // Transition from day to night
    } else if (skyTimer < 1.0 - transitionPeriod / 2) {
        dayTimeBlender = 1.0; // Full night
    } else {
        dayTimeBlender = 1.0 - (skyTimer - (1.0 - transitionPeriod / 2)) / transitionPeriod; // Transition from night to day
    }
    glUniform1f(glGetUniformLocation(program, "dayTimeBlender"), dayTimeBlender);

    float diffuseStrength = calculateValue(dayTimeBlender, 0.2, 0.6);
    float ambientStrength = calculateValue(dayTimeBlender, 0.025, 0.2);
    glUniform1f(glGetUniformLocation(program, "diffuseStrength"), diffuseStrength);
    glUniform1f(glGetUniformLocation(program, "ambientStrength"), ambientStrength);
}


void drawSkybox(float cameraAngleY, float cameraAngleX) 
{
	glDisable(GL_DEPTH_TEST);
    glDisable(GL_CULL_FACE);

    updateTime();

	glUniform1i(specularLightEnabledLoc, false);
    glUniform1i(ambientLightEnabledLoc, false);
    glUniform1i(diffuseLightEnabledLoc, false);
	glUniform1i(textureEnabledLoc, true);
	glUniform1i(isSkyLoc, true);

    // --- Top skybox ---
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, skyTexTop);
	glUniform1i(glGetUniformLocation(program, "tex"), 0);  // Texture unit 0
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, skyTexNightTop);
	glUniform1i(glGetUniformLocation(program, "tex2"), 1);  // Texture unit 1

	mat4 translationMatrixSkybox = Mult(Rx(cameraAngleY), Ry(cameraAngleX));
	glUniformMatrix4fv(glGetUniformLocation(program, "mdlMatrix"), 1, GL_TRUE, translationMatrixSkybox.m);
	DrawModel(skybox, program, "inPosition", "inNormal", "inTexCoord");
    // ---------------------

	// --- Bottom skybox ---
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, skyTexBottom);
	glUniform1i(glGetUniformLocation(program, "tex"), 0);  // Texture unit 0
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, skyTexNightBottom);
	glUniform1i(glGetUniformLocation(program, "tex2"), 1);  // Texture unit 1

	mat4 translationMatrixSkyboxBottom = Mult(Rx(cameraAngleY + M_PI), Ry(- cameraAngleX));
	glUniformMatrix4fv(glGetUniformLocation(program, "mdlMatrix"), 1, GL_TRUE, translationMatrixSkyboxBottom.m);
	DrawModel(skybox, program, "inPosition", "inNormal", "inTexCoord");
    // ---------------------

	glEnable(GL_CULL_FACE);
	glEnable(GL_DEPTH_TEST);

	glUniform1i(isSkyLoc, false);
}
