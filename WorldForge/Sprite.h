#pragma once
#include "Animation.h"
#include "Rect.h"

class Tile;

class Sprite
{
protected:
	vec2 position;
	std::vector<Animation>& animations;
	int currentAnimation;
	vec2 scale;
	fRect imageRect;
	std::optional<std::vector<fRect>> hitBoxes;
public:
	Sprite() = delete;
	Sprite(std::vector<Animation>& animations);
	Sprite(vec2 pos, std::vector<Animation>& animations, int startingAnimation, vec2 scale = { 1.0f,1.0f }, std::vector<fRect> hit_boxes = {});
	void Move(vec2 delta);
	void SetPosition(vec2 pos);
	const vec2& GetPosition() const;
	const float& GetWidth() const;
	const float& GetHeight() const;
	vec2 GetSize() const;
	Animation& GetAnimation(int index);
	const Animation& GetAnimation(int index) const;
	void SetAnimationIndex(int animation);
	const int& GetAnimationIndex() const;
	void Scale(vec2 scalar);
	void SetScale(vec2 scale);
	const vec2& GetScale() const;
	const fRect& GetRect() const;
	bool HasHitBoxes() const;
	const std::vector<fRect>& GetHitBoxes() const;
	bool CollidedWith(const Sprite& sprite) const;
	bool CollidedWith(const Tile& tile) const;
	const Image& GetCurrentImage() const;
	virtual const Image& Update(float time_ellapsed);
	virtual bool UpdateAndCheck(float time_ellapsed);
	virtual bool Draw(Graphics& gfx, int layer = 0) const;
	virtual bool DrawWithTransparency(Graphics& gfx, int layer = 0) const;
};

