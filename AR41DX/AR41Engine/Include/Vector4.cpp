#include "Vector4.h"

Vector4 Vector4::Black(0.f, 0.f, 0.f, 1.f);
Vector4 Vector4::White(1.f, 1.f, 1.f, 1.f);
Vector4 Vector4::Red(1.f, 0.f, 0.f, 1.f);
Vector4 Vector4::Green(0.f, 1.f, 0.f, 1.f);
Vector4 Vector4::Blue(0.f, 0.f, 1.f, 1.f);

Vector4::Vector4() :
    x(0.f),
    y(0.f),
    z(0.f),
    w(0.f)
{
}

Vector4::Vector4(float _x, float _y, float _z, float _w) :
    x(_x),
    y(_y),
    z(_z),
    w(_w)
{
}

Vector4::Vector4(const Vector4& v) :
    x(v.x),
    y(v.y),
    z(v.z),
    w(v.w)
{
}

Vector4::Vector4(const DirectX::XMVECTOR& v)
{
    // XMVECTOR를 XMFLOAT4로 변환해준다.
    // XMFLOAT4와 이 구조체는 메모리 구조가 같으므로 강제로 형변환해서
    // 변환해주도록 한다.
    DirectX::XMStoreFloat4((DirectX::XMFLOAT4*)this, v);
}

Vector4& Vector4::operator=(const Vector4& v)
{
    x = v.x;
    y = v.y;
    z = v.z;
    w = v.w;

    return *this;
}

Vector4& Vector4::operator=(const DirectX::XMVECTOR& v)
{
    DirectX::XMStoreFloat4((DirectX::XMFLOAT4*)this, v);

    return *this;
}

Vector4& Vector4::operator=(float f)
{
    x = f;
    y = f;
    z = f;
    w = f;

    return *this;
}

bool Vector4::operator==(const Vector4& v) const
{
    return x == v.x && y == v.y && z == v.z && w == v.w;
}

bool Vector4::operator==(const DirectX::XMVECTOR& v) const
{
    Vector4 v1;
    DirectX::XMStoreFloat4((DirectX::XMFLOAT4*)&v1, v);

    return x == v1.x && y == v1.y && z == v1.z && w == v1.w;
}

bool Vector4::operator!=(const Vector4& v) const
{
    return x != v.x || y != v.y || z != v.z || w != v.w;
}

bool Vector4::operator!=(const DirectX::XMVECTOR& v) const
{
    Vector4 v1;
    DirectX::XMStoreFloat4((DirectX::XMFLOAT4*)&v1, v);

    return x != v1.x || y != v1.y || z != v1.z || w != v1.w;
}

float& Vector4::operator[](int Index)
{
    if (Index < 0 || Index > 3)
    {
        assert(false);
    }

    if (Index == 0)
        return x;

    else if (Index == 1)
        return y;

    else if (Index == 2)
        return z;

    return w;
}

// operator +
Vector4 Vector4::operator+(const Vector4& v) const
{
    return Vector4(x + v.x, y + v.y, z + v.z, w + v.w);
}

Vector4 Vector4::operator+(const DirectX::XMVECTOR& v) const
{
    Vector4 v1(v);

    return Vector4(x + v1.x, y + v1.y, z + v1.z, w + v1.w);
}

Vector4 Vector4::operator+(float f) const
{
    return Vector4(x + f, y + f, z + f, w + f);
}

// operator +=
void Vector4::operator+=(const Vector4& v)
{
    x += v.x;
    y += v.y;
    z += v.z;
    w += v.w;
}

void Vector4::operator+=(const DirectX::XMVECTOR& v)
{
    Vector4 v1(v);

    x += v1.x;
    y += v1.y;
    z += v1.z;
    w += v1.w;
}

void Vector4::operator+=(float f)
{
    x += f;
    y += f;
    z += f;
    w += f;
}

