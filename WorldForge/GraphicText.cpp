#include "GraphicText.h"
#include <assert.h>

GraphicText::GraphicText(Color* p_paper, int2 paper_dim)
	:
	pPaper(p_paper),
	paperDim(paper_dim),
	CharacterWidth(16),
	CharacterHeight(16),
	startChar(32),
	charTableDim({ 16,6 }),
	cursorLimit({ paperDim.x / CharacterWidth, paperDim.y / CharacterHeight }),
	tlMargins({ 0,0 }),
	brMargins({ 0,0 }),
	Cursor({ 0,0 }),
	TextColor(Colors::White),
	TextScale(1),
	autoCursor(true),
	lineFeed(true),
	lineSpacing(1),
	TextTexture(),
	isUsingTexture(TextTexture)
{
	Image defaultCharset = Image("charsets\\default.bmp");
	CharBitmaps.resize(charTableDim.x * charTableDim.y);
	for (std::vector<bool>& cb : CharBitmaps)
	{
		cb.resize(CharacterWidth * CharacterHeight);
	}
	for (int i = 0; i < CharBitmaps.size(); ++i)
	{
		Image charImage = defaultCharset.Cropped(CharacterWidth, CharacterHeight, (i % charTableDim.x) * CharacterWidth, (i / charTableDim.x) * CharacterHeight);
		for (int y = 0; y < CharacterHeight; ++y)
		{
			for (int x = 0; x < CharacterWidth; ++x)
			{
				CharBitmaps[i][y * CharacterWidth + x] = (charImage.GetPixel(x, y) != Colors::White);
			}
		}
	} 
}

GraphicText::GraphicText(Color* p_paper, int2 paper_dim, Image charset, char2 charTableDim, char startChar, int2 cursor_pos, Color txtColor, int txtScale, bool doubleSpaced, bool auto_cursor, bool line_feed, int2 tl_margins, int2 br_margins, std::optional<Image> txtTexture)
	:
	pPaper(p_paper),
	paperDim(paper_dim),
	CharacterWidth(charset.GetWidth() / charTableDim.x),
	CharacterHeight(charset.GetHeight() / charTableDim.y),
	startChar(startChar),
	charTableDim(charTableDim),
	cursorLimit({ paperDim.x / (CharacterWidth * txtScale),paperDim.y / (CharacterHeight * txtScale) }),
	tlMargins(tl_margins),
	brMargins(br_margins),
	Cursor(cursor_pos),
	TextColor(txtColor),
	TextScale(txtScale),
	autoCursor(auto_cursor),
	lineFeed(line_feed),
	lineSpacing(1 + doubleSpaced),
	TextTexture(txtTexture),
	isUsingTexture(TextTexture)
{
	assert(tl_margins.x + br_margins.x < cursorLimit.x);
	assert(tl_margins.y + br_margins.y < cursorLimit.y / 2);
	assert(cursor_pos.x >= tlMargins.x);
	assert(cursor_pos.y >= tlMargins.y);
	assert(cursor_pos.x < cursorLimit.x - brMargins.x);
	assert(cursor_pos.y < cursorLimit.y - brMargins.y);
	if (TextTexture)
	{
		assert(TextTexture->GetWidth() == CharacterWidth);
		assert(TextTexture->GetHeight() == CharacterHeight);
	}
	CharBitmaps.resize(charTableDim.x * charTableDim.y);
	for (std::vector<bool>& cb : CharBitmaps)
	{
		cb.resize(CharacterWidth * CharacterHeight);
	}
	for (int i = 0; i < CharBitmaps.size(); ++i)
	{
		Image charImage = charset.Cropped(CharacterWidth, CharacterHeight, (i % charTableDim.x) * CharacterWidth, (i / charTableDim.x) * CharacterHeight);
		for (int y = 0; y < CharacterHeight; ++y)
		{
			for (int x = 0; x < CharacterWidth; ++x)
			{
				CharBitmaps[i][y * CharacterWidth + x] = (charImage.GetPixel(x, y) != Colors::White);
			}
		}
	}
}

void GraphicText::SetTopLeftMargins(const int2& tl_margins)
{
	assert(tl_margins.x + brMargins.x < cursorLimit.x);
	assert(tl_margins.y + brMargins.y < cursorLimit.y);
	if (Cursor.x < tl_margins.x)
	{
		Cursor.x = tl_margins.x;
	}
	if (Cursor.y < tl_margins.y)
	{
		Cursor.y = tl_margins.y;
	}
	tlMargins = tl_margins;
	ClearText();
}

void GraphicText::SetBottomRightMargins(const int2& br_margins)
{
	assert(tlMargins.x + br_margins.x < cursorLimit.x);
	assert(tlMargins.y + br_margins.y < cursorLimit.y);
	if (Cursor.x >= cursorLimit.x - br_margins.x)
	{
		Cursor.x = cursorLimit.x - br_margins.x - 1;
	}
	if (Cursor.y >= cursorLimit.y - br_margins.y)
	{
		Cursor.y = cursorLimit.y - br_margins.y - 1;
	}
	brMargins = br_margins;
	ClearText();
}

