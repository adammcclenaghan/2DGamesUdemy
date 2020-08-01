#ifndef SPRITECOMPONENT_H
#define SPRITECOMPONENT_H

#include <SDL2/SDL.h>
#include "../TextureManager.h"
#include "../AssetManager.h"
#include "./TransformComponent.h"
#include "../Animation.h"

class SpriteComponent: public Component {
private:
  TransformComponent* transform;
  SDL_Texture* texture;
  SDL_Rect sourceRect;
  SDL_Rect destRect;
  bool isAnimated;
  int numFrames;
  int animationSpeed;
  bool isFixed; // aka doesn't matter what happens it is fixed and always stays in the samr position, useful for things such as labels that are fixed in the screen.
  std::map<std::string, Animation> animations;
  std::string currentAnimationName;
  unsigned int animationIndex = 0; // row on the sprite sheet

public:
  SDL_RendererFlip spriteFlip = SDL_FLIP_NONE;
  // const char* filePath was old arg to this constructor
  SpriteComponent(std::string assetTextureId) {
    isAnimated = false;
    isFixed = false;
    SetTexture(assetTextureId);
  }

  // Animated sprite component constructor
  SpriteComponent(std::string id, int numFrames, int animationSpeed, bool hasDirections, bool isFixed) {
    isAnimated = true;
    this->numFrames = numFrames;
    this->animationSpeed = animationSpeed;
    this->isFixed = isFixed;

    if (hasDirections) {
      Animation downAnimation = Animation(0, numFrames, animationSpeed);
      Animation rightAnimation = Animation(1, numFrames, animationSpeed);
      Animation leftAnimation = Animation(2, numFrames, animationSpeed);
      Animation upAnimation = Animation(3, numFrames, animationSpeed);

      animations.emplace("DownAnimation", downAnimation);
      animations.emplace("RightAnimation", rightAnimation);
      animations.emplace("LeftAnimation", leftAnimation);
      animations.emplace("UpAnimation", upAnimation);

      // Start everything with the down direction for now
      //TODO : This isn't ideal polish it up later
      this->animationIndex = 0;
      this->currentAnimationName = "DownAnimation";
      
    }
    else {
      Animation singleAnimation = Animation(0, numFrames, animationSpeed);
      animations.emplace("SingleAnimation", singleAnimation);
      this->animationIndex = 0;
      this->currentAnimationName = "SingleAnimation";
    }

    Play(this->currentAnimationName);
    SetTexture(id);
  }

  void Play(std::string animationName) {
    numFrames = animations[animationName].numFrames;
    animationIndex = animations[animationName].index;
    animationSpeed = animations[animationName].animationSpeed;
    currentAnimationName = animationName;
    
  }

  void SetTexture(std::string assetTextureId) {
    texture = Game::assetManager->GetTexture(assetTextureId);
  }

  void Initialize() override {
    // Get the transform component for the entity owner of this sprite
    transform = owner->GetComponent<TransformComponent>();
    sourceRect.x = 0;
    sourceRect.y = 0;
    sourceRect.w = transform->width;
    sourceRect.h = transform->height;
  }

  void Update(float deltaTime) override {
    if (isAnimated) {
      sourceRect.x = (sourceRect.w * static_cast<int>((SDL_GetTicks() / animationSpeed) % numFrames));
    }
    sourceRect.y = animationIndex * transform->height;

    
    destRect.x = static_cast<int>(transform->position.x) - (isFixed ? 0 : Game::camera.x);
    destRect.y = static_cast<int>(transform->position.y) - (isFixed ? 0 : Game::camera.y);
    destRect.w = transform->width * transform->scale;
    destRect.h = transform->height * transform->scale;
  }

  void Render() override {
    TextureManager::Draw(texture, sourceRect, destRect, spriteFlip);
  }
};

#endif
