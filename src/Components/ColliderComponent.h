#ifndef COLLIDERCOMPONENT_H
#define COLLIDERCOMPONENT_H

#include <SDL2/SDL.h>
#include "../Game.h"
#include "../EntityManager.h"
#include "./TransformComponent.h"

class ColliderComponent: public Component {
public:
  std::string colliderTag;
  SDL_Rect collider;
  SDL_Rect sourceRectangle;
  SDL_Rect destinationRectangle;
  TransformComponent* transform;

  ColliderComponent(std::string colliderTag, int x, int y, int w, int h) {
    this->colliderTag = colliderTag;
    this->collider = {x, y, w, h}; // struct of type rectangle
  }

  void Initialize() override {
    if (owner->HasComponent<TransformComponent>()) {
      transform = owner->GetComponent<TransformComponent>();
      sourceRectangle = {0, 0, transform->width, transform->height};
      destinationRectangle = {collider.x, collider.y, collider.w, collider.h};
    }
  }

  void Update(float deltaTime) override {
    //TODO: Add protection here for when there is no transform component 
    // Set the collider x and y to follow the owner entity
    collider.x = static_cast<int>(transform->position.x);
    collider.y = static_cast<int>(transform->position.y);
    //Handle any scale changes
    collider.w = transform->width * transform->scale;
    collider.h = transform->height * transform->scale;
    //Collider needs to respond to the camera
    destinationRectangle.x = collider.x - Game::camera.x;
    destinationRectangle.y = collider.y - Game::camera.y;
  }
  
};
#endif
