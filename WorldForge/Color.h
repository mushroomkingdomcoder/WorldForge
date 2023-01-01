#pragma once
#include "Vector.h"

class Color
{
private:
	unsigned char b = 0;
	unsigned char g = 0;
	unsigned char r = 0;
	unsigned char a = 255;
public:
	Color() = default;
	constexpr Color(unsigned char r, unsigned char g, unsigned char b, unsigned char a = 255)
		:
		r(r),
		g(g),
		b(b),
		a(a)
	{}
	constexpr Color(int r, int g, int b, int a = 255)
		:
		Color((unsigned char)r, (unsigned char)g, (unsigned char)b, (unsigned char)a)
	{}
	constexpr Color(float r, float g, float b, float a = 1.0f)
		:
		Color(int(r * 255.0f), int(g * 255.0f), int(b * 255.0f), int(a * 255.0f))
	{}
	constexpr Color(const float* ColorRGBA)
		:
		Color(ColorRGBA[0], ColorRGBA[1], ColorRGBA[2], ColorRGBA[3])
	{}
	constexpr Color(int DWORD_BGRA)
		:
		r((DWORD_BGRA & 0x0000FF00) >> 8),
		g((DWORD_BGRA & 0x00FF0000) >> 16),
		b((DWORD_BGRA & 0xFF000000) >> 24),
		a((DWORD_BGRA & 0x000000FF))
	{}
	constexpr Color(const vec4& v4)
		:
		Color(v4.z, v4.y, v4.x, v4.w)
	{}
	constexpr Color(const float4& f4)
		:
		Color(f4.z, f4.y, f4.x, f4.w)
	{}
	const unsigned char& GetR() const
	{
		return r;
	}
	const unsigned char& GetG() const
	{
		return g;
	}
	const unsigned char& GetB() const
	{
		return b;
	}
	const unsigned char& GetA() const
	{
		return a;
	}
	float GetRn() const
	{
		return float(r) / 255.0f;
	}
	float GetGn() const
	{
		return float(g) / 255.0f;
	}
	float GetBn() const
	{
		return float(b) / 255.0f;
	}
	float GetAn() const
	{
		return float(a) / 255.0f;
	}
	void SetR(unsigned char _r)
	{
		r = _r;
	}
	void SetG(unsigned char _g)
	{
		g = _g;
	}
	void SetB(unsigned char _b)
	{
		b = _b;
	}
	void SetA(unsigned char _a)
	{
		a = _a;
	}
	void SetRn(float _r)
	{
		r = unsigned char(_r * 255.0f);
	}
	void SetGn(float _g)
	{
		g = unsigned char(_g * 255.0f);
	}
	void SetBn(float _b)
	{
		b = unsigned char(_b * 255.0f);
	}
	void SetAn(float _a)
	{
		a = unsigned char(_a * 255.0f);
	}
	vec4 GetVector() const
	{
		return vec4(GetBn(), GetGn(), GetRn(), GetAn());
	}
	float4 GetStruct() const
	{
		return float4{ GetBn(),GetGn(),GetRn(),GetAn() };
	}
	Color operator +(const Color& color) const
	{
		return Color(r + color.r, g + color.g, b + color.b, int(a));
	}
	Color& operator +=(const Color& color)
	{
		return *this = *this + color;
	}
	Color operator -(const Color& color) const
	{
		return Color(r - color.r, g - color.g, b - color.b, int(a));
	}
	Color& operator -=(const Color& color)
	{
		return *this = *this - color;
	}
	Color operator *(float intensifier) const
	{
		return Color(this->GetVector() * intensifier);
	}
	Color& operator *=(float intensifier)
	{
		return *this = *this * intensifier;
	}
	Color operator *(const vec4& intensifiers) const
	{
		return Color(this->GetVector() * intensifiers);
	}
	Color& operator *=(const vec4& intensifiers)
	{
		return *this = *this * intensifiers;
	}
	Color Inverted() const
	{
		return Color(255 - r, 255 - g, 255 - b);
	}
	Color& Invert()
	{
		return *this = this->Inverted();
	}
	Color BlendedWith(const Color& color) const
	{
		return Color((*this + color) * 0.5f);
	}
	Color& BlendWith(const Color& color)
	{
		return *this = this->BlendedWith(color);
	}
	bool operator ==(const Color& color) const
	{
		return (r == color.r && g == color.g && b == color.b);
	}
	bool CompletelyEquals(const Color& color) const
	{
		return (*this == color && a == color.a);
	}
	bool operator !=(const Color& color) const
	{
		return !(*this == color);
	}
	bool DoesNotCompletelyEqual(const Color& color) const
	{
		return !this->CompletelyEquals(color);
	}
};

namespace Colors
{
	constexpr Color Black = Color(0, 0, 0);
	constexpr Color DarkGrey = Color(64, 64, 64);
	constexpr Color MedGrey = Color(128, 128, 128);
	constexpr Color LightGrey = Color(192, 192, 192);
	constexpr Color White = Color(255, 255, 255);
	constexpr Color DarkRed = Color(64, 0, 0);
	constexpr Color MedRed = Color(128, 0, 0);
	constexpr Color LightRed = Color(192, 0, 0);
	constexpr Color BrightRed = Color(255, 0, 0);
	constexpr Color DarkGreen = Color(0, 64, 0);
	constexpr Color MedGreen = Color(0, 128, 0);
	constexpr Color LightGreen = Color(0, 192, 0);
	constexpr Color BrightGreen = Color(0, 255, 0);
	constexpr Color DarkBlue = Color(0, 0, 64);
	constexpr Color MedBlue = Color(0, 0, 128);
	constexpr Color LightBlue = Color(0, 0, 192);
	constexpr Color BrightBlue = Color(0, 0, 255);
	constexpr Color DarkCyan = Color(0, 64, 64);
	constexpr Color MedCyan = Color(0, 128, 128);
	constexpr Color LightCyan = Color(0, 192, 192);
	constexpr Color BrightCyan = Color(0, 255, 255);
	constexpr Color DarkPurple = Color(64, 0, 64);
	constexpr Color MedPurple = Color(128, 0, 128);
	constexpr Color LightPurple = Color(192, 0, 192);
	constexpr Color Magenta = Color(255, 0, 255);
	constexpr Color DarkPink = Color(255, 64, 255);
	constexpr Color Pink = Color(255, 128, 255);
	constexpr Color LightPink = Color(255, 192, 255);
	constexpr Color DarkYellow = Color(64, 64, 0);
	constexpr Color MedYellow = Color(128, 128, 0);
	constexpr Color LightYellow = Color(192, 192, 0);
	constexpr Color BrightYellow = Color(255, 255, 0);
	constexpr Color DarkBrown = Color(64, 32, 0);
	constexpr Color Brown = Color(128, 64, 0);
	constexpr Color DarkOrange = Color(192, 96, 0);
	constexpr Color Orange = Color(255, 128, 0);
	constexpr Color DarkPeach = Color(255, 160, 64);
	constexpr Color Peach = Color(255, 192, 128);
	constexpr Color LightPeach = Color(255, 224, 192);
	constexpr Color Transparent = Color(0, 0, 0, 0);
}


