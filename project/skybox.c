#include "skybox.h"
#include <time.h>

const float TIME_SCALE = 5.0f;

// Model *tm, *skybox;
Model *skybox, *skyboxBottom;
GLint isSkyLoc;
GLuint texUnit1, texUnit2;
GLuint skytex1, skytex2, skytex1_night, skytex2_night;
float timer;


void skyboxInit() {
    skybox = LoadModel("labskybox.obj");
	isSkyLoc = glGetUniformLocation(program, "isSky");
    printError("init terrain");

    LoadTGATextureSimple("assets/skybox/labskybox-top.tga", &skytex1); // update skytex1 and skytex2 values
    LoadTGATextureSimple("assets/skybox/labskybox-bottom.tga", &skytex2);
    LoadTGATextureSimple("assets/skybox/labskybox-bottom-night.tga", &skytex1_night); // update skytex1 and skytex2 values
    LoadTGATextureSimple("assets/skybox/labskybox-bottom-night.tga", &skytex2_night);

    // Note: No need to set the uniform here, as the texture unit doesn't change.
}

float calculateValue(float input, float minVal, float maxVal) {
    return maxVal - (input * (maxVal - minVal));
}



void update_time() {
    float currentTime = clock() / (float) CLOCKS_PER_SEC;
    static float lastTime = 0.0f;
    float deltaTime;

    if (lastTime == 0.0f) deltaTime = 0.0f; 
    else deltaTime = currentTime - lastTime; 

    lastTime = currentTime;

    // Add the elapsed time to the timer, scaled by timeScale, and wrap around every 2π
    timer += deltaTime * TIME_SCALE;
    if (timer > 2.0f * M_PI)
        timer -= 2.0f * M_PI;

    printf("timer: %f\n", timer);

    // float dayTimeBlender = (sin(timer) + 1.0) / 2.0;
    float transitionPeriod = 0.2; // Transition period is 20% of the cycle

    if (timer >= 1.0 + transitionPeriod / 2) {
        timer = 0.0; // Reset timer after a full day-night cycle
    }

    float dayTimeBlender;

    if (timer < 0.5 - transitionPeriod / 2) {
        dayTimeBlender = 0.0; // Full day
    } else if (timer < 0.5 + transitionPeriod / 2) {
        dayTimeBlender = (timer - (0.5 - transitionPeriod / 2)) / transitionPeriod; // Transition from day to night
    } else if (timer < 1.0 - transitionPeriod / 2) {
        dayTimeBlender = 1.0; // Full night
    } else {
        dayTimeBlender = 1.0 - (timer - (1.0 - transitionPeriod / 2)) / transitionPeriod; // Transition from night to day
    }



    printf("dayTimeBlender: %f\n", dayTimeBlender);
    glUniform1f(glGetUniformLocation(program, "dayTimeBlender"), dayTimeBlender);

    float diffuseStrength = calculateValue(dayTimeBlender, 0.2, 0.6);
    float ambientStrength = calculateValue(dayTimeBlender, 0.025, 0.2);
    printf("\ndiffuseStrength: %f\n",  diffuseStrength);
    printf("ambientStrength: %f\n",  ambientStrength);


    glUniform1f(glGetUniformLocation(program, "diffuseStrength"), diffuseStrength);
    glUniform1f(glGetUniformLocation(program, "ambientStrength"), ambientStrength);
    // glUniform1f(glGetUniformLocation(program, "timer"), timer);
}

void drawSkybox(float cameraAngleY, float cameraAngleX) 
{
	glDisable(GL_DEPTH_TEST);
    glDisable(GL_CULL_FACE);

    update_time();

	glUniform1i(specularLightEnabledLoc, false);
    glUniform1i(ambientLightEnabledLoc, false);
    glUniform1i(diffuseLightEnabledLoc, false);
	glUniform1i(textureEnabledLoc, true);
	glUniform1i(isSkyLoc, true);

    // Bind the texture for the top skybox and tell the shader to use texture unit 0
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, skytex1);
	glUniform1i(glGetUniformLocation(program, "tex"), 0);  // Texture unit 0
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, skytex1_night);
	glUniform1i(glGetUniformLocation(program, "tex2"), 1);  // Texture unit 1

	mat4 translationMatrixSkybox = Mult(Rx(cameraAngleY), Ry(cameraAngleX));
	glUniformMatrix4fv(glGetUniformLocation(program, "mdlMatrix"), 1, GL_TRUE, translationMatrixSkybox.m);
	DrawModel(skybox, program, "inPosition", "inNormal", "inTexCoord");

	// Bind the texture for the bottom skybox and tell the shader to use texture unit 0
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, skytex2);
	glUniform1i(glGetUniformLocation(program, "tex"), 0);  // Texture unit 0
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, skytex2_night);
	glUniform1i(glGetUniformLocation(program, "tex2"), 1);  // Texture unit 1

	// Draw the bottom skybox
	mat4 translationMatrixSkyboxBottom = Mult(Rx(cameraAngleY + M_PI), Ry(- cameraAngleX));
	glUniformMatrix4fv(glGetUniformLocation(program, "mdlMatrix"), 1, GL_TRUE, translationMatrixSkyboxBottom.m);
	DrawModel(skybox, program, "inPosition", "inNormal", "inTexCoord");

	glEnable(GL_CULL_FACE);
	glEnable(GL_DEPTH_TEST);

	glUniform1i(isSkyLoc, false);
}
