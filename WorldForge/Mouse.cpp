#include "Mouse.h"
#include "Win32Includes.h"

void Mouse::OnMouseMove(int _x, int _y)
{
	x = _x;
	y = _y;
	eventQueue.push(Event(Event::Type::MouseMove, x, y, leftIsClicked, rightIsClicked, middleIsClicked, isInWindow));
	TrimEventQueue();
}

void Mouse::OnLeftClick(int _x, int _y)
{
	x = _x;
	y = _y;
	leftIsClicked = true;
	eventQueue.push(Event(Event::Type::LeftClick, x, y, leftIsClicked, rightIsClicked, middleIsClicked, isInWindow));
	TrimEventQueue();
}

void Mouse::OnLeftDoubleClick(int _x, int _y)
{
	x = _x;
	y = _y;
	eventQueue.push(Event(Event::Type::LeftDoubleClick, x, y, leftIsClicked, rightIsClicked, middleIsClicked, isInWindow));
	TrimEventQueue();
}

void Mouse::OnRightClick(int _x, int _y)
{
	x = _x;
	y = _y;
	rightIsClicked = true;
	eventQueue.push(Event(Event::Type::RightClick, x, y, leftIsClicked, rightIsClicked, middleIsClicked, isInWindow));
	TrimEventQueue();
}

void Mouse::OnRightDoubleClick(int _x, int _y)
{
	x = _x;
	y = _y;
	eventQueue.push(Event(Event::Type::RightDoubleClick, x, y, leftIsClicked, rightIsClicked, middleIsClicked, isInWindow));
	TrimEventQueue();
}

void Mouse::OnMiddleClick(int _x, int _y)
{
	x = _x;
	y = _y;
	middleIsClicked = true;
	eventQueue.push(Event(Event::Type::MiddleClick, x, y, leftIsClicked, rightIsClicked, middleIsClicked, isInWindow));
	TrimEventQueue();
}

void Mouse::OnLeftRelease(int _x, int _y)
{
	x = _x;
	y = _y;
	leftIsClicked = false;
	eventQueue.push(Event(Event::Type::LeftRelease, x, y, leftIsClicked, rightIsClicked, middleIsClicked, isInWindow));
	TrimEventQueue();
}

void Mouse::OnRightRelease(int _x, int _y)
{
	x = _x;
	y = _y;
	rightIsClicked = false;
	eventQueue.push(Event(Event::Type::RightRelease, x, y, leftIsClicked, rightIsClicked, middleIsClicked, isInWindow));
	TrimEventQueue();
}

void Mouse::OnMiddleRelease(int _x, int _y)
{
	x = _x;
	y = _y;
	middleIsClicked = false;
	eventQueue.push(Event(Event::Type::MiddleRelease, x, y, leftIsClicked, rightIsClicked, middleIsClicked, isInWindow));
	TrimEventQueue();
}

void Mouse::OnWheelScroll(int z)
{
	scrollBuffer += z;
	if (z > 0)
	{
		while (scrollBuffer > 0)
		{
			scrollBuffer -= WHEEL_DELTA;
			OnScrollUp();
		}
	}
	else
	{
		while (scrollBuffer < 0)
		{
			scrollBuffer += WHEEL_DELTA;
			OnScrollDown(); 
		}
	}
}

void Mouse::OnScrollUp()
{
	eventQueue.push(Event(Event::Type::ScrollUp, x, y, leftIsClicked, rightIsClicked, middleIsClicked, isInWindow));
	TrimEventQueue();
}

void Mouse::OnScrollDown()
{
	eventQueue.push(Event(Event::Type::ScrollDown, x, y, leftIsClicked, rightIsClicked, middleIsClicked, isInWindow));
	TrimEventQueue();
}

void Mouse::OnMouseEnter()
{
	isInWindow = true;
	eventQueue.push(Event(Event::Type::EnterWindow, x, y, leftIsClicked, rightIsClicked, middleIsClicked, isInWindow));
	TrimEventQueue();
}

void Mouse::OnMouseLeave()
{
	isInWindow = false;
	eventQueue.push(Event(Event::Type::LeaveWindow, x, y, leftIsClicked, rightIsClicked, middleIsClicked, isInWindow));
	TrimEventQueue();
}

void Mouse::TrimEventQueue()
{
	while (eventQueue.size() >= BufferSize)
	{
		eventQueue.pop();
	}
}

int Mouse::GetX() const
{
	return x;
}

int Mouse::GetY() const
{
	return y;
}

bool Mouse::LeftIsClicked() const
{
	return leftIsClicked;
}

bool Mouse::RightIsClicked() const
{
	return rightIsClicked;
}

bool Mouse::MiddleIsClicked() const
{
	return middleIsClicked;
}

bool Mouse::IsInWindow() const
{
	return isInWindow;
}

bool Mouse::isInUse() const
{
	return 
		(leftIsClicked || rightIsClicked || middleIsClicked);
}

void Mouse::ClearMouseStates()
{
	leftIsClicked = false;
	rightIsClicked = false;
	middleIsClicked = false;
}

Mouse::Event Mouse::Read()
{
	if (!eventQueue.empty())
	{
		const auto e = eventQueue.front();
		eventQueue.pop();
		return e;
	}
	return Event();
}

bool Mouse::isActive() const
{
	return !eventQueue.empty();
}

void Mouse::EmptyEventQueue()
{
	eventQueue = std::queue<Event>();
}

void Mouse::Reset()
{
	*this = Mouse();
}
