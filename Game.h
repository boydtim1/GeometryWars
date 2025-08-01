#pragma once

#include <SFML/Graphics.hpp>
#include "EntityManager.h"
#include "Entity.h"
#include "Vec2.h"
#include "Components.h"

struct PlayerConfig {int SR, CR, FR, FG, FB, OR, OG, OB, OT, V; float S;}; //SR = Shape Radius, CR = Circle Radius, FR = Fill Red, FG = Fill Green, FB = Fill Blue, OR = Outline Red, OG = Outline Green, OB = Outline Blue, OT = Outline Thickness, V = Vertices, S = Speed
struct EnemyConfig {int SR, CR, OR, OG, OB, OT, VMIN, VMAX, L, SI; float SMIN, SMAX;}; //SR = Shape Radius, CR = Circle Radius, OR = Outline Red, OG = Outline Green, OB = Outline Blue, OT = Outline Thickness, VMIN = Minimum Vertices, VMAX = Maximum Vertices, L = Life Span, SI = Spawn Interval, SMIN = Speed Min, SMAX = Speed Max
struct BulletConfig {int SR, CR, FR, FG, FB, OR, OG, OB, OT, V, L; float S;}; //SR = Shape Radius, CR = Circle Radius, FR = Fill Red, FG = Fill Green, FB = Fill Blue, OR = Outline Red, OG = Outline Green, OB = Outline Blue, OT = Outline Thickness, V = Vertices, L = Life Span, S = Speed

class Game
{
    private:
        sf::RenderWindow m_window;
        EntityManager m_entities; // Might Change to be a pointer to EntityManager
        sf::Font m_font;
        std::shared_ptr<sf::Text> m_Text;
        PlayerConfig m_playerConfig;
        EnemyConfig m_enemyConfig;
        BulletConfig m_bulletConfig;
        int m_score = 0;
        int m_currentFrame = 0;
        int m_lastEnemySpawnTime = 0;
        int m_lastBulletSpawnTime = 0;
        bool m_paused = false;
        bool m_running = true;

        std::shared_ptr<Entity> m_player;

        void init(const std::string& configFile);
        void setPaused(bool paused);

        void sMovement(); //System: Entity position / movement update Done
        void sUserInput(); //System: Player input Done
        void sLifeSpan(); //System: Entity lifespan update Done
        void sRender(); //System: Entity rendering
        void sCollision(); //System: Entity collision update Done

        void spawnPlayer(); //Spawns the player Done
        void spawnEnemy(); //Spawns the enemy Done
        void spawnSmallEnemies(std::shared_ptr<Entity> enemy);
        void spawnBullet(std::shared_ptr<Entity> entity, const Vec2& direction); 
        void spawnSpecialAbility(std::shared_ptr<Entity> entity);
         
    public:
        Game(const std::string& configFile);
        void run();
};