#include "terrain_controller_shared.h"

static int collided = 0;
static float playerX = 0.0;
static float playerY = 0.0;
static float playerZ = 0.0;
static float terrainHeight = 0.0;

void setPlayerPos(float x, float y, float z) {
    playerX = x;
    playerY = y;
    playerZ = z;
}

void getPlayerPos(float* x, float* y, float* z) {
    *x = playerX;
    *y = playerY;
    *z = playerZ;
}

void setCollision(int hasCollided) {
    collided = hasCollided;
}

int getCollision() {
    return collided;
}

void setTerrainHeight(float height) {
    terrainHeight = height;
}

float getTerrainHeight() {
    return terrainHeight;
}