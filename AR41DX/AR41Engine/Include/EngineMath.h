#pragma once

#include <DirectXMath.h>
#include <DirectXPackedVector.h>
#include <assert.h>
#include <string>

#include "Flag.h"

#define	PI	3.14159f

static float DegreeToRadian(float Degree)
{
	return Degree * PI / 180.f;
}

static float RadianToDegree(float Radian)
{
	return Radian * 180.f / PI;
}
