#include "Image.h"
#include <fstream>
#include "BaseException.h"
#include <wingdi.h>
#include <assert.h>

Image::Image(const Image& image)
	:
	width(image.width),
	height(image.height),
	pImage(std::make_unique<Color[]>(width * height))
{
	const int nImageBytes = width * height * sizeof(Color);
	memcpy(pImage.get(), image.pImage.get(), nImageBytes);
}

Image& Image::operator=(const Image& image)
{
	width = image.width;
	height = image.height;
	const int nPixels = width * height;
	pImage = std::make_unique<Color[]>(nPixels);
	const int nImageBytes = nPixels * sizeof(Color);
	memcpy(pImage.get(), image.pImage.get(), nImageBytes);
	return *this;
}

Image::Image(int width, int height, Color color)
	:
	width(width),
	height(height),
	pImage(std::make_unique<Color[]>(width * height))
{
	for (int i = 0; i < width * height; ++i)
	{
		pImage[i] = color;
	}
}

Image::Image(const char* filename)
{
	std::ifstream bitmapIN{ filename, std::ios::binary };
	if (bitmapIN.fail())
	{
		throw EXCPT_NOTE("Bitmap file not found! Check directory and/or file name spelling and retry.");
	}
	BITMAPFILEHEADER fileHead = {};
	bitmapIN.read(reinterpret_cast<char*>(&fileHead), sizeof(fileHead));
	if (fileHead.bfType != ('B' + ('M' << 8)))
	{
		throw EXCPT_NOTE("Only .bmp image files supported! Check image file extension and retry.");
	}
	BITMAPINFOHEADER infoHead = {};
	bitmapIN.read(reinterpret_cast<char*>(&infoHead), sizeof(infoHead));
	if (infoHead.biCompression != BI_RGB)
	{
		throw EXCPT_NOTE("Only uncompressed bitmaps supported! Decompress image and retry.");
	}
	if (infoHead.biBitCount != 24 && infoHead.biBitCount != 32)
	{
		throw EXCPT_NOTE("Only bitmaps of either 24bpp or 32bpp allowed. Reset color depth of image and retry.");
	}
	bitmapIN.seekg(fileHead.bfOffBits, std::ios::beg);
	width = infoHead.biWidth;
	height = std::abs(infoHead.biHeight);
	const int nPixels = width * height;
	const int nImageBytes = nPixels * sizeof(Color);
	pImage = std::make_unique<Color[]>(nPixels);
	if (infoHead.biBitCount == 32 && infoHead.biHeight < 0)
	{
		bitmapIN.read(reinterpret_cast<char*>(pImage.get()), nImageBytes);
	}
	else
	{
		int padding = 0;
		if (infoHead.biBitCount == 24)
		{
			padding = (4 - ((width * 3) % 4)) % 4;
		}
		int startY = 0;
		int deltaY = 1;
		int endY = height;
		if (infoHead.biHeight > 0)
		{
			startY = height - 1;
			deltaY = -1;
			endY = -1;
		}
		for (int y = startY; y != endY; y += deltaY)
		{
			for (int x = 0; x < width; ++x)
			{
				const int pxl = y * width + x;
				if (infoHead.biBitCount == 24)
				{
					bitmapIN.read(reinterpret_cast<char*>(&pImage[pxl]), sizeof(Color) - 1);
					pImage[pxl].SetA(255);
				}
				else
				{
					bitmapIN.read(reinterpret_cast<char*>(&pImage[pxl]), sizeof(Color));
				}
			}
			bitmapIN.seekg(padding, std::ios::cur);
		}
	}
	if (bitmapIN.fail())
	{
		throw EXCPT_NOTE("Critical error in reading bitmap file! Please retry.");
	}
	bitmapIN.close();
}

Image::Image(const std::vector<Color>& image, int image_width)
	:
	width(image_width),
	height((int)image.size() / width),
	pImage(std::make_unique<Color[]>(image.size()))
{
	const int nImageBytes = (int)image.size() * sizeof(Color);
	memcpy(pImage.get(), image.data(), nImageBytes);
}

