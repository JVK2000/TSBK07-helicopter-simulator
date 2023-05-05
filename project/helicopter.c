#include "helicopter.h"
#include "helicopter_controller_shared.h"


Model *helicopter_body, *helicopter_components_1, *helicopter_components_2, *helicopter_blade_1, *helicopter_blade_2;
GLint isHelicopterLoc, helicopterColorLoc;
mat4 globalRotation, globalTranslation, globalScaling;
GLfloat rotor_timer;

vec4 primaryColor = { 0.3098, 0.4510, 0.3255, 1.0 };
vec4 secondaryAttachmentColor = { 0.6314, 0.2784, 0.1686, 1.0 };
vec4 attachmentColor = { 0.2, 0.2, 0.2, 1.0 };
vec4 rotorColor = { 0.325, 0.325, 0.325, 1.0 };


void helicopterInit() 
{
    helicopter_body = LoadModel("assets/helicopter/helicopter_body.obj");
    helicopter_components_1 = LoadModel("assets/helicopter/helicopter_components_1.obj");
    helicopter_components_2 = LoadModel("assets/helicopter/helicopter_components_2.obj");
    helicopter_blade_1 = LoadModel("assets/helicopter/helicopter_blade_1.obj");
    helicopter_blade_2 = LoadModel("assets/helicopter/helicopter_blade_2.obj");

	isHelicopterLoc = glGetUniformLocation(program, "isHelicopter");
	helicopterColorLoc = glGetUniformLocation(program, "helicopterColor");
}

void setHelicopterColor(vec4 colorVec4) 
{
	glUniform4f(helicopterColorLoc, colorVec4.x, colorVec4.y, colorVec4.z, colorVec4.w);
}


void drawHelicopterBody(mat4 cameraMatrix) 
{
	mat4 trans = globalScaling;
	trans = Mult(T(1, 0, 0), trans);
	trans = Mult(globalRotation, trans);
	trans = Mult(globalTranslation, trans);

	glUniformMatrix4fv(translationMatrixLoc, 1, GL_TRUE, trans.m);
	mat4 modelViewMatrix = Mult(InvertMat4(cameraMatrix), trans); // Invert for light to rotate in the right direction
	mat3 normalMatrix = InverseTranspose(modelViewMatrix);

	glUniformMatrix3fv(glGetUniformLocation(program, "normalMatrixMat3"), 1, GL_TRUE, normalMatrix.m);
	glUniformMatrix4fv(normalMatrixLoc, 1, GL_TRUE, modelViewMatrix.m);	
	glUniformMatrix4fv(mdlMatrixLoc, 1, GL_TRUE, trans.m);	

	setHelicopterColor(primaryColor);
	DrawModel(helicopter_body, program, "inPosition", "inNormal", "inTexCoord");

	setHelicopterColor(secondaryAttachmentColor);
	DrawModel(helicopter_components_1, program, "inPosition", "inNormal", "inTexCoord");

	setHelicopterColor(attachmentColor);
	DrawModel(helicopter_components_2, program, "inPosition", "inNormal", "inTexCoord");
}


void drawHelicopterMainRotor(mat4 cameraMatrix, float rotationMainRotor) 
{
	mat4 blade_trans = globalScaling;
	blade_trans = Mult(Ry(rotationMainRotor), blade_trans);
	blade_trans = Mult(T(0.2, 0, 2), blade_trans);
	blade_trans = Mult(globalRotation, blade_trans);
	blade_trans = Mult(globalTranslation, blade_trans);

    glUniformMatrix4fv(translationMatrixLoc, 1, GL_TRUE, blade_trans.m);
	mat4 modelViewMatrix = Mult(InvertMat4(cameraMatrix), blade_trans);
	mat3 normalMatrix = InverseTranspose(modelViewMatrix);

	glUniformMatrix3fv(glGetUniformLocation(program, "normalMatrixMat3"), 1, GL_TRUE, normalMatrix.m);
	glUniformMatrix4fv(mdlMatrixLoc, 1, GL_TRUE, blade_trans.m);	
	setHelicopterColor(rotorColor);

	DrawModel(helicopter_blade_1, program, "inPosition", "inNormal", "inTexCoord");
}


void drawHelicopterTailRotor(mat4 cameraMatrix, float rotationTailRotor) 
{
	mat4 blade_trans = globalScaling;
	blade_trans = Mult(Rx(rotationTailRotor), blade_trans);
	blade_trans = Mult(T(-1.5, 3.85, -19.57), blade_trans);
	blade_trans = Mult(globalRotation, blade_trans);
	blade_trans = Mult(globalTranslation, blade_trans);

    glUniformMatrix4fv(translationMatrixLoc, 1, GL_TRUE, blade_trans.m);
	mat4 modelViewMatrix = Mult(InvertMat4(cameraMatrix), blade_trans);
	mat3 normalMatrix = InverseTranspose(modelViewMatrix);

	glUniformMatrix3fv(glGetUniformLocation(program, "normalMatrixMat3"), 1, GL_TRUE, normalMatrix.m);
	glUniformMatrix4fv(mdlMatrixLoc, 1, GL_TRUE, blade_trans.m);
	setHelicopterColor(rotorColor);

	DrawModel(helicopter_blade_2, program, "inPosition", "inNormal", "inTexCoord");
}


void drawHelicopter(mat4 cameraMatrix, float cameraAngleY) 
{
    glUniform1i(isHelicopterLoc, true);
    glUniform1i(textureEnabledLoc, false);

    rotor_timer = (GLfloat)glutGet(GLUT_ELAPSED_TIME);
    float rotationMainRotor = 20 * (rotor_timer / 1000);
    float rotationTailRotor = 20 * (rotor_timer / 1000);

	globalScaling = S(0.1, 0.1, 0.1);
	globalRotation = Mult(Rx(getYAngle() + getXTilt()), Mult(Ry(M_PI), Rz(getZTilt())));
	globalTranslation = T(0, -10, -50);

    glUniform1i(specularLightEnabledLoc, false);
    glUniform1i(ambientLightEnabledLoc, true); 
    glUniform1i(diffuseLightEnabledLoc, true);

	drawHelicopterBody(cameraMatrix);
	drawHelicopterMainRotor(cameraMatrix, rotationMainRotor);
	drawHelicopterTailRotor(cameraMatrix, rotationTailRotor);

	glUniform1i(isHelicopterLoc, false);
}
