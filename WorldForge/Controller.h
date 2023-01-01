#pragma once
#include "Win32Includes.h"
#include "Vector.h"
#include <Xinput.h>
#include <optional>
#include <assert.h>

class Controller
{
public:
	class Event
	{
		friend Controller;
	private:
		std::optional<WORD> action;
		std::optional<WORD> type;
	private:
		Event()
			:
			action(),
			type()
		{}
		Event(WORD action, WORD type)
			:
			action(action),
			type(type)
		{}
	public:
		bool isEndOfQueue() const
		{
			return !type;
		}
		bool isKeyUp() const
		{
			assert(type);
			return *type & XINPUT_KEYSTROKE_KEYUP;
		}
		bool isKeyDown() const
		{
			assert(type);
			return *type & XINPUT_KEYSTROKE_KEYDOWN;
		}
		bool isRepeat() const
		{
			assert(type);
			return *type & XINPUT_KEYSTROKE_REPEAT;
		}
		WORD GetAction() const
		{
			assert(action);
			return *action;
		}
	};
public:
	Controller() = delete;
	static bool isConnected(int id = 0);
	static bool isActive(int id = 0);
	static Event Read(int id = 0);
	static bool ButtonIsPressed(WORD button_code, int id = 0);
	static int2 GetLeftStickPos(int id = 0);
	static int2 GetRightStickPos(int id = 0);
	static int GetLeftTriggerPos(int id = 0);
	static int GetRightTriggerPos(int id = 0);
	static void VibrateMotors(WORD left_motor_hz = 32768u, WORD right_motor_hz = 32768u, int id = 0);
	static bool isWireless(int id = 0);
	static BYTE GetBatteryType(int id = 0);
	static std::optional<BYTE> GetBatteryLevel(int id = 0);
	static std::optional<BYTE> GetType(int id = 0);
	static std::optional<WORD> GetFeatures(int id = 0);
	static std::optional<XINPUT_GAMEPAD> GetSupportedControls(int id = 0);
	static std::optional<XINPUT_VIBRATION> GetSupportedVibrations(int id = 0);
};


