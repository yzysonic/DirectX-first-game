#pragma once

class Vector2
{
public:
	static const Vector2 zero;
	static const Vector2 one;

	static Vector2 Lerp(Vector2 const& a, Vector2 const& b, float t);
	static float Dot(Vector2 const& a, Vector2 const& b);
	static float Cross(Vector2 const& a, Vector2 const& b);
public:
	float x;
	float y;

	Vector2& operator += (const Vector2 v);
	Vector2& operator -= (const Vector2 v);
	Vector2& operator *= (const float f);
	Vector2& operator /= (const float f);

	Vector2 operator + (const Vector2 v) const;
	Vector2 operator - (const Vector2 v) const;
	Vector2 operator * (const float f) const;
	Vector2 operator / (const float f) const;

	friend Vector2 operator * (float f, const class Vector2& v);

	bool operator == (const Vector2 v) const;
	bool operator != (const Vector2 v) const;

	Vector2();
	Vector2(const Vector2&);
	Vector2(float x, float y);

	float length(void);
	float sqrLength(void);
	Vector2 normalized(void);
};

class Vector3
{
public:
	static const Vector3 zero;
	static const Vector3 one;

	static Vector3 Lerp(Vector3 const& a, Vector3 const& b, float t);
	static float Dot(Vector3 const& a, Vector3 const& b);
	static Vector3 Cross(Vector3 const& a, Vector3 const& b);
public:
	float x;
	float y;
	float z;

	Vector3& operator += (const Vector3 v);
	Vector3& operator -= (const Vector3 v);
	Vector3& operator *= (const float f);
	Vector3& operator /= (const float f);

	Vector3 operator + (const Vector3 v) const;
	Vector3 operator - (const Vector3 v) const;
	Vector3 operator * (const float f) const;
	Vector3 operator / (const float f) const;

	friend Vector3 operator * (float f, const class Vector3& v);

	bool operator == (const Vector3 v) const;
	bool operator != (const Vector3 v) const;

	Vector3();
	Vector3(const Vector3&);
	Vector3(float x, float y, float z);

	float length(void) const;
	float sqrLength(void) const;
	Vector3 normalized(void) const;
	Vector2 toVector2(void) const;
};