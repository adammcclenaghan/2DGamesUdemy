#ifndef CONSTANTS_H
#define CONSTANTS_H

const unsigned int WINDOW_WIDTH = 800;
const unsigned int WINDOW_HEIGHT = 600;

const unsigned int FPS = 60;
const unsigned int FRAME_TARGET_TIME_MS = 1000 / FPS;

const float MAX_DELTA_TIME = 0.05f;

enum CollisionType {
  NO_COLLISION,
  PLAYER_ENEMY_COLLISION,
  PLAYER_PROJECTILE_COLLISION,
  ENEMY_PROJECTILE_COLLISION,
  PLAYER_VEGETATION_COLLISION,
  PLAYER_LEVEL_COMPLETE_COLLISION
};

/*
If you update this, make sure to update NUM_LAYERS!!!
 */
enum LayerType {
  TILEMAP_LAYER = 0,
  VEGETATION_LAYER = 1,
  ENEMY_LAYER = 2,
  PLAYER_LAYER = 3,
  OBSTACLE_LAYER = 4,
  PROJECTILES = 5,
  UI_LAYER = 6,
};
// Use this to help us enumere over LayerType
const unsigned int NUM_LAYERS = 7;

#endif
