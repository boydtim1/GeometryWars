#include "Game.h"
#include <fstream>
#include <iostream>
#include <cmath>

void Game::init(const std::string& configFile)
{
    srand(static_cast<unsigned int>(time(0)));
    std::ifstream file(configFile);
    if (!file.is_open())
    {
        std::cerr << "Error: Could not open config file: " << configFile << std::endl;
    }
    std::string line;
    int frameRate;
    unsigned int windowWidth;
    unsigned int windowHeight;
    bool fullscreen;
    std::string fontPath;
    int fontSize;
    int fontRed;
    int fontGreen;
    int fontBlue;
    // Read config values
    std::string label;
    file >> label >> windowWidth >> windowHeight >> frameRate >> fullscreen;
    file >> label >> fontPath;
    // Remove quotes from fontPath if present
    if (fontPath.front() == '"' && fontPath.back() == '"') {
        fontPath = fontPath.substr(1, fontPath.length() - 2);
    }
    file >> fontSize >> fontRed >> fontGreen >> fontBlue;
    file >> label >> m_playerConfig.SR >> m_playerConfig.CR >> m_playerConfig.FR >> m_playerConfig.FG >> m_playerConfig.FB >> m_playerConfig.OR >> m_playerConfig.OG >> m_playerConfig.OB >> m_playerConfig.OT >> m_playerConfig.V >> m_playerConfig.S;
    file >> label >> m_enemyConfig.SR >> m_enemyConfig.CR >> m_enemyConfig.OR >> m_enemyConfig.OG >> m_enemyConfig.OB >> m_enemyConfig.OT >> m_enemyConfig.VMIN >> m_enemyConfig.VMAX >> m_enemyConfig.L >> m_enemyConfig.SI >> m_enemyConfig.SMIN >> m_enemyConfig.SMAX;
    file >> label >> m_bulletConfig.SR >> m_bulletConfig.CR >> m_bulletConfig.FR >> m_bulletConfig.FG >> m_bulletConfig.FB >> m_bulletConfig.OR >> m_bulletConfig.OG >> m_bulletConfig.OB >> m_bulletConfig.OT >> m_bulletConfig.V >> m_bulletConfig.L >> m_bulletConfig.S;
    
    if(!m_font.openFromFile(fontPath))
    {
        std::cerr << "Error: Could not load font: " << fontPath << std::endl;
    }
    m_Text = std::make_shared<sf::Text>(m_font);
    m_Text->setFillColor(sf::Color(fontRed, fontGreen, fontBlue));

    m_window.create(sf::VideoMode({windowWidth, windowHeight}), "Geometry Wars");
    m_window.setFramerateLimit(frameRate);

    spawnPlayer();
}

Game::Game(const std::string& configFile)
{
    init(configFile);
}

void Game::spawnPlayer()
{
    //TODO: Read player config from file

    //Add player entity
    auto entity = m_entities.addEntity("player");

    float centerX = m_window.getSize().x / 2;
    float centerY = m_window.getSize().y / 2;
    float speed = 5.0f;
    
    //Add tranform component
    entity->cTransform = std::make_shared<CTransform>(Vec2{centerX, centerY}, Vec2{m_playerConfig.S,m_playerConfig.S}, sf::degrees(0.0));

    //Add shape component
    entity->cShape = std::make_shared<CShape>(32.0f, 8, sf::Color(10,10,10), sf::Color(255, 0,0), 4.0f);

    //Add Input component
    entity->cInput = std::make_shared<CInput>();

    //Add special ability component
    entity->cSpecialAbility = std::make_shared<CSpecialAbility>();

    //This goes slightly against the EntityManager paradigm, but we use the player so much that it makes sense
    m_player = entity;
}

