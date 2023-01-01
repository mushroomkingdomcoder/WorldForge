#pragma once
#include <chrono>

class Clock
{
private:
	std::chrono::steady_clock::time_point mark;
public:
	Clock()
		:
		mark(std::chrono::steady_clock::now())
	{}
	float Peek() const
	{
		return std::chrono::duration<float>(std::chrono::steady_clock::now() - mark).count();
	}
	float Mark()
	{
		const auto last_mark = mark;
		mark = std::chrono::steady_clock::now();
		const std::chrono::duration<float> ellapsed = mark - last_mark;
		return ellapsed.count();
	}
};
