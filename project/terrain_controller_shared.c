#include "terrain_controller_shared.h"

static int collided = 0;
static float player_x = 0.0;
static float player_y = 0.0;
static float player_z = 0.0;

void set_player_pos(float x, float y, float z) {
    player_x = x;
    player_y = y;
    player_z = z;
}

void get_player_pos(float* x, float* y, float* z) {
    *x = player_x;
    *y = player_y;
    *z = player_z;
}

void set_collision(int has_collided) {
    collided = has_collided;
}

int get_collision() {
    return collided;
}
