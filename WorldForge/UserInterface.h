#pragma once
#include "GraphicText.h"
#include "Sprite.h"
#include <functional>

class UserInterface
{
public:
	class Object
	{
		friend class UserInterface;
	public:
		int x;
		int y;
		std::function<bool(std::unique_ptr<Object>&, float)> update;
		char* pData;
	public:
		Object() = delete;
		Object(const Object& obj) = delete;
		Object operator =(const Object& obj) = delete;
		Object(int x, int y, std::function<bool(std::unique_ptr<Object>&, float)> update = nullptr, char* pp_data = nullptr)
			:
			x(x),
			y(y),
			update(update),
			pData(pp_data)
		{}
		virtual void Draw(Graphics& gfx, int layer, GraphicText& gfc_text) const = 0;
		~Object()
		{
			pData = nullptr;
		}
	};
	class TextObject : public Object
	{
	public:
		std::string text;
		Color textColor;
		int textScale;
	public:
		TextObject(int x, int y, std::string text, Color text_color, int text_scale, std::function<bool(std::unique_ptr<Object>&, float)> update = nullptr, char* pp_data = nullptr)
			:
			Object(x, y, update, pp_data),
			text(text),
			textColor(text_color),
			textScale(text_scale)
		{
			assert(!text.empty());
		}
		const std::string& GetText() const
		{
			return text;
		}
		void SetText(const std::string& new_text)
		{
			text = new_text;
		}
		const Color& GetTextColor() const
		{
			return textColor;
		}
		void SetTextColor(const Color& new_color)
		{
			textColor = new_color;
		}
		const int& GetTextScale() const
		{
			return textScale;
		}
		void SetTextScale(int new_scale)
		{
			textScale = new_scale;
		}
	};
	class PlainText : public TextObject
	{
	public:
		PlainText(int x, int y, std::string text, Color text_color, int text_scale, std::function<bool(std::unique_ptr<Object>&, float)> update = nullptr, char* pp_data = nullptr)
			:
			TextObject(x, y, text, text_color, text_scale, update, pp_data)
		{}
		void Draw(Graphics& gfx, int layer, GraphicText& gfc_text) const override
		{
			gfc_text.SetTextColor(textColor);
			gfc_text.SetTextScale(textScale);
			gfc_text.FreeformWrite(text, x, y);
		}
	};
	class Button : public TextObject
	{
	public:
		Animation button;
		int border;
	public:
		Button(int x, int y, Animation button, int border, std::string text, Color text_color, int text_scale, std::function<bool(std::unique_ptr<Object>&, float)> update = nullptr, char* pp_data = nullptr)
			:
			TextObject(x, y, text, text_color, text_scale, update, pp_data),
			button(button),
			border(border)
		{
			assert(!text.empty());
		}
		const Animation& GetButton() const
		{
			return button;
		}
		const int& GetBorder() const
		{
			return border;
		}
		void Draw(Graphics& gfx, int layer, GraphicText& gfc_text) const override
		{
			button.Draw(gfx, x, y, layer);
			gfc_text.SetTextColor(textColor);
			gfc_text.SetTextScale(textScale);
			gfc_text.FreeformWrite(text, x + border, y + border);
		}
	};
	template <typename gfc>
	class Graphic : public Object
	{
	public:
		gfc graphic;
	public:
		Graphic(int x, int y, gfc graphic, std::function<bool(std::unique_ptr<Object>&, float)> update = nullptr, char* pp_data = nullptr)
			:
			Object(x, y, update, pp_data),
			graphic(graphic)
		{}
		const gfc& GetGraphic() const
		{
			return graphic;
		}
		void Draw(Graphics& gfx, int layer, GraphicText& gfc_text) const override
		{
			graphic.Draw(gfx, x, y, layer);
		}
	};
private:
	Graphics& gfx;
	int layer;
	GraphicText gfcText;
	std::vector<std::unique_ptr<Object>> pObjects;
	bool drawFlag;
public:
	UserInterface() = delete;
	UserInterface(Graphics& gfx, std::string text_font, char2 char_table_dim, int start_char, int layer = 0);
	void AddInterface(std::unique_ptr<Object>& pInterface);
	void Update(float time_ellapsed);
	void Draw();
};



