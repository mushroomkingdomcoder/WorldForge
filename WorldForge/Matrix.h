#pragma once
#include "Math.h"
#include <assert.h>

#define ZERO (type)0.0
#define ONE (type)1.0
#define TWO (type)2.0

/*

	Matrix2D

*/

template <typename type>
class Matrix2D
{
public:
	type data[2][2];
public:
	constexpr Matrix2D()
	{
		for (int y = 0; y < 2; ++y)
		{
			for (int x = 0; x < 2; ++x)
			{
				data[x][y] = (type)0.0;
			}
		}
	}
	constexpr Matrix2D(type x1, type y1, type x2, type y2)
	{
		data[0][0] = x1; data[1][0] = y1;
		data[0][1] = x2; data[1][1] = y2;
	}
	template <typename mType>
	explicit constexpr Matrix2D(const Matrix2D<mType>& m2)
	{
		for (int y = 0; y < 2; ++y)
		{
			for (int x = 0; x < 2; ++x)
			{
				data[x][y] = (type)m2.data[x][y];
			}
		}
	}
	template <typename mType>
	constexpr Matrix2D& operator =(const Matrix2D<mType>& m2)
	{
		for (int y = 0; y < 2; ++y)
		{
			for (int x = 0; x < 2; ++x)
			{
				data[x][y] = (type)m2.data[x][y];
			}
		}
		return *this;
	}
	Matrix2D operator *(const Matrix2D& m2) const
	{
		Matrix2D result;
		for (int i = 0; i < 2; ++i)
		{
			for (int j = 0; j < 2; ++j)
			{
				for (int k = 0; k < 2; ++k)
				{
					result.data[i][j] += this->data[i][k] * m2.data[k][j];
				}
			}
		}
		return result;
	}
	Matrix2D& operator *=(const Matrix2D& m2)
	{
		return *this = *this * m2;
	}
public:
	static constexpr Matrix2D Identity()
	{
		return Matrix2D
		(
			ONE,	ZERO,
			ZERO,	ONE
		);
	}
	static constexpr Matrix2D Scaling(const type& xScale, const type& yScale)
	{
		return Matrix2D
		(
			xScale,	ZERO,
			ZERO,	yScale
		);
	}
	static constexpr Matrix2D Rotation(const type& radians)
	{
		const type cosR = (type)cos((double)radians);
		const type sinR = (type)sin((double)radians);
		return Matrix2D
		(
			cosR,	-sinR,
			sinR,	cosR
		);
	}
};
using mat2 = Matrix2D<float>;
using mat2i = Matrix2D<int>;
using mat2d = Matrix2D<double>;

/*

	Matrix3D

*/

template <typename type>
class Matrix3D
{
public:
	type data[3][3];
public:
	constexpr Matrix3D()
	{
		for (int y = 0; y < 3; ++y)
		{
			for (int x = 0; x < 3; ++x)
			{
				data[x][y] = (type)0.0;
			}
		}
	}
	constexpr Matrix3D(type x1, type y1, type z1, type x2, type y2, type z2, type x3, type y3, type z3)
	{
		data[0][0] = x1; data[1][0] = y1; data[2][0] = z1;
		data[0][1] = x2; data[1][1] = y2; data[2][1] = z2;
		data[0][2] = x3; data[1][2] = y3; data[2][2] = z3;
	}
	template <typename mType>
	explicit constexpr Matrix3D(const Matrix3D<mType>& m3)
	{
		for (int y = 0; y < 3; ++y)
		{
			for (int x = 0; x < 3; ++x)
			{
				data[x][y] = (type)m3.data[x][y];
			}
		}
	}
	explicit constexpr Matrix3D(const Matrix2D<type>& m2, type z1 = ZERO, type z2 = ZERO, type x3 = ZERO, type y3 = ZERO, type z3 = ONE)
	{
		for (int y = 0; y < 2; ++y)
		{
			for (int x = 0; x < 2; ++x)
			{
				data[x][y] = m2.data[x][y];
			}
		}
		/* ^ Matrix2D Initialization ^  */data[2][0] = z1;
		/*								*/data[2][1] = z2;
		data[0][2] = x3; data[1][2] = y3; data[2][2] = z3;
	}
	template <typename mType>
	constexpr Matrix3D& operator =(const Matrix3D<mType>& m3)
	{
		for (int y = 0; y < 3; ++y)
		{
			for (int x = 0; x < 3; ++x)
			{
				data[x][y] = (type)m3.data[x][y];
			}
		}
		return *this;
	}
	Matrix3D operator *(const Matrix3D& m3) const
	{
		Matrix3D result;
		for (int i = 0; i < 3; ++i)
		{
			for (int j = 0; j < 3; ++j)
			{
				for (int k = 0; k < 3; ++k)
				{
					result.data[i][j] += this->data[i][k] * m3.data[k][j];
				}
			}
		}
		return result;
	}
	Matrix3D& operator *=(const Matrix3D& m3)
	{
		return *this = *this * m3;
	}
public:
	static constexpr Matrix3D Identity()
	{
		return Matrix3D
		(
			ONE,	ZERO,	ZERO,
			ZERO,	ONE,	ZERO,
			ZERO,	ZERO,	ONE
		);
	}
	static constexpr Matrix3D Scaling(const type& xScale, const type& yScale, const type& zScale)
	{
		return Matrix3D
		(
			xScale,	ZERO,	ZERO,
			ZERO,	yScale,	ZERO,
			ZERO,	ZERO,	zScale
		);
	}
	static constexpr Matrix3D RotationX(const type& radians)
	{
		const type cosR = (type)cos((double)radians);
		const type sinR = (type)sin((double)radians);
		return Matrix3D
		(
			ONE,	ZERO,	ZERO,
			ZERO,	cosR,	-sinR,
			ZERO,	sinR,	cosR
		);
	}
	static constexpr Matrix3D RotationY(const type& radians)
	{
		const type cosR = (type)cos((double)radians);
		const type sinR = (type)sin((double)radians);
		return Matrix3D
		(
			cosR,	ZERO,	-sinR,
			ZERO,	ONE,	ZERO,
			sinR,	ZERO,	cosR
		);
	}
	static constexpr Matrix3D RotationZ(const type& radians)
	{
		const type cosR = (type)cos((double)radians);
		const type sinR = (type)sin((double)radians);
		return Matrix3D
		(
			cosR,	-sinR,	ZERO,
			sinR,	cosR,	ZERO,
			ZERO,	ZERO,	ONE
		);
	}
	static constexpr Matrix3D Translation(const type& xTrans, const type& yTrans)
	{
		return Matrix3D
		(
			ONE,	ZERO,	xTrans,
			ZERO,	ONE,	yTrans,
			ZERO,	ZERO,	ONE
		);
	}
};
using mat3 = Matrix3D<float>;
using mat3i = Matrix3D<int>;
using mat3d = Matrix3D<double>;