void Game::spawnEnemy()
{
    auto entity = m_entities.addEntity("enemy");

    float xSpawn = m_enemyConfig.SR + static_cast<float>(rand()) / static_cast<float>(RAND_MAX/(m_window.getSize().x - m_enemyConfig.SR)); //don't need to to multiply by 2 because we setOrigin in CShape
    float ySpawn = m_enemyConfig.SR + static_cast<float>(rand()) / static_cast<float>(RAND_MAX/(m_window.getSize().y - m_enemyConfig.SR));

    float speed = m_enemyConfig.SMIN + static_cast<float>(rand()) / static_cast<float>(RAND_MAX) * (m_enemyConfig.SMAX - m_enemyConfig.SMIN);

    // Generate random direction for velocity
    float angle = static_cast<float>(rand()) / static_cast<float>(RAND_MAX) * 2.0f * M_PI;
    float velX = speed * cos(angle);
    float velY = speed * sin(angle);

    entity->cTransform = std::make_shared<CTransform>(Vec2{xSpawn, ySpawn}, Vec2{velX, velY}, sf::degrees(0.0));

    float randR = static_cast<float>(rand()) / static_cast<float>(RAND_MAX) * 255;
    float randG = static_cast<float>(rand()) / static_cast<float>(RAND_MAX) * 255;
    float randB = static_cast<float>(rand()) / static_cast<float>(RAND_MAX) * 255;

    int vertices = m_enemyConfig.VMIN + static_cast<int>(rand()) / static_cast<int>(RAND_MAX/(m_enemyConfig.VMAX - m_enemyConfig.VMIN));

    //Add shape component
    entity->cShape = std::make_shared<CShape>(m_enemyConfig.SR, vertices, sf::Color(randR, randG, randB), sf::Color(m_enemyConfig.OR, m_enemyConfig.OG, m_enemyConfig.OB), m_enemyConfig.OT);

    //Add score component
    entity->cScore = std::make_shared<CScore>(vertices * 100);

    //Add life span component
    entity->cLifeSpan = std::make_shared<CLifeSpan>(m_enemyConfig.L);
}

void Game::spawnBullet(std::shared_ptr<Entity> entity, const Vec2& mousePos)
{
    auto bullet = m_entities.addEntity("bullet");
    //Use player position and direction. Bullet will be spawned at player position and will travel in the direction of the vector at given speed

    float speed = m_bulletConfig.S;

    Vec2 normalizedDirection = (mousePos - m_player->cTransform->pos).normalized();

    bullet->cTransform = std::make_shared<CTransform>(m_player->cTransform->pos, normalizedDirection * speed, sf::degrees(0.0));

    bullet->cShape = std::make_shared<CShape>(m_bulletConfig.SR, m_bulletConfig.V, sf::Color(m_bulletConfig.FR, m_bulletConfig.FG, m_bulletConfig.FB), sf::Color(m_bulletConfig.OR, m_bulletConfig.OG, m_bulletConfig.OB), m_bulletConfig.OT);

    bullet->cLifeSpan = std::make_shared<CLifeSpan>(m_bulletConfig.L);


    
}


void Game::sLifeSpan()
{
    for(auto entity : m_entities.getEntities())
    {
        if(entity->cLifeSpan && entity->cTransform && entity->getTag() != "enemy")
        {
            entity->cLifeSpan->remaining--;
            if(entity->cLifeSpan->remaining <= 0)
            {
                entity->destroy();
            }
        }
    }
}

void Game::sRender()
{
    m_window.clear();
    
    // Check if player exists and has required components
    if (m_player && m_player->cShape && m_player->cTransform)
    {
        m_player->cShape->circle.setPosition({m_player->cTransform->pos.x, m_player->cTransform->pos.y});

        m_player->cTransform->angle += sf::degrees(1.0f);
        m_player->cShape->circle.setRotation(m_player->cTransform->angle);

        m_window.draw(m_player->cShape->circle);
    }

    for(auto entity : m_entities.getEntities())
    {
        if(entity->cShape && entity->cTransform && entity->getTag() != "player")
        {
            entity->cShape->circle.setPosition({entity->cTransform->pos.x, entity->cTransform->pos.y});
            entity->cTransform->angle += sf::degrees(1.0f);
            entity->cShape->circle.setRotation(m_player->cTransform->angle);
            if(entity->cLifeSpan && entity->cLifeSpan->remaining > 0 && entity->getTag() != "player" && entity->getTag() != "enemy")
            {
                sf::Color currentColor = entity->cShape->circle.getFillColor();
                sf::Color currentOutlineColor = entity->cShape->circle.getOutlineColor();
                float alphaRatio = static_cast<float>(entity->cLifeSpan->remaining) / static_cast<float>(entity->cLifeSpan->total);
                unsigned int newAlpha = static_cast<unsigned int>(255 * alphaRatio);
                sf::Color newColor(currentColor.r, currentColor.g, currentColor.b, newAlpha);
                sf::Color newOutlineColor(currentOutlineColor.r, currentOutlineColor.g, currentOutlineColor.b, newAlpha);
                entity->cShape->circle.setFillColor(newColor);
                entity->cShape->circle.setOutlineColor(newOutlineColor);
            }
            m_window.draw(entity->cShape->circle);
        }
    }

    m_Text->setString("Score: " + std::to_string(m_score));
    m_Text->setPosition({10, 10});
    m_window.draw(*m_Text);

    m_window.display();
}


