#pragma once
#include <queue>
#include <optional>

class Mouse
{
	friend class Window;
public:
	static constexpr char BufferSize = 32;
public:
	class Event
	{
	public:
		enum class Type
		{
			MouseMove,
			LeftClick,
			LeftDoubleClick,
			RightClick,
			RightDoubleClick,
			MiddleClick,
			LeftRelease,
			RightRelease,
			MiddleRelease,
			ScrollUp,
			ScrollDown,
			EnterWindow,
			LeaveWindow
		};
	private:
		std::optional<Type> type = std::optional<Type>();
		int x = 0;
		int y = 0;
		bool leftWasClicked = false;
		bool rightWasClicked = false;
		bool middleWasClicked = false;
		bool wasInWindow = false;
	public:
		Event() = default;
		Event(Type type, int x, int y, bool l, bool r, bool m, bool w)
			:
			type(type),
			x(x),
			y(y),
			leftWasClicked(l),
			rightWasClicked(r),
			middleWasClicked(m),
			wasInWindow(w)
		{}
		Type GetType() const
		{
			return *type;
		}
		int GetMouseXAtEvent() const
		{
			return x;
		}
		int GetMouseYAtEvent() const
		{
			return y;
		}
		bool LeftWasClicked() const
		{
			return leftWasClicked;
		}
		bool RightWasClicked() const
		{
			return rightWasClicked;
		}
		bool MiddleWasClicked() const
		{
			return middleWasClicked;
		}
		bool MouseWasInWindow() const
		{
			return wasInWindow;
		}
		bool isMouseMove() const
		{
			return *type == Type::MouseMove;
		}
		bool isLeftClick() const
		{
			return *type == Type::LeftClick;
		}
		bool isLeftDoubleClick() const
		{
			return *type == Type::LeftDoubleClick;
		}
		bool isRightClick() const
		{
			return *type == Type::RightClick;
		}
		bool isRightDoubleClick() const
		{
			return *type == Type::RightDoubleClick;
		}
		bool isMiddleClick() const
		{
			return *type == Type::MiddleClick;
		}
		bool isLeftRelease() const
		{
			return *type == Type::LeftRelease;
		}
		bool isRightRelease() const
		{
			return *type == Type::RightRelease;
		}
		bool isMiddleRelease() const
		{
			return *type == Type::MiddleRelease;
		}
		bool isScrollUp() const
		{
			return *type == Type::ScrollUp;
		}
		bool isScrollDown() const
		{
			return *type == Type::ScrollDown;
		}
		bool isMouseEnterWindow() const
		{
			return *type == Type::EnterWindow;
		}
		bool isMouseLeaveWindow() const
		{
			return *type == Type::LeaveWindow;
		}
		bool isEndOfQueue() const
		{
			return !type;
		}
	};
private:
	int x = 0;
	int y = 0;
	bool leftIsClicked = false;
	bool rightIsClicked = false;
	bool middleIsClicked = false;
	bool isInWindow = false;
	int scrollBuffer = 0;
	std::queue<Event> eventQueue = std::queue<Event>();
private:
	void OnMouseMove(int _x, int _y);
	void OnLeftClick(int _x, int _y);
	void OnLeftDoubleClick(int _x, int _y);
	void OnRightClick(int _x, int _y);
	void OnRightDoubleClick(int _x, int _y);
	void OnMiddleClick(int _x, int _y);
	void OnLeftRelease(int _x, int _y);
	void OnRightRelease(int _x, int _y);
	void OnMiddleRelease(int _x, int _y);
	void OnWheelScroll(int z);
	void OnScrollUp();
	void OnScrollDown();
	void OnMouseEnter();
	void OnMouseLeave();
	void TrimEventQueue();
public:
	Mouse() = default;
	int GetX() const;
	int GetY() const;
	bool LeftIsClicked() const;
	bool RightIsClicked() const;
	bool MiddleIsClicked() const;
	bool IsInWindow() const;
	bool isInUse() const;
	void ClearMouseStates();
	Event Read();
	bool isActive() const;
	void EmptyEventQueue();
	void Reset();
};
