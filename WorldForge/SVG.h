#pragma once
#include "Graphics.h"
#include "Transformable.h"
#include <vector>

class SVG : public Transformable
{
protected:
	std::vector<std::pair<vec2, vec2>> lineBuffer;
public:
	SVG() = delete;
	SVG(std::vector<std::pair<vec2, vec2>> line_buffer);
	SVG(std::vector<std::pair<vec2, vec2>> line_buffer, vec2 pos, float rotation, vec2 scale);
	const std::vector<std::pair<vec2, vec2>> GetLineBuffer() const;
public:
	static SVG GenerateLine(vec2 p0, vec2 p1);
	static SVG GeneratePolygon(int nSides, vec2 pos = { 0.0f,0.0f }, float rot = 0.0f, vec2 scale = { 1.0f,1.0f });
	static SVG GenerateStar(int nPoints, float inner_radius_ratio, vec2 pos = { 0.0f,0.0f }, float rot = 0.0f, vec2 scale = { 1.0f,1.0f });
};
