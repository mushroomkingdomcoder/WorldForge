#pragma once
#include <bitset>
#include <queue>
#include <optional>

class Keyboard
{
	friend class Window;
public:
	static constexpr unsigned char nKeys = 255u;
	static constexpr char BufferSize = 32;
public:
	class Event
	{
	public:
		enum class Type
		{
			KeyDown,
			KeyUp,
			Char
		};
	private:
		char keycode = 0;
		std::optional<Type> type = std::optional<Type>();
	public:
		Event() = default;
		Event(char keycode, Type type)
			:
			keycode(keycode),
			type(type)
		{}
		char GetKeycode() const
		{
			return keycode;
		}
		Type GetType() const
		{
			return *type;
		}
		bool isKeyDown() const
		{
			return *type == Type::KeyDown;
		}
		bool isKeyUp() const
		{
			return *type == Type::KeyUp;
		}
		bool isChar() const
		{
			return *type == Type::Char;
		}
		bool isEndOfQueue() const
		{
			return !type;
		}
	};
private:
	std::bitset<nKeys> keystates = std::bitset<nKeys>();
	std::queue<Event> eventQueue = std::queue<Event>();
	std::queue<char> charBuffer = std::queue<char>();
	bool autorepeat = false;
private:
	void OnKeyDown(char keycode);
	void OnKeyUp(char keycode);
	void OnChar(char character);
	template <typename buffer>
	void TrimBuffer(buffer& buf)
	{
		while (buf.size() >= BufferSize)
		{
			buf.pop();
		}
	}
public:
	Keyboard() = default;
	bool KeyIsPressed(char key) const;
	bool isInUse() const;
	void ClearKeystates();
	Event Read();
	bool isActive() const;
	void EmptyEventQueue();
	char Get();
	bool isEmpty() const;
	void EmptyCharacterBuffer();
	bool AutorepeatIsEnabled() const;
	void EnableAutorepeat();
	void DisableAutorepeat();
};

