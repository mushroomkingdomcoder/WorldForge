#pragma once
#include "Image.h"

class Animation
{
private:
	std::vector<Image> frames;
	int currentFrame;
	const int frameWidth;
	const int frameHeight;
	const int nFrames;
	float currentFrameTime;
	float secsPerFrame;
public:
	Animation() = delete;
	Animation(Image sprite_sheet, int2 sprite_size, int2 sheet_dim, int fps);
	const int& GetFrameWidth() const;
	const int& GetFrameHeight() const;
	vec2i GetFrameSize() const;
	const int& GetCurrentFrameIndex() const;
	void SetCurrentFrameIndex(int frame);
	const float& GetCurrentFrameTime() const;
	void SetCurrentFrameTime(float time);
	float GetFPS() const;
	void SetFPS(int fps);
	const Image& GetCurrentFrame() const;
	const Image& Play(float time_ellapsed);
	bool PlayAndCheck(float time_ellapsed);
	void Draw(Graphics& gfx, int x, int y, int layer = 0) const;
	void DrawWithTransparency(Graphics& gfx, int x, int y, int layer = 0) const;
};



