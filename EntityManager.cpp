#include "EntityManager.h"
#include <iostream>

EntityManager::EntityManager() = default;

std::shared_ptr<Entity> EntityManager::addEntity(const std::string& tag)
{
    auto entity = std::shared_ptr<Entity>(new Entity(tag, ++m_totalEntities));
    m_toAdd.push_back(entity);
    return entity;
}

void EntityManager::update()
{
    for (auto& entity : m_toAdd)
    {
        m_entities.push_back(entity);
        m_entityMap[entity->getTag()].push_back(entity);
    }
    m_toAdd.clear();
    
    // Remove dead entities from m_entities
    m_entities.erase(
        std::remove_if(m_entities.begin(), m_entities.end(), 
            [](const std::shared_ptr<Entity>& entity) { return !entity->isAlive(); }),
        m_entities.end()
    );
    
    // Remove dead entities from m_entityMap
    for (auto& [tag, entityVec] : m_entityMap)
    {
        entityVec.erase(
            std::remove_if(entityVec.begin(), entityVec.end(),
                [](const std::shared_ptr<Entity>& entity) { return !entity->isAlive(); }),
            entityVec.end()
        );
    }
}

EntityVec& EntityManager::getEntities()
{
    return m_entities;

}

EntityVec& EntityManager::getEntities(const std::string& tag)
{
    return m_entityMap[tag];
}