// operator -
Vector4 Vector4::operator-(const Vector4& v) const
{
    return Vector4(x - v.x, y - v.y, z - v.z, w - v.w);
}

Vector4 Vector4::operator-(const DirectX::XMVECTOR& v) const
{
    Vector4 v1(v);

    return Vector4(x - v1.x, y - v1.y, z - v1.z, w - v1.w);
}

Vector4 Vector4::operator-(float f) const
{
    return Vector4(x - f, y - f, z - f, w - f);
}

// operator -=
void Vector4::operator-=(const Vector4& v)
{
    x -= v.x;
    y -= v.y;
    z -= v.z;
    w -= v.w;
}

void Vector4::operator-=(const DirectX::XMVECTOR& v)
{
    Vector4 v1(v);

    x -= v1.x;
    y -= v1.y;
    z -= v1.z;
    w -= v1.w;
}

void Vector4::operator-=(float f)
{
    x -= f;
    y -= f;
    z -= f;
    w -= f;
}

// operator *
Vector4 Vector4::operator*(const Vector4& v) const
{
    return Vector4(x * v.x, y * v.y, z * v.z, w * v.w);
}

Vector4 Vector4::operator*(const DirectX::XMVECTOR& v) const
{
    Vector4 v1(v);

    return Vector4(x * v1.x, y * v1.y, z * v1.z, w * v1.w);
}

Vector4 Vector4::operator*(float f) const
{
    return Vector4(x * f, y * f, z * f, w * f);
}

// operator *=
void Vector4::operator*=(const Vector4& v)
{
    x *= v.x;
    y *= v.y;
    z *= v.z;
    w *= v.w;
}

void Vector4::operator*=(const DirectX::XMVECTOR& v)
{
    Vector4 v1(v);

    x *= v1.x;
    y *= v1.y;
    z *= v1.z;
    w *= v1.w;
}

void Vector4::operator*=(float f)
{
    x *= f;
    y *= f;
    z *= f;
    w *= f;
}

// operator /
Vector4 Vector4::operator/(const Vector4& v) const
{
    return Vector4(x / v.x, y / v.y, z / v.z, w / v.w);
}

Vector4 Vector4::operator/(const DirectX::XMVECTOR& v) const
{
    Vector4 v1(v);

    return Vector4(x / v1.x, y / v1.y, z / v1.z, w / v1.w);
}

Vector4 Vector4::operator/(float f) const
{
    return Vector4(x / f, y / f, z / f, w / f);
}

// operator /=
void Vector4::operator/=(const Vector4& v)
{
    x /= v.x;
    y /= v.y;
    z /= v.z;
    w /= v.w;
}

void Vector4::operator/=(const DirectX::XMVECTOR& v)
{
    Vector4 v1(v);

    x /= v1.x;
    y /= v1.y;
    z /= v1.z;
    w /= v1.w;
}

void Vector4::operator/=(float f)
{
    x /= f;
    y /= f;
    z /= f;
    w /= f;
}

const Vector4& Vector4::operator++()
{
    x += 1.f;
    y += 1.f;
    z += 1.f;
    w += 1.f;

    return *this;
}

const Vector4& Vector4::operator++(int)
{
    x += 1.f;
    y += 1.f;
    z += 1.f;
    w += 1.f;

    return *this;
}

const Vector4& Vector4::operator--()
{
    x -= 1.f;
    y -= 1.f;
    z -= 1.f;
    w -= 1.f;

    return *this;
}

const Vector4& Vector4::operator--(int)
{
    x -= 1.f;
    y -= 1.f;
    z -= 1.f;
    w -= 1.f;

    return *this;
}



DirectX::XMVECTOR Vector4::Convert()    const
{
    return DirectX::XMLoadFloat4((DirectX::XMFLOAT4*)this);
}

void Vector4::Convert(const DirectX::XMVECTOR& v)
{
    DirectX::XMStoreFloat4((DirectX::XMFLOAT4*)this, v);
}

