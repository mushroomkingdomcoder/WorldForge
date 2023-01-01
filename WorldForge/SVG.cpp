#include "SVG.h"
#include "Math.h"

SVG::SVG(std::vector<std::pair<vec2, vec2>> line_buffer)
	:
	Transformable(),
	lineBuffer(line_buffer)
{
	assert(!line_buffer.empty());
}

SVG::SVG(std::vector<std::pair<vec2, vec2>> line_buffer, vec2 pos, float rotation, vec2 scale)
	:
	Transformable(pos, rotation, scale),
	lineBuffer(line_buffer)
{
	assert(!line_buffer.empty());
}

const std::vector<std::pair<vec2, vec2>> SVG::GetLineBuffer() const
{
	return lineBuffer;
}

SVG SVG::GenerateLine(vec2 p0, vec2 p1)
{
	std::vector<std::pair<vec2, vec2>> lineBuffer;
	lineBuffer.emplace_back(p0, p1);
	vec2 pos{ (p1.x + p0.x) / 2,(p1.y + p0.y) / 2 };
	return SVG(lineBuffer, pos, 0.0f, { 1.0f,1.0f });
}

SVG SVG::GeneratePolygon(int nSides, vec2 pos, float rot, vec2 scale)
{
	std::vector<std::pair<vec2, vec2>> lineBuffer;
	float radians_per_angle = 2.0f * (float)M_PI / (float)nSides;
	vec2 last_point{ 1.0f,0.0f };
	float current_radians = 0.0f;
	for (int i = 1; i <= nSides; ++i)
	{
		current_radians = (float)i * radians_per_angle;
		vec2 this_point{ cosf(current_radians),sinf(current_radians) };
		lineBuffer.emplace_back(last_point, this_point);
		last_point = this_point;
	}
	return SVG(lineBuffer, pos, rot, scale);
}

SVG SVG::GenerateStar(int nPoints, float inner_radius_ratio, vec2 pos, float rot, vec2 scale)
{
	std::vector<std::pair<vec2, vec2>> lineBuffer;
	float radians_per_angle = 2.0f * (float)M_PI / (float)nPoints;
	float half_radians_per_angle = radians_per_angle / 2.0f;
	vec2 last_point{ 1.0f,0.0f };
	float current_radians = 0.0f;
	for (int i = 1; i <= nPoints; ++i)
	{
		current_radians += half_radians_per_angle;
		vec2 inner_point{ cosf(current_radians) * inner_radius_ratio,sinf(current_radians) * inner_radius_ratio };
		lineBuffer.emplace_back(last_point, inner_point);
		current_radians = (float)i * radians_per_angle;
		vec2 outer_point{ cosf(current_radians),sinf(current_radians) };
		lineBuffer.emplace_back(inner_point, outer_point);
		last_point = outer_point;
	}
	return SVG(lineBuffer, pos, rot, scale);
}


