#pragma once

#include "Vec2.h"
#include <SFML/Graphics.hpp>

class CTransform 
{
    public:
        Vec2 pos = {0.0, 0.0};
        Vec2 vel = {0.0, 0.0};
        sf::Angle angle = sf::degrees(0.0);
        CTransform(const Vec2& p, const Vec2& v, sf::Angle a)
            : pos(p), vel(v), angle(a) {}
            

};

class CShape
{
    public:
        sf::CircleShape circle;
        CShape(float radius, int vertices, const sf::Color& fill, const sf::Color& outline, float outlineThickness)
            : circle(radius, vertices)
        {
            circle.setFillColor(fill);
            circle.setOutlineColor(outline);
            circle.setOutlineThickness(outlineThickness);
            circle.setOrigin({radius,radius});
        }
};

class CCollision
{
    public:
        float radius = 0.0; //radius of collision box/circle otherwise known as the hitbox
        CCollision(float r)
            : radius(r) {}
};

class CScore
{
    public:
        int score = 0;
        CScore(int s)
            : score(s) {}
};

class CLifeSpan
{
    public:
        int remaining = 0; //remaining lifespan in frames
        int total = 0; //total lifespan in frames
        CLifeSpan(int l)
            : remaining(l), total(l) {}
};

class CInput
{
    public:
        bool up = false;
        bool down = false;
        bool left = false;
        bool right = false;
        bool shoot = false;
        bool special = false;
        CInput(bool u, bool d, bool l, bool r, bool s, bool sp)
            : up(u), down(d), left(l), right(r), shoot(s), special(sp) {}
        CInput() = default;
};

class CSpecialAbility
{
    public:
        int cooldown = 0; //cooldown in frames
};