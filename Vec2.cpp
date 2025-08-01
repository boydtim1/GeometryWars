#include "Vec2.h"
#include <cmath>

Vec2::Vec2(float xin, float yin)
{
    x = xin;
    y = yin;
}

Vec2 Vec2::operator + (const Vec2& other) const
{
    return Vec2(x + other.x, y + other.y);
}

Vec2 Vec2::operator - (const Vec2& other) const
{
    return Vec2(x - other.x, y - other.y);
}

Vec2 Vec2::operator * (const float scalar) const
{
    return Vec2(x * scalar, y * scalar);
}
Vec2 Vec2::operator / (const float scalar) const
{
    return Vec2(x / scalar, y / scalar);
}

void Vec2::operator += (const Vec2& other)
{
    x += other.x;
    y += other.y;
}

void Vec2::operator -= (const Vec2& other)
{
    x -= other.x;
    y -= other.y;
}

void Vec2::operator *= (const float scalar)
{
    x *= scalar;
    y *= scalar;
}
void Vec2::operator /= (const float scalar)
{
    x /= scalar;
    y /= scalar; 
}

float Vec2::distance(const Vec2& other) const
{
    float dx = x - other.x;
    float dy = y - other.y;
    return sqrtf(dx * dx + dy * dy);
}

float Vec2::length() const
{
    return sqrtf(x * x + y * y);
}

Vec2 Vec2::normalized() const
{
    float length = this->length();
    return Vec2(x / length, y/length); //this is a vector with the same direction but length of 1
}
