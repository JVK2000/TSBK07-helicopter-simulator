#include "controller.h"


const float ACCELERATION_HORIZONTAL = 0.1f;
const float ACCELERATION_VERTICAL = 0.02f;
const float FRICTION = 0.9f;
const float MOUSE_MOVE_SPEED = 400;
const float MAX_VELOCITY_HORIZONTAL = 2.0f;
const float MAX_VELOCITY_VERTICAL = 1.0f;
const float VELOCITY_AMPLIFIER = 2.0f;

bool first_iteration = true;
float init_x = 0;
float init_z = 0;
float cameraAngleZ;
float cameraAngleX;
float pos_x = 0;
float pos_z = 0;
vec3 cameraPosition;
vec3 lookAtPosition;
vec3 worldUpVector;
mat4 cameraMatrix;


void controllerInit()
{
    cameraAngleZ = 0;
    cameraAngleX = 0;
    cameraPosition = (vec3){0, 10, 50};
    lookAtPosition = (vec3){0, 10, 0};
    worldUpVector = (vec3){0, 1, 0};
}


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

void keyboardMovement()
{
    float pos_x = 0;
    float pos_z = 0;
    float pos_y = 0;
    static float vel_x = 0;
    static float vel_z = 0;
    static float vel_y = 0;

    bool moveForwardKeyDown = glutKeyIsDown('w');
    bool moveBackwardKeyDown = glutKeyIsDown('s');
    bool moveLeftKeyDown = glutKeyIsDown('a');
    bool moveRightKeyDown = glutKeyIsDown('d');
    bool moveUpKeyDown = glutKeyIsDown('e');
    bool moveDownKeyDown = glutKeyIsDown('q');

    if (moveForwardKeyDown) {
        moveForward(&vel_x, &vel_z, 1);
    }
    if (moveBackwardKeyDown) {
        moveBackward(&vel_x, &vel_z, 1);
    }
    if (moveLeftKeyDown) {
        moveLeft(&vel_x, &vel_z, 1);
    }
    if (moveRightKeyDown) {
        moveRight(&vel_x, &vel_z, 1);
    }

    if (moveDownKeyDown) {
        vel_y -= ACCELERATION_VERTICAL;
    }
    if (moveUpKeyDown) {
        vel_y += ACCELERATION_VERTICAL;
    }

    // Apply friction
    if (!moveForwardKeyDown && !moveBackwardKeyDown) {
        vel_x *= FRICTION;
        vel_z *= FRICTION;
    }
    if (!moveLeftKeyDown && !moveRightKeyDown) {
        vel_x *= FRICTION;
        vel_z *= FRICTION;
    }
    if (!moveDownKeyDown && !moveUpKeyDown) {
        vel_y *= FRICTION;
    }

    // Limit the maximum velocity
    float currentVelocity = fabs(vel_x) + fabs(vel_z);
    if (currentVelocity > MAX_VELOCITY_HORIZONTAL) {
        float ratio = MAX_VELOCITY_HORIZONTAL / currentVelocity;
        vel_x *= ratio;
        vel_z *= ratio;
    }

    if (vel_y > MAX_VELOCITY_VERTICAL) {
        vel_y = MAX_VELOCITY_VERTICAL;
    } 
    else if (vel_y < -MAX_VELOCITY_VERTICAL) {
        vel_y = -MAX_VELOCITY_VERTICAL;
    } 

    // Update position
    pos_x += vel_x;
    pos_z += vel_z;
    pos_y += vel_y;

    cameraPosition.x += pos_x; 
	cameraPosition.z += pos_z; 
	cameraPosition.y += pos_y; 
	lookAtPosition.x += pos_x;
	lookAtPosition.z += pos_z;
	lookAtPosition.y += pos_y;   

    const float radius = -50.0f;
    vec3 tempCamPos = (vec3){cameraPosition.x, cameraPosition.y, cameraPosition.z - 50.0f}; 
    tempCamPos.x = tempCamPos.x + sin(cameraAngleX) * radius;
    tempCamPos.z = tempCamPos.z + cos(cameraAngleX) * radius;
    // cameraPosition.y = cos(cameraAngleZ) * radius;


    printf("Angle: %f\n", cameraAngleX);
    printf("cam: %f, %f, %f\n", cameraPosition.x, cameraPosition.y, cameraPosition.z);
    printf("look: %f, %f, %f\n", lookAtPosition.x, lookAtPosition.y, lookAtPosition.z);


	cameraMatrix = IdentityMatrix();
    // cameraMatrix = Mult(cameraMatrix, Rx(cameraAngleZ));
    // cameraMatrix = Mult(cameraMatrix, Ry(cameraAngleX));
    cameraMatrix = Mult(cameraMatrix, lookAtv(tempCamPos, lookAtPosition, worldUpVector));

}



void printMatrix(float matrix[4][4]) {
    for (int row = 0; row < 4; ++row) {
        for (int col = 0; col < 4; ++col) {
            printf("%f ", matrix[row][col]);
        }
        printf("\n");
    }
}

void moveRight(float *vel_x, float *vel_z, float fraction) 
{
    *vel_x += VELOCITY_AMPLIFIER * ACCELERATION_HORIZONTAL * cos(cameraAngleX) * fraction;
    *vel_z += VELOCITY_AMPLIFIER * ACCELERATION_HORIZONTAL * sin(cameraAngleX) * fraction;
}

void moveLeft(float *vel_x, float *vel_z, float fraction) 
{
    *vel_x -= VELOCITY_AMPLIFIER * ACCELERATION_HORIZONTAL * cos(cameraAngleX) * fraction;
    *vel_z -= VELOCITY_AMPLIFIER * ACCELERATION_HORIZONTAL * sin(cameraAngleX) * fraction;
}

void moveForward(float *vel_x, float *vel_z, float fraction) 
{
    *vel_x += VELOCITY_AMPLIFIER * ACCELERATION_HORIZONTAL * sin(cameraAngleX) * fraction;
    *vel_z -= VELOCITY_AMPLIFIER * ACCELERATION_HORIZONTAL * cos(cameraAngleX) * fraction;
}

void moveBackward(float *vel_x, float *vel_z, float fraction) 
{
    *vel_x -= VELOCITY_AMPLIFIER * ACCELERATION_HORIZONTAL * sin(cameraAngleX) * fraction;
    *vel_z += VELOCITY_AMPLIFIER * ACCELERATION_HORIZONTAL * cos(cameraAngleX) * fraction;
}

