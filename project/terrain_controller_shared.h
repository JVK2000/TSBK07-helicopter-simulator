#ifndef TERRAIN_CONTROLLER_SHARED_H
#define TERRAIN_CONTROLLER_SHARED_H

void set_player_pos(float x, float y, float z);
void get_player_pos(float* x, float* y, float* z);
void set_collision(int has_collided);
int get_collision();
void setTerrainHeight(float height);
float getTerrainHeight();


#endif /* TERRAIN_CONTROLLER_SHARED_H */