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

/*
  Returns an empty string if there are no collisions

  TODO: Should this return a reference to the other entity that it collides with
  instead of a string? This will allow us to perform actions on the entity that
  we collide with 
 */
std::string EntityManager::CheckEntityCollisions(Entity& myEntity) const {
  // Loop over all entities and see if any are colliding with the param entity
  //TODO: Put some protection in here to check that this component type exists for entity
  ColliderComponent* myCollider = myEntity.GetComponent<ColliderComponent>();
  for (auto& entity: entities) {
    //TODO: Probably not safe to rely on the name, can do it in terms of a better equality check later
    // Bypassing the tiles to improve performance!!
    if (entity->name.compare(myEntity.name) != 0 && entity->name.compare("Tile") !=0) {
      
      if (entity->HasComponent<ColliderComponent>()) {
	ColliderComponent* otherCollider = entity->GetComponent<ColliderComponent>();
	// Don't test the entity with itself
	if (Collision::CheckRectangleCollision(myCollider->collider, otherCollider->collider)) {
	  return otherCollider->colliderTag;
	}
      }
    }
  }

  return std::string();
  
}
