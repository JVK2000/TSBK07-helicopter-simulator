#include "helicopter.h"
#include "helicopter_controller_shared.h"


Model *helicopter_body, *helicopter_components_1, *helicopter_components_2, *helicopter_blade_1, *helicopter_blade_2;

void helicopterInit() {
    helicopter_body = LoadModel("assets/helicopter_body.obj");
    helicopter_components_1 = LoadModel("assets/helicopter_components_1.obj");
    helicopter_components_2 = LoadModel("assets/helicopter_components_2.obj");
    helicopter_blade_1 = LoadModel("assets/helicopter_blade_1.obj");
    helicopter_blade_2 = LoadModel("assets/helicopter_blade_2.obj");
}


void drawHelicopter(mat4 cameraMatrix, float cameraAngleY) {
	GLint isHelicopterLoc = glGetUniformLocation(program, "isHelicopter");
    GLint specularEnabledLoc = glGetUniformLocation(program, "specularEnabled");
    GLint ambientEnabledLoc = glGetUniformLocation(program, "ambientEnabled");
    GLint diffuseEnabledLoc = glGetUniformLocation(program, "diffuseEnabled");
    GLint textureEnabledLoc = glGetUniformLocation(program, "textureEnabled");
    GLint translationMatrixLoc = glGetUniformLocation(program, "translationMatrix");
    GLint mdlMatrixLoc = glGetUniformLocation(program, "mdlMatrix");
    
    glUniform1i(isHelicopterLoc, true);
    glUniform1i(specularEnabledLoc, false);
    glUniform1i(ambientEnabledLoc, false);
    glUniform1i(diffuseEnabledLoc, true);
    glUniform1i(textureEnabledLoc, false);

    GLfloat t = (GLfloat)glutGet(GLUT_ELAPSED_TIME);
    float rotation_speed_blade1 = 15 * (t / 1000);
    float rotation_speed_blade2 = (t / 1000);

	mat4 globalRotation;
	mat4 globalTranslation;
	mat4 globalScaling = S(0.1, 0.1, 0.1);
	globalRotation = Mult(Rx(getYAngle() + getXTilt()), Mult(Ry(M_PI), Rz(getZTilt())));
	globalTranslation = T(0, -10, -50);


    // Main helicopter body
	mat4 trans = IdentityMatrix();
	trans = Mult(globalScaling, trans);
	trans = Mult(T(1, 0, 0), trans);
	trans = Mult(globalRotation, trans);
	trans = Mult(globalTranslation, trans);
	glUniformMatrix4fv(translationMatrixLoc, 1, GL_TRUE, trans.m);

	mat4 modelViewMatrix = InvertMat4(Mult(cameraMatrix, trans)); // Invert for light to rotate in right direction
	glUniformMatrix4fv(glGetUniformLocation(program, "normalMatrix"), 1, GL_TRUE, modelViewMatrix.m);	
	glUniformMatrix4fv(mdlMatrixLoc, 1, GL_TRUE, trans.m);	
    DrawModel(helicopter_body, program, "inPosition", "inNormal", "inTexCoord");
    DrawModel(helicopter_components_1, program, "inPosition", "inNormal", "inTexCoord");
    DrawModel(helicopter_components_2, program, "inPosition", "inNormal", "inTexCoord");

    // Main helicopter blade
	mat4 blade_trans = IdentityMatrix();
	blade_trans = Mult(globalScaling, blade_trans);
	blade_trans = Mult(Ry(rotation_speed_blade1), blade_trans);
	blade_trans = Mult(T(0.2, 0, 2), blade_trans);
	blade_trans = Mult(globalRotation, blade_trans);
	blade_trans = Mult(globalTranslation, blade_trans);
    glUniformMatrix4fv(translationMatrixLoc, 1, GL_TRUE, blade_trans.m);
	glUniformMatrix4fv(mdlMatrixLoc, 1, GL_TRUE, blade_trans.m);	
    DrawModel(helicopter_blade_1, program, "inPosition", "inNormal", "inTexCoord");

    // Secondarily helicopter blade
	blade_trans = IdentityMatrix();
	blade_trans = Mult(globalScaling, blade_trans);
	blade_trans = Mult(Rx(rotation_speed_blade2), blade_trans);
	blade_trans = Mult(T(-1.5, 3.85, -19.57), blade_trans);
	blade_trans = Mult(globalRotation, blade_trans);
	blade_trans = Mult(globalTranslation, blade_trans);
    glUniformMatrix4fv(translationMatrixLoc, 1, GL_TRUE, blade_trans.m);
	mat4 total = Mult(cameraMatrix, blade_trans);
	glUniformMatrix4fv(mdlMatrixLoc, 1, GL_TRUE, total.m);
    DrawModel(helicopter_blade_2, program, "inPosition", "inNormal", "inTexCoord");
    
	glUniform1i(isHelicopterLoc, false);
}

