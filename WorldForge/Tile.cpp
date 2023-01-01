#include "Tile.h"

Tile::Tile(Animation& animation)
	:
	position(0.0f, 0.0f),
	image(animation),
	imageRect(position, (float)animation.GetFrameWidth(), (float)animation.GetFrameHeight()),
	scale(1.0f, 1.0f),
	hitBoxes()
{}

Tile::Tile(vec2 pos, Animation& animation, vec2 scale, std::vector<fRect> hit_boxes)
	:
	position(pos),
	image(animation),
	imageRect(position, vec2(animation.GetFrameSize()) * scale),
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
	assert(scale.x > 0.0f && scale.y > 0.0f);
}

void Tile::Move(vec2 delta)
{
	imageRect += delta;
	if (hitBoxes)
	{
		for (fRect& hb : *hitBoxes)
		{
			hb += delta;
		}
	}
	position += delta;
}

void Tile::SetPosition(vec2 pos)
{
	imageRect.SetPosition(pos);
	if (hitBoxes)
	{
		for (fRect& hb : *hitBoxes)
		{
			hb.SetPosition(pos);
		}
	}
	position = pos;
}

const vec2& Tile::GetPosition() const
{
	return position;
}

const float& Tile::GetWidth() const
{
	return imageRect.GetWidth();
}

const float& Tile::GetHeight() const
{
	return imageRect.GetHeight();
}

vec2 Tile::GetSize() const
{
	return vec2(imageRect.GetWidth(), imageRect.GetHeight());
}

Animation& Tile::GetAnimation()
{
	return image;
}

const Animation& Tile::GetAnimation() const
{
	return image;
}

const fRect& Tile::GetRect() const
{
	return imageRect;
}

void Tile::Scale(vec2 scalar)
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
	scale *= scalar;
}

void Tile::SetScale(vec2 scale)
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

const vec2& Tile::GetScale()
{
	return scale;
}

bool Tile::HasHitBoxes() const
{
	return hitBoxes.has_value();
}

const std::vector<fRect>& Tile::GetHitBoxes() const
{
	assert(hitBoxes.has_value());
	return *hitBoxes;
}

bool Tile::CollidedWith(const Tile& tile) const
{
	if (hitBoxes)
	{
		if (tile.hitBoxes)
		{
			for (const fRect& thb : *hitBoxes)
			{
				for (const fRect& ohb : *tile.hitBoxes)
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
				if (thb.IsTouching(tile.imageRect))
				{
					return true;
				}
			}
			return false;
		}
	}
	else
	{
		if (tile.hitBoxes)
		{
			for (const fRect& ohb : *tile.hitBoxes)
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
			return imageRect.IsTouching(tile.imageRect);
		}
	}
}

const Image& Tile::GetImage() const
{
	return image.GetCurrentFrame();
}

const Image& Tile::Update(float time_ellapsed)
{
	return image.Play(time_ellapsed);
}

bool Tile::UpdateAndCheck(float time_ellapsed)
{
	return image.PlayAndCheck(time_ellapsed);
}

bool Tile::Draw(Graphics& gfx, int layer) const
{
	if (imageRect.IsTouching(gfx.GetRect_FLOAT(layer)))
	{
		if (scale != vec2(1.0f, 1.0f))
		{
			image.GetCurrentFrame().Draw(gfx, (int)position.x, (int)position.y, (int)GetWidth(), (int)GetHeight(), layer);
		}
		else
		{
			image.GetCurrentFrame().Draw(gfx, (int)position.x, (int)position.y, layer);
		}
		return true;
	}
	else
	{
		return false;
	}
}

bool Tile::DrawWithTransparency(Graphics& gfx, int layer) const
{
	if (imageRect.IsTouching(gfx.GetRect_FLOAT(layer)))
	{
		if (scale != vec2(1.0f, 1.0f))
		{
			image.GetCurrentFrame().DrawWithTransparency(gfx, (int)position.x, (int)position.y, (int)GetWidth(), (int)GetHeight(), layer);
		}
		else
		{
			image.GetCurrentFrame().DrawWithTransparency(gfx, (int)position.x, (int)position.y, layer);
		}
		return true;
	}
	else
	{
		return false;
	}
}