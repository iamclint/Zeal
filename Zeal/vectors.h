#pragma once
#define _USE_MATH_DEFINES
#include <math.h>
#include <sstream>
#include <iomanip>
#include "json.hpp"
typedef float vec_t;
typedef vec_t vec2_t[2];
typedef vec_t vec3_t[3];
typedef vec_t vec4_t[4];
typedef vec_t vec5_t[5];

struct Vec2;
struct Vec3;
struct Vec4;
struct Vec2
{
public:
	Vec2() { x = 0;  y = 0; };
	Vec2(float vec_x, float vec_y) { x = vec_x; y = vec_y; }
	Vec2(const Vec2& v) { x = v.x; y = v.y; }
	inline Vec2 operator + (const Vec2& v) const { return Vec2(x + v.x, y + v.y); }
	inline Vec2 operator - (const Vec2& v) const { return Vec2(x - v.x, y - v.y); }
	inline Vec2 operator / (const Vec2& v) const { return Vec2(x / v.x, y / v.y); }
	inline Vec2 operator * (const Vec2& v) const { return Vec2(x * v.x, y * v.y); }
	inline bool operator == (const Vec2& v) const { return x == v.x && y == v.y; }
	inline bool operator != (const Vec2& v) const { return x != v.x && y != v.y; }
	inline Vec2& operator += (const Vec2& v) { this->x += v.x; this->y += v.y; return *this; }
	inline Vec2& operator -= (const Vec2& v) { this->x += v.x; this->y += v.y; return *this; }
	inline Vec2& operator *= (const Vec2& v) { this->x *= v.x, this->y *= v.y; return *this; }
	inline Vec2& operator /= (const Vec2& v) { this->x += v.x; this->y += v.y; return *this; }
	inline float& operator [] (int index) { float f = FLT_MIN; if (index == 0) return x;  if (index == 1) return y; return f; }
	const float& operator [] (int index) const { if (index == 0) return x;  if (index == 1) return y; return FLT_MIN; }
	//ImVec2 toImVec2() const { return ImVec2(x, y); }
	inline double Length() { return sqrt(pow(x,2) + pow(y,2)); }
	//static Vec2 toVec2(const ImVec2& v) { return Vec2(v.x, v.y); }
	inline double Dist(const Vec2 v) { return (*this - v).Length(); }
	float x, y;
};


struct Vec3
{
	float x, y, z;
	Vec3() { x = 0; y = 0; z = 0; };
	Vec3(vec3_t vec) { x = vec[0]; y = vec[1]; z = vec[2]; }
	Vec3(float vec_x, float vec_y, float vec_z) { x = vec_x; y = vec_y; z = vec_z; }
	Vec3(const Vec3& v) { x = v.x; y = v.y; z = v.z; }
	inline Vec3 operator + (const Vec3& v) { return Vec3(x + v.x, y + v.y, z + v.z); }
	inline Vec3 operator - (const Vec3& v) { return Vec3(x - v.x, y - v.y, z - v.z); }
	inline Vec3 operator / (const Vec3& v) { return Vec3(x / v.x, y / v.y, z / v.z); }
	inline Vec3 operator * (const Vec3& v) { return Vec3(x * v.x, y * v.y, z * v.z); }
	inline bool operator == (const Vec3& v) { return x == v.x && y == v.y && z == v.z; }
	inline bool operator != (const Vec3& v) { return x != v.x || y != v.y || z != v.z; }
	inline float& operator [] (int index) { if (index == 0) return x;  if (index == 1) return y; if (index == 2) return z; return z; }
	const float& operator [] (int index) const { if (index == 0) return x;  if (index == 1) return y; if (index == 2) return z; return FLT_MIN; }
	inline Vec3& operator += (const Vec3& v) { this->x += v.x; this->y += v.y; this->z += v.z; return *this; }
	inline Vec3& operator -= (const Vec3& v) { this->x -= v.x; this->y -= v.y; this->z -= v.z; return *this; }
	inline Vec3& operator *= (const Vec3& v) { this->x *= v.x; this->y *= v.y; this->z *= v.z; return *this; }
	inline Vec3& operator /= (const Vec3& v) { this->x /= v.x; this->y /= v.y; this->z /= v.z; return *this; }
	inline nlohmann::json toJson() { nlohmann::json data = { {"x", x}, {"y", y}, {"z", z} }; return data; }
	inline std::string toString() { std::stringstream ss; ss << std::fixed << std::setprecision(6) << x << " " << y << " " << z; return ss.str(); }
	inline double LengthSquared() const { return x * x + y * y + z * z;	}
	inline double Length() { return sqrt(pow(x,2) + pow(y,2) + pow(z,2)); }
	inline double Length2D() { return sqrt(pow(x,2) + pow(y,2)); }
	inline double Length2DRounded() { return sqrt(pow(roundf(x), 2) + pow(roundf(y), 2)); }
	inline double LengthZ() { return sqrt(pow(y,2)); }
	inline Vec3 CrossProduct(const Vec3 v) const {	return Vec3(y * v.z - z * v.y, z * v.x - x * v.z, x * v.y - y * v.x); }
	inline float DotProduct(const Vec3 v) { return (x * v.x + y * v.y + z * v.z); }
	inline Vec3 Rounded() { return Vec3(roundf(this->x), roundf(this->y), roundf(this->z)); }
	inline Vec3 Floored() { return Vec3(floorf(this->x), floorf(this->y), floorf(this->z)); }
	inline Vec3 Ceiled() { return Vec3(ceilf(this->x), ceilf(this->y), ceilf(this->z)); }
	inline double Dist(const Vec3 v) { return (*this - v).Length(); }
	inline double Dist2D(const Vec3 v) { return (Vec2(x, y) - Vec2(v.x, v.y)).Length(); }
	inline float* v3t() { float x[3]; x[0] = this->x; x[1] = this->y; x[2] = this->z;  return x; }
	inline Vec3 normalize360() { return Vec3(fmodf(this->x + 360, 360),fmodf(this->y + 360, 360),fmodf(this->z + 360, 360)); }
	inline Vec3 getFractional() { float whole; return { std::modf(x, &whole), std::modf(y, &whole),std::modf(z, &whole) }; }
	inline Vec3 getFractionalAbs() { float whole; return { fabs(std::modf(x, &whole)), fabs(std::modf(y, &whole)), fabs(std::modf(z, &whole)) }; }
//	inline ImColor toImColor(float alpha = 1.0f) { return ImColor(x, y, z, alpha); }
	inline Vec2 toVec2() { return Vec2(x, y); }
};