const int2& GraphicText::GetTopLeftMargins() const
{
	return tlMargins;
}

const int2& GraphicText::GetBottomRightMargins() const
{
	return brMargins;
}

void GraphicText::CursorUp()
{
	for (int i = 0; i < lineSpacing; ++i)
	{
		if (Cursor.y == tlMargins.y)
		{
			if (!lineFeed)
			{
				Cursor.y = cursorLimit.y - brMargins.y - 1;
			}
			else
			{
				LineFeedDown();
			}
		}
		else
		{
			--Cursor.y;
		}
	}
}

void GraphicText::CursorRight()
{
	if (Cursor.x == cursorLimit.x - brMargins.x - 1)
	{
		Cursor.x = tlMargins.x;
		CursorDown();
	}
	else
	{
		++Cursor.x;
	}
}

void GraphicText::CursorDown()
{
	for (int i = 0; i < lineSpacing; ++i)
	{
		if (Cursor.y == cursorLimit.y - brMargins.y - 1)
		{
			if (!lineFeed)
			{
				Cursor.y = tlMargins.y;
			}
			else
			{
				LineFeedUp();
			}
		}
		else
		{
			++Cursor.y;
		}
	}
}

void GraphicText::CursorLeft()
{
	if (Cursor.x == tlMargins.x)
	{
		Cursor.x = cursorLimit.x - brMargins.x - 1;
		CursorUp();
	}
	else
	{
		--Cursor.x;
	}
}

void GraphicText::SetCursorPosition(const int2& new_pos)
{
	assert(new_pos.x >= tlMargins.x);
	assert(new_pos.y >= tlMargins.y);
	assert(new_pos.x < cursorLimit.x - brMargins.x);
	assert(new_pos.y < cursorLimit.y - brMargins.y);
	Cursor = new_pos;
}

const int2& GraphicText::GetCursorPosition() const
{
	return Cursor;
}

void GraphicText::SetTextColor(const Color& color)
{
	TextColor = color;
	isUsingTexture = false;
}

const Color& GraphicText::GetTextColor() const
{
	return TextColor;
}

void GraphicText::SetTextScale(int scale)
{
	assert(CharacterWidth * scale < (paperDim.x - ((tlMargins.x + brMargins.x) * scale)));
	assert(CharacterHeight * scale < (paperDim.y - ((tlMargins.y + brMargins.y) * scale)));
	TextScale = scale;
	cursorLimit = { paperDim.x / (CharacterWidth * TextScale),paperDim.y / (CharacterHeight * TextScale) };
}

const int& GraphicText::GetTextScale() const
{
	return TextScale;
}

void GraphicText::SetManualCursor()
{
	autoCursor = false;
}

void GraphicText::SetAutoCursor()
{
	autoCursor = true;
}

const bool& GraphicText::AutoCursorIsOn() const
{
	return autoCursor;
}

void GraphicText::SetTextWrapping()
{
	lineFeed = false;
}

void GraphicText::SetLineFeed()
{
	lineFeed = true;
}

const bool& GraphicText::LineFeedIsEnabled() const
{
	return lineFeed;
}

void GraphicText::SetDoubleSpacing()
{
	lineSpacing = 2;
}

void GraphicText::SetSingleSpacing()
{
	lineSpacing = 1;
}

bool GraphicText::IsDoubleSpaced() const
{
	return lineSpacing == 2;
}

void GraphicText::SetTextTexture(const Image& image)
{
	assert(image.GetWidth() == CharacterWidth);
	assert(image.GetHeight() == CharacterHeight);
	TextTexture = image;
	isUsingTexture = true;
}

const Image& GraphicText::GetTextTexture() const
{
	assert(TextTexture);
	return *TextTexture;
}

void GraphicText::UseTextTexture()
{
	assert(TextTexture);
	isUsingTexture = true;
}

bool GraphicText::isUsingTextTexture() const
{
	return isUsingTexture;
}

void GraphicText::UseTextColor()
{
	isUsingTexture = false;
}

bool GraphicText::isUsingTextColor() const
{
	return !isUsingTexture;
}

