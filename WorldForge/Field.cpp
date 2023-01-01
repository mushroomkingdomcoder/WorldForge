#include "Field.h"

#define CHECK_XY(x, y) { assert(x >= 0 && x < fieldDim.x); assert(y >= 0 && y < fieldDim.y); }

Field::Field(Graphics& gfx, std::vector<Image> images, int default_image, int layer)
	:
	gfx(gfx),
	layer(layer),
	imageDim({ images[0].GetWidth(),images[0].GetHeight() }),
	fieldDim({ gfx.GetWidth(layer) / imageDim.x,gfx.GetHeight(layer) / imageDim.y }),
	images(images),
	drawFlag(true)
{
	assert(!images.empty());
	field.resize(fieldDim.x * fieldDim.y, default_image);
	gfx.ManuallyManage(layer);
}

Field::Field(Graphics& gfx, std::vector<Image> images, std::vector<int> field_data, int layer)
	:
	gfx(gfx),
	layer(layer),
	imageDim({ images[0].GetWidth(),images[0].GetHeight() }),
	fieldDim({ gfx.GetWidth(layer) / imageDim.x,gfx.GetHeight(layer) / imageDim.y }),
	images(images),
	drawFlag(true)
{
	assert(!images.empty());
	field.resize(fieldDim.x * fieldDim.y, 0);
	assert(field_data.size() <= field.size());
	for (int i = 0; i < field_data.size(); ++i)
	{
		field[i] = field_data[i];
	}
	gfx.ManuallyManage(layer);
}

const int2& Field::GetFieldDimensions() const
{
	return fieldDim;
}

const int& Field::GetImageId(int x, int y) const
{
	CHECK_XY(x, y);
	return field[y * fieldDim.x + x];
}

void Field::UpdateField(int x, int y, int image_id)
{
	CHECK_XY(x, y);
	field[y * fieldDim.x + x] = image_id;
	drawFlag = true;
}

void Field::UpdateFieldRow(int y, std::vector<int> image_ids)
{
	assert(y >= 0 && y < fieldDim.y);
	assert(image_ids.size() <= fieldDim.x);
	for (int i = 0; i < image_ids.size(); ++i)
	{
		field[y * fieldDim.x + i] = image_ids[i];
	}
	drawFlag = true;
}

void Field::UpdateFieldColumn(int x, std::vector<int> image_ids)
{
	assert(x >= 0 && x < fieldDim.x);
	assert(image_ids.size() <= fieldDim.y);
	for (int i = 0; i < image_ids.size(); ++i)
	{
		field[i * fieldDim.x + x] = image_ids[i];
	}
	drawFlag = true;
}

void Field::UpdateField(std::vector<int> image_ids)
{
	assert(image_ids.size() <= field.size());
	for (int i = 0; i < image_ids.size(); ++i)
	{
		field[i] = image_ids[i];
	}
	drawFlag = true;
}

void Field::UpdateImage(int image_id, Image new_image)
{
	images[image_id] = new_image;
	drawFlag = true;
}

const Image& Field::GetImage(int image_id) const
{
	return images[image_id];
}

void Field::AddImage(Image new_image)
{
	images.emplace_back(new_image);
}

void Field::RemoveImage(int image_id)
{
	auto index = images.begin();
	index += image_id;
	images.erase(index);
	for (int& id : field)
	{
		if (id > image_id)
		{
			--id;
		}
	}
}

void Field::Render()
{
	if (drawFlag)
	{
		for (int i = 0; i < field.size(); ++i)
		{
			int y = i / fieldDim.x;
			int x = i % fieldDim.x;
			images[field[i]].Draw(gfx, x * imageDim.x, y * imageDim.y, layer);
		}
		drawFlag = false;
	}
}