#include "Keyboard.h"

void Keyboard::OnKeyDown(char keycode)
{
	keystates[keycode] = true;
	eventQueue.push(Event(keycode, Event::Type::KeyDown));
	TrimBuffer(eventQueue);
}

void Keyboard::OnKeyUp(char keycode)
{
	keystates[keycode] = false;
	eventQueue.push(Event(keycode, Event::Type::KeyUp));
	TrimBuffer(eventQueue);
}

void Keyboard::OnChar(char character)
{
	charBuffer.push(character);
	eventQueue.push(Event(character, Event::Type::Char));
	TrimBuffer(charBuffer);
	TrimBuffer(eventQueue);
}

bool Keyboard::KeyIsPressed(char key) const
{
	return keystates[key];
}

bool Keyboard::isInUse() const
{
	return keystates.any();
}

void Keyboard::ClearKeystates()
{
	keystates.reset();
}

Keyboard::Event Keyboard::Read()
{
	if (!eventQueue.empty())
	{
		const auto e = eventQueue.front();
		eventQueue.pop();
		return e;
	}
	else
	{
		return Event();
	}
}

bool Keyboard::isActive() const
{
	return !eventQueue.empty();
}

void Keyboard::EmptyEventQueue()
{
	eventQueue = std::queue<Event>();
}

char Keyboard::Get()
{
	if (!charBuffer.empty())
	{
		char character = charBuffer.front();
		charBuffer.pop();
		return character;
	}
	return 0;
}

bool Keyboard::isEmpty() const
{
	return charBuffer.empty();
}

void Keyboard::EmptyCharacterBuffer()
{
	charBuffer = std::queue<char>();
}

bool Keyboard::AutorepeatIsEnabled() const
{
	return autorepeat;
}

void Keyboard::EnableAutorepeat()
{
	autorepeat = true;
}

void Keyboard::DisableAutorepeat()
{
	autorepeat = false;
}

