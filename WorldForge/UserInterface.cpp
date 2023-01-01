#include "UserInterface.h"

UserInterface::UserInterface(Graphics& gfx, std::string text_font, char2 char_table_dim, int start_char, int layer)
	:
	gfx(gfx),
	layer(layer),
	gfcText(gfx.GetPixelMap(layer).data(), gfx.GetDimensions(layer).GetVStruct(), Image(text_font.c_str()), char_table_dim, start_char),
	pObjects(),
	drawFlag(true)
{}

void UserInterface::AddInterface(std::unique_ptr<Object>& pInterface)
{
	assert(pInterface != nullptr);
	pObjects.emplace_back(std::move(pInterface));
}

void UserInterface::Update(float time_ellapsed)
{
	for (auto& pObject : pObjects)
	{
		if (pObject->update)
		{
			if (pObject->update(pObject, time_ellapsed))
			{
				drawFlag = true;
			}
		}
	}
}

void UserInterface::Draw()
{
	if (drawFlag)
	{
		for (auto& pObject : pObjects)
		{
			pObject->Draw(gfx, layer, gfcText);
		}
		drawFlag = false;
	}
}


