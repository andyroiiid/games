//
// Created by Andrew Huang on 7/9/2022.
//

#ifndef GAMES_VECTOR_H
#define GAMES_VECTOR_H

#include <cmath>

struct Vec2;
struct Vec3;
struct Vec4;

struct IntVec2;


// Vec2
struct Vec2 {
    float x = 0.0f;
    float y = 0.0f;

    // constructors

    constexpr Vec2() = default;

    constexpr explicit Vec2(const float v) : x(v), y(v) {}

    constexpr Vec2(const float x, const float y) : x(x), y(y) {}

    // constants

    static constexpr Vec2 Zero() { return {0.0f, 0.0f}; }

    static constexpr Vec2 One() { return {1.0f, 1.0f}; }

    static constexpr Vec2 UnitX() { return {1.0f, 0.0f}; }

    static constexpr Vec2 UnitY() { return {0.0f, 1.0f}; }

    // casts

    constexpr explicit operator Vec3() const;

    constexpr explicit operator Vec4() const;

    // operators

    constexpr bool operator==(const Vec2 &v) const { return x == v.x && y == v.y; }

    constexpr bool operator!=(const Vec2 &v) const { return x != v.x || y != v.y; }

    constexpr Vec2 operator+() const { return *this; }

    constexpr Vec2 operator-() const { return {-x, -y}; }

    constexpr Vec2 operator+(const Vec2 &v) const { return {x + v.x, y + v.y}; }

    constexpr Vec2 operator-(const Vec2 &v) const { return {x - v.x, y - v.y}; }

    constexpr Vec2 operator*(const float f) const { return {x * f, y * f}; }

    constexpr Vec2 operator/(const float f) const { return {x / f, y / f}; }

    // vector math

    [[nodiscard]] constexpr float Dot(const Vec2 &v) const { return x * v.x + y * v.y; }

    [[nodiscard]] constexpr float LengthSquared() const { return Dot(*this); }

    [[nodiscard]] inline float Length() const { return std::sqrt(LengthSquared()); }

    [[nodiscard]] inline Vec2 Normalize() const { return *this / Length(); }
};

static constexpr Vec2 operator*(const float f, const Vec2 &v) { return v * f; }

static_assert(sizeof(Vec2) == 8);


// Vec3
struct Vec3 {
    float x = 0.0f;
    float y = 0.0f;
    float z = 0.0f;

    // constructors

    constexpr Vec3() = default;

    constexpr explicit Vec3(const float v) : x(v), y(v), z(v) {}

    constexpr Vec3(const float x, const float y, const float z) : x(x), y(y), z(z) {}

    // constants

    static constexpr Vec3 Zero() { return {0.0f, 0.0f, 0.0f}; }

    static constexpr Vec3 One() { return {1.0f, 1.0f, 1.0f}; }

    static constexpr Vec3 UnitX() { return {1.0f, 0.0f, 0.0f}; }

    static constexpr Vec3 UnitY() { return {0.0f, 1.0f, 0.0f}; }

    static constexpr Vec3 UnitZ() { return {0.0f, 0.0f, 1.0f}; }

    // casts

    constexpr explicit operator Vec2() const;

    constexpr explicit operator Vec4() const;

    // operators

    constexpr bool operator==(const Vec3 &v) const { return x == v.x && y == v.y && z == v.z; }

    constexpr bool operator!=(const Vec3 &v) const { return x != v.x || y != v.y || z != v.z; }

    constexpr Vec3 operator+() const { return *this; }

    constexpr Vec3 operator-() const { return {-x, -y, -z}; }

    constexpr Vec3 operator+(const Vec3 &v) const { return {x + v.x, y + v.y, z + v.z}; }

    constexpr Vec3 operator-(const Vec3 &v) const { return {x - v.x, y - v.y, z - v.z}; }

    constexpr Vec3 operator*(const float f) const { return {x * f, y * f, z * f}; }

    constexpr Vec3 operator/(const float f) const { return {x / f, y / f, z / f}; }

    // vector math

    [[nodiscard]] constexpr float Dot(const Vec3 &v) const { return x * v.x + y * v.y + z * v.z; }

    [[nodiscard]] constexpr float LengthSquared() const { return Dot(*this); }

    [[nodiscard]] inline float Length() const { return std::sqrt(LengthSquared()); }

