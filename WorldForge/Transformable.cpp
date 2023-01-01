#include "Transformable.h"

Transformable::Transformable()
	:
	pos(0.0f, 0.0f),
	rotation(0.0f),
	scale(1.0f, 1.0f)
{}

Transformable::Transformable(vec2 pos, float rot, vec2 scale)
	:
	pos(pos),
	rotation(rot),
	scale(scale)
{}

void Transformable::Move(vec2 delta)
{
	pos += delta;
}

void Transformable::SetPosition(vec2 new_pos)
{
	pos = new_pos;
}

const vec2& Transformable::GetPosition() const
{
	return pos;
}

void Transformable::Rotate(float radians)
{
	rotation += radians;
}

void Transformable::SetRotation(float radians)
{
	rotation = radians;
}

const float& Transformable::GetRotation() const
{
	return rotation;
}

void Transformable::Scale(vec2 scalar)
{
	assert(scalar.x > 0.0f && scalar.y > 0.0f);
	scale *= scalar;
}

void Transformable::SetScale(vec2 new_scale)
{
	assert(new_scale.x > 0.0f && new_scale.y > 0.0f);
	scale = new_scale;
}

const vec2& Transformable::GetScale() const
{
	return scale;
}

mat3 Transformable::GetTransformationMatrix() const
{
	return
		mat3::RotationZ(rotation) *
		mat3::Scaling(scale.x, scale.y, 1.0f) *
		mat3::Translation(pos.x, pos.y);
}