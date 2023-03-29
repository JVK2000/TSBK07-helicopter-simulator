#include "controller.h"


float cameraAngleZ;
float cameraAngleX;
vec3 cameraPosition;
vec3 lookAtPosition;
vec3 worldUpVector;
mat4 cameraMatrix;

void controllerInit()
{
    cameraAngleZ = 0;
    cameraAngleX = 0;
    cameraPosition = (vec3){0, 10, 10};
    lookAtPosition = (vec3){0, 10, 0};
    worldUpVector = (vec3){0, 1, 0};
}

float MOUSE_MOVE_SPEED = 400;
float init_x = 0;
float init_z = 0;
bool first_iteration = true;
bool leftMouseDown = false;
void mouseMovement(int x, int y)
{
	if (first_iteration) {
		init_x = x;
		init_z = y;
		first_iteration = false;
	}
	float curr_x = x - init_x;
	float curr_y = y - init_z;
	cameraAngleX = (curr_x/MOUSE_MOVE_SPEED)*M_PI;
	cameraAngleZ = (curr_y/MOUSE_MOVE_SPEED)*M_PI;
}

float MOVEMENT_SPEED = 0.8;
float pos_x = 0;
float pos_z = 0;
float const_ang = M_PI/4;
void keyboardMovement()
{
	float pos_x = 0;
	float pos_z = 0;
	float pos_y = 0;
	if (glutKeyIsDown('d')) {
		pos_x += MOVEMENT_SPEED * cos(cameraAngleX); 
		pos_z += MOVEMENT_SPEED * sin(cameraAngleX); 
	} if (glutKeyIsDown('a')) {
		pos_x -= MOVEMENT_SPEED * cos(cameraAngleX); 
		pos_z -= MOVEMENT_SPEED * sin(cameraAngleX); 
	} if (glutKeyIsDown('w')) {
		pos_x += MOVEMENT_SPEED * sin(cameraAngleX); 
		pos_z -= MOVEMENT_SPEED * cos(cameraAngleX);
	} if (glutKeyIsDown('s')) {
		pos_x -= MOVEMENT_SPEED * sin(cameraAngleX); 
		pos_z += MOVEMENT_SPEED * cos(cameraAngleX); 
	} if (glutKeyIsDown('q')) {
		pos_y -= MOVEMENT_SPEED;
	} if (glutKeyIsDown('e')) {
		pos_y += MOVEMENT_SPEED;
	}
	cameraPosition.x += pos_x; 
	cameraPosition.z += pos_z; 
	cameraPosition.y += pos_y; 
	lookAtPosition.x += pos_x;
	lookAtPosition.z += pos_z;
	lookAtPosition.y += pos_y;

	cameraMatrix = Mult(Rx(cameraAngleZ), Mult(Ry(cameraAngleX), lookAtv(cameraPosition, lookAtPosition, worldUpVector)));
	glUniformMatrix4fv(glGetUniformLocation(program, "cameraMatrix"), 1, GL_TRUE, cameraMatrix.m);
}
