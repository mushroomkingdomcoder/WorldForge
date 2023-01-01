#pragma once
#include "Camera2D.h"

class NDCCamera2D : public Camera2D
{
private:
	const vec2 halfGfxDim;
public:
	NDCCamera2D() = delete;
	NDCCamera2D(vec2 gfx_dim);
	NDCCamera2D(vec2 gfx_dim, vec2 pos, float rot, float zoom);
	void Move(const vec2 delta) override;
	void SetPosition(vec2 pos) override;
	vec2 GetPosition() const override;
};