int Image::GetWidth() const
{
	return width;
}

int Image::GetHeight() const
{
	return height;
}

iRect Image::GetRect(int x, int y) const
{
	return iRect(vec2i(x, y), width, height);
}

const Color* Image::GetPtrToImage() const
{
	return pImage.get();
}

void Image::SetPixel(int x, int y, const Color& color)
{
	assert(x < width && y < height);
	const int pxl = y * width + x;
	pImage[pxl] = color;
}

const Color& Image::GetPixel(int x, int y) const
{
	assert(x < width && y < height);
	const int pxl = y * width + x;
	return pImage[pxl];
}

Image Image::Cropped(int new_width, int new_height, int x_off, int y_off) const
{
	assert(new_width + x_off <= width && new_width > 0);
	assert(new_height + y_off <= height && new_height > 0);
	Image image{ new_width, new_height };
	const int pitch = new_width * sizeof(Color);
	for (int y = 0; y < new_height; ++y)
	{
		const int dst_pxl = y * new_width;
		const int src_pxl = (y + y_off) * width + (x_off);
		memcpy(&image.pImage[dst_pxl], &pImage[src_pxl], pitch);
	}
	return image;
}

Image& Image::Crop(int new_width, int new_height, int x_off, int y_off)
{
	return *this = this->Cropped(new_width, new_height, x_off, y_off);
}

Image Image::FlippedV() const
{
	Image flippedV{ width,height };
	for (int y = 0; y < height; ++y)
	{
		for (int x = 0; x < width; ++x)
		{
			const int dst_pxl = y * width + x;
			const int src_pxl = (height - y - 1) * width + x;
			flippedV.pImage[dst_pxl] = pImage[src_pxl];
		}
	}
	return flippedV;
}

Image& Image::FlipV()
{
	return *this = this->FlippedV();
}

Image Image::FlippedH() const
{
	Image flippedH{ width,height };
	for (int y = 0; y < height; ++y)
	{
		for (int x = 0; x < width; ++x)
		{
			const int dst_pxl = y * width + x;
			const int src_pxl = y * width + (width - x - 1);
			flippedH.pImage[dst_pxl] = pImage[src_pxl];
		}
	}
	return flippedH;
}

Image& Image::FlipH()
{
	return *this = this->FlippedH();
}

Image Image::Rotated90() const
{
	Image rotated{ height,width };
	for (int y = 0; y < height; ++y)
	{
		for (int x = 0; x < width; ++x)
		{
			const int src_pxl = y * width + x;
			const int dst_pxl = (width - x - 1) * height + y;
			rotated.pImage[dst_pxl] = pImage[src_pxl];
		}
	}
	return rotated;
}

Image& Image::Rotate90()
{
	return *this = this->Rotated90();
}

Image Image::Rotated180() const
{
	Image rotated{ width,height };
	for (int y = 0; y < height; ++y)
	{
		for (int x = 0; x < width; ++x)
		{
			const int src_pxl = y * width + x;
			const int dst_pxl = (height - y - 1) * width + (width - x - 1);
			rotated.pImage[dst_pxl] = pImage[src_pxl];
		}
	}
	return rotated;
}

Image& Image::Rotate180()
{
	return *this = this->Rotated180();
}

Image Image::Rotated270() const
{
	Image rotated{ height,width };
	for (int y = 0; y < height; ++y)
	{
		for (int x = 0; x < width; ++x)
		{
			const int src_pxl = y * width + x;
			const int dst_pxl = x * height + (height - y - 1);
			rotated.pImage[dst_pxl] = pImage[src_pxl];
		}
	}
	return rotated;
}

Image& Image::Rotate270()
{
	return *this = this->Rotated270();
}

