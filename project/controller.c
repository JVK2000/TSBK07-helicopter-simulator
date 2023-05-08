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

float cameraAngleY;
float cameraAngleX;
float posX = 0;
float posZ = 0;
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


void handleCollision() 
{
    setPlayerPos(lookAtPosition.x, lookAtPosition.y, lookAtPosition.z);
    int collisionDetected = getCollision();
    if (collisionDetected) {
        float newHeight = getTerrainHeight() + HELICOPTER_HEIGHT;
        if (cameraPosition.y < newHeight) {
            controllerInit();
        }
    }
}


void manageAngle() 
{
    static float angularVelX = 0;
    static float angularVelZ = 0;

    bool rotateLeft = glutKeyIsDown(GLUT_KEY_LEFT);
    bool rotateRight = glutKeyIsDown(GLUT_KEY_RIGHT);
    bool rotateDown = glutKeyIsDown(GLUT_KEY_UP);
    bool rotateUp = glutKeyIsDown(GLUT_KEY_DOWN);

    if (rotateUp) {
        angularVelZ -= ANGULAR_ACCELERATION_VERTICAL;
    }
    if (rotateDown) {
        angularVelZ += ANGULAR_ACCELERATION_VERTICAL;
    }
    if (rotateLeft) {
        angularVelX -= ANGULAR_ACCELERATION_HORIZONTAL;
    }
    if (rotateRight) {
        angularVelX += ANGULAR_ACCELERATION_HORIZONTAL;
    }

    // Apply angular friction
    if (!rotateUp && !rotateDown) {
        angularVelZ *= ANGULAR_FRICTION_VERTICAL;
    }
    if (!rotateLeft && !rotateRight) {
        angularVelX *= ANGULAR_FRICTION_HORIZONTAL;
    }

    // Update camera angles
    cameraAngleX += angularVelX;
    cameraAngleY += angularVelZ;
    if (CAMERA_Y_ANGLE_BOUNDARY < cameraAngleY) {
        cameraAngleY = CAMERA_Y_ANGLE_BOUNDARY;
    } else if (cameraAngleY < -CAMERA_Y_ANGLE_BOUNDARY) {
        cameraAngleY = -CAMERA_Y_ANGLE_BOUNDARY;
    }
    setYAngle(cameraAngleY);
}


void updateTilt(float velX, float velY, float velZ) 
{
    float tiltZ = cos(cameraAngleX) * (velX / 10) + sin(cameraAngleX) * (velZ / 10);
    float tiltX = cos(cameraAngleX) * (velZ / 10) - sin(cameraAngleX) * (velX / 10);
    setZTilt(tiltZ);
    setXTilt(tiltX);
}


void moveRight(float *velX, float *velZ, float fraction) 
{    
    *velX += VELOCITY_AMPLIFIER * ACCELERATION_HORIZONTAL * cos(cameraAngleX) * fraction;
    *velZ += VELOCITY_AMPLIFIER * ACCELERATION_HORIZONTAL * sin(cameraAngleX) * fraction;
}


void moveLeft(float *velX, float *velZ, float fraction) 
{
    *velX -= VELOCITY_AMPLIFIER * ACCELERATION_HORIZONTAL * cos(cameraAngleX) * fraction;
    *velZ -= VELOCITY_AMPLIFIER * ACCELERATION_HORIZONTAL * sin(cameraAngleX) * fraction;
}


void moveForward(float *velX, float *velZ, float fraction) 
{
    *velX += VELOCITY_AMPLIFIER * ACCELERATION_HORIZONTAL * sin(cameraAngleX) * fraction;
    *velZ -= VELOCITY_AMPLIFIER * ACCELERATION_HORIZONTAL * cos(cameraAngleX) * fraction;
}


void moveBackward(float *velX, float *velZ, float fraction) 
{
    *velX -= VELOCITY_AMPLIFIER * ACCELERATION_HORIZONTAL * sin(cameraAngleX) * fraction;
    *velZ += VELOCITY_AMPLIFIER * ACCELERATION_HORIZONTAL * cos(cameraAngleX) * fraction;
}


void manageVelocity() 
{
    static float velX = 0;
    static float velZ = 0;
    static float velY = 0;

    bool moveForwardKeyDown = glutKeyIsDown('w');
    bool moveBackwardKeyDown = glutKeyIsDown('s');
    bool moveLeftKeyDown = glutKeyIsDown('a');
    bool moveRightKeyDown = glutKeyIsDown('d');
    bool moveUpKeyDown = glutKeyIsDown('e');
    bool moveDownKeyDown = glutKeyIsDown('q');

    if (moveForwardKeyDown) {
        moveForward(&velX, &velZ, 1);
    }
    if (moveBackwardKeyDown) {
        moveBackward(&velX, &velZ, 1);
    }
    if (moveLeftKeyDown) {
        moveLeft(&velX, &velZ, 1);
    }
    if (moveRightKeyDown) {
        moveRight(&velX, &velZ, 1);
    }

    if (moveDownKeyDown) {
        velY -= ACCELERATION_VERTICAL;
    }
    if (moveUpKeyDown) {
        velY += ACCELERATION_VERTICAL;
    }

    // Apply friction
    if (!moveForwardKeyDown && !moveBackwardKeyDown) {
        velX *= FRICTION;
        velZ *= FRICTION;
    }
    if (!moveLeftKeyDown && !moveRightKeyDown) {
        velX *= FRICTION;
        velZ *= FRICTION;
    }
    if (!moveDownKeyDown && !moveUpKeyDown) {
        velY *= FRICTION;
    }

    // Limit the maximum velocity
    float currentVelocity = fabs(velX) + fabs(velZ);
    if (currentVelocity > MAX_VELOCITY_HORIZONTAL) {
        float ratio = MAX_VELOCITY_HORIZONTAL / currentVelocity;
        velX *= ratio;
        velZ *= ratio;
    }

    if (velY > MAX_VELOCITY_VERTICAL) {
        velY = MAX_VELOCITY_VERTICAL;
    } 
    else if (velY < -MAX_VELOCITY_VERTICAL) {
        velY = -MAX_VELOCITY_VERTICAL;
    } 

    updateTilt(velX, velY, velZ);

    cameraPosition.x += velX; 
	cameraPosition.z += velZ; 
	cameraPosition.y += velY; 
	lookAtPosition.x += velX;
	lookAtPosition.z += velZ;
	lookAtPosition.y += velY;  
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


void controllerHandler() 
{
    keyboardMovement();
    handleCollision();
}