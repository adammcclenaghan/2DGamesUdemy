#include "./EntityManager.h"
#include "./Collision.h"
#include "./Components/ColliderComponent.h"
#include <iostream>
#include <typeinfo>

void EntityManager::ClearData() {
  for (auto& entity: entities) {
    entity->Destroy();
  }
}

bool EntityManager::HasNoEntities() {
  return entities.size() == 0;
}

void EntityManager::Update(float deltaTime) {
  for (auto& entity: entities) {
    entity->Update(deltaTime);
  }
  DestroyInactiveEntities();
}

void EntityManager::DestroyInactiveEntities() {
  for (int i = 0; i < entities.size(); i++) {
    if (!entities[i]->IsActive()) {
      entities.erase(entities.begin() + i);
    }
  }
}

//TODO: Optimise this later if needed. We may need to optimise the number of loops
void EntityManager::Render() {
  for (int layerNumber = 0; layerNumber < NUM_LAYERS; layerNumber++) {
    for (auto& entity: GetEntitiesByLayer(static_cast<LayerType>(layerNumber))) {
      entity->Render();
      }
  }
}

Entity& EntityManager::AddEntity(std::string entityName, LayerType layer) {
  Entity *entity = new Entity(*this, entityName, layer);
  entities.emplace_back(entity);
  return *entity;
}

std::vector<Entity*> EntityManager::GetEntities() const {
  return entities;
}

std::vector<Entity*> EntityManager::GetEntitiesByLayer(LayerType layer) const {
  std::vector<Entity*> selectedEntities;
  for (auto& entity: entities) {
    if (entity -> layer == layer) {
      selectedEntities.emplace_back(entity);
    }
  }

  return selectedEntities;
}

unsigned int EntityManager::GetEntityCount() {
  return entities.size();
}

void EntityManager::ListAllEntities() const {
  for (auto& entity : entities) {
    std::cout << entity->name << std::endl;;
    entity->ListAllComponents();
  }
}

CollisionType EntityManager::CheckCollisions() const {
  //TODO: This really does not seem great. Improve it.
  // The performance is probably not great, we return on each collision
  // so I don't think that any more than a single collision is processed
  // per call, also I think I'm going to way some reference to the two entities
  // that are colliding so that I can do something with them.
  for (auto& thisEntity: entities) {
    if (thisEntity->HasComponent<ColliderComponent>()) {
      ColliderComponent* thisCollider = thisEntity->GetComponent<ColliderComponent>();
      for (auto& thatEntity: entities) {
	if (thisEntity->name.compare(thatEntity->name) != 0 && thatEntity->HasComponent<ColliderComponent>()) {
	  ColliderComponent* thatCollider = thatEntity->GetComponent<ColliderComponent>();
	  if (Collision::CheckRectangleCollision(thisCollider->collider, thatCollider->collider)) {
	    if (thisCollider->colliderTag.compare("PLAYER") == 0 &&
		thatCollider->colliderTag.compare("ENEMY") == 0) {
	      return PLAYER_ENEMY_COLLISION;
	    }
	    else if (thisCollider->colliderTag.compare("PLAYER") == 0 &&
		     thatCollider->colliderTag.compare("PROJECTILE") ==0) {
	      return PLAYER_PROJECTILE_COLLISION;
	    }
	    else if(thisCollider->colliderTag.compare("ENEMY") == 0 &&
		    thatCollider->colliderTag.compare("FRIENDLY_PROJECTILE") == 0) {
	      return ENEMY_PROJECTILE_COLLISION;
	    }
	    else if (thisCollider->colliderTag.compare("PLAYER") == 0 &&
		     thatCollider->colliderTag.compare("LEVEL_COMPLETE") == 0) {
	      return PLAYER_LEVEL_COMPLETE_COLLISION;
	    }
	  }
	}
      }
    }
  }
  return NO_COLLISION;
}

