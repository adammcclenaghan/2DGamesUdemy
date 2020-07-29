#include "./EntityManager.h"
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
