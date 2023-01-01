#pragma once
#include "Image.h"

class Field
{
private:
	Graphics& gfx;
	int layer;
	const int2 imageDim;
	const int2 fieldDim;
	std::vector<int> field;
	std::vector<Image> images;
	bool drawFlag;
public:
	Field() = delete;
	Field(Graphics& gfx, std::vector<Image> images, int default_image = 0, int layer = 0);
	Field(Graphics& gfx, std::vector<Image> images, std::vector<int> field_data, int layer = 0);
	const int2& GetFieldDimensions() const;
	const int& GetImageId(int x, int y) const;
	void UpdateField(int x, int y, int image_id);
	void UpdateFieldRow(int y, std::vector<int> image_ids);
	void UpdateFieldColumn(int x, std::vector<int> image_ids);
	void UpdateField(std::vector<int> image_ids);
	void UpdateImage(int image_id, Image new_image);
	const Image& GetImage(int image_id) const;
	void AddImage(Image new_image);
	void RemoveImage(int image_id);
	void Render();
};
