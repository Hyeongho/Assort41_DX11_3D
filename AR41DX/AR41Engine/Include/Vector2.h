#pragma once

#include "EngineMath.h"

struct Vector2
{
	float	x, y;

	Vector2();
	Vector2(float _x, float _y);
	Vector2(const Vector2& v);
	Vector2(const struct Vector3& v);
	Vector2(const DirectX::XMVECTOR& v);

	// operator =
	Vector2& operator = (const Vector2& v);
	Vector2& operator = (const DirectX::XMVECTOR& v);
	Vector2& operator = (float f);
	Vector2& operator = (const struct Vector3& v);

	// operator ==, !=
	bool operator == (const Vector2& v)	const;
	bool operator == (const DirectX::XMVECTOR& v)	const;
	bool operator != (const Vector2& v)	const;
	bool operator != (const DirectX::XMVECTOR& v)	const;

	float& operator [](int Index);

	// operator +
	Vector2 operator + (const Vector2& v)	const;
	Vector2 operator + (const DirectX::XMVECTOR& v)	const;
	Vector2 operator + (float f)	const;

	// operator +=
	void operator += (const Vector2& v);
	void operator += (const DirectX::XMVECTOR& v);
	void operator += (float f);


	// operator -
	Vector2 operator -();
	Vector2 operator - (const Vector2& v)	const;
	Vector2 operator - (const DirectX::XMVECTOR& v)	const;
	Vector2 operator - (float f)	const;

	// operator -=
	void operator -= (const Vector2& v);
	void operator -= (const DirectX::XMVECTOR& v);
	void operator -= (float f);


	// operator *
	Vector2 operator * (const Vector2& v)	const;
	Vector2 operator * (const DirectX::XMVECTOR& v)	const;
	Vector2 operator * (float f)	const;

	// operator *=
	void operator *= (const Vector2& v);
	void operator *= (const DirectX::XMVECTOR& v);
	void operator *= (float f);


	// operator /
	Vector2 operator / (const Vector2& v)	const;
	Vector2 operator / (const DirectX::XMVECTOR& v)	const;
	Vector2 operator / (float f)	const;

	// operator /=
	void operator /= (const Vector2& v);
	void operator /= (const DirectX::XMVECTOR& v);
	void operator /= (float f);

	// operator ++
	const Vector2& operator ++ ();
	const Vector2& operator ++ (int);

	// operator --
	const Vector2& operator -- ();
	const Vector2& operator -- (int);

	bool Iszero();
	float Length()	const;
	void Normalize();
	float Distance(const Vector2& v)	const;
	float Dot(const Vector2& v)	const;
	float Angle(const Vector2& v)	const;



	DirectX::XMVECTOR Convert()	const;
	void Convert(const DirectX::XMVECTOR& v);
};

