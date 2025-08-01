#pragma once

#include <string>
#include <memory>
#include "Components.h"

class EntityManager;

class Entity
{
    friend class EntityManager;
    private: 
        const size_t m_id = 0;
        const std::string m_tag = "Default";
        bool m_isAlive = true;
        Entity(const std::string& tag, const size_t id);
    public:
        std::shared_ptr<CTransform>  cTransform;
        std::shared_ptr<CCollision> cCollision;
        std::shared_ptr<CScore> cScore;
        std::shared_ptr<CInput> cInput;
        std::shared_ptr<CShape> cShape;
        std::shared_ptr<CLifeSpan> cLifeSpan;
        std::shared_ptr<CSpecialAbility> cSpecialAbility;

        const std::string& getTag() const {return m_tag;};
        const size_t getId() const {return m_id;}
        bool isAlive() const {return m_isAlive;}
        void destroy() {m_isAlive = false;}

};