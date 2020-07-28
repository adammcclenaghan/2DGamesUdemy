#ifndef KEYBOARDCONTROLCOMPONENT_H
#define KEYBOARDCONTROLCOMPONENT_H

#include "../Game.h"
#include "../EntityManager.h"
#include "../Components/TransformComponent.h"
#include "../Components/SpriteComponent.h"

class KeyboardControlComponent: public Component {
public:
  std::string upKey;
  std::string downKey;
  std::string leftKey;
  std::string rightKey;
  std::string shootKey;
  TransformComponent *transform;
  SpriteComponent *sprite;

  KeyboardControlComponent() {
  }

  KeyboardControlComponent(std::string upKey, std::string rightKey, std::string downKey, std::string leftKey, std::string shootKey) {
    this->upKey = GetSDLKeyStringCode(upKey);
    this->downKey = GetSDLKeyStringCode(downKey);
    this->leftKey = GetSDLKeyStringCode(leftKey);
    this->rightKey = GetSDLKeyStringCode(rightKey);
    this->shootKey = GetSDLKeyStringCode(shootKey);
  }

  std::string GetSDLKeyStringCode(std::string key) {
    //TODO: Create a map for this 
    if (key.compare("up") == 0) return "1073741906";
    if (key.compare("down") == 0) return "1073741905";
    if (key.compare("left") == 0) return "1073741904";
    if (key.compare("right") == 0) return "1073741903";
    if (key.compare("space") == 0) return "32";

    // If it is a letter character we just need the ascii code
    return std::to_string(static_cast<int>(key[0]));
  }

  void Initialize() override {
    transform = owner->GetComponent<TransformComponent>();
    sprite = owner->GetComponent<SpriteComponent>();
  }

  void Update(float deltaTime) override {

    //TODO: Probably want to include some form of speed multiplier to multiply to the velocity changes rather than hardcoding it

    int speedScale = 50;

    //TODO: Debug why some keys don't seem to be working correctly
    if (Game::event.type == SDL_KEYDOWN) {
      std::string keyCode = std::to_string(Game::event.key.keysym.sym);
      if (keyCode.compare(upKey) == 0) {
	//Velocity y gets negative
	transform->velocity.y = -1 * speedScale;
	transform->velocity.x = 0;
	sprite->Play("UpAnimation");
      }
      else if (keyCode.compare(rightKey) == 0) {
	transform->velocity.y = 0;
	transform->velocity.x = 1 * speedScale;
	sprite->Play("RightAnimation");
      }
      else if (keyCode.compare(downKey) == 0) {
      	transform->velocity.y = 1 * speedScale;
	transform->velocity.x = 0;
	sprite->Play("DownAnimation");

      }
      else if (keyCode.compare(leftKey) == 0) {
       	transform->velocity.y = 0;
	transform->velocity.x = -1 * speedScale;
	sprite->Play("LeftAnimation");

      }
      else if (keyCode.compare(shootKey) == 0) {
	//TODO: Shoot projectiles when shoot key is pressed
      }
    }

    //TODO: Separate keyup/keydown into separate methods for readability

    if (Game::event.type == SDL_KEYUP) {
      std::string keyCode = std::to_string(Game::event.key.keysym.sym);
      if (keyCode.compare(upKey) == 0) {
	//TODO
	transform->velocity.y = 0;
      }
      else if (keyCode.compare(rightKey) == 0) {
	transform->velocity.x = 0;
      }
      else if (keyCode.compare(downKey) == 0) {
	transform->velocity.y = 0;
      }
      else if (keyCode.compare(leftKey) == 0) {
	transform->velocity.x = 0;
      }
      else if (keyCode.compare(shootKey) == 0) {
	//TODO:
      }
    }
  }
private:
  
  
};

#endif
