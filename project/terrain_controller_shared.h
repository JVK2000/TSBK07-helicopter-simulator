#ifndef TERRAIN_CONTROLLER_SHARED_H
#define TERRAIN_CONTROLLER_SHARED_H

void setPlayerPos(float x, float y, float z);
void getPlayerPos(float* x, float* y, float* z);
void setCollision(int hasCollided);
int getCollision();
void setTerrainHeight(float height);
float getTerrainHeight();


#endif /* TERRAIN_CONTROLLER_SHARED_H */