void Game::spawnSpecialAbility(std::shared_ptr<Entity> entity) //Adds bullets in a circle around the player
{
    if(entity->cSpecialAbility->cooldown == 0)
    {
        int numBullets = 8; // Fixed number of bullets instead of 360/vertices
        for (int i = 0; i < numBullets; i++)
        {
            auto bullet = m_entities.addEntity("specialBullet"); // Different tag for special bullets
            float angleRad = (2.0f * M_PI * i) / numBullets; // Convert to radians
            float x = entity->cTransform->pos.x + entity->cShape->circle.getRadius() * cos(angleRad);
            float y = entity->cTransform->pos.y + entity->cShape->circle.getRadius() * sin(angleRad);
            bullet->cTransform = std::make_shared<CTransform>(Vec2{x, y}, Vec2{0, 0}, sf::degrees(0.0));
            bullet->cShape = std::make_shared<CShape>(entity->cShape->circle.getRadius() * 2, entity->cShape->circle.getPointCount(), entity->cShape->circle.getFillColor(), entity->cShape->circle.getOutlineColor(), entity->cShape->circle.getOutlineThickness());
            bullet->cLifeSpan = std::make_shared<CLifeSpan>(60);
        }
        entity->cSpecialAbility->cooldown = 60;
    }
}

void Game::sUserInput()
{

    while(auto event = m_window.pollEvent())
    {
        if(event->is<sf::Event::Closed>())
        {
            m_running = false;
        }
        if (event->is<sf::Event::KeyPressed>())
        {
            const auto* keyPressed =event->getIf<sf::Event::KeyPressed>();
            switch(keyPressed->scancode)
            {
                case sf::Keyboard::Scan::Left:
                    m_player->cInput->left = true;
                    break;
                case sf::Keyboard::Scan::A:
                    m_player->cInput->left = true;
                    break;
                case sf::Keyboard::Scan::Right:
                    m_player->cInput->right = true;
                    break;
                case sf::Keyboard::Scan::D:
                    m_player->cInput->right = true;
                    break;
                case sf::Keyboard::Scan::Up:
                    m_player->cInput->up = true;
                    break;
                case sf::Keyboard::Scan::W:
                    m_player->cInput->up = true;
                    break;
                case sf::Keyboard::Scan::Down:
                    m_player->cInput->down = true;
                    break;
                case sf::Keyboard::Scan::S:
                    m_player->cInput->down = true;
                    break;
                case sf::Keyboard::Scan::Space:
                    m_player->cInput->special = true;
                    break;
                case sf::Keyboard::Scan::P:
                    m_paused = !m_paused;
                    break;
                default:
                    break;
            }
        }
        if (event->is<sf::Event::KeyReleased>())
        {
            const auto* keyReleased = event->getIf<sf::Event::KeyReleased>();
            switch(keyReleased->scancode)
            {
                case sf::Keyboard::Scan::Left:
                    m_player->cInput->left = false;
                    break;
                case sf::Keyboard::Scan::A:
                    m_player->cInput->left = false;
                    break;
                case sf::Keyboard::Scan::Right:
                    m_player->cInput->right = false;
                    break;
                case sf::Keyboard::Scan::D:
                    m_player->cInput->right = false;
                    break;
                case sf::Keyboard::Scan::Up:
                    m_player->cInput->up = false;
                    break;
                case sf::Keyboard::Scan::W: 
                    m_player->cInput->up = false;
                    break;
                case sf::Keyboard::Scan::Down:
                    m_player->cInput->down = false;
                    break;
                case sf::Keyboard::Scan::S:
                    m_player->cInput->down = false;
                    break;
                case sf::Keyboard::Scan::Space:
                    m_player->cInput->special = false;
                    break;
                default:
                    break;  
            }
        }
        if(event->is<sf::Event::MouseButtonPressed>())
        {
            const auto* mousePressed = event->getIf<sf::Event::MouseButtonPressed>();
            if(mousePressed->button == sf::Mouse::Button::Left)
            {
                m_player->cInput->shoot = true;
            }
        }
        if(event->is<sf::Event::MouseButtonReleased>())
        {
            const auto* mouseReleased = event->getIf<sf::Event::MouseButtonReleased>();
            if(mouseReleased->button == sf::Mouse::Button::Left)
            {
                m_player->cInput->shoot = false;
            }
        }
    }
}

