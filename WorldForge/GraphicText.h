#pragma once
#include "Image.h"
#include "Vector.h"
#include <vector>
#include <optional>

class GraphicText
{
private:
	Color* pPaper;
	int2 paperDim;
	const char CharacterWidth;
	const char CharacterHeight;
	std::vector<std::vector<bool>> CharBitmaps;
	const char startChar;
	const char2 charTableDim;
	int2 cursorLimit;
	int2 tlMargins;
	int2 brMargins;
	int2 Cursor;
	Color TextColor;
	int TextScale;
	bool autoCursor;
	bool lineFeed;
	int lineSpacing;
	std::optional<Image> TextTexture;
	bool isUsingTexture;
public:
	GraphicText() = delete;
	GraphicText(const GraphicText& gfc_text) = delete;
	GraphicText operator =(const GraphicText& gfc_text) = delete;
	GraphicText(Color* p_paper, int2 paper_dim);
	GraphicText(Color* p_paper, int2 paper_dim, Image charset, char2 charTableDim, char startChar, int2 cursor_pos = { 0,0 }, Color txtColor = Colors::White, int txtScale = 1, bool double_spaced = false, bool autoCursor = true, bool lineFeed = true, int2 tl_margins = { 0,0 }, int2 br_margins = { 0,0 }, std::optional<Image> txtTexture = std::optional<Image>());
	void SetTopLeftMargins(const int2& tl_margins);
	void SetBottomRightMargins(const int2& br_margins);
	const int2& GetTopLeftMargins() const;
	const int2& GetBottomRightMargins() const;
	void CursorUp();
	void CursorRight();
	void CursorDown();
	void CursorLeft();
	void SetCursorPosition(const int2& new_pos);
	const int2& GetCursorPosition() const;
	void SetTextColor(const Color& color);
	const Color& GetTextColor() const;
	void SetTextScale(int scale);
	const int& GetTextScale() const;
	void SetManualCursor();
	void SetAutoCursor();
	const bool& AutoCursorIsOn() const;
	void SetTextWrapping();
	void SetLineFeed();
	const bool& LineFeedIsEnabled() const;
	void SetDoubleSpacing();
	void SetSingleSpacing();
	bool IsDoubleSpaced() const;
	void SetTextTexture(const Image& image);
	const Image& GetTextTexture() const;
	void UseTextTexture();
	bool isUsingTextTexture() const;
	void UseTextColor();
	bool isUsingTextColor() const;
	void Write(std::string text);
	void FreeformWrite(std::string text, int x, int y);
	void PutChar(const char& chr, int2 pos);
	void LineFeedUp();
	void LineFeedDown();
	void ClearText();
	~GraphicText();
};