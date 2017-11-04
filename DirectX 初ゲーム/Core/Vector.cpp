#include "Vector.h"
#include <math.h>

const Vector2 Vector2::zero(0.0f, 0.0f);
const Vector2 Vector2::one(1.0f, 1.0f);

const Vector3 Vector3::zero(0.0f, 0.0f, 0.0f);
const Vector3 Vector3::one(1.0f, 1.0f, 1.0f);

Vector2 Vector2::Lerp(Vector2 const& a, Vector2 const& b, float t)
{
	if (t >= 1)
		return b;
	else if (t <= 0)
		return a;
	else
		return (a + (b - a)*t);
}

float Vector2::Dot(Vector2 const& a, Vector2 const& b)
{
	return a.x * b.x + a.y * b.y;
}

float Vector2::Cross(Vector2 const & a, Vector2 const & b)
{
	return a.x * b.y - a.y * b.x;
}

Vector2 & Vector2::operator+=(const Vector2 v)
{
	this->x += v.x;
	this->y += v.y;
	return *this;
}

Vector2 & Vector2::operator-=(const Vector2 v)
{
	this->x -= v.x;
	this->y -= v.y;
	return *this;
}

Vector2 & Vector2::operator*=(const float f)
{
	this->x *= f;
	this->y *= f;
	return *this;
}

Vector2 & Vector2::operator/=(const float f)
{
	this->x /= f;
	this->y /= f;
	return *this;
}

Vector2 Vector2::operator+(const Vector2 v) const
{
	return Vector2(this->x + v.x, this->y + v.y);
}

Vector2 Vector2::operator-(const Vector2 v) const
{
	return Vector2(this->x - v.x, this->y - v.y);
}

Vector2 Vector2::operator*(const float f) const
{
	return Vector2(this->x * f, this->y * f);
}

Vector2 Vector2::operator/(const float f) const
{
	return Vector2(this->x / f, this->y / f);
}

bool Vector2::operator==(const Vector2 v) const
{
	return (this->x == v.x) && (this->y == v.y);
}

bool Vector2::operator!=(const Vector2 v) const
{
	return (this->x != v.x) && (this->y != v.y);
}

Vector2::Vector2() : Vector2(Vector2::zero) {}

Vector2::Vector2(const Vector2 & vector2)
{
	*this = vector2;
}

Vector2::Vector2(float x, float y)
{
	this->x = x;
	this->y = y;
}

float Vector2::length(void)
{
	return sqrtf(this->x * this->x + this->y * this->y);
}

float Vector2::sqrLength(void)
{
	return this->x * this->x + this->y * this->y;
}

Vector2 Vector2::normalized(void)
{
	float m = this->length();
	return Vector2(this->x / m, this->y / m);
}


Vector3 Vector3::Lerp(Vector3 const& a, Vector3 const& b, float t)
{
	if (t >= 1)
		return b;
	else if (t <= 0)
		return a;
	else
		return (a + (b - a)*t);
}

float Vector3::Dot(Vector3 const& a, Vector3 const& b)
{
	return a.x * b.x + a.y * b.y + a.z * b.z;
}

Vector3 Vector3::Cross(Vector3 const & a, Vector3 const & b)
{
	return Vector3(a.y * b.z - a.z * b.y, a.z * b.x - a.x * b.z, a.x * b.y - a.y * b.x);
}

Vector3 & Vector3::operator+=(const Vector3 v)
{
	this->x += v.x;
	this->y += v.y;
	this->z += v.z;
	return *this;
}

Vector3 & Vector3::operator-=(const Vector3 v)
{
	this->x -= v.x;
	this->y -= v.y;
	this->z -= v.z;
	return *this;
}

Vector3 & Vector3::operator*=(const float f)
{
	this->x *= f;
	this->y *= f;
	this->z *= f;
	return *this;
}

Vector3 & Vector3::operator/=(const float f)
{
	this->x /= f;
	this->y /= f;
	this->z /= f;
	return *this;
}

Vector3 Vector3::operator+(const Vector3 v) const
{
	return Vector3(this->x + v.x, this->y + v.y, this->z + v.z);
}

Vector3 Vector3::operator-(const Vector3 v) const
{
	return Vector3(this->x - v.x, this->y - v.y, this->z - v.z);
}

Vector3 Vector3::operator*(const float f) const
{
	return Vector3(this->x * f, this->y * f, this->z * f);
}

Vector3 Vector3::operator/(const float f) const
{
	return Vector3(this->x / f, this->y / f, this->z / f);
}

bool Vector3::operator==(const Vector3 v) const
{
	return (this->x == v.x) && (this->y == v.y) && (this->z == v.z);
}

bool Vector3::operator!=(const Vector3 v) const
{
	return (this->x != v.x) && (this->y != v.y) && (this->z != v.z);
}

Vector3::Vector3() : Vector3(Vector3::zero) {}

Vector3::Vector3(const Vector3 & vector3)
{
	*this = vector3;
}

Vector3::Vector3(float x, float y, float z)
{
	this->x = x;
	this->y = y;
	this->z = z;
}

float Vector3::length(void) const
{
	return sqrtf(this->x * this->x + this->y * this->y + this->z * this->z);
}

float Vector3::sqrLength(void) const
{
	return this->x * this->x + this->y * this->y + this->z * this->z;
}

Vector3 Vector3::normalized(void) const
{
	float m = this->length();
	return Vector3(this->x / m, this->y / m, this->z / m);
}

Vector2 Vector3::toVector2(void) const
{
	return Vector2(this->x, this->y);
}

Vector2 operator*(float f, const Vector2 & v)
{
	return v * f;
}

Vector3 operator*(float f, const Vector3 & v)
{
	return v * f;
}
