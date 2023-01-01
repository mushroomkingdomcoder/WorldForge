#include "Sprite.h"
#include "Tile.h"

Sprite::Sprite(std::vector<Animation>& animations)
	:
	position(0.0f, 0.0f),
	animations(animations),
	currentAnimation(0),
	scale(1.0f, 1.0f),
	hitBoxes()
{
	assert(!animations.empty());
	imageRect = fRect(position, (float)animations[currentAnimation].GetFrameWidth(), (float)animations[currentAnimation].GetFrameHeight());
#ifdef _DEBUG
	for (const Animation& a : animations)
	{
		for (const Animation& oa : animations)
		{
			assert(a.GetFrameSize() == oa.GetFrameSize());
		}
	}
#endif
}

Sprite::Sprite(vec2 pos, std::vector<Animation>& animations, int startingAnimation, vec2 scale, std::vector<fRect> hit_boxes)
	:
	position(pos),
	animations(animations),
	currentAnimation(startingAnimation),
	scale(scale),
	hitBoxes()
{
	if (!hit_boxes.empty())
	{
		hitBoxes.emplace(hit_boxes);
		for (fRect& hb : *hitBoxes)
		{
			hb *= scale;
			hb += position;
		}
	}
	assert(!animations.empty());
	assert(startingAnimation < animations.size());
	assert(scale.x > 0.0f && scale.y > 0.0f);
	imageRect = fRect(position, vec2(animations[currentAnimation].GetFrameSize()) * scale);
#ifdef _DEBUG
	for (const Animation& a : animations)
	{
		for (const Animation& oa : animations)
		{
			assert(a.GetFrameSize() == oa.GetFrameSize());
		}
	}
#endif
}

void Sprite::Move(vec2 delta)
{
	position += delta;
	imageRect += delta;
	if (hitBoxes)
	{
		for (fRect& hb : *hitBoxes)
		{
			hb += delta;
		}
	}
}

void Sprite::SetPosition(vec2 pos)
{
	position = pos;
	imageRect.SetPosition(position);
	if (hitBoxes)
	{
		for (fRect& hb : *hitBoxes)
		{
			hb.SetPosition(position);
		}
	}
}

const vec2& Sprite::GetPosition() const
{
	return position;
}

const float& Sprite::GetWidth() const
{
	return imageRect.GetWidth();
}

const float& Sprite::GetHeight() const
{
	return imageRect.GetHeight();
}

vec2 Sprite::GetSize() const
{
	return vec2(imageRect.GetWidth(), imageRect.GetHeight());
}

Animation& Sprite::GetAnimation(int index)
{
	assert(index < animations.size());
	return animations[index];
}

const Animation& Sprite::GetAnimation(int index) const
{
	assert(index < animations.size());
	return animations[index];
}

void Sprite::SetAnimationIndex(int animation)
{
	assert(animation < animations.size());
	currentAnimation = animation;
}

const int& Sprite::GetAnimationIndex() const
{
	return currentAnimation;
}

void Sprite::Scale(vec2 scalar)
{
	assert(scalar.x > 0.0f && scalar.y > 0.0f);
	imageRect *= scalar;
	if (hitBoxes)
	{
		for (fRect& hb : *hitBoxes)
		{
			hb *= scalar;
		}
	}
	this->scale *= scalar;
}

void Sprite::SetScale(vec2 scale)
{
	assert(scale.x > 0.0f && scale.y > 0.0f);
	imageRect *= scale / this->scale;
	if (hitBoxes)
	{
		for (fRect& hb : *hitBoxes)
		{
			hb *= scale / this->scale;
		}
	}
	this->scale = scale;
}

const vec2& Sprite::GetScale() const
{
	return scale;
}

const fRect& Sprite::GetRect() const
{
	return imageRect;
}

bool Sprite::HasHitBoxes() const
{
	return hitBoxes.has_value();
}

const std::vector<fRect>& Sprite::GetHitBoxes() const
{
	assert(hitBoxes.has_value());
	return *hitBoxes;
}

bool Sprite::CollidedWith(const Sprite& sprite) const
{
	if (hitBoxes)
	{
		if (sprite.hitBoxes)
		{
			for (const fRect& thb : *hitBoxes)
			{
				for (const fRect& ohb : *sprite.hitBoxes)
				{
					if (thb.IsTouching(ohb))
					{
						return true;
					}
				}
			}
			return false;
		}
		else
		{
			for (const fRect& thb : *hitBoxes)
			{
				if (thb.IsTouching(sprite.imageRect))
				{
					return true;
				}
			}
			return false;
		}
	}
	else
	{
		if (sprite.hitBoxes)
		{
			for (const fRect& ohb : *sprite.hitBoxes)
			{
				if (imageRect.IsTouching(ohb))
				{
					return true;
				}
			}
			return false;
		}
		else
		{
			return imageRect.IsTouching(sprite.imageRect);
		}
	}
}

bool Sprite::CollidedWith(const Tile& tile) const
{
	if (hitBoxes)
	{
		if (tile.HasHitBoxes())
		{
			for (const fRect& thb : *hitBoxes)
			{
				for (const fRect& ohb : tile.GetHitBoxes())
				{
					if (thb.IsTouching(ohb))
					{
						return true;
					}
				}
			}
			return false;
		}
		else
		{
			for (const fRect& thb : *hitBoxes)
			{
				if (thb.IsTouching(tile.GetRect()))
				{
					return true;
				}
			}
			return false;
		}
	}
	else
	{
		if (tile.HasHitBoxes())
		{
			for (const fRect& ohb : tile.GetHitBoxes())
			{
				if (imageRect.IsTouching(ohb))
				{
					return true;
				}
			}
			return false;
		}
		else
		{
			return imageRect.IsTouching(tile.GetRect());
		}
	}
}

const Image& Sprite::GetCurrentImage() const
{
	return animations[currentAnimation].GetCurrentFrame();
}

const Image& Sprite::Update(float time_ellapsed)
{
	return animations[currentAnimation].Play(time_ellapsed);
}

bool Sprite::UpdateAndCheck(float time_ellapsed)
{
	return animations[currentAnimation].PlayAndCheck(time_ellapsed);
}

bool Sprite::Draw(Graphics& gfx, int layer) const
{
	if (imageRect.IsTouching(gfx.GetRect_FLOAT(layer)))
	{
		if (scale != vec2(1.0f, 1.0f))
		{
			animations[currentAnimation].GetCurrentFrame().Draw(gfx, (int)position.x, (int)position.y, (int)imageRect.GetWidth(), (int)imageRect.GetHeight(), layer);
		}
		else
		{
			animations[currentAnimation].GetCurrentFrame().Draw(gfx, (int)position.x, (int)position.y, layer);
		}
		return true;
	}
	else
	{
		return false;
	}
}

bool Sprite::DrawWithTransparency(Graphics& gfx, int layer) const
{
	if (imageRect.IsTouching(gfx.GetRect_FLOAT(layer)))
	{
		if (scale != vec2(1.0f, 1.0f))
		{
			animations[currentAnimation].GetCurrentFrame().DrawWithTransparency(gfx, (int)position.x, (int)position.y, (int)imageRect.GetWidth(), (int)imageRect.GetHeight(), layer);
		}
		else
		{
			animations[currentAnimation].GetCurrentFrame().DrawWithTransparency(gfx, (int)position.x, (int)position.y, layer);
		}
		return true;
	}
	else
	{
		return false;
	}
}