Image Image::AdjustedSize(float x_adjust, float y_adjust) const
{
	float newWidth = (float)width * x_adjust;
	float newHeight = (float)height * y_adjust;
	Image adjusted{ int(newWidth), int(newHeight) };
	const float xPxlsPerPxl = (float)width / newWidth;
	const float yPxlsPerPxl = (float)height / newHeight;
	for (int y = 0; y < adjusted.height; ++y)
	{
		for (int x = 0; x < adjusted.height; ++x)
		{
			adjusted.pImage[y * adjusted.width + x] = pImage[int((float)y * yPxlsPerPxl) * width + int((float)x * xPxlsPerPxl)];
		}
	}
	return adjusted;
}

Image& Image::AdjustSize(float x_adjust, float y_adjust)
{
	return *this = this->AdjustedSize(x_adjust, y_adjust);
}

Image Image::WithAddedTransparencyFromChroma(const Color& chroma) const
{
	Image transparentCopy{ width,height };
	for (int y = 0; y < height; ++y)
	{
		for (int x = 0; x < width; ++x)
		{
			const int pxl = y * width + x;
			if (pImage[pxl] == chroma)
			{
				transparentCopy.pImage[pxl] = Colors::Transparent;
			}
			else
			{
				transparentCopy.pImage[pxl] = pImage[pxl];
			}
		}
	}
	return transparentCopy;
}

Image& Image::AddTransparencyFromChroma(const Color& chroma)
{
	return *this = this->WithAddedTransparencyFromChroma(chroma);
}

Image Image::WithInvertedColors() const
{
	Image inverted{ width,height };
	for (int i = 0; i < width * height; ++i)
	{
		if (pImage[i].GetA())
		{
			inverted.pImage[i] = pImage[i].Inverted();
		}
		else
		{
			inverted.pImage[i] = Colors::Transparent;
		}
	}
	return inverted;
}

Image& Image::InvertColors()
{
	return *this = this->WithInvertedColors();
}

Image Image::WithSubstitutedColors(std::vector<Color> targets, std::vector<Color> replacements) const
{
	assert(targets.size() <= replacements.size());
	Image substituted{ width,height };
	for (int i = 0; i < width * height; ++i)
	{
		for (int j = 0; j < targets.size(); ++j)
		{
			if (pImage[i] == targets[j])
			{
				substituted.pImage[i] = replacements[j];
			}
			else
			{
				substituted.pImage[i] = pImage[i];
			}
		}
	}
	return substituted;
}

Image& Image::SubstituteColors(std::vector<Color> targets, std::vector<Color> replacements)
{
	return *this = this->WithSubstitutedColors(targets, replacements);
}

Image Image::Monochromatic(const Color& color) const
{
	Image monochromatic{ width,height };
	for (int i = 0; i < width * height; ++i)
	{
		if (pImage[i].GetA())
		{
			monochromatic.pImage[i] = color;
		}
		else
		{
			monochromatic.pImage[i] = Colors::Transparent;
		}
	}
	return monochromatic;
}

Image& Image::MakeMonochromatic(const Color& color)
{
	return *this = this->Monochromatic(color);
}

Image Image::ColorScaled(const Color& scale) const
{
	Image scaled{ width,height };
	for (int i = 0; i < width * height; ++i)
	{
		if (pImage[i].GetA())
		{
			const float pxl_avg = (pImage[i].GetRn() + pImage[i].GetGn() + pImage[i].GetBn()) / 3.0f;
			scaled.pImage[i] = Color(pxl_avg * scale.GetRn(), pxl_avg * scale.GetGn(), pxl_avg * scale.GetBn());
		}
		else
		{
			scaled.pImage[i] = Colors::Transparent;
		}
	}
	return scaled;
}

Image& Image::ColorScale(const Color& scale)
{
	return *this = this->ColorScaled(scale);
}

Image Image::Filtered(const Color& filter) const
{
	Image filtered{ width,height };
	vec4 vFilter = filter.GetVector();
	for (int i = 0; i < width * height; ++i)
	{
		filtered.pImage[i] = pImage[i] * vFilter;
	}
	return filtered;
}

