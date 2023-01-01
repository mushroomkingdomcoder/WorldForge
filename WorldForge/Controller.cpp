#include "Controller.h"
#include "BaseException.h"
#pragma comment(lib, "Xinput.lib")

#define XINEXCPT Controller::Exception(__LINE__, __FILE__);
#define XINEXCPT_NOTE(note) Controller::Exception(__LINE__, __FILE__, note);

bool Controller::isConnected(int id)
{
	XINPUT_STATE state = {};
	return XInputGetState(id, &state) == ERROR_SUCCESS;
}

bool Controller::isActive(int id)
{
	assert(id >= 0 && id < 4);
	XINPUT_KEYSTROKE keyStroke = {};
	return XInputGetKeystroke(id, 0, &keyStroke) == ERROR_SUCCESS;
}

Controller::Event Controller::Read(int id)
{
	XINPUT_KEYSTROKE keyStroke = {};
	DWORD result = XInputGetKeystroke(id, 0, &keyStroke);
	switch (result)
	{
	case ERROR_SUCCESS:
		return Event(keyStroke.VirtualKey, keyStroke.Flags);
	case ERROR_EMPTY:
	case ERROR_NOT_CONNECTED:
		return Event();
	default:
		throw EXCPT_NOTE(BaseException::GetErrorCodeDesc((HRESULT)result) + "\n\nFailed to retrieve controller keystroke!");
	}
}

bool Controller::ButtonIsPressed(WORD button_code, int id )
{
	assert(id >= 0 && id < 4);
	XINPUT_STATE cState = {};
	if (XInputGetState(id, &cState) == ERROR_SUCCESS)
	{
		return cState.Gamepad.wButtons & button_code;
	}
	return false;
}

int2 Controller::GetLeftStickPos(int id)
{
	assert(id >= 0 && id < 4);
	XINPUT_STATE cState = {};
	if (XInputGetState(id, &cState) == ERROR_SUCCESS)
	{
		int stickX = 0, stickY = 0;
		if (abs(cState.Gamepad.sThumbLX) > XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE)
		{
			stickX = cState.Gamepad.sThumbLX - (cState.Gamepad.sThumbLX / abs(cState.Gamepad.sThumbLX)) * XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE;
		}
		if (abs(cState.Gamepad.sThumbLY) > XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE)
		{
			stickY = cState.Gamepad.sThumbLY - (cState.Gamepad.sThumbLY / abs(cState.Gamepad.sThumbLY)) * XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE;
		}
		return { stickX,stickY };
	}
	return { 0,0 };
}

int2 Controller::GetRightStickPos(int id)
{
	assert(id >= 0 && id < 4);
	XINPUT_STATE cState = {};
	if (XInputGetState(id, &cState) == ERROR_SUCCESS)
	{
		int stickX = 0, stickY = 0;
		if (abs(cState.Gamepad.sThumbRX) > XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE)
		{
			stickX = cState.Gamepad.sThumbRX - (cState.Gamepad.sThumbRX / abs(cState.Gamepad.sThumbRX)) * XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE;
		}
		if (abs(cState.Gamepad.sThumbRY) > XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE)
		{
			stickY = cState.Gamepad.sThumbRY - (cState.Gamepad.sThumbRY / abs(cState.Gamepad.sThumbRY)) * XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE;
		}
		return { stickX,stickY };
	}
	return { 0,0 };
}

int Controller::GetLeftTriggerPos(int id)
{
	assert(id >= 0 && id < 4);
	XINPUT_STATE cState = {};
	if (XInputGetState(id, &cState) == ERROR_SUCCESS)
	{
		if (cState.Gamepad.bLeftTrigger > XINPUT_GAMEPAD_TRIGGER_THRESHOLD)
		{
			return cState.Gamepad.bLeftTrigger - XINPUT_GAMEPAD_TRIGGER_THRESHOLD;
		}
	}
	return 0;
}

