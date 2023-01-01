#pragma once
#include "Graphics.h"
#include "Vector.h"
#include <functional>

template <typename type>
class Rect
{
public:
	Vector2D<type> pos = { ZERO,ZERO };
	type width = ONE;
	type height = ONE;
public:
	constexpr Rect() = default;
	constexpr Rect(Vector2D<type> pos, type width, type height)
		:
		pos(pos),
		width(width),
		height(height)
	{
		assert(width >= ZERO);
		assert(height >= ZERO);
	}
	constexpr Rect(Vector2D<type> pos, Vector2D<type> dim)
		:
		Rect(pos, dim.x, dim.y)
	{}
	template <typename rType>
	explicit constexpr Rect(const Rect<rType>& rect)
		:
		Rect(Vector2D<type>(rect.pos), (type)rect.width, (type)rect.height)
	{}
	template <typename rType>
	Rect& operator =(const Rect<rType>& rect)
	{
		pos = Vector2D<type>(rect.pos);
		width = (type)rect.width;
		height = (type)rect.height;
		return *this;
	}
	Rect operator +(const Vector2D<type>& delta_pos) const
	{
		return Rect(pos + delta_pos, width, height);
	}
	Rect& operator +=(const Vector2D<type>& delta_pos)
	{
		return *this = *this + delta_pos;
	}
	Rect operator -(const Vector2D<type>& delta_pos) const
	{
		return Rect(pos - delta_pos, width, height);
	}
	Rect& operator -=(const Vector2D<type>& delta_pos)
	{
		return *this = *this - delta_pos;
	}
	Rect operator *(const type& scalar) const
	{
		return Rect(pos, width * scalar, height * scalar);
	}
	Rect& operator *=(const type& scalar)
	{
		return *this = *this * scalar;
	}
	Rect operator *(const Vector2D<type>& scalar) const
	{
		return Rect(pos, width * scalar.x, height * scalar.y);
	}
	Rect& operator *=(const Vector2D<type>& scalar)
	{
		return *this = *this * scalar;
	}
	Rect operator /(const type& scalar) const
	{
		return Rect(pos, width / scalar, height / scalar);
	}
	Rect& operator /=(const type& scalar)
	{
		return *this = *this / scalar;
	}
	Rect operator /(const Vector2D<type>& scalar) const
	{
		return Rect(pos, width / scalar.x, height / scalar.y);
	}
	Rect& operator /=(const Vector2D<type>& scalar)
	{
		return *this = *this / scalar;
	}
	void Move(const Vector2D<type>& delta_pos)
	{
		pos += delta_pos;
	}
	void SetPosition(const Vector2D<type>& new_pos)
	{
		pos = new_pos;
	}
	const Vector2D<type>& GetPosition() const
	{
		return pos;
	}
	void SetWidth(const type& new_width)
	{
		assert(new_width >= ZERO);
		width = new_width;
	}
	const type& GetWidth() const
	{
		return width;
	}
	void SetHeight(const type& new_height)
	{
		assert(new_height >= ZERO);
		height = new_height;
	}
	const type& GetHeight() const
	{
		return height;
	}
	type GetAspectRatio() const
	{
		return width / height;
	}
	type GetInvAspectRatio() const
	{
		return height / width;
	}
	bool IsHorizontallyAllignedWith(const Rect& rect) const
	{
		return pos.x + width >= rect.pos.x && pos.x < rect.pos.x + rect.width;
	}
	bool IsVerticallyAllignedWith(const Rect& rect) const
	{
		return pos.y + height >= rect.pos.y && pos.y < rect.pos.y + rect.height;
	}
	bool TopIsTouching(const Rect& rect) const
	{
		return
		(
			IsHorizontallyAllignedWith(rect) &&
			pos.y < rect.pos.y + rect.height && pos.y >= rect.pos.y
		);
	}
	bool RightIsTouching(const Rect& rect) const
	{
		return
		(
			IsVerticallyAllignedWith(rect) &&
			pos.x + width >= rect.pos.x && pos.x + width < rect.pos.x + rect.width
		);
	}
	bool BottomIsTouching(const Rect& rect) const
	{
		return
		(
			IsHorizontallyAllignedWith(rect) &&
			pos.y + height >= rect.pos.y && pos.y + height < rect.pos.y + rect.height
		);
	}
	bool LeftIsTouching(const Rect& rect) const
	{
		return
		(
			IsVerticallyAllignedWith(rect) &&
			pos.x < rect.pos.x + rect.width && pos.x >= rect.pos.x
		);
	}
	bool IsTouching(const Rect& rect) const
	{
		return IsHorizontallyAllignedWith(rect) && IsVerticallyAllignedWith(rect);
	}
	bool IsContainedWithin(const Rect& rect) const
	{
		return 
		(
			pos.x >= rect.pos.x && pos.x + width < rect.pos.x + rect.width &&
			pos.y >= rect.pos.y && pos.y + height < rect.pos.y + rect.height
		);
	}
	bool ContainsPoint(const Vector2D<type>& point) const
	{
		return (point.x >= pos.x && point.x < pos.x + width && point.y >= pos.y && point.y < pos.y + height);
	}
	Rect ClippedTo(const Rect& rect) const
	{
		Rect clipped = *this;
		clipped.pos.x = clipped.pos.x * (clipped.pos.x >= rect.pos.x) + rect.pos.x * (clipped.pos.x < rect.pos.x);
		clipped.width -= clipped.pos.x - this->pos.x;
		clipped.pos.y = clipped.pos.y * (clipped.pos.y >= rect.pos.y) + rect.pos.y * (clipped.pos.y < rect.pos.y);
		clipped.height -= clipped.pos.y - this->pos.y;
		clipped.width = 
			clipped.width * (clipped.pos.x + clipped.width <= rect.pos.x + rect.width) + 
			(rect.pos.x + rect.width - clipped.pos.x) * (clipped.pos.x + clipped.width > rect.pos.x + rect.width);
		clipped.height = 
			clipped.height * (clipped.pos.y + clipped.height <= rect.pos.y + rect.height) + 
			(rect.pos.y + rect.height - clipped.pos.y) * (clipped.pos.y + clipped.height > rect.pos.y + rect.height);
		return clipped;
	}
	Rect& ClipTo(const Rect& rect)
	{
		return *this = this->ClippedTo(rect);
	}
	void Draw(Graphics& gfx, const Color& color, int layer = 0) const
	{
		const Rect<int> gfxRect = gfx.GetRect(layer);
		Rect<int> drawRect = Rect<int>(*this);
		assert(drawRect.IsTouching(gfxRect)); 
		drawRect.ClipTo(gfxRect);
		const int mapWidth = (int)gfx.GetWidth(layer);
		std::vector<Color>& pxlMap = gfx.GetPixelMap(layer);
		const auto yIterBegin = pxlMap.begin() + drawRect.pos.y * mapWidth;
		for (auto yIter = yIterBegin; yIter < yIterBegin + drawRect.height * mapWidth; yIter += mapWidth)
		{
			const auto xIterBegin = yIter + drawRect.pos.x;
			const auto xIterEnd = xIterBegin + drawRect.width;
			std::fill(xIterBegin, xIterEnd, color);
		}
	}
	void Draw(Graphics& gfx, std::function<Color(int, int)> color_func, int layer = 0) const
	{
		const Rect gfxRect = gfx.GetRect(layer);
		Rect<int> drawRect = Rect<int>(*this);
		assert(drawRect.IsTouching(gfxRect));
		drawRect.ClipTo(gfxRect);
		const int mapWidth = gfx.GetWidth(layer);
		for (int y = drawRect.pos.y; y < drawRect.pos.y + drawRect.height; ++y)
		{
			for (int x = drawRect.pos.x; x < drawRect.pos.x + drawRect.width; ++x)
			{
				gfx.SetPixel(x, y, color_func(x, y), layer);
			}
		}
	}
	void DrawOutline(Graphics& gfx, const Color& color, int layer = 0) const
	{
		const Rect<int> gfxRect = gfx.GetRect(layer);
		Rect<int> drawRect = Rect<int>(*this);
		assert(drawRect.IsTouching(gfxRect));
		const int bottomY = drawRect.pos.y + drawRect.height - 1;
		const int rightX = drawRect.pos.x + drawRect.width - 1;
		for (int x = drawRect.pos.x; x <= rightX; ++x)
		{
			if (gfxRect.ContainsPoint({ x,drawRect.pos.y }))
			{
				gfx.SetPixel(x, drawRect.pos.y, color, layer);
			}
			if (gfxRect.ContainsPoint({ x,bottomY }))
			{
				gfx.SetPixel(x, bottomY, color, layer);
			}
		}
		for (int y = drawRect.pos.y + 1; y < bottomY; ++y)
		{
			if (gfxRect.ContainsPoint({ drawRect.pos.x,y }))
			{
				gfx.SetPixel(drawRect.pos.x, y, color, layer);
			}
			if (gfxRect.ContainsPoint({ rightX,y }))
			{
				gfx.SetPixel(rightX, y, color, layer);
			}
		}
	}
};
using iRect = Rect<int>;
using uRect = Rect<int>;
using fRect = Rect<float>;



