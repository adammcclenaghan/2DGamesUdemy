#ifndef TRANSFORM_COMPONENT_H
#define TRANSFORM_COMPONENT_H

#include "../Game.h"
#include "../EntityManager.h"
#include "../../lib/glm/glm.hpp"
#include <SDL2/SDL.h>

class TransformComponent: public Component {
public:
  glm::vec2 position;
  glm::vec2 velocity;
  int width;
  int height;
  int scale;

  TransformComponent(int posX, int posY, int velX, int velY, int w, int h, int s) {
    position = glm::vec2(posX, posY);
    velocity = glm::vec2(velX, velY);
    width = w;
    height = h;
    scale = s;
  }

  void Initialize() override {
  }

  void Update(float deltaTime) override {
    //TODO: Update the position/velocity as a function of delta time
    position.x += velocity.x * deltaTime;
    position.y += velocity.y * deltaTime;
  }

  void Render() override {
  }
  
private:
};


#endif
