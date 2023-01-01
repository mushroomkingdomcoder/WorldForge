#pragma once
#include "Math.h"
#include "Matrix.h"
#include <assert.h>

/*

	Simple Vector Structs

*/

template <typename type>
struct type1
{
	type a;
};
using char1		= type1<char>;
using int1		= type1<int>;
using float1	= type1<float>;
using double1	= type1<double>;
template <typename type>
struct type2
{
	type x;
	type y;
};
using char2		= type2<char>;
using int2		= type2<int>;
using float2	= type2<float>;
using double2	= type2<double>;
template <typename type>
struct type3
{
	type x;
	type y;
	type z;
};
using char3		= type3<char>;
using int3		= type3<int>;
using float3	= type3<float>;
using double3	= type3<double>;
template <typename type>
struct type4
{
	type x;
	type y;
	type z;
	type w;
};
using char4		= type4<char>;
using int4		= type4<int>;
using float4	= type4<float>;
using double4	= type4<double>;

/*

	Forward Declare Vector Types

*/

template <typename type>
class Vector3D;
template <typename type>
class Vector4D;

/*

	Vector2D

*/

template <typename type>
class Vector2D
{
public:
	type x = ZERO;
	type y = ZERO;
public:
	constexpr Vector2D() = default;
	constexpr Vector2D(type x, type y)
		:
		x(x),
		y(y)
	{}
	constexpr Vector2D(type2<type> v2)
		:
		Vector2D(v2.x, v2.y)
	{}
	template <typename vType>
	explicit constexpr Vector2D(const Vector2D<vType>& v2)
		:
		Vector2D((type)v2.x, (type)v2.y)
	{}
	explicit constexpr Vector2D(const Vector3D<type>& v3)
		:
		x(v3.x),
		y(v3.y)
	{}
	template <typename vType>
	constexpr Vector2D& operator =(const Vector2D<vType>& v2)
	{
		x = (type)v2.x;
		y = (type)v2.y;
		return *this;
	}
	type2<type> GetVStruct() const
	{
		return type2<type>{ x,y };
	}
	Vector2D& operator =(const Vector3D<type>& v3)
	{
		x = v3.x;
		y = v3.y;
		return *this;
	}
	Vector2D& operator =(const Vector4D<type>& v4)
	{
		x = v4.x;
		y = v4.y;
		return *this;
	}
	Vector2D operator +(const Vector2D& v2) const
	{
		return Vector2D(x + v2.x, y + v2.y);
	}
	Vector2D& operator +=(const Vector2D& v2)
	{
		return *this = *this + v2;
	}
	Vector2D operator -(const Vector2D& v2) const
	{
		return Vector2D(x - v2.x, y - v2.y);
	}
	Vector2D& operator -=(const Vector2D& v2)
	{
		return *this = *this - v2;
	}
	Vector2D operator *(const Vector2D& scale) const
	{
		return Vector2D(x * scale.x, y * scale.y);
	}
	Vector2D& operator *=(const Vector2D& scale)
	{
		return *this = *this * scale;
	}
	Vector2D operator /(const Vector2D& inv_scale) const
	{
		return Vector2D(x / inv_scale.x, y / inv_scale.y);
	}
	Vector2D& operator /=(const Vector2D& inv_scale)
	{
		return *this = *this / inv_scale;
	}
	Vector2D operator *(const type& scale) const
	{
		return Vector2D(x * scale, y * scale);
	}
	Vector2D& operator *=(const type& scale)
	{
		return *this = *this * scale;
	}
	Vector2D operator /(const type& inv_scale) const
	{
		return Vector2D(x / inv_scale, y / inv_scale);
	}
	Vector2D& operator /=(const type& inv_scale)
	{
		return *this = *this / inv_scale;
	}
	Vector2D operator *(const Matrix2D<type>& mat2) const
	{
		Vector2D result = {};
		for (int y = 0; y < 2; ++y)
		{
			for (int x = 0; x < 2; ++x)
			{
				result[y] += (*this)[x] * mat2.data[x][y];
			}
		}
		return result;
	}
	Vector2D& operator *=(const Matrix2D<type>& mat2)
	{
		return *this = *this * mat2;
	}
	type LengthSq() const
	{
		return sq(x) + sq(y);
	}
	type Length() const
	{
		return (type)sqrt(LengthSq());
	}
	Vector2D Normalized() const
	{
		const type lengthsq = LengthSq();
		if (lengthsq == ZERO)
		{
			return *this;
		}
		else
		{
			return *this / (type)sqrt(lengthsq);
		}
	}
	Vector2D& Normalize()
	{
		return *this = this->Normalized();
	}
	type DotProduct(const Vector2D& v2) const
	{
		return type(x * v2.x + y * v2.y);
	}
	Vector2D Rotated90() const
	{
		return Vector2D(-y, x);
	}
	Vector2D& Rotate90()
	{
		return *this = this->Rotated90();
	}
	Vector2D Rotated180() const
	{
		return Vector2D(-x, -y);
	}
	Vector2D& Rotate180()
	{
		return *this = this->Rotated180();
	}
	Vector2D Rotated270() const
	{
		return Vector2D(y, -x);
	}
	Vector2D& Rotate270()
	{
		return *this = this->Rotated270();
	}
	Vector2D Rotated(const type& radians) const
	{
		const type cosR = (type)cos((double)radians);
		const type sinR = (type)sin((double)radians);
		return Vector2D(x * cosR - y * sinR, x * sinR + y * cosR);
	}
	Vector2D& Rotate(const type& radians)
	{
		return *this = this->Rotated(radians);
	}
	Vector2D InterpolatedTo(const Vector2D& v2) const
	{
		return Vector2D(v2 - *this).Normalized();
	}
	Vector2D& InterpolateTo(const Vector2D& v2)
	{
		return *this = this->InterpolatedTo(v2);
	}
	type GetAspectRatio() const
	{
		return x / y;
	}
	type GetInvAspectRatio() const
	{
		return y / x;
	}
	const type& operator [](const int& index) const
	{
		assert(index < 2);
		switch (index)
		{
			case 1:
			{
				return y;
			}
			default:
			{
				return x;
			}
		}
	}
	type& operator [](const int& index)
	{
		assert(index < 2);
		switch (index)
		{
			case 1:
			{
				return y;
			}
			default:
			{
				return x;
			}
		}
	}
	bool operator ==(const Vector2D& v2) const
	{
		return (x == v2.x && y == v2.y);
	}
	bool operator !=(const Vector2D& v2) const
	{
		return !(*this == v2);
	}
	bool operator <(const Vector2D& v2) const
	{
		return (this->LengthSq() < v2.LengthSq());
	}
	bool operator <=(const Vector2D& v2) const
	{
		return (this->LengthSq() <= v2.LengthSq());
	}
	bool operator >(const Vector2D& v2) const
	{
		return (this->LengthSq() > v2.LengthSq());
	}
	bool operator >=(const Vector2D& v2) const
	{
		return (this->LengthSq() >= v2.LengthSq());
	}
};
using vec2 = Vector2D<float>;
using vec2i = Vector2D<int>;
using vec2d = Vector2D<double>;

