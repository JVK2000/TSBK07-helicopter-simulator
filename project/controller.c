#include "controller.h"
#include "helicopter_controller_shared.h"
#include "terrain_controller_shared.h"


const float ANGULAR_ACCELERATION_HORIZONTAL = 0.0015f;
const float ANGULAR_ACCELERATION_VERTICAL = 0.0025f;
const float ANGULAR_FRICTION_HORIZONTAL = 0.90f;
const float ANGULAR_FRICTION_VERTICAL = 0.6f;

const float ACCELERATION_HORIZONTAL = 0.1f;
const float ACCELERATION_VERTICAL = 0.02f;
const float FRICTION = 0.9f;
const float MOUSE_MOVE_SPEED = 400;
const float MAX_VELOCITY_HORIZONTAL = 2.0f;
const float MAX_VELOCITY_VERTICAL = 1.0f;
const float VELOCITY_AMPLIFIER = 2.0f;
const float CAMERA_Y_ANGLE_BOUNDARY = (M_PI/2.0) * 0.99 ;
const float START_CAMERA_Y_ANGLE = M_PI/16;
const float START_CAMERA_x_ANGLE = M_PI;
const float START_POS_X = 25;
const float START_POS_Y = 20;
const float START_POS_Z = 25;
const float HELICOPTER_HEIGHT = 15;

bool first_iteration = true;
float init_x = 0;
float init_z = 0;
float cameraAngleY;
float cameraAngleX;
float pos_x = 0;
float pos_z = 0;
vec3 cameraPosition;
vec3 lookAtPosition;
vec3 worldUpVector;
mat4 cameraMatrix;


void controllerInit()
{
    cameraAngleY = START_CAMERA_Y_ANGLE;
    cameraAngleX = START_CAMERA_x_ANGLE;
    cameraPosition = (vec3){START_POS_X, START_POS_Y, 45 + START_POS_Z};
    lookAtPosition = (vec3){START_POS_X, START_POS_Y, START_POS_Z};
    worldUpVector = (vec3){0, 1, 0};
}

void controllerHandler() {
    keyboardMovement();
    handleCollision();
}

void handleCollision() {
    set_player_pos(lookAtPosition.x, lookAtPosition.y, lookAtPosition.z);
    int collision_detected = get_collision();
    printf("\ncollision: %d", collision_detected);
    if (collision_detected) {
        float newHeight = getTerrainHeight() + HELICOPTER_HEIGHT;
        if (cameraPosition.y < newHeight) {
            controllerInit();
        }
    }
}

void keyboardMovement()
{
    manageVelocity();
    manageAngle();

    const float radius = -45.0f;
    vec3 newCamPos = (vec3){cameraPosition.x, cameraPosition.y, cameraPosition.z + radius}; 

    // Calculate the X and Y axis rotation matrices
    float x = cos(cameraAngleY) * sin(cameraAngleX);
    float y = sin(cameraAngleY);
    float z = cos(cameraAngleY) * cos(cameraAngleX);

    // Apply the rotation to the camera position
    newCamPos.x = newCamPos.x + x * radius;
    newCamPos.y = newCamPos.y - y * radius;
    newCamPos.z = newCamPos.z - z * radius;

	cameraMatrix = IdentityMatrix();
    cameraMatrix = Mult(cameraMatrix, lookAtv(newCamPos, lookAtPosition, worldUpVector));

    setViewMatrix(cameraMatrix);
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
        angular_vel_z -= ANGULAR_ACCELERATION_VERTICAL;
    }
    if (rotateDown) {
        angular_vel_z += ANGULAR_ACCELERATION_VERTICAL;
    }
    if (rotateLeft) {
        angular_vel_x -= ANGULAR_ACCELERATION_HORIZONTAL;
    }
    if (rotateRight) {
        angular_vel_x += ANGULAR_ACCELERATION_HORIZONTAL;
    }

    // Apply angular friction
    if (!rotateUp && !rotateDown) {
        angular_vel_z *= ANGULAR_FRICTION_VERTICAL;
    }
    if (!rotateLeft && !rotateRight) {
        angular_vel_x *= ANGULAR_FRICTION_HORIZONTAL;
    }

    // Update camera angles
    cameraAngleX += angular_vel_x;
    cameraAngleY += angular_vel_z;
    if (CAMERA_Y_ANGLE_BOUNDARY < cameraAngleY) {
        cameraAngleY = CAMERA_Y_ANGLE_BOUNDARY;
    } else if (cameraAngleY < -CAMERA_Y_ANGLE_BOUNDARY) {
        cameraAngleY = -CAMERA_Y_ANGLE_BOUNDARY;
    }
    setYAngle(cameraAngleY);
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

    updateTilt(vel_x, vel_y, vel_z);

    cameraPosition.x += vel_x; 
	cameraPosition.z += vel_z; 
	cameraPosition.y += vel_y; 
	lookAtPosition.x += vel_x;
	lookAtPosition.z += vel_z;
	lookAtPosition.y += vel_y;  
}


void updateTilt(float vel_x, float vel_y, float vel_z) {
    float tiltZ = cos(cameraAngleX) * (vel_x / 10) + sin(cameraAngleX) * (vel_z / 10);
    float tiltX = cos(cameraAngleX) * (vel_z / 10) - sin(cameraAngleX) * (vel_x / 10);
    setZTilt(tiltZ);
    setXTilt(tiltX);
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


void printMatrix(float matrix[4][4]) {
    for (int row = 0; row < 4; ++row) {
        for (int col = 0; col < 4; ++col) {
            printf("%f ", matrix[row][col]);
        }
        printf("\n");
    }
}