void GraphicText::Write(std::string text)
{
	int Y, X;
	for (int i = 0; i < text.length(); ++i)
	{
		bool isBackspace = false;
		bool isDelete = false;
		if (text[i] == '\b')
		{
			CursorLeft();
			text[i] = ' ';
			isBackspace = true;
		}
		else if (text[i] == 0x7F)
		{
			text[i] = ' ';
			isDelete = true;
		}
		else if (text[i] == '\r')
		{
			CursorDown();
			Cursor.x = tlMargins.x;
			continue;
		}
		assert(text[i] >= startChar);
		assert(text[i] < startChar + charTableDim.x * charTableDim.y);
		text[i] -= startChar;
		Y = Cursor.y * (CharacterHeight * TextScale);
		X = Cursor.x * (CharacterWidth * TextScale);
		for (int y = 0; y < CharacterHeight * TextScale; ++y)
		{
			const int yPxl = (Y + y) * paperDim.x;
			const int yBit = (y / TextScale) * CharacterWidth;
			for (int x = 0; x < CharacterWidth * TextScale; ++x)
			{
				if (isUsingTexture)
				{
					const int xBit = x / TextScale;
					pPaper[yPxl + (X + x)] = (TextTexture->GetPtrToImage()[yBit + xBit] * (float)CharBitmaps[text[i]][yBit + xBit]);
				}
				else
				{
					pPaper[yPxl + (X + x)] = (TextColor * (float)CharBitmaps[text[i]][yBit + (x / TextScale)]);
				}
			}
		}
		if (isBackspace)
		{
			continue;
		}
		else
		{
			if (autoCursor && !isDelete)
			{
				CursorRight();
			}
		}
	}
}

void GraphicText::FreeformWrite(std::string text, int x, int y)
{
	int Y = y;
	for (int i = 0; i < text.length(); ++i)
	{
		assert(text[i] >= startChar);
		assert(text[i] < startChar + charTableDim.x * charTableDim.y);
		text[i] -= startChar;
		int X = x + i * (CharacterWidth * TextScale);
		for (int y = 0; y < CharacterHeight * TextScale; ++y)
		{
			const int yPxl = (Y + y) * paperDim.x;
			const int yBit = (y / TextScale) * CharacterWidth;
			for (int x = 0; x < CharacterWidth * TextScale; ++x)
			{
				if (isUsingTexture)
				{
					const int xBit = x / TextScale;
					pPaper[yPxl + (X + x)] = (TextTexture->GetPtrToImage()[yBit + xBit] * (float)CharBitmaps[text[i]][yBit + xBit]);
				}
				else
				{
					pPaper[yPxl + (X + x)] = (TextColor * (float)CharBitmaps[text[i]][yBit + (x / TextScale)]);
				}
			}
		}
	}
}

void GraphicText::PutChar(const char& chr, int2 pos)
{
	assert(pos.x >= tlMargins.x);
	assert(pos.y >= tlMargins.y);
	assert(pos.x < cursorLimit.x - brMargins.x);
	assert(pos.y < cursorLimit.y - brMargins.y);
	int2 cursorStore = Cursor;
	Cursor = pos;
	Write(&chr);
	Cursor = cursorStore;
}

void GraphicText::LineFeedUp()
{
	const int lineHeight = (CharacterHeight * TextScale);
	int startY = (tlMargins.y + 1) * lineHeight;
	const int startX = tlMargins.x * lineHeight;
	const int endY = (cursorLimit.y - brMargins.y) * lineHeight;
	const int endX = (cursorLimit.x - brMargins.x) * lineHeight;
	const int pitchBytes = (endX - startX) * sizeof(Color);
	for (int y = startY; y < endY; ++y)
	{
		memcpy(&pPaper[(y - lineHeight) * paperDim.x + startX], &pPaper[y * paperDim.x + startX], pitchBytes);
	}
	startY = endY - lineHeight;
	for (int y = startY; y < endY; ++y)
	{
		memset(&pPaper[y * paperDim.x + startX], 0, pitchBytes);
	}
}

void GraphicText::LineFeedDown()
{
	const int lineHeight = (CharacterHeight * TextScale);
	int startY = (cursorLimit.y - brMargins.y - 1) * lineHeight;
	const int startX = tlMargins.x * lineHeight;
	const int endY = tlMargins.y * lineHeight;
	const int endX = (cursorLimit.x - brMargins.x) * lineHeight;
	const int pitchBytes = (endX - startX) * sizeof(Color);
	for (int y = startY - 1; y >= endY; --y)
	{
		memcpy(&pPaper[(y + lineHeight) * paperDim.x + startX], &pPaper[y * paperDim.x + startX], pitchBytes);
	}
	startY = endY + lineHeight;
	for (int y = startY - 1; y >= endY; --y)
	{
		memset(&pPaper[y * paperDim.x + startX], 0, pitchBytes);
	}
}

void GraphicText::ClearText()
{
	const int lineHeight = (CharacterHeight * TextScale);
	const int startY = tlMargins.y * lineHeight;
	const int startX = tlMargins.x * lineHeight;
	const int endY = (cursorLimit.y - brMargins.y) * lineHeight;
	const int endX = (cursorLimit.x - brMargins.x) * lineHeight;
	const int pitchBytes = (endX - startX) * sizeof(Color);
	for (int y = startY; y < endY; ++y)
	{
		memset(&pPaper[y * paperDim.x + startX], 0, pitchBytes);
	}
	Cursor = { tlMargins.x,tlMargins.y };
}

GraphicText::~GraphicText()
{
	pPaper = nullptr;
}
