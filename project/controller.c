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
    cameraPosition = (vec3){0, 0, 45};
    lookAtPosition = (vec3){0, 0, 0};
    worldUpVector = (vec3){0, 1, 0};
}


void mouseMovement(int x, int y)
{
}


#define ANGULAR_ACCELERATION 0.005f
#define ANGULAR_FRICTION 0.8f


void keyboardMovement()
{
    manageVelocity();
    manageAngle();

    const float radius = -45.0f;
    vec3 tempCamPos = (vec3){cameraPosition.x, cameraPosition.y, cameraPosition.z + radius}; 
    tempCamPos.x = tempCamPos.x + cos(cameraAngleX) * radius;
    tempCamPos.z = tempCamPos.z + sin(cameraAngleX) * radius;
    tempCamPos.y = tempCamPos.y - sin(cameraAngleZ) * radius;

    // printf("Angle: %f\n", cameraAngleZ);
    // printf("cam: %f, %f, %f\n", cameraPosition.x, cameraPosition.y, cameraPosition.z);
    // printf("look: %f, %f, %f\n", lookAtPosition.x, lookAtPosition.y, lookAtPosition.z);

	cameraMatrix = IdentityMatrix();
    cameraMatrix = Mult(cameraMatrix, lookAtv(tempCamPos, lookAtPosition, worldUpVector));
}


void manageAngle() 
{
    static float angular_vel_x = 0;
    static float angular_vel_z = 0;

    bool rotateLeft = glutKeyIsDown(GLUT_KEY_LEFT);
    bool rotateRight = glutKeyIsDown(GLUT_KEY_RIGHT);
    bool rotateDown = glutKeyIsDown(GLUT_KEY_UP);
    bool rotateUp = glutKeyIsDown(GLUT_KEY_DOWN);

    if (rotateUp) {
        angular_vel_z -= ANGULAR_ACCELERATION;
    }
    if (rotateDown) {
        angular_vel_z += ANGULAR_ACCELERATION;
    }
    if (rotateLeft) {
        angular_vel_x -= ANGULAR_ACCELERATION;
    }
    if (rotateRight) {
        angular_vel_x += ANGULAR_ACCELERATION;
    }

    // Apply angular friction
    if (!rotateUp && !rotateDown) {
        angular_vel_z *= ANGULAR_FRICTION;
    }
    if (!rotateLeft && !rotateRight) {
        angular_vel_x *= ANGULAR_FRICTION;
    }

    // Update camera angles
    cameraAngleX += angular_vel_x;
    cameraAngleZ += angular_vel_z;

}


void manageVelocity() 
{
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

    cameraPosition.x += vel_x; 
	cameraPosition.z += vel_z; 
	cameraPosition.y += vel_y; 
	lookAtPosition.x += vel_x;
	lookAtPosition.z += vel_z;
	lookAtPosition.y += vel_y;  
}


void moveRight(float *vel_x, float *vel_z, float fraction) 
{    
    *vel_x -= VELOCITY_AMPLIFIER * ACCELERATION_HORIZONTAL * sin(cameraAngleX) * fraction;
    *vel_z += VELOCITY_AMPLIFIER * ACCELERATION_HORIZONTAL * cos(cameraAngleX) * fraction;
}

void moveLeft(float *vel_x, float *vel_z, float fraction) 
{
    *vel_x += VELOCITY_AMPLIFIER * ACCELERATION_HORIZONTAL * sin(cameraAngleX) * fraction;
    *vel_z -= VELOCITY_AMPLIFIER * ACCELERATION_HORIZONTAL * cos(cameraAngleX) * fraction;
}

void moveForward(float *vel_x, float *vel_z, float fraction) 
{
    *vel_x += VELOCITY_AMPLIFIER * ACCELERATION_HORIZONTAL * cos(cameraAngleX) * fraction;
    *vel_z += VELOCITY_AMPLIFIER * ACCELERATION_HORIZONTAL * sin(cameraAngleX) * fraction;
}

void moveBackward(float *vel_x, float *vel_z, float fraction) 
{
    *vel_x -= VELOCITY_AMPLIFIER * ACCELERATION_HORIZONTAL * cos(cameraAngleX) * fraction;
    *vel_z -= VELOCITY_AMPLIFIER * ACCELERATION_HORIZONTAL * sin(cameraAngleX) * fraction;
}

void printMatrix(float matrix[4][4]) {
    for (int row = 0; row < 4; ++row) {
        for (int col = 0; col < 4; ++col) {
            printf("%f ", matrix[row][col]);
        }
        printf("\n");
    }
}