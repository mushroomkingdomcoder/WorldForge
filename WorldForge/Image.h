#pragma once
#include "Graphics.h"
#include "Rect.h"
#include <vector>
#include <memory>
#include <functional>

class Image
{
private:
	int width = 0;
	int height = 0;
	std::unique_ptr<Color[]> pImage = nullptr;
public:
	Image() = default;
	Image(const Image& image);
	Image& operator =(const Image& image);
	Image(int width, int height, Color color = Colors::White);
	Image(const char* filename);
	Image(const std::vector<Color>& image, int image_width);
	int GetWidth() const;
	int GetHeight() const;
	iRect GetRect(int x = 0, int y = 0) const;
	const Color* GetPtrToImage() const;
	void SetPixel(int x, int y, const Color& color);
	const Color& GetPixel(int x, int y) const;
	Image Cropped(int new_width, int new_height, int x_off, int y_off) const;
	Image& Crop(int new_width, int new_height, int x_off, int y_off);
	Image FlippedV() const;
	Image& FlipV();
	Image FlippedH() const;
	Image& FlipH();
	Image Rotated90() const;
	Image& Rotate90();
	Image Rotated180() const;
	Image& Rotate180();
	Image Rotated270() const;
	Image& Rotate270();
	Image AdjustedSize(float x_adjust, float y_adjust) const;
	Image& AdjustSize(float x_adjust, float y_adjust);
	Image WithAddedTransparencyFromChroma(const Color& chroma) const;
	Image& AddTransparencyFromChroma(const Color& chroma);
	Image WithInvertedColors() const;
	Image& InvertColors();
	Image WithSubstitutedColors(std::vector<Color> targets, std::vector<Color> replacements) const;
	Image& SubstituteColors(std::vector<Color> targets, std::vector<Color> replacements);
	Image Monochromatic(const Color& color) const;
	Image& MakeMonochromatic(const Color& color);
	Image ColorScaled(const Color& scale) const;
	Image& ColorScale(const Color& scale);
	Image Filtered(const Color& filter) const;
	Image& Filter(const Color& filter);
	Image WithMosaicEffect(int2 img_divs) const;
	Image& MakeMosaic(int2 img_divs);
	Image Silhouetted(const Color& background, const Color& silhouette) const;
	Image& Silhouette(const Color& background, const Color& silhouette);
	void Load(const char* filename);
	void Save(const char* filename) const;
	void Import(const std::vector<Color>& image, int image_width);
	std::vector<Color> Export() const;
	void Draw(Graphics& gfx, int X, int Y, int layer = 0) const;
	void Draw(Graphics& gfx, int X, int Y, int width, int height, int layer = 0) const;
	void Draw(Graphics& gfx, int X, int Y, std::function<Color(const Image&, int, int, int)> color_func, int layer = 0) const;
	void Draw(Graphics& gfx, int X, int Y, int width, int height, std::function<Color(const Image&, int, int, int)> color_func, int layer = 0) const;
	void DrawWithTransparency(Graphics& gfx, int X, int Y, int layer = 0) const;
	void DrawWithTransparency(Graphics& gfx, int X, int Y, int width, int height, int layer = 0) const;
	void DrawWithTransparency(Graphics& gfx, int X, int Y, std::function<Color(const Image&, int, int, int)> color_func, int layer = 0) const;
	void DrawWithTransparency(Graphics& gfx, int X, int Y, int width, int height, std::function<Color(const Image&, int, int, int)> color_func, int layer = 0) const;
};

namespace ImageEffects
{
	Color InvertColors(const Image& image, int img_x, int img_y, int img_pxl);
	Color GreyScale(const Image& image, int img_x, int img_y, int img_pxl);
	Color White(const Image& image, int img_x, int img_y, int img_pxl);
	Color Black(const Image& image, int img_x, int img_y, int img_pxl);
	Color FlipH(const Image& image, int img_x, int img_y, int img_pxl);
	Color FlipV(const Image& image, int img_x, int img_y, int img_pxl);
	Color FlipHV(const Image& image, int img_x, int img_y, int img_pxl);
}