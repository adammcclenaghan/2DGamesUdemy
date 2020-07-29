#ifndef ENTITY_H
#define ENTITY_H

#include <vector>
#include <string>
#include <map>
#include <typeinfo>
#include "./EntityManager.h"
#include "./Component.h"
#include "./Constants.h"

class EntityManager;
class Component;

class Entity {
private:
  EntityManager& manager;
  bool isActive;
  std::map<const std::type_info*, Component*> componentTypeMap;
  std::vector<Component*> components;
public:
  std::string name;
  LayerType layer;
  Entity(EntityManager& manager);
  Entity(EntityManager& manager, std::string name, LayerType layer);
  void Update(float deltaTime);
  void Render();
  void Destroy();
  bool IsActive() const;
  void ListAllComponents() const;

  template <typename T, typename... TArgs>
  T& AddComponent(TArgs&&... args) {
    //std::forward helps get the type of the components (static_cast)
    T* newComponent(new T(std::forward<TArgs>(args)...));
    newComponent->owner = this;
    components.emplace_back(newComponent);
    componentTypeMap[&typeid(*newComponent)] = newComponent;
    newComponent->Initialize();
    return *newComponent;
  }

  template <typename T>
  T* GetComponent() {
    return static_cast<T*>(componentTypeMap[&typeid(T)]);
  }

  template <typename T>
  bool HasComponent() const {
    //TODO
    //returns true if our entity has a component of type T
    return true;
  }
  
};

#endif