void Game::sMovement()
{
    m_player->cTransform->vel = Vec2{0.0f, 0.0f};
    if(m_player->cInput->left && m_player->cTransform->pos.x - m_playerConfig.S - m_playerConfig.SR > 0)
    {
        m_player->cTransform->vel.x -= m_playerConfig.S;
    }
    if(m_player->cInput->right && m_player->cTransform->pos.x + m_playerConfig.S + m_playerConfig.SR < m_window.getSize().x)
    {
        m_player->cTransform->vel.x += m_playerConfig.S;
    }
    if(m_player->cInput->up && m_player->cTransform->pos.y - m_playerConfig.S - m_playerConfig.SR > 0)
    {
        m_player->cTransform->vel.y -= m_playerConfig.S;
    }
    if(m_player->cInput->down && m_player->cTransform->pos.y + m_playerConfig.S + m_playerConfig.SR < m_window.getSize().y)
    {
        m_player->cTransform->vel.y += m_playerConfig.S;
    }
    m_player->cTransform->pos += m_player->cTransform->vel;

    for(auto entity : m_entities.getEntities())
    {
        if(entity->cTransform && entity->cShape && (entity->getTag() == "enemy" || entity->getTag() == "smallEnemy"))
        {
            if(entity->cTransform->pos.x + entity->cTransform->vel.x - entity->cShape->circle.getRadius() > 0 && entity->cTransform->pos.x + entity->cTransform->vel.x + entity->cShape->circle.getRadius() < m_window.getSize().x && entity->cTransform->pos.y + entity->cTransform->vel.y - entity->cShape->circle.getRadius() > 0 && entity->cTransform->pos.y + entity->cTransform->vel.y + entity->cShape->circle.getRadius() < m_window.getSize().y)
            {
                entity->cTransform->pos += entity->cTransform->vel;
            }
            else
            {
                entity->cTransform->vel.x *= -1;
                entity->cTransform->vel.y *= -1;
                entity->cTransform->pos += entity->cTransform->vel;
            }
        }
    }
    for(auto entity : m_entities.getEntities())
    {
        if(entity->cTransform && entity->cShape && entity->getTag() == "bullet")
        {
            if(entity->cTransform->pos.x + entity->cTransform->vel.x - entity->cShape->circle.getRadius() > 0 && entity->cTransform->pos.x + entity->cTransform->vel.x + entity->cShape->circle.getRadius() < m_window.getSize().x && entity->cTransform->pos.y + entity->cTransform->vel.y - entity->cShape->circle.getRadius() > 0 && entity->cTransform->pos.y + entity->cTransform->vel.y + entity->cShape->circle.getRadius() < m_window.getSize().y)
            entity->cTransform->pos += entity->cTransform->vel;
        }
    }
}

