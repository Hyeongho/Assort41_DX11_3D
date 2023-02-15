#pragma once

#include "EngineMath.h"

struct Vector4
{
	float	x, y, z, w;

	Vector4();
	Vector4(float _x, float _y, float _z, float _w);
	Vector4(const Vector4& v);
	Vector4(const DirectX::XMVECTOR& v);

	// operator =
	Vector4& operator = (const Vector4& v);
	Vector4& operator = (const DirectX::XMVECTOR& v);
	Vector4& operator = (float f);

	// operator ==, !=
	bool operator == (const Vector4& v)	const;
	bool operator == (const DirectX::XMVECTOR& v)	const;
	bool operator != (const Vector4& v)	const;
	bool operator != (const DirectX::XMVECTOR& v)	const;

	float& operator [](int Index);

	// operator +
	Vector4 operator + (const Vector4& v)	const;
	Vector4 operator + (const DirectX::XMVECTOR& v)	const;
	Vector4 operator + (float f)	const;

	// operator +=
	void operator += (const Vector4& v);
	void operator += (const DirectX::XMVECTOR& v);
	void operator += (float f);


	// operator -
	Vector4 operator - (const Vector4& v)	const;
	Vector4 operator - (const DirectX::XMVECTOR& v)	const;
	Vector4 operator - (float f)	const;

	// operator -=
	void operator -= (const Vector4& v);
	void operator -= (const DirectX::XMVECTOR& v);
	void operator -= (float f);


	// operator *
	Vector4 operator * (const Vector4& v)	const;
	Vector4 operator * (const DirectX::XMVECTOR& v)	const;
	Vector4 operator * (float f)	const;

	// operator *=
	void operator *= (const Vector4& v);
	void operator *= (const DirectX::XMVECTOR& v);
	void operator *= (float f);


	// operator /
	Vector4 operator / (const Vector4& v)	const;
	Vector4 operator / (const DirectX::XMVECTOR& v)	const;
	Vector4 operator / (float f)	const;

	// operator /=
	void operator /= (const Vector4& v);
	void operator /= (const DirectX::XMVECTOR& v);
	void operator /= (float f);

	// operator ++
	const Vector4& operator ++ ();
	const Vector4& operator ++ (int);

	// operator --
	const Vector4& operator -- ();
	const Vector4& operator -- (int);

	DirectX::XMVECTOR Convert()	const;
	void Convert(const DirectX::XMVECTOR& v);

	static Vector4	Black;
	static Vector4	White;
	static Vector4	Red;
	static Vector4	Green;
	static Vector4	Blue;
	static Vector4	Yellow;
};