Image& Image::Filter(const Color& filter)
{
	return *this = this->Filtered(filter);
}

Image Image::WithMosaicEffect(int2 img_divs) const
{
	assert(height % img_divs.y == 0);
	assert(width % img_divs.x == 0);
	Image mosaic{ width,height };
	const int pxlsPerDivY = height / img_divs.y;
	const int pxlsPerDivX = width / img_divs.x;
	for (int y = 0; y < height; ++y)
	{
		const int row = y * width;
		const int mos_y = y / pxlsPerDivY * pxlsPerDivY;
		const int mos_row = mos_y * width;
		for (int x = 0; x < width; ++x)
		{
			const int mos_x = x / pxlsPerDivX * pxlsPerDivX;
			mosaic.pImage[row + x] = pImage[mos_row + mos_x];
		}
	}
	return mosaic;
}

Image& Image::MakeMosaic(int2 img_divs)
{
	return *this = this->WithMosaicEffect(img_divs);
}

Image Image::Silhouetted(const Color& background, const Color& silhouette) const
{
	Image silhouetted{ width,height };
	for (int i = 0; i < width * height; ++i)
	{
		if (pImage[i].GetA())
		{
			if (pImage[i] != background)
			{
				silhouetted.pImage[i] = silhouette;
			}
			else
			{
				silhouetted.pImage[i] = background;
			}
		}
		else
		{
			silhouetted.pImage[i] = Colors::Transparent;
		}
	}
	return silhouetted;
}

Image& Image::Silhouette(const Color& background, const Color& silhouette)
{
	return *this = this->Silhouetted(background, silhouette);
}

void Image::Load(const char* filename)
{
	*this = Image(filename);
}

void Image::Save(const char* filename) const
{
	const int nPixels = width * height;
	const int nImageBytes = nPixels * sizeof(Color);
	const int headerSectionSize = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);
	std::ofstream bitmapOUT{ filename, std::ios::binary };
	if (bitmapOUT.fail())
	{
		throw EXCPT_NOTE("Cannot write to specified file! Check directory and/or file name spelling and retry.");
	}
	BITMAPFILEHEADER fileHead;
	fileHead.bfType = 'B' + ('M' << 8);
	fileHead.bfSize = headerSectionSize + nImageBytes;
	fileHead.bfReserved1 = 0;
	fileHead.bfReserved2 = 0;
	fileHead.bfOffBits = headerSectionSize;
	BITMAPINFOHEADER infoHead;
	infoHead.biSize = sizeof(BITMAPINFOHEADER);
	infoHead.biWidth = width;
	infoHead.biHeight = -int(height);	// Top-down DIB
	infoHead.biPlanes = 1;				// Always set to 1
	infoHead.biBitCount = 32;
	infoHead.biCompression = BI_RGB; 
	infoHead.biSizeImage = nImageBytes;
	infoHead.biXPelsPerMeter = 0;		// No target device
	infoHead.biYPelsPerMeter = 0;		// No target device
	infoHead.biClrUsed = 0;				// Use all possible colors
	infoHead.biClrImportant = 0;		// All colors are required
	bitmapOUT.write(reinterpret_cast<char*>(&fileHead), sizeof(BITMAPFILEHEADER));
	bitmapOUT.write(reinterpret_cast<char*>(&infoHead), sizeof(BITMAPINFOHEADER));
	bitmapOUT.write(reinterpret_cast<char*>(pImage.get()), nImageBytes);
	if (bitmapOUT.fail())
	{
		throw EXCPT_NOTE("Critical error writing bitmap file! Please retry.");
	}
	bitmapOUT.close();
}

void Image::Import(const std::vector<Color>& image, int image_width)
{
	*this = Image(image, image_width);
}