struct Vec4
{
	float x, y, z, w;
	Vec4() { x = 0; y = 0; z = 0; w = 0; };
	Vec4(vec4_t vec) { x = vec[0]; y = vec[1]; z = vec[2]; w = vec[3]; }
	Vec4(float vec_x, float vec_y, float vec_z, float vec_w) { x = vec_x; y = vec_y; z = vec_z; w = vec_w; }
	Vec4(const Vec4& v) { x = v.x; y = v.y; z = v.z; w = v.w; }
	inline Vec4 operator + (const Vec4& v) { return Vec4(x + v.x, y + v.y, z + v.z, w+v.w); }
	inline Vec4 operator - (const Vec4& v) { return Vec4(x - v.x, y - v.y, z - v.z, w - v.w); }
	inline Vec4 operator / (const Vec4& v) { return Vec4(x / v.x, y / v.y, z / v.z, w / v.w); }
	inline Vec4 operator * (const Vec4& v) { return Vec4(x * v.x, y * v.y, z * v.z, w * v.w); }
	inline bool operator == (const Vec4& v) { return x == v.x && y == v.y && z == v.z && w == v.w;	}
	inline bool operator != (const Vec4& v) { return x != v.x || y != v.y || z != v.z || w != v.w; }
	inline float& operator [] (int index) { if (index == 0) return x;  if (index == 1) return y; if (index == 2) return z; if (index == 3) return w; return w; }
	const float& operator [] (int index) const { if (index == 0) return x;  if (index == 1) return y; if (index == 2) return z; if (index == 3) return w; return FLT_MIN; }
	inline Vec4& operator += (const Vec4& v) { this->x += v.x; this->y += v.y; this->z += v.z; this->w += v.w; return *this; }
	inline Vec4& operator -= (const Vec4& v) { this->x -= v.x; this->y -= v.y; this->z -= v.z; this->w -= v.w; return *this; }
	inline Vec4& operator *= (const Vec4& v) { this->x *= v.x; this->y *= v.y; this->z *= v.z; this->w *= v.w; return *this; }
	inline Vec4& operator /= (const Vec4& v) { this->x /= v.x; this->y /= v.y; this->z /= v.z; this->w /= v.w; return *this; }
	inline float* v4t() { float x[4]; x[0] = this->x; x[1] = this->y; x[2] = this->z; x[3] = this->w; return x; }
	//inline ImVec4 toImVec4() { return { x, y, z, w }; }
//	inline ImColor toImColor() { return { x, y, z, w }; }
};



