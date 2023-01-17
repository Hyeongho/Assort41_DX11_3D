#include "Matrix.h"
#include "Vector2.h"
#include "Vector3.h"
#include "Vector4.h"

Matrix::Matrix()
{
    m = DirectX::XMMatrixIdentity();
}

Matrix::Matrix(const Matrix& _m)
{
    m = _m.m;
}

Matrix::Matrix(const DirectX::XMMATRIX& _m)
{
    m = _m;
}

Matrix::Matrix(Vector4 _v[4])
{
    memcpy(v, _v, sizeof(Vector4) * 4);
}

Vector4& Matrix::operator[](int Index)
{
    if (Index < 0 || Index > 3)
    {
        assert(false);
    }

    return v[Index];
}

Matrix Matrix::operator=(const Matrix& _m)
{
    m = _m.m;
    return *this;
}

Matrix Matrix::operator=(const DirectX::XMMATRIX& _m)
{
    m = _m;
    return *this;
}

Matrix Matrix::operator*(const Matrix& _m) const
{
    return Matrix(m * _m.m);
}

Matrix Matrix::operator*(const DirectX::XMMATRIX& _m) const
{
    return Matrix(m * _m);
}

void Matrix::Identity()
{
    m = DirectX::XMMatrixIdentity();
}

void Matrix::Transpose()
{
    m = DirectX::XMMatrixTranspose(m);
}

void Matrix::Inverse()
{
    DirectX::XMVECTOR   det = DirectX::XMMatrixDeterminant(m);
    m = DirectX::XMMatrixInverse(&det, m);
}

void Matrix::Scaling(const Vector3& _v)
{
    m = DirectX::XMMatrixScaling(_v.x, _v.y, _v.z);
}

void Matrix::Scaling(float x, float y, float z)
{
    m = DirectX::XMMatrixScaling(x, y, z);
}

void Matrix::Scaling(const Vector2& _v)
{
    m = DirectX::XMMatrixScaling(_v.x, _v.y, 1.f);
}

void Matrix::Scaling(float x, float y)
{
    m = DirectX::XMMatrixScaling(x, y, 1.f);
}

void Matrix::Rotation(const Vector3& _v)
{
    Vector3 Rot = _v.ConvertAngle();

    DirectX::XMVECTOR   Qut = DirectX::XMQuaternionRotationRollPitchYaw(Rot.x, Rot.y, Rot.z);

    RotationQuaternion(Qut);
}

void Matrix::Rotation(float x, float y, float z)
{
    Vector3 v(x, y, z);
    Vector3 Rot = v.ConvertAngle();

    DirectX::XMVECTOR   Qut = DirectX::XMQuaternionRotationRollPitchYaw(Rot.x, Rot.y, Rot.z);

    RotationQuaternion(Qut);
}

void Matrix::RotationX(float x)
{
    m = DirectX::XMMatrixRotationX(DegreeToRadian(x));
}

void Matrix::RotationY(float y)
{
    m = DirectX::XMMatrixRotationY(DegreeToRadian(y));
}

void Matrix::RotationZ(float z)
{
    m = DirectX::XMMatrixRotationZ(DegreeToRadian(z));
}

void Matrix::RotationQuaternion(const Vector4& q)
{
    m = DirectX::XMMatrixRotationQuaternion(q.Convert());
}

void Matrix::RotationAxis(const Vector3& Axis, float Angle)
{
    m = DirectX::XMMatrixRotationAxis(Axis.Convert(), DegreeToRadian(Angle));
}

void Matrix::Translation(const Vector3& _v)
{
    m = DirectX::XMMatrixTranslation(_v.x, _v.y, _v.z);
}

void Matrix::Translation(float x, float y, float z)
{
    m = DirectX::XMMatrixTranslation(x, y, z);
}

void Matrix::Translation(const Vector2& _v)
{
    m = DirectX::XMMatrixTranslation(_v.x, _v.y, 0.f);
}

void Matrix::Translation(float x, float y)
{
    m = DirectX::XMMatrixTranslation(x, y, 0.f);
}

Matrix Matrix::StaticTranspose(const Matrix& _m)
{
    return DirectX::XMMatrixTranspose(_m.m);
}

Matrix Matrix::StaticInverse(const Matrix& _m)
{
    DirectX::XMVECTOR   det = DirectX::XMMatrixDeterminant(_m.m);
    return DirectX::XMMatrixInverse(&det, _m.m);
}

Matrix Matrix::StaticScaling(const Vector3& _v)
{
    return DirectX::XMMatrixScaling(_v.x, _v.y, _v.z);
}

Matrix Matrix::StaticScaling(float x, float y, float z)
{
    return DirectX::XMMatrixScaling(x, y, z);
}

Matrix Matrix::StaticScaling(const Vector2& _v)
{
    return DirectX::XMMatrixScaling(_v.x, _v.y, 1.f);
}

Matrix Matrix::StaticScaling(float x, float y)
{
    return DirectX::XMMatrixScaling(x, y, 1.f);
}

Matrix Matrix::StaticRotation(const Vector3& _v)
{
    Vector3 Rot = _v.ConvertAngle();

    DirectX::XMVECTOR   Qut = DirectX::XMQuaternionRotationRollPitchYaw(Rot.x, Rot.y, Rot.z);

    return StaticRotationQuaternion(Qut);
}

Matrix Matrix::StaticRotation(float x, float y, float z)
{
    Vector3 _v(x, y, z);
    Vector3 Rot = _v.ConvertAngle();

    DirectX::XMVECTOR   Qut = DirectX::XMQuaternionRotationRollPitchYaw(Rot.x, Rot.y, Rot.z);

    return StaticRotationQuaternion(Qut);
}

Matrix Matrix::StaticRotationX(float x)
{
    return DirectX::XMMatrixRotationX(DegreeToRadian(x));
}

Matrix Matrix::StaticRotationY(float y)
{
    return DirectX::XMMatrixRotationY(DegreeToRadian(y));
}

Matrix Matrix::StaticRotationZ(float z)
{
    return DirectX::XMMatrixRotationZ(DegreeToRadian(z));
}

Matrix Matrix::StaticRotationQuaternion(const Vector4& q)
{
    return DirectX::XMMatrixRotationQuaternion(q.Convert());;
}

Matrix Matrix::StaticRotationAxis(const Vector3& Axis, float Angle)
{
    return DirectX::XMMatrixRotationAxis(Axis.Convert(), DegreeToRadian(Angle));
}

Matrix Matrix::StaticTranslation(const Vector3& _v)
{
    return DirectX::XMMatrixTranslation(_v.x, _v.y, _v.z);
}

Matrix Matrix::StaticTranslation(float x, float y, float z)
{
    return DirectX::XMMatrixTranslation(x, y, z);
}

Matrix Matrix::StaticTranslation(const Vector2& _v)
{
    return DirectX::XMMatrixTranslation(_v.x, _v.y, 0.f);
}

Matrix Matrix::StaticTranslation(float x, float y)
{
    return DirectX::XMMatrixTranslation(x, y, 0.f);
}
