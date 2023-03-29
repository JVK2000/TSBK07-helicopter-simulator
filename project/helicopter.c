#include "helicopter.h"


Model *helicopter_body, *helicopter_components_1, *helicopter_components_2, *helicopter_blade_1, *helicopter_blade_2;

void helicopterInit() {
    helicopter_body = LoadModel("assets/helicopter_body.obj");
    helicopter_components_1 = LoadModel("assets/helicopter_components_1.obj");
    helicopter_components_2 = LoadModel("assets/helicopter_components_2.obj");
    helicopter_blade_1 = LoadModel("assets/helicopter_blade_1.obj");
    helicopter_blade_2 = LoadModel("assets/helicopter_blade_2.obj");
}


void drawHelicopter(GLuint program, mat4 cameraMatrix) {
	glUniform1i(glGetUniformLocation(program, "isHelicopter"), true);
	glUniform1i(glGetUniformLocation(program, "shadingEnabled"), true);
	glUniform1i(glGetUniformLocation(program, "textureEnabled"), true);
	
	GLfloat t = (GLfloat)glutGet(GLUT_ELAPSED_TIME);
	float rotation_speed_blade1 = 15 * (t/1000);
	float rotation_speed_blade2 = (t/1000);

	// glUniformMatrix4fv(glGetUniformLocation(program, "cameraMatrix"), 1, GL_TRUE, IdentityMatrix().m);

	// mat4 globalRotation = Mult(Rx(M_PI/16), Mult(Ry(M_PI), Rz(0)));
    // mat4 globalTranslation = T(0, -10, -50);
	// mat4 globalScaling = S(0.1, 0.1, 0.1);
	mat4 globalRotation = Ry(t/1000);
    mat4 globalTranslation = T(10, 10, 10);
	mat4 globalScaling = S(0.1, 0.1, 0.1);

	// Static helicopter parts
	mat4 trans = IdentityMatrix();
	trans = Mult(globalScaling, trans);
	trans = Mult(T(1, 0, 0), trans);
	trans = Mult(globalRotation, trans);
	trans = Mult(globalTranslation, trans);
	glUniformMatrix4fv(glGetUniformLocation(program, "translationMatrix"), 1, GL_TRUE, trans.m);
	mat4 total = Mult(cameraMatrix, trans);
	glUniformMatrix4fv(glGetUniformLocation(program, "mdlMatrix"), 1, GL_TRUE, total.m);	
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
	glUniformMatrix4fv(glGetUniformLocation(program, "translationMatrix"), 1, GL_TRUE, blade_trans.m);
	total = Mult(cameraMatrix, blade_trans);
	glUniformMatrix4fv(glGetUniformLocation(program, "mdlMatrix"), 1, GL_TRUE, total.m);
	DrawModel(helicopter_blade_1, program, "inPosition", "inNormal", "inTexCoord");
	
	// Secondarily helicopter blade
	blade_trans = IdentityMatrix();
	blade_trans = Mult(globalScaling, blade_trans);
	blade_trans = Mult(Rx(rotation_speed_blade2), blade_trans);
	blade_trans = Mult(T(-1.5, 3.85, -19.57), blade_trans);
	blade_trans = Mult(globalRotation, blade_trans);
	blade_trans = Mult(globalTranslation, blade_trans);
	glUniformMatrix4fv(glGetUniformLocation(program, "translationMatrix"), 1, GL_TRUE, blade_trans.m);
	total = Mult(cameraMatrix, blade_trans);
	glUniformMatrix4fv(glGetUniformLocation(program, "mdlMatrix"), 1, GL_TRUE, total.m);	
	// glUniformMatrix4fv(glGetUniformLocation(program, "mdlMatrix"), 1, GL_TRUE, blade_trans.m);	
	DrawModel(helicopter_blade_2, program, "inPosition", "inNormal", "inTexCoord");
	glUniform1i(glGetUniformLocation(program, "isHelicopter"), false);

	// mat4 cameraMatrix = Mult(Rx(cameraAngleZ), Mult(Ry(cameraAngleX), lookAtv(p, l, v)));
	// glUniformMatrix4fv(glGetUniformLocation(program, "cameraMatrix"), 1, GL_TRUE, cameraMatrix.m);
}