/*

	Vector3D

*/

template <typename type>
class Vector3D
{
public:
	type x = ZERO;
	type y = ZERO;
	type z = ZERO;
public:
	constexpr Vector3D() = default;
	constexpr Vector3D(type x, type y, type z)
		:
		x(x),
		y(y),
		z(z)
	{}
	constexpr Vector3D(type3<type> v3)
		:
		Vector3D(v3.x, v3.y, v3.z)
	{}
	template <typename vType>
	explicit constexpr Vector3D(const Vector3D<vType>& v3)
		:
		Vector3D((type)v3.x, (type)v3.y, (type)v3.z)
	{}
	explicit constexpr Vector3D(const Vector2D<type>& v2, type z = ONE)
		:
		x(v2.x),
		y(v2.y),
		z(z)
	{}
	explicit constexpr Vector3D(const Vector4D<type>& v4)
		:
		x(v4.x),
		y(v4.y),
		z(v4.z)
	{}
	template <typename vType>
	constexpr Vector3D& operator =(const Vector3D<vType>& v3)
	{
		x = (type)v3.x;
		y = (type)v3.y;
		z = (type)v3.z;
		return *this;
	}
	type3<type> GetVStruct() const
	{
		return type3<type>{ x,y,z };
	}
	Vector3D& operator =(const Vector2D<type>& v2)
	{
		x = v2.x;
		y = v2.y;
		z = ONE;
		return *this;
	}
	Vector3D& operator =(const Vector4D<type>& v4)
	{
		x = v4.x;
		y = v4.y;
		z = v4.z;
		return *this;
	}
	Vector3D operator +(const Vector3D& v3) const
	{
		return Vector3D(x + v3.x, y + v3.y, z + v3.z);
	}
	Vector3D& operator +=(const Vector3D& v3)
	{
		return *this = *this + v3;
	}
	Vector3D operator -(const Vector3D& v3) const
	{
		return Vector3D(x - v3.x, y - v3.y, z - v3.z);
	}
	Vector3D& operator -=(const Vector3D& v3)
	{
		return *this = *this - v3;
	}
	Vector3D operator *(const Vector3D& scale) const
	{
		return Vector3D(x * scale.x, y * scale.y, z * scale.z);
	}
	Vector3D& operator *=(const Vector3D& scale)
	{
		return *this = *this * scale;
	}
	Vector3D operator /(const Vector3D& inv_scale) const
	{
		return Vector3D(x / inv_scale.x, y / inv_scale.y, z * inv_scale.z);
	}
	Vector3D& operator /=(const Vector3D& inv_scale)
	{
		return *this = *this / inv_scale;
	}
	Vector3D operator *(const type& scale) const
	{
		return Vector3D(x * scale, y * scale, z * scale);
	}
	Vector3D& operator *=(const type& scale)
	{
		return *this = *this * scale;
	}
	Vector3D operator /(const type& inv_scale) const
	{
		return Vector3D(x / inv_scale, y / inv_scale, z / inv_scale);
	}
	Vector3D& operator /=(const type& inv_scale)
	{
		return *this = *this / inv_scale;
	}
	Vector3D operator *(const Matrix3D<type>& mat3) const
	{
		Vector3D result = {};
		for (int y = 0; y < 3; ++y)
		{
			for (int x = 0; x < 3; ++x)
			{
				result[y] += (*this)[x] * mat3.data[x][y];
			}
		}
		return result;
	}
	Vector3D& operator *=(const Matrix3D<type>& mat3)
	{
		return *this = *this * mat3;
	}
	type LengthSq() const
	{
		return sq(x) + sq(y) + sq(z);
	}
	type Length() const
	{
		return (type)sqrt(LengthSq());
	}
	Vector3D Normalized() const
	{
		const type lengthsq = LengthSq();
		if (lengthsq == ZERO)
		{
			return *this;
		}
		else
		{
			return *this / (type)sqrt(lengthsq);
		}
	}
	Vector3D& Normalize()
	{
		return *this = this->Normalized();
	}
	type DotProduct(const Vector3D& v3) const
	{
		return type(x * v3.x + y * v3.y + z * v3.z);
	}
	Vector3D CrossProduct(const Vector3D& v3) const
	{
		return Vector3D(y * v3.z - z * v3.y, -(x * v3.z - z * v3.x), x * v3.y - y * v3.x);
	}
	Vector3D RotatedAroundX(const type& radians) const
	{
		const type cosR = (type)cos((double)radians);
		const type sinR = (type)sin((double)radians);
		return Vector3D(x, y * cosR - z * sinR, y * sinR + z * cosR);
	}
	Vector3D& RotateAroundX(const type& radians)
	{
		return *this = this->RotatedAroundX(radians);
	}
	Vector3D RotatedAroundY(const type& radians) const
	{
		const type cosR = (type)cos((double)radians);
		const type sinR = (type)sin((double)radians);
		return Vector3D(x * cosR - z * sinR, y, x * sinR + z * cosR);
	}
	Vector3D& RotateAroundY(const type& radians)
	{
		return *this = this->RotatedAroundY(radians);
	}
	Vector3D RotatedAroundZ(const type& radians) const
	{
		const type cosR = (type)cos((double)radians);
		const type sinR = (type)sin((double)radians);
		return Vector3D(x * cosR - y * sinR, x * sinR + y * cosR, z);
	}
	Vector3D& RotateAroundZ(const type& radians)
	{
		return *this = this->RotatedAroundZ(radians);
	}
	Vector3D InterpolatedTo(const Vector3D& v3) const
	{
		return Vector3D(v3 - *this).Normalized();
	}
	Vector3D& InterpolateTo(const Vector3D& v3)
	{
		return *this = this->InterpolatedTo(v3);
	}
	const type& operator [](const int& index) const
	{
		assert(index < 3);
		switch (index)
		{
			case 2:
			{
				return z;
			}
			case 1:
			{
				return y;
			}
			default:
			{
				return x;
			}
		}
	}
	type& operator [](const int& index)
	{
		assert(index < 3);
		switch (index)
			{
			case 2:
			{
				return z;
			}
			case 1:
			{
				return y;
			}
			default:
			{
				return x;
			}
		}
	}
	bool operator ==(const Vector3D& v3) const
	{
		return (x == v3.x && y == v3.y && z == v3.z);
	}
	bool operator !=(const Vector3D& v3) const
	{
		return !(*this == v3);
	}
	bool operator <(const Vector3D& v3) const
	{
		return (this->LengthSq() < v3.LengthSq());
	}
	bool operator <=(const Vector3D& v3) const
	{
		return (this->LengthSq() <= v3.LengthSq());
	}
	bool operator >(const Vector3D& v3) const
	{
		return (this->LengthSq() > v3.LengthSq());
	}
	bool operator >=(const Vector3D& v3) const
	{
		return (this->LengthSq() >= v3.LengthSq());
	}
};
using vec3 = Vector3D<float>;
using vec3i = Vector3D<int>;
using vec3d = Vector3D<double>;