/*

	Matrix4D

*/

template <typename type>
class Matrix4D
{
public:
	type data[4][4];
public:
	constexpr Matrix4D()
	{
		for (int y = 0; y < 4; ++y)
		{
			for (int x = 0; x < 4; ++x)
			{
				data[x][y] = ZERO;
			}
		}
	}
	constexpr Matrix4D(type x1, type y1, type z1, type w1, type x2, type y2, type z2, type w2, type x3, type y3, type z3, type w3, type x4, type y4, type z4, type w4)
	{
		data[0][0] = x1; data[1][0] = y1; data[2][0] = z1; data[3][0] = w1;
		data[0][1] = x2; data[1][1] = y2; data[2][1] = z2; data[3][1] = w2;
		data[0][2] = x3; data[1][2] = y3; data[2][2] = z3; data[3][2] = w3;
		data[0][3] = x4; data[1][3] = y4; data[2][3] = z4; data[3][3] = w4;
	}
	template <typename mType>
	explicit constexpr Matrix4D(const Matrix4D<mType>& m4)
	{
		for (int y = 0; y < 4; ++y)
		{
			for (int x = 0; x < 4; ++x)
			{
				data[x][y] = (type)m4.data[x][y];
			}
		}
	}
	explicit constexpr Matrix4D(const Matrix3D<type>& m3, type w1 = ZERO, type w2 = ZERO, type w3 = ZERO, type x4 = ZERO, type y4 = ZERO, type z4 = ZERO, type w4 = ONE)
	{
		for (int y = 0; y < 3; ++y)
		{
			for (int x = 0; x < 3; ++x)
			{
				data[x][y] = m3.data[x][y];
			}
		}
		/*												 */data[3][0] = w1;
		/*			^ Matrix3D Initialization ^			 */data[3][1] = w2;
		/*												 */data[3][2] = w3;
		data[0][3] = x4; data[1][3] = y4; data[2][3] = z4; data[3][3] = w4;
	}
	template <typename mType>
	constexpr Matrix4D& operator =(const Matrix4D<mType>& m4)
	{
		for (int y = 0; y < 4; ++y)
		{
			for (int x = 0; x < 4; ++x)
			{
				data[x][y] = (type)m4.data[x][y];
			}
		}
		return *this;
	}
	Matrix4D operator *(const Matrix4D& m4) const
	{
		Matrix4D result;
		for (int i = 0; i < 4; ++i)
		{
			for (int j = 0; j < 4; ++j)
			{
				for (int k = 0; k < 4; ++k)
				{
					result.data[i][j] += this->data[i][k] * m4.data[k][j];
				}
			}
		}
		return result;
	}
	Matrix4D& operator *=(const Matrix4D& m4)
	{
		return *this = *this * m4;
	}
public:
	static constexpr Matrix4D Identity()
	{
		return Matrix4D
		(
			ONE,	ZERO,	ZERO,	ZERO,
			ZERO,	ONE,	ZERO,	ZERO,
			ZERO,	ZERO,	 ONE,	ZERO,
			ZERO,	ZERO,	ZERO,	ONE
		);
	}
	static constexpr Matrix4D Scaling(const type& xScale, const type& yScale, const type& zScale, const type& wScale)
	{
		return Matrix4D
		(
			xScale,	ZERO,	ZERO,	ZERO,
			ZERO,	yScale,	ZERO,	ZERO,
			ZERO,	ZERO,	zScale,	ZERO,
			ZERO,	ZERO,	ZERO,	wScale
		);
	}
	static constexpr Matrix4D Translation(const type& xTrans, const type& yTrans, const type& zTrans)
	{
		return Matrix4D
		(
			ONE,	ZERO,	ZERO,	xTrans,
			ZERO,	ONE,	ZERO,	yTrans,
			ZERO,	ZERO,	ONE,	zTrans,
			ZERO,	ZERO,	ZERO,	ONE
		);
	}
};
using mat4 = Matrix4D<float>;
using mat4i = Matrix4D<int>;
using mat4d = Matrix4D<double>;