std::vector<Color> Image::Export() const
{
	std::vector<Color> image;
	const int nPixels = width * height;
	image.resize(nPixels);
	const int nImageBytes = nPixels * sizeof(Color);
	memcpy(image.data(), pImage.get(), nImageBytes);
	return image;
}

void Image::Draw(Graphics& gfx, int X, int Y, int layer) const
{
	const int& xRes = gfx.GetWidth(layer);
	const int& yRes = gfx.GetHeight(layer);
	assert(X < (int)xRes && X + width > 0);
	assert(Y < (int)yRes && Y + height > 0);
	const int startX =
		(0) * (X >= 0) +
		(-X) * (X < 0);
	const int startY =
		(0) * (Y >= 0) +
		(-Y) * (Y < 0);
	const int slicePitch =
		((xRes - X - startX) * sizeof(Color)) * (width + X > xRes) +
		((width - startX) * sizeof(Color)) * (width + X <= xRes);
	const int endY =
		(yRes - Y) * (height + Y > yRes) +
		(height) * (height + Y <= yRes);
	for (int y = startY; y < endY; ++y)
	{
		const int dst_pxl = (Y + y) * xRes + X + startX;
		const int src_pxl = y * width + startX;
		memcpy(&gfx.GetPixelMap(layer)[dst_pxl], &pImage[src_pxl], slicePitch);
	}
}

void Image::Draw(Graphics& gfx, int X, int Y, int width, int height, int layer) const
{
	const int& xRes = gfx.GetWidth(layer);
	const int& yRes = gfx.GetHeight(layer);
	assert(width != 0 && height != 0);
	assert(X < (int)xRes && X + width > 0);
	assert(Y < (int)yRes && Y + height > 0);
	const float xPxlsPerPxl = (float)this->width / (float)width;
	const float yPxlsPerPxl = (float)this->height / (float)height;
	const int startX =
		(0) * (X >= 0) +
		(-X) * (X < 0);
	const int startY =
		(0) * (Y >= 0) +
		(-Y) * (Y < 0);
	const int endX =
		(xRes - X) * (width + X > xRes) +
		(width) * (width + X <= xRes);
	const int endY =
		(yRes - Y) * (height + Y > yRes) +
		(height) * (height + Y <= yRes);
	for (int y = startY; y < endY; ++y)
	{
		for (int x = startX; x < endX; ++x)
		{
			const int dest_pxl = (Y + y) * xRes + X + x;
			const int src_pxl = int((float)y * yPxlsPerPxl) * this->width + int((float)x * xPxlsPerPxl);
			gfx.GetPixelMap(layer)[dest_pxl] = pImage[src_pxl];
		}
	}
}

void Image::Draw(Graphics& gfx, int X, int Y, std::function<Color(const Image&, int, int, int)> color_func, int layer) const
{
	const int& xRes = gfx.GetWidth(layer);
	const int& yRes = gfx.GetHeight(layer);
	assert(X < (int)xRes && X + width > 0);
	assert(Y < (int)yRes && Y + height > 0);
	const int startX =
		(0) * (X >= 0) +
		(-X) * (X < 0);
	const int startY =
		(0) * (Y >= 0) +
		(-Y) * (Y < 0);
	const int endX =
		(xRes - X) * (width + X > xRes) +
		(width) * (height + X <= xRes);
	const int endY =
		(yRes - Y) * (height + Y > yRes) +
		(height) * (height + Y <= yRes);
	for (int y = startY; y < endY; ++y)
	{
		for (int x = startX; x < endX; ++x)
		{
			const int src_pxl = y * width + x;
			const int dest_pxl = (Y + y) * xRes + X + x;
			gfx.GetPixelMap(layer)[dest_pxl] = color_func(*this, x, y, src_pxl);
		}
	}
}

