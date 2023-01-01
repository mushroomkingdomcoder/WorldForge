#include "TypeWriter.h"

TypeWriter::TypeWriter(GraphicText& src)
	:
	target(src)
{
	target.SetTextColor(currentColor);
}

TypeWriter::TypeWriter(GraphicText& src, Color color, int chars_per_second, std::string text)
	:
	target(src),
	currentColor(color),
	secPerChar(1.0f / (float)chars_per_second),
	textBuffer(text)
{
	target.SetTextColor(currentColor);
}

void TypeWriter::ChangeColor(Color color)
{
	currentColor = color;
	target.SetTextColor(currentColor);
}

void TypeWriter::ChangeSpeed(int chars_per_second)
{
	secPerChar = 1.0f / (float)chars_per_second;
}

bool TypeWriter::BindNewBuffer(std::string text)
{
	bool kosher = textBuffer.empty();
	textBuffer = std::move(text);
	return kosher;
}

bool TypeWriter::Update(float time_ellapsed)
{
	if (!textBuffer.empty())
	{
		curCharTime += time_ellapsed;
		while (curCharTime >= secPerChar)
		{
			curCharTime -= secPerChar;
			target.Write(textBuffer.substr(curChar, 1));
			++curChar;
			if (curChar >= textBuffer.length())
			{
				textBuffer = "";
				curCharTime = 0.0f;
				curChar = 0;
			}
		}
		return true;
	}
	else
	{
		return false;
	}
}

const Color& TypeWriter::GetColor() const
{
	return currentColor;
}

int TypeWriter::GetCharactersPerSec() const
{
	return int(1.0f / secPerChar);
}

const char& TypeWriter::GetCurrentCharacter() const
{
	assert(!textBuffer.empty());
	return textBuffer.at(curChar);
}

const std::string& TypeWriter::GetTextBuffer() const
{
	return textBuffer;
}


