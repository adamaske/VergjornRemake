#pragma once
#include <cmath>

struct Vec2 {
    float x = 0.f;
    float y = 0.f;

    float length()  const { return std::sqrt(x*x + y*y); }
    float lengthSq()const { return x*x + y*y; }

    Vec2 normalized() const {
        float l = length();
        return l > 1e-6f ? Vec2{x/l, y/l} : Vec2{0.f, 0.f};
    }

    float distTo(const Vec2& o) const {
        float dx = x - o.x, dy = y - o.y;
        return std::sqrt(dx*dx + dy*dy);
    }

    float distSqTo(const Vec2& o) const {
        float dx = x - o.x, dy = y - o.y;
        return dx*dx + dy*dy;
    }

    Vec2 operator+(const Vec2& o) const { return {x+o.x, y+o.y}; }
    Vec2 operator-(const Vec2& o) const { return {x-o.x, y-o.y}; }
    Vec2 operator*(float s)       const { return {x*s,   y*s};   }
    Vec2& operator+=(const Vec2& o)     { x+=o.x; y+=o.y; return *this; }
};
