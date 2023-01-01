#pragma once
#include "GraphicText.h"

class TypeWriter
{
private:
	GraphicText& target;
	Color currentColor = Colors::White;
	float secPerChar = 0.1f;
	float curCharTime = 0.0f;
	int curChar = 0;
	std::string textBuffer;
public:
	TypeWriter() = delete;
	TypeWriter(GraphicText& src);
	TypeWriter(GraphicText& src, Color color, int chars_per_second, std::string text);
	void ChangeColor(Color color);
	void ChangeSpeed(int chars_per_second);
	bool BindNewBuffer(std::string text);
	bool Update(float time_ellapsed);
	const Color& GetColor() const;
	int GetCharactersPerSec() const;
	const char& GetCurrentCharacter() const;
	const std::string& GetTextBuffer() const;
};