/*

	Vector4D

*/

template <typename type>
class Vector4D
{
public:
	type x = ZERO;
	type y = ZERO;
	type z = ZERO;
	type w = ZERO;
public:
	constexpr Vector4D() = default;
	constexpr Vector4D(type x, type y, type z, type w)
		:
		x(x),
		y(y),
		z(z),
		w(w)
	{}
	constexpr Vector4D(type4<type> v4)
		:
		Vector4D(v4.x, v4.y, v4.z, v4.w)
	{}
	template <typename vType>
	explicit constexpr Vector4D(const Vector4D<vType>& v4)
		:
		Vector4D((type)v4.x, (type)v4.y, (type)v4.z, (type)v4.w)
	{}
	explicit constexpr Vector4D(const Vector3D<type>& v3, type w = ONE)
		:
		x(v3.x),
		y(v3.y),
		z(v3.z),
		w(w)
	{}
	template <typename vType>
	constexpr Vector4D& operator =(const Vector4D<vType>& v4)
	{
		x = (type)v4.x;
		y = (type)v4.y;
		z = (type)v4.z;
		return *this;
	}
	type4<type> GetVStruct() const
	{
		return type4<type>{ x,y,z,w };
	}
	Vector4D& operator =(const Vector2D<type>& v2)
	{
		x = v2.x;
		y = v2.y;
		z = ZERO;
		w = ONE;
		return *this;
	}
	Vector4D& operator =(const Vector3D<type>& v3)
	{
		x = v3.x;
		y = v3.y;
		z = v3.z;
		w = ONE;
	}
	Vector4D operator +(const Vector4D& v4) const
	{
		return Vector4D(x + v4.x, y + v4.y, z + v4.z, w + v4.w);
	}
	Vector4D& operator +=(const Vector4D& v4)
	{
		return *this = *this + v4;
	}
	Vector4D operator -(const Vector4D& v4) const
	{
		return Vector4D(x - v4.x, y - v4.y, z - v4.z, w - v4.w);
	}
	Vector4D& operator -=(const Vector4D& v4)
	{
		return *this = *this - v4;
	}
	Vector4D operator *(const Vector4D& scale) const
	{
		return Vector4D(x * scale.x, y * scale.y, z * scale.z, w * scale.w);
	}
	Vector4D& operator *=(const Vector4D& scale)
	{
		return *this = *this * scale;
	}
	Vector4D operator /(const Vector4D& inv_scale) const
	{
		return Vector4D(x / inv_scale.x, y / inv_scale.y, z / inv_scale.z, w / inv_scale.w);
	}
	Vector4D& operator /=(const Vector4D& inv_scale)
	{
		return *this = *this / inv_scale;
	}
	Vector4D operator *(const type& scale) const
	{
		return Vector4D(x * scale, y * scale, z * scale, w * scale);
	}
	Vector4D& operator *=(const type& scale)
	{
		return *this = *this * scale;
	}
	Vector4D operator /(const type& inv_scale) const
	{
		return Vector4D(x / inv_scale, y / inv_scale, z / inv_scale, w / inv_scale);
	}
	Vector4D& operator /=(const type& inv_scale)
	{
		return *this = *this / inv_scale;
	}
	Vector4D operator *(const Matrix4D<type>& mat4) const
	{
		Vector4D result = {};
		for (int y = 0; y < 4; ++y)
		{
			for (int x = 0; x < 4; ++x)
			{
				result[y] += (*this)[x] * mat4.data[x][y];
			}
		}
		return result;
	}
	Vector4D& operator *=(const Matrix4D<type>& mat4)
	{
		return *this = *this * mat4;
	}
	type LengthSq() const
	{
		return sq(x) + sq(y) + sq(z) + sq(w);
	}
	type Length() const
	{
		return (type)sqrt(LengthSq());
	}
	Vector4D Normalized() const
	{
		const type lengthsq = LengthSq();
		if (lengthsq == ZERO)
		{
			return *this;
		}
		else
		{
			return *this / (type)sqrt(lengthsq);
		}
	}
	Vector4D& Normalize()
	{
		return *this = this->Normalized();
	}
	Vector4D InterpolatedTo(const Vector4D& v4) const
	{
		return Vector4D(v4 - *this).Normalized();
	}
	Vector4D& InterpolateTo(const Vector4D& v4)
	{
		return *this = this->InterpolatedTo(v4);
	}
	const type& operator [](const int& index) const
	{
		assert(index < 4);
		switch (index)
		{
			case 3:
			{
				return w;
			}
			case 2:
			{
				return z;
			}
			case 1:
			{
				return y;
			}
			default:
			{
				return x;
			}
		}
	}
	type& operator [](const int& index)
	{
		assert(index < 4);
			switch (index)
			{
			case 3:
			{
				return w;
			}
			case 2:
			{
				return z;
			}
			case 1:
			{
				return y;
			}
			default:
			{
				return x;
			}
		}
	}
	bool operator ==(const Vector4D& v4) const
	{
		return (x == v4.x && y == v4.y && z == v4.z && w == v4.w);
	}
	bool operator !=(const Vector4D& v4) const
	{
		return !(*this == v4);
	}
	bool operator <(const Vector4D& v4) const
	{
		return (this->LengthSq() < v4.LengthSq());
	}
	bool operator <=(const Vector4D& v4) const
	{
		return (this->LengthSq() <= v4.LengthSq());
	}
	bool operator >(const Vector4D& v4) const
	{
		return (this->LengthSq() > v4.LengthSq());
	}
	bool operator >=(const Vector4D& v4) const
	{
		return (this->LengthSq() >= v4.LengthSq());
	}
};
using vec4 = Vector4D<float>;
using vec4i = Vector4D<int>;
using vec4d = Vector4D<double>;


