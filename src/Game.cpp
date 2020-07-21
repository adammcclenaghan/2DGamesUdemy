#include <iostream>
#include "./Constants.h"
#include "./Game.h"
#include "../lib/glm/glm.hpp"

Game::Game() {
  this->isRunning = false;
}

Game::~Game() {
}

bool Game::IsRunning() const {

  return this->isRunning;
}

glm::vec2 projectilePosition = glm::vec2(0.0f, 0.0f);
glm::vec2 projectileVelocity = glm::vec2(20.0f, 20.0f);

void Game::Initialize(int width, int height)
{
  if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
  {
    std::cerr << "Error initializing SDL." << std::endl;
    return;
  }

  // Create a window
  window = SDL_CreateWindow(NULL,
			    SDL_WINDOWPOS_CENTERED,
			    SDL_WINDOWPOS_CENTERED,
			    width,
			    height,
			    SDL_WINDOW_BORDERLESS);

  if (!window)
  {
    std::cerr << "Error creating SDL window" << std::endl;
    return;
  }

  renderer = SDL_CreateRenderer(window, -1, 0);
  if (!renderer)
  {
    std::cerr << "Error creating SDL renderer." << std::endl;
    return;
  }

  isRunning = true;
  return;
}

void Game::ProcessInput() {
  SDL_Event event;
  SDL_PollEvent(&event);

  switch(event.type) {

  case SDL_QUIT: {
    isRunning = false;
    break;
  }
  case SDL_KEYDOWN : {
    if (event.key.keysym.sym == SDLK_ESCAPE) {
      isRunning = false;
    }
  }
  default: {
    break;
  } 
  }
}

void Game::Update() {
  // Delta time is the difference in ticks from last frame converted to seconds
  unsigned int currentTicks = SDL_GetTicks();
  
  // Wait until our target time has elapsed since the last frame
  int timeToWait = FRAME_TARGET_TIME_MS - (SDL_GetTicks() - ticksLastFrame);
  // Only call delay if we are too fast to process this frame
  if (timeToWait > 0 && timeToWait <= FRAME_TARGET_TIME_MS) {
    SDL_Delay(timeToWait);
  }
  
  while (!SDL_TICKS_PASSED(SDL_GetTicks(), ticksLastFrame + FRAME_TARGET_TIME_MS));
  
  // sets the new ticks for the current frame to be used in the next pass - IN SECONDS
  float deltaTime = (currentTicks - ticksLastFrame) / 1000.0f;

  // Clamp the deltaTime to a maximum value - this is useful for example when
  // we are debugging where the delta time might grow to a very large value when
  // we are sitting at a breakpoint

  deltaTime = (deltaTime > MAX_DELTA_TIME) ? MAX_DELTA_TIME : deltaTime;
  
  ticksLastFrame = currentTicks;

  projectilePosition = glm::vec2(projectilePosition.x + projectileVelocity.x * deltaTime,
				 projectilePosition.y + projectileVelocity.y * deltaTime);
}

void Game::Render() {
  SDL_SetRenderDrawColor(renderer, 21, 21, 21, 255);
  SDL_RenderClear(renderer);

  //struct
  SDL_Rect projectile {
      (int) projectilePosition.x,
      (int) projectilePosition.y,
      10,
      10
      };

  SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
  SDL_RenderFillRect(renderer, &projectile);

  // Swap the buffers
  SDL_RenderPresent(renderer);
}

void Game::Destroy() {
  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  SDL_Quit();
}

