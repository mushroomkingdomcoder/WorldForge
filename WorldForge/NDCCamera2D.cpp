#include "NDCCamera2D.h"

NDCCamera2D::NDCCamera2D(vec2 gfx_dim)
	:
	Camera2D(),
	halfGfxDim(gfx_dim / 2.0f)
{}

NDCCamera2D::NDCCamera2D(vec2 gfx_dim, vec2 pos, float rot, float zoom)
	:
	Camera2D(pos, rot, zoom),
	halfGfxDim(gfx_dim / 2.0f)
{}

void NDCCamera2D::Move(vec2 delta)
{
	position += (delta / halfGfxDim).Rotated(rotation) * zoom;
}

void NDCCamera2D::SetPosition(vec2 pos)
{
	position = pos / halfGfxDim;
}

vec2 NDCCamera2D::GetPosition() const
{
	return position * halfGfxDim;
}

