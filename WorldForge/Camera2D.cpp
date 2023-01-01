#include "Camera2D.h"

Camera2D::Camera2D()
	:
	position(0.0f, 0.0f),
	rotation(0.0f),
	zoom(1.0f)
{}

Camera2D::Camera2D(vec2 pos, float rot, float zoom)
	:
	position(pos),
	rotation(rot),
	zoom(zoom)
{}

void Camera2D::Move(vec2 delta)
{
	position += delta.Rotated(rotation) * zoom;
}

void Camera2D::SetPosition(vec2 pos)
{
	position = pos;
}

vec2 Camera2D::GetPosition() const
{
	return position;
}

void Camera2D::Rotate(float radians)
{
	rotation += radians;
}

void Camera2D::SetRotation(float radians)
{
	rotation = radians;
}

const float& Camera2D::GetRotation() const
{
	return rotation;
}

void Camera2D::Zoom(float zoom_factor)
{
	zoom *= zoom_factor;
}

void Camera2D::SetZoom(float new_zoom)
{
	zoom = new_zoom;
}

const float& Camera2D::GetZoom() const
{
	return zoom;
}

mat3 Camera2D::GetTransformationMatrix() const
{
	return
		mat3::Translation(-position.x, -position.y) *
		mat3::RotationZ(-rotation) *
		mat3::Scaling(zoom, zoom, 1.0f);
}

mat4 Camera2D::GetTransformationMatrix4D() const
{
	return mat4(GetTransformationMatrix());
}

