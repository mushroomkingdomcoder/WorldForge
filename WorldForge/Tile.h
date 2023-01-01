#pragma once
#include "Animation.h"
#include "Rect.h"

class Tile
{
protected:
	vec2 position;
	Animation& image;
	fRect imageRect;
	vec2 scale;
	std::optional<std::vector<fRect>> hitBoxes;
public:
	Tile() = delete;
	Tile(Animation& animation);
	Tile(vec2 pos, Animation& animation, vec2 scale = { 1.0f,1.0f }, std::vector<fRect> hit_boxes = {});
	void Move(vec2 delta);
	void SetPosition(vec2 pos);
	const vec2& GetPosition() const;
	const float& GetWidth() const;
	const float& GetHeight() const;
	vec2 GetSize() const;
	Animation& GetAnimation();
	const Animation& GetAnimation() const;
	const fRect& GetRect() const;
	void Scale(vec2 scalar);
	void SetScale(vec2 scale);
	const vec2& GetScale();
	bool HasHitBoxes() const;
	const std::vector<fRect>& GetHitBoxes() const;
	bool CollidedWith(const Tile& tile) const;
	const Image& GetImage() const;
	virtual const Image& Update(float time_ellapsed);
	virtual bool UpdateAndCheck(float time_ellapsed);
	virtual bool Draw(Graphics& gfx, int layer = 0) const;
	virtual bool DrawWithTransparency(Graphics& gfx, int layer = 0) const;
};