void Image::Draw(Graphics& gfx, int X, int Y, int width, int height, std::function<Color(const Image&, int, int, int)> color_func, int layer) const
{
	const int& xRes = gfx.GetWidth(layer);
	const int& yRes = gfx.GetHeight(layer);
	assert(width != 0 && height != 0);
	assert(X < (int)xRes && X + width > 0);
	assert(Y < (int)yRes && Y + height > 0);
	const float xPxlsPerPxl = (float)this->width / (float)width;
	const float yPxlsPerPxl = (float)this->height / (float)height;
	const int startX =
		(0) * (X >= 0) +
		(-X) * (X < 0);
	const int startY =
		(0) * (Y >= 0) +
		(-Y) * (Y < 0);
	const int endX =
		(xRes - X) * (width + X > xRes) +
		(width) * (width + X <= xRes);
	const int endY =
		(yRes - Y) * (height + Y > yRes) +
		(height) * (height + Y <= yRes);
	for (int y = startY; y < endY; ++y)
	{
		for (int x = startX; x < endX; ++x)
		{
			const int dest_pxl = (Y + y) * xRes + X + x;
			const int src_x = int((float)x * xPxlsPerPxl);
			const int src_y = int((float)y * yPxlsPerPxl);
			const int src_pxl = src_y * this->width + src_x;
			gfx.GetPixelMap(layer)[dest_pxl] = color_func(*this, src_x, src_y, src_pxl);
		}
	}
}

void Image::DrawWithTransparency(Graphics& gfx, int X, int Y, int layer) const
{
	const int& xRes = gfx.GetWidth(layer);
	const int& yRes = gfx.GetHeight(layer);
	assert(X < (int)xRes && X + width > 0);
	assert(Y < (int)yRes && Y + height > 0);
	const int startX =
		(0) * (X >= 0) +
		(-X) * (X < 0);
	const int startY =
		(0) * (Y >= 0) +
		(-Y) * (Y < 0);
	const int endX =
		(xRes - X) * (width + X > xRes) +
		(width) * (height + X <= xRes);
	const int endY =
		(yRes - Y) * (height + Y > yRes) +
		(height) * (height + Y <= yRes);
	for (int y = startY; y < endY; ++y)
	{
		for (int x = startX; x < endX; ++x)
		{
			const int src_pxl = y * width + x;
			if (pImage[src_pxl].GetA())
			{
				const int dest_pxl = (Y + y) * xRes + X + x;
				gfx.GetPixelMap(layer)[dest_pxl] = pImage[src_pxl];
			}
		}
	}
}

void Image::DrawWithTransparency(Graphics& gfx, int X, int Y, int width, int height, int layer) const
{
	const int& xRes = gfx.GetWidth(layer);
	const int& yRes = gfx.GetHeight(layer);
	assert(width != 0 && height != 0);
	assert(X < (int)xRes && X + width > 0);
	assert(Y < (int)yRes && Y + height > 0);
	const float xPxlsPerPxl = (float)this->width / (float)width;
	const float yPxlsPerPxl = (float)this->height / (float)height;
	const int startX =
		(0) * (X >= 0) +
		(-X) * (X < 0);
	const int startY =
		(0) * (Y >= 0) +
		(-Y) * (Y < 0);
	const int endX =
		(xRes - X) * (width + X > xRes) +
		(width) * (width + X <= xRes);
	const int endY =
		(yRes - Y) * (height + Y > yRes) +
		(height) * (height + Y <= yRes);
	for (int y = startY; y < endY; ++y)
	{
		for (int x = startX; x < endX; ++x)
		{
			const int dest_pxl = (Y + y) * xRes + X + x;
			const int src_pxl = int((float)y * yPxlsPerPxl) * this->width + int((float)x * xPxlsPerPxl);
			if (pImage[src_pxl].GetA())
			{
				gfx.GetPixelMap(layer)[dest_pxl] = pImage[src_pxl];
			}
		}
	}
}

