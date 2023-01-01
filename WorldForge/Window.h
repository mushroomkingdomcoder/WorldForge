#pragma once
#include "Graphics.h"
#include "Keyboard.h"
#include "Mouse.h"
#include <optional>
#include <memory>

class Window
{
private:
	class WndClass
	{
	private:
		const char* name = "FantasyForgeWnd";
		HINSTANCE hInstance;
	public:
		WndClass()
			:
			hInstance(GetModuleHandle(NULL))
		{
			WNDCLASSEX wcd = {};
			wcd.cbSize = sizeof(wcd);
			wcd.lpfnWndProc = WndMsgSetup;
			wcd.lpszClassName = name;
			wcd.hInstance = hInstance;
			wcd.cbClsExtra = 0;
			wcd.cbWndExtra = 0;
			wcd.hbrBackground = nullptr;
			wcd.hCursor = nullptr;
			wcd.hIcon = static_cast<HICON>(LoadImage(hInstance, nullptr, IMAGE_ICON, 32, 32, 0));
			wcd.hIconSm = static_cast<HICON>(LoadImage(hInstance, nullptr, IMAGE_ICON, 16, 16, 0));
			wcd.lpszMenuName = nullptr;
			wcd.style = CS_OWNDC | CS_DBLCLKS;
			RegisterClassEx(&wcd);
		}
		const char* GetName() const
		{
			return name;
		}
		HINSTANCE GetInstance() const
		{
			return hInstance;
		}
		~WndClass()
		{
			UnregisterClass(name, hInstance);
		}
	};
public:
	class Exception : public BaseException
	{
	private:
		HRESULT hr;
	public:
		Exception() = delete;
		Exception(int line, std::string file, HRESULT hr, std::string note = "") noexcept
			:
			BaseException(line, file, note),
			hr(hr)
		{}
		const char* what() const noexcept override
		{
			std::ostringstream wht;
			wht << "Exception Type: " << GetType() << std::endl
				<< "Error Code: " << hr << std::endl
				<< "Error Description: " << GetErrorCodeDesc(hr) << std::endl
				<< "File Name: " << GetFile() << std::endl
				<< "Line Number: " << GetLine() << std::endl
				<< "Additional Info: " << GetNote() << std::endl;
			whatBuffer = wht.str();
			return whatBuffer.c_str();
		}
		const char* GetType() const noexcept override
		{
			return "FantasyForge Win32 Exception";
		}
	};
private:
	HWND hWnd;
	DWORD style;
	int width;
	int height;
	std::string title;
	std::unique_ptr<Graphics> pGFX = nullptr;
	bool pseudoFullscreen;
private:
	static WndClass wndcls;
public:
	Keyboard kbd;
	Mouse mouse;
	Graphics& gfx()
	{
		if (!pGFX)
		{
			throw GFXEXCPT("Cannot retrieve an unitialized GFX device!");
		}
		else
		{
			return *pGFX;
		}
	}
private:
	static LRESULT WINAPI WndMsgSetup(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam);
	static LRESULT WINAPI WndMsgForward(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam);
	LRESULT WindowMessageProceedure(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam);
public:
	Window(int w, int h, std::string _title, std::vector<int2> display_layer_dims, DWORD style = WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX);
	std::string GetWindowTitle() const;
	void SetWindowTitle(const std::string& _title);
	void SetWindowPosition(int x, int y);
	int2 GetWindowPosition() const;
	void SetWindowDimensions(int width, int height);
	int2 GetWindowDimensions() const;
	void SetPseudoFullscreen();
	bool isPseudoFullscreen() const;
	~Window();
public:
	static std::optional<int> ProcessMessages();
};
inline Window::WndClass Window::wndcls;



