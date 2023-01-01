#pragma once
#define _USE_MATH_DEFINES
#include <math.h>

template <typename root>
inline root sq(const root& x)
{
	return x * x;
}

template <typename angle>
inline angle angle_wrap(angle theta)
{
	const angle TWO_PI = (angle)2.0 * (angle)M_PI;
	while (theta >= TWO_PI)
	{
		theta -= TWO_PI;
	}
	while (theta < (angle)0.0)
	{
		theta += TWO_PI;
	}
	return theta;
}

template <typename Vector2D>
inline Vector2D absvec2(const Vector2D& v2)
{
	return { abs(v2.x),abs(v2.y) };
}