void Game::sCollision()
{
    for(auto entity : m_entities.getEntities())
    {
        if(entity->cTransform && entity->cShape && entity->getTag() == "bullet")
        {
            for(auto otherEntity : m_entities.getEntities("enemy"))
            {
                float distance = entity->cTransform->pos.distance(otherEntity->cTransform->pos);
                if(distance < entity->cShape->circle.getRadius() + otherEntity->cShape->circle.getRadius())
                {
                    m_score += otherEntity->cScore->score; // Regular enemies give 1 point
                    spawnSmallEnemies(otherEntity);
                    entity->destroy();
                    otherEntity->destroy();
                }
            }
            for(auto otherEntity : m_entities.getEntities("smallEnemy"))
            {
                float distance = entity->cTransform->pos.distance(otherEntity->cTransform->pos);
                if(distance < entity->cShape->circle.getRadius() + otherEntity->cShape->circle.getRadius())
                {
                    m_score += otherEntity->cScore->score; // Small enemies give 2 points
                    entity->destroy();
                    otherEntity->destroy();
                }
            }
        }
        if(entity->cTransform && entity->cShape && entity->getTag() == "player")
        {
            for(auto otherEntity : m_entities.getEntities("enemy"))
            {
                float distance = entity->cTransform->pos.distance(otherEntity->cTransform->pos);
                if(distance < entity->cShape->circle.getRadius() + otherEntity->cShape->circle.getRadius())
                {
                    m_player->cTransform->pos = Vec2{static_cast<float>(m_window.getSize().x / 2), static_cast<float>(m_window.getSize().y / 2)};
                    otherEntity->destroy();
                }
            }
        }
        
        // Handle special bullets - only count as one collision per enemy
        if(entity->cTransform && entity->cShape && entity->getTag() == "specialBullet")
        {
            for(auto otherEntity : m_entities.getEntities("enemy"))
            {
                float distance = entity->cTransform->pos.distance(otherEntity->cTransform->pos);
                if(distance < entity->cShape->circle.getRadius() + otherEntity->cShape->circle.getRadius())
                {
                    m_score += 1; // Only 1 point per enemy, not per bullet
                    spawnSmallEnemies(otherEntity);
                    otherEntity->destroy();
                    // Don't destroy the special bullet - let it continue
                }
            }
            for(auto otherEntity : m_entities.getEntities("smallEnemy"))
            {
                float distance = entity->cTransform->pos.distance(otherEntity->cTransform->pos);
                if(distance < entity->cShape->circle.getRadius() + otherEntity->cShape->circle.getRadius())
                {
                    m_score += 2; // Only 2 points per enemy, not per bullet
                    otherEntity->destroy();
                    // Don't destroy the special bullet - let it continue
                }
            }
        }
    }
}

void Game::spawnSmallEnemies(std::shared_ptr<Entity> enemy)
{
    int numSmallEnemies = enemy->cShape->circle.getPointCount();
    for(int i = 0; i < numSmallEnemies; i++)
    {
        // Convert degrees to radians for cos/sin
        float angleRad = (2.0f * M_PI * i) / numSmallEnemies;
        float x = enemy->cTransform->pos.x + enemy->cShape->circle.getRadius() * cos(angleRad);
        float y = enemy->cTransform->pos.y + enemy->cShape->circle.getRadius() * sin(angleRad);
        
        auto smallEnemy = m_entities.addEntity("smallEnemy");
        smallEnemy->cTransform = std::make_shared<CTransform>(Vec2{x, y}, Vec2{enemy->cTransform->vel.x, enemy->cTransform->vel.y}, sf::degrees(0.0));
        smallEnemy->cShape = std::make_shared<CShape>(enemy->cShape->circle.getRadius() / 2, enemy->cShape->circle.getPointCount(), enemy->cShape->circle.getFillColor(), enemy->cShape->circle.getOutlineColor(), enemy->cShape->circle.getOutlineThickness());
        // Give small enemies a lifespan
        smallEnemy->cScore = std::make_shared<CScore>(enemy->cScore->score * 2);
        smallEnemy->cLifeSpan = std::make_shared<CLifeSpan>(60); // 60 frames = 1 second at 60 FPS
    }
}


void Game::run()
{
    while (m_running)
    {
        if(m_paused)
        {
            sRender();
            sUserInput();
        }
        else
        {

        m_entities.update();

        sUserInput();
        sMovement();
        sLifeSpan();
        sCollision();

        sRender();

        if(m_currentFrame % m_enemyConfig.SI == 0) // 600 frames = 10 seconds later set with config file
        {
            spawnEnemy();
        }
        if(m_player->cInput->shoot)
        {
            if (m_currentFrame - m_lastBulletSpawnTime > 20) //can only shoot every 20 frames
            {
                sf::Vector2i mousePos = sf::Mouse::getPosition(m_window);
                Vec2 mousePosVec = Vec2(static_cast<float>(mousePos.x), static_cast<float>(mousePos.y));
                spawnBullet(m_player, mousePosVec);
                m_lastBulletSpawnTime = m_currentFrame;
            }
        }
        if(m_player->cInput->special)
        {
            spawnSpecialAbility(m_player);
            m_player->cSpecialAbility->cooldown = 60;
        }
        m_currentFrame++;
        if(m_player->cSpecialAbility->cooldown > 0)
        {
            m_player->cSpecialAbility->cooldown--;
        }
        // Small delay to prevent overwhelming the system
        sf::sleep(sf::milliseconds(16)); // ~60 FPS
    }
    
    }
}