int Controller::GetRightTriggerPos(int id)
{
	assert(id >= 0 && id < 4);
	XINPUT_STATE cState = {};
	if (XInputGetState(id, &cState) == ERROR_SUCCESS)
	{
		if (cState.Gamepad.bRightTrigger > XINPUT_GAMEPAD_TRIGGER_THRESHOLD)
		{
			return cState.Gamepad.bRightTrigger - XINPUT_GAMEPAD_TRIGGER_THRESHOLD;
		}
	}
	return 0;
}

void Controller::VibrateMotors(WORD left_motor_hz, WORD right_motor_hz, int id)
{
	assert(id >= 0 && id < 4);
	XINPUT_VIBRATION vibration = {};
	vibration.wLeftMotorSpeed = left_motor_hz;
	vibration.wRightMotorSpeed = right_motor_hz;
	XInputSetState(id, &vibration);
}

bool Controller::isWireless(int id)
{
	return GetBatteryType(id) != BATTERY_TYPE_WIRED;
}

BYTE Controller::GetBatteryType(int id)
{
	XINPUT_BATTERY_INFORMATION bInfo = {};
	XInputGetBatteryInformation(id, XINPUT_DEVTYPE_GAMEPAD, &bInfo);
	return bInfo.BatteryType;
}

std::optional<BYTE> Controller::GetBatteryLevel(int id)
{
	XINPUT_BATTERY_INFORMATION bInfo = {};
	if (XInputGetBatteryInformation(id, XINPUT_DEVTYPE_GAMEPAD, &bInfo) == ERROR_SUCCESS)
	{
		return bInfo.BatteryLevel;
	}
	return {};
}

std::optional<BYTE> Controller::GetType(int id)
{
	XINPUT_CAPABILITIES capabilities = {};
	DWORD result = XInputGetCapabilities(id, XINPUT_FLAG_GAMEPAD, &capabilities);
	switch (result)
	{
	case ERROR_SUCCESS:
		return capabilities.SubType;
	case ERROR_DEVICE_NOT_CONNECTED:
		return {};
	default:
		throw EXCPT_NOTE(BaseException::GetErrorCodeDesc((HRESULT)result) + "\n\nFailed to retrieve controller type!");
	}
}

std::optional<WORD> Controller::GetFeatures(int id)
{
	XINPUT_CAPABILITIES capabilities = {};
	DWORD result = XInputGetCapabilities(id, XINPUT_FLAG_GAMEPAD, &capabilities);
	switch (result)
	{
	case ERROR_SUCCESS:
		return capabilities.Flags;
	case ERROR_DEVICE_NOT_CONNECTED:
		return {};
	default:
		throw EXCPT_NOTE(BaseException::GetErrorCodeDesc((HRESULT)result) + "\n\nFailed to retrieve controller features!");
	}
}

std::optional<XINPUT_GAMEPAD> Controller::GetSupportedControls(int id)
{
	XINPUT_CAPABILITIES capabilities = {};
	DWORD result = XInputGetCapabilities(id, XINPUT_FLAG_GAMEPAD, &capabilities);
	switch (result)
	{
	case ERROR_SUCCESS:
		return capabilities.Gamepad;
	case ERROR_DEVICE_NOT_CONNECTED:
		return {};
	default:
		throw EXCPT_NOTE(BaseException::GetErrorCodeDesc((HRESULT)result) + "\n\nFailed to detect controller buttons!");
	}
}

std::optional<XINPUT_VIBRATION> Controller::GetSupportedVibrations(int id)
{
	XINPUT_CAPABILITIES capabilities = {};
	DWORD result = XInputGetCapabilities(id, XINPUT_FLAG_GAMEPAD, &capabilities);
	switch (result)
	{
	case ERROR_SUCCESS:
		return capabilities.Vibration;
	case ERROR_DEVICE_NOT_CONNECTED:
		return {};
	default:
		throw EXCPT_NOTE(BaseException::GetErrorCodeDesc((HRESULT)result) + "\n\nFailed to detect controller vibration tech!");
	}
}