    [[nodiscard]] inline Vec3 Normalize() const { return *this / Length(); }
};

static constexpr Vec3 operator*(const float f, const Vec3 &v) { return v * f; }

static_assert(sizeof(Vec3) == 12);


// Vec4
struct Vec4 {
    float x = 0.0f;
    float y = 0.0f;
    float z = 0.0f;
    float w = 0.0f;

    // constructors

    constexpr Vec4() = default;

    constexpr explicit Vec4(const float v) : x(v), y(v), z(v), w(v) {}

    constexpr Vec4(const float x, const float y, const float z, const float w) : x(x), y(y), z(z), w(w) {}

    // constants

    static constexpr Vec4 Zero() { return {0.0f, 0.0f, 0.0f, 0.0f}; }

    static constexpr Vec4 One() { return {1.0f, 1.0f, 1.0f, 1.0f}; }

    static constexpr Vec4 UnitX() { return {1.0f, 0.0f, 0.0f, 0.0f}; }

    static constexpr Vec4 UnitY() { return {0.0f, 1.0f, 0.0f, 0.0f}; }

    static constexpr Vec4 UnitZ() { return {0.0f, 0.0f, 1.0f, 0.0f}; }

    static constexpr Vec4 UnitW() { return {0.0f, 0.0f, 0.0f, 1.0f}; }

    // casts

    constexpr explicit operator Vec2() const;

    constexpr explicit operator Vec3() const;

    // operators

    constexpr bool operator==(const Vec4 &v) const { return x == v.x && y == v.y && z == v.z && w == v.w; }

    constexpr bool operator!=(const Vec4 &v) const { return x != v.x || y != v.y || z != v.z || w != v.w; }

    constexpr Vec4 operator+() const { return *this; }

    constexpr Vec4 operator-() const { return {-x, -y, -z, -w}; }

    constexpr Vec4 operator+(const Vec4 &v) const { return {x + v.x, y + v.y, z + v.z, w + v.w}; }

    constexpr Vec4 operator-(const Vec4 &v) const { return {x - v.x, y - v.y, z - v.z, w + v.w}; }

    constexpr Vec4 operator*(const float f) const { return {x * f, y * f, z * f, w * f}; }

    constexpr Vec4 operator/(const float f) const { return {x / f, y / f, z / f, w / f}; }

    // vector math

    [[nodiscard]] constexpr float Dot(const Vec4 &v) const { return x * v.x + y * v.y + z * v.z + w * v.w; }

    [[nodiscard]] constexpr float LengthSquared() const { return Dot(*this); }

    [[nodiscard]] inline float Length() const { return std::sqrt(LengthSquared()); }

    [[nodiscard]] inline Vec4 Normalize() const { return *this / Length(); }
};

static constexpr Vec4 operator*(const float f, const Vec4 &v) { return v * f; }

static_assert(sizeof(Vec4) == 16);


// IntVec2
struct IntVec2 {
    int x = 0;
    int y = 0;

    // constructors

    constexpr IntVec2() = default;

    constexpr explicit IntVec2(const int v) : x(v), y(v) {}

    constexpr IntVec2(const int x, const int y) : x(x), y(y) {}

    // constants

    static constexpr IntVec2 Zero() { return {0, 0}; }

    static constexpr IntVec2 One() { return {1, 1}; }

    static constexpr IntVec2 UnitX() { return {1, 0}; }

    static constexpr IntVec2 UnitY() { return {0, 1}; }
};


// casts

constexpr Vec2::operator Vec3() const { return {x, y, 0}; }

constexpr Vec2::operator Vec4() const { return {x, y, 0, 0}; }

constexpr Vec3::operator Vec2() const { return {x, y}; }

constexpr Vec3::operator Vec4() const { return {x, y, 0, 0}; }

constexpr Vec4::operator Vec2() const { return {x, y}; }

constexpr Vec4::operator Vec3() const { return {x, y, z}; }


// vector math

static inline float Distance(const Vec2 &from, const Vec2 &to) { return (to - from).Length(); }

static inline float Distance(const Vec3 &from, const Vec3 &to) { return (to - from).Length(); }

static inline float Distance(const Vec4 &from, const Vec4 &to) { return (to - from).Length(); }

#endif //GAMES_VECTOR_H