void Image::DrawWithTransparency(Graphics& gfx, int X, int Y, std::function<Color(const Image&, int, int, int)> color_func, int layer) const
{
	const int& xRes = gfx.GetWidth(layer);
	const int& yRes = gfx.GetHeight(layer);
	assert(X < (int)xRes && X + width > 0);
	assert(Y < (int)yRes && Y + height > 0);
	const int startX =
		(0) * (X >= 0) +
		(-X) * (X < 0);
	const int startY =
		(0) * (Y >= 0) +
		(-Y) * (Y < 0);
	const int endX =
		(xRes - X) * (width + X > xRes) +
		(width) * (height + X <= xRes);
	const int endY =
		(yRes - Y) * (height + Y > yRes) +
		(height) * (height + Y <= yRes);
	for (int y = startY; y < endY; ++y)
	{
		for (int x = startX; x < endX; ++x)
		{
			const int src_pxl = y * width + x;
			if (pImage[src_pxl].GetA())
			{
				const int dest_pxl = (Y + y) * xRes + X + x;
				gfx.GetPixelMap(layer)[dest_pxl] = color_func(*this, x, y, src_pxl);
			}
		}
	}
}

void Image::DrawWithTransparency(Graphics& gfx, int X, int Y, int width, int height, std::function<Color(const Image&, int, int, int)> color_func, int layer) const
{
	const int& xRes = gfx.GetWidth(layer);
	const int& yRes = gfx.GetHeight(layer);
	assert(width != 0 && height != 0);
	assert(X < (int)xRes && X + width > 0);
	assert(Y < (int)yRes && Y + height > 0);
	const float xPxlsPerPxl = (float)this->width / (float)width;
	const float yPxlsPerPxl = (float)this->height / (float)height;
	const int startX =
		(0) * (X >= 0) +
		(-X) * (X < 0);
	const int startY =
		(0) * (Y >= 0) +
		(-Y) * (Y < 0);
	const int endX =
		(xRes - X) * (width + X > xRes) +
		(width) * (width + X <= xRes);
	const int endY =
		(yRes - Y) * (height + Y > yRes) +
		(height) * (height + Y <= yRes);
	for (int y = startY; y < endY; ++y)
	{
		for (int x = startX; x < endX; ++x)
		{
			const int dest_pxl = (Y + y) * xRes + X + x;
			const int src_x = int((float)x * xPxlsPerPxl);
			const int src_y = int((float)y * yPxlsPerPxl);
			const int src_pxl = src_y * this->width + src_x;
			if (pImage[src_pxl].GetA())
			{
				gfx.GetPixelMap(layer)[dest_pxl] = color_func(*this, src_x, src_y, src_pxl);
			}
		}
	}
}

Color ImageEffects::InvertColors(const Image& image, int img_x, int img_y, int img_pxl)
{
	return image.GetPtrToImage()[img_pxl].Inverted();
}

Color ImageEffects::GreyScale(const Image& image, int img_x, int img_y, int img_pxl)
{
	const Color& pxl = image.GetPtrToImage()[img_pxl];
	const unsigned char scale = unsigned char(((int)pxl.GetR() + (int)pxl.GetG() + (int)pxl.GetB()) / 3);
	return Color(scale, scale, scale, pxl.GetA());
}

Color ImageEffects::White(const Image& image, int img_x, int img_y, int img_pxl)
{
	return Colors::White;
}

Color ImageEffects::Black(const Image& image, int img_x, int img_y, int img_pxl)
{
	return Colors::Black;
}

Color ImageEffects::FlipH(const Image& image, int img_x, int img_y, int img_pxl)
{
	return image.GetPixel(image.GetWidth() - img_x - 1, img_y);
}

Color ImageEffects::FlipV(const Image& image, int img_x, int img_y, int img_pxl)
{
	return image.GetPixel(img_x, image.GetHeight() - img_y - 1);
}

Color ImageEffects::FlipHV(const Image& image, int img_x, int img_y, int img_pxl)
{
	return image.GetPixel(image.GetWidth() - img_x - 1, image.GetHeight() - img_y - 1);
}