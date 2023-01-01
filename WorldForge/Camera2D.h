#pragma once
#include "Vector.h"

class Camera2D
{
protected:
	vec2 position;
	float rotation;
	float zoom;
public:
	Camera2D();
	Camera2D(vec2 pos, float rot, float zoom);
	virtual void Move(vec2 delta);
	virtual void SetPosition(vec2 pos);
	virtual vec2 GetPosition() const;
	void Rotate(float radians);
	void SetRotation(float radians);
	const float& GetRotation() const;
	void Zoom(float zoom_factor);
	void SetZoom(float new_zoom);
	const float& GetZoom() const;
	mat3 GetTransformationMatrix() const;
	mat4 GetTransformationMatrix4D() const;
};
