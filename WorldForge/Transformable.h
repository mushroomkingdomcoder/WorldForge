#pragma once
#include "Vector.h"

class Transformable
{
protected:
	vec2 pos;
	float rotation;
	vec2 scale;
public:
	Transformable();
	Transformable(vec2 pos, float rot, vec2 scale);
	virtual void Move(vec2 delta);
	virtual void SetPosition(vec2 new_pos);
	virtual const vec2& GetPosition() const;
	virtual void Rotate(float radians);
	virtual void SetRotation(float radians);
	virtual const float& GetRotation() const;
	virtual void Scale(vec2 scalar);
	virtual void SetScale(vec2 new_scale);
	virtual const vec2& GetScale() const;
	virtual mat3 GetTransformationMatrix() const;
};

