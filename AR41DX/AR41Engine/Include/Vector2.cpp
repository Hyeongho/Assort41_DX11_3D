#include "Vector2.h"
#include "Vector3.h"

Vector2::Vector2()  :
    x(0.f),
    y(0.f)
{
}

Vector2::Vector2(float _x, float _y)    :
    x(_x),
    y(_y)
{
}

Vector2::Vector2(const Vector2& v)  :
    x(v.x),
    y(v.y)
{ 
}

Vector2::Vector2(const Vector3& v)  :
    x(v.x),
    y(v.y)
{
}

Vector2::Vector2(const DirectX::XMVECTOR& v)
{
    // XMVECTOR를 XMFLOAT2로 변환해준다.
    // XMFLOAT2와 이 구조체는 메모리 구조가 같으므로 강제로 형변환해서
    // 변환해주도록 한다.
    DirectX::XMStoreFloat2((DirectX::XMFLOAT2*)this, v);
}

Vector2& Vector2::operator=(const Vector2& v)
{
    x = v.x;
    y = v.y;

    return *this;
}

Vector2& Vector2::operator=(const DirectX::XMVECTOR& v)
{
    DirectX::XMStoreFloat2((DirectX::XMFLOAT2*)this, v);

    return *this;
}

Vector2& Vector2::operator=(float f)
{
    x = f;
    y = f;

    return *this;
}

Vector2& Vector2::operator=(const Vector3& v)
{
    x = v.x;
    y = v.y;

    return *this;
}

bool Vector2::operator==(const Vector2& v) const
{
    return x == v.x && y == v.y;
}

bool Vector2::operator==(const DirectX::XMVECTOR& v) const
{
    Vector2 v1;
    DirectX::XMStoreFloat2((DirectX::XMFLOAT2*)&v1, v);

    return x == v1.x && y == v1.y;
}

bool Vector2::operator!=(const Vector2& v) const
{
    return x != v.x || y != v.y;
}

bool Vector2::operator!=(const DirectX::XMVECTOR& v) const
{
    Vector2 v1;
    DirectX::XMStoreFloat2((DirectX::XMFLOAT2*)&v1, v);

    return x != v1.x || y != v1.y;
}

float& Vector2::operator[](int Index)
{
    if (Index < 0 || Index > 1)
    {
        assert(false);
    }

    if (Index == 0)
        return x;

    return y;
}

// operator +
Vector2 Vector2::operator+(const Vector2& v) const
{
    return Vector2(x + v.x, y + v.y);
}

Vector2 Vector2::operator+(const DirectX::XMVECTOR& v) const
{
    Vector2 v1(v);

    return Vector2(x + v1.x, y + v1.y);
}

Vector2 Vector2::operator+(float f) const
{
    return Vector2(x + f, y + f);
}

// operator +=
void Vector2::operator+=(const Vector2& v)
{
    x += v.x;
    y += v.y;
}

void Vector2::operator+=(const DirectX::XMVECTOR& v)
{
    Vector2 v1(v);

    x += v1.x;
    y += v1.y;
}

void Vector2::operator+=(float f)
{
    x += f;
    y += f;
}

Vector2 Vector2::operator-()
{
    return Vector2(-x, -y);
}

// operator -
Vector2 Vector2::operator-(const Vector2& v) const
{
    return Vector2(x - v.x, y - v.y);
}

Vector2 Vector2::operator-(const DirectX::XMVECTOR& v) const
{
    Vector2 v1(v);

    return Vector2(x - v1.x, y - v1.y);
}

Vector2 Vector2::operator-(float f) const
{
    return Vector2(x - f, y - f);
}

// operator -=
void Vector2::operator-=(const Vector2& v)
{
    x -= v.x;
    y -= v.y;
}

void Vector2::operator-=(const DirectX::XMVECTOR& v)
{
    Vector2 v1(v);

    x -= v1.x;
    y -= v1.y;
}

void Vector2::operator-=(float f)
{
    x -= f;
    y -= f;
}

// operator *
Vector2 Vector2::operator*(const Vector2& v) const
{
    return Vector2(x * v.x, y * v.y);
}

Vector2 Vector2::operator*(const DirectX::XMVECTOR& v) const
{
    Vector2 v1(v);

    return Vector2(x * v1.x, y * v1.y);
}

Vector2 Vector2::operator*(float f) const
{
    return Vector2(x * f, y * f);
}

// operator *=
void Vector2::operator*=(const Vector2& v)
{
    x *= v.x;
    y *= v.y;
}

void Vector2::operator*=(const DirectX::XMVECTOR& v)
{
    Vector2 v1(v);

    x *= v1.x;
    y *= v1.y;
}

void Vector2::operator*=(float f)
{
    x *= f;
    y *= f;
}

// operator /
Vector2 Vector2::operator/(const Vector2& v) const
{
    return Vector2(x / v.x, y / v.y);
}

Vector2 Vector2::operator/(const DirectX::XMVECTOR& v) const
{
    Vector2 v1(v);

    return Vector2(x / v1.x, y / v1.y);
}

Vector2 Vector2::operator/(float f) const
{
    return Vector2(x / f, y / f);
}

// operator /=
void Vector2::operator/=(const Vector2& v)
{
    x /= v.x;
    y /= v.y;
}

void Vector2::operator/=(const DirectX::XMVECTOR& v)
{
    Vector2 v1(v);

    x /= v1.x;
    y /= v1.y;
}

void Vector2::operator/=(float f)
{
    x /= f;
    y /= f;
}

const Vector2& Vector2::operator++()
{
    x += 1.f;
    y += 1.f;

    return *this;
}

const Vector2& Vector2::operator++(int)
{
    x += 1.f;
    y += 1.f;

    return *this;
}

const Vector2& Vector2::operator--()
{
    x -= 1.f;
    y -= 1.f;

    return *this;
}

const Vector2& Vector2::operator--(int)
{
    x -= 1.f;
    y -= 1.f;

    return *this;
}

bool Vector2::Iszero()
{
    if (x == 0.f && y == 0.f)
    {
        return true;
    }
    return false;
}

float Vector2::Length() const
{
    // XMVector2Length함수는 해당 벡터의 크기를 구해서 XMVECTOR 타입으로 변환해서 리턴한다.
    // 이때 X값만을 뽑아내기 위해 XMVectorGetX 함수를 이용하여 크기 값만을
    // 얻어온다.
    return DirectX::XMVectorGetX(DirectX::XMVector2Length(Convert()));
}

void Vector2::Normalize()
{
    *this = DirectX::XMVector2Normalize(Convert());
}

float Vector2::Distance(const Vector2& v) const
{
    Vector2 v1 = *this - v;

    return v1.Length();
}

float Vector2::Dot(const Vector2& v) const
{
    return DirectX::XMVectorGetX(DirectX::XMVector2Dot(Convert(), v.Convert()));
}

float Vector2::Angle(const Vector2& v) const
{
    return 0.0f;
}




DirectX::XMVECTOR Vector2::Convert()    const
{
    return DirectX::XMLoadFloat2((DirectX::XMFLOAT2*)this);
}

void Vector2::Convert(const DirectX::XMVECTOR& v)
{
    DirectX::XMStoreFloat2((DirectX::XMFLOAT2*)this, v);
}
