#include "Window.h"
#include <assert.h>

#define WNDEXCPT Window::Exception(__LINE__, __FILE__, GetLastError())
#define WNDEXCPT_NOTE(note) Window::Exception(__LINE__, __FILE__, GetLastError(), note)

LRESULT WINAPI Window::WndMsgSetup(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam)
{
	if (Msg == WM_NCCREATE)
	{
		const CREATESTRUCT* pCreateStruct = reinterpret_cast<CREATESTRUCT*>(lParam);
		Window const* pWindow = static_cast<Window*>(pCreateStruct->lpCreateParams);
		SetWindowLongPtr(hWnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(pWindow));
		SetWindowLongPtr(hWnd, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(WndMsgForward));
	}
	return DefWindowProc(hWnd, Msg, wParam, lParam);
}

LRESULT WINAPI Window::WndMsgForward(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam)
{
	Window* pWindow = reinterpret_cast<Window*>(GetWindowLongPtr(hWnd, GWLP_USERDATA));
	return pWindow->WindowMessageProceedure(hWnd, Msg, wParam, lParam);
}

LRESULT Window::WindowMessageProceedure(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam)
{
	switch (Msg)
	{
		case WM_SYSKEYDOWN:
		case WM_KEYDOWN:
		{
			if (kbd.AutorepeatIsEnabled() || !(lParam & 0x40000000))
			{
				kbd.OnKeyDown((char)wParam);
			}
			break;
		}
		case WM_SYSKEYUP:
		case WM_KEYUP:
		{
			kbd.OnKeyUp((char)wParam);
			break;
		}
		case WM_CHAR:
		{
			kbd.OnChar((char)wParam);
			break;
		}
		case WM_MOUSEMOVE:
		{
			const auto coords = MAKEPOINTS(lParam);
			if (coords.x >= 0 && coords.x < (int)width && coords.y >= 0 && coords.y < (int)height)
			{
				if (!mouse.IsInWindow())
				{
					mouse.OnMouseEnter();
					SetCapture(hWnd);
				}
				mouse.OnMouseMove(coords.x, coords.y);
			}
			else
			{
				if (mouse.LeftIsClicked() || mouse.RightIsClicked())
				{
					mouse.OnMouseMove(coords.x, coords.y);
				}
				else
				{
					mouse.OnMouseLeave();
					ReleaseCapture();
				}
			}
			break;
		}
		case WM_LBUTTONDOWN:
		{
			const auto coords = MAKEPOINTS(lParam);
			mouse.OnLeftClick(coords.x, coords.y);
			break;
		}
		case WM_LBUTTONDBLCLK:
		{
			const auto coords = MAKEPOINTS(lParam);
			mouse.OnLeftDoubleClick(coords.x, coords.y);
			break;
		}
		case WM_LBUTTONUP:
		{
			const auto coords = MAKEPOINTS(lParam);
			mouse.OnLeftRelease(coords.x, coords.y);
			break;
		}
		case WM_RBUTTONDOWN:
		{
			const auto coords = MAKEPOINTS(lParam);
			mouse.OnRightClick(coords.x, coords.y);
			break;
		}
		case WM_RBUTTONDBLCLK:
		{
			const auto coords = MAKEPOINTS(lParam);
			mouse.OnRightDoubleClick(coords.x, coords.y);
			break;
		}
		case WM_RBUTTONUP:
		{
			const auto coords = MAKEPOINTS(lParam);
			mouse.OnRightRelease(coords.x, coords.y);
			break;
		}
		case WM_MBUTTONDOWN:
		{
			const auto coords = MAKEPOINTS(lParam);
			mouse.OnMiddleClick(coords.x, coords.y);
			break;
		}
		case WM_MBUTTONUP:
		{
			const auto coords = MAKEPOINTS(lParam);
			mouse.OnMiddleRelease(coords.x, coords.y);
			break;
		}
		case WM_MOUSEWHEEL:
		{
			const auto z = GET_WHEEL_DELTA_WPARAM(wParam);
			mouse.OnWheelScroll(z);
			break;
		}
		case WM_SIZE:
		{
			if (wParam != SIZE_MINIMIZED)
			{
				if (pseudoFullscreen)
				{
					SetWindowDimensions(width, height);
					pseudoFullscreen = true;
				}
				else if (pGFX)
				{
					int newWidth = lParam & 0x0000FFFF;
					if (!newWidth) newWidth = 1;
					int newHeight = (lParam & 0xFFFF0000) >> 16;
					if (!newHeight) newHeight = 1;
					float xScale = float((float)newWidth / (float)width);
					float yScale = float((float)newHeight / (float)height);
					pGFX->UpdateViewportsAndFrameManager(xScale, yScale);
					width = newWidth;
					height = newHeight;
					pseudoFullscreen = false;
				}
			}
			break;
		}
		case WM_KILLFOCUS:
		{
			kbd.ClearKeystates();
			break;
		}
		case WM_CLOSE:
		{
			PostQuitMessage(0);
			return 0;
		}
	}
	return DefWindowProc(hWnd, Msg, wParam, lParam);
}

Window::Window(int w, int h, std::string _title, std::vector<int2> display_layer_dims, DWORD style)
	:
	style(style),
	width(w),
	height(h),
	title(_title),
	kbd(),
	mouse()
{
	RECT window = {};
	window.left = 0;
	window.right = width;
	window.top = 0;
	window.bottom = height;
	if (!AdjustWindowRect(&window, style, FALSE))
	{
		throw WNDEXCPT_NOTE("Failed to adjust window dimensions!");
	}
	hWnd = CreateWindowEx(
		0, wndcls.GetName(), title.c_str(), style, CW_USEDEFAULT, CW_USEDEFAULT,
		window.right - window.left, window.bottom - window.top, nullptr, nullptr, wndcls.GetInstance(), this);
	if (!hWnd)
	{
		throw WNDEXCPT_NOTE("Failed to create window!");
	}
	ShowWindow(hWnd, SW_SHOW);
	assert(!display_layer_dims.empty());
#ifdef _DEBUG
	for (int i = 0; i < display_layer_dims.size(); ++i)
	{
		assert(display_layer_dims[i].x % 32 == 0);
	}
#endif
	pGFX = std::make_unique<Graphics>(hWnd, width, height, display_layer_dims);
}

std::string Window::GetWindowTitle() const
{
	return title;
}

void Window::SetWindowTitle(const std::string& _title)
{
	title = _title;
	SetWindowText(hWnd, title.c_str());
}

void Window::SetWindowPosition(int x, int y)
{
	assert(!pGFX->isFullscreen());
	if (!SetWindowPos(hWnd, HWND_TOP, x, y, width, height, SWP_NOSIZE))
	{
		throw WNDEXCPT_NOTE("Failed setting new window position!");
	}
	pseudoFullscreen = false;
}

int2 Window::GetWindowPosition() const
{
	RECT winRect;
	GetWindowRect(hWnd, &winRect);
	return { winRect.left,winRect.top };
}

void Window::SetWindowDimensions(int width, int height)
{
	assert(!pGFX->isFullscreen());
	RECT window = {};
	window.left = 0;
	window.right = width;
	window.top = 0;
	window.bottom = height;
	if (!AdjustWindowRect(&window, style, FALSE))
	{
		throw WNDEXCPT_NOTE("Failed to adjust window dimensions!");
	}
	if (!SetWindowPos(hWnd, HWND_TOP, 0, 0, window.right - window.left, window.bottom - window.top, SWP_NOMOVE | SWP_NOSENDCHANGING))
	{
		throw WNDEXCPT_NOTE("Failed setting new window dimensions!");
	}
}

int2 Window::GetWindowDimensions() const
{
	return { width,height };
}

void Window::SetPseudoFullscreen()
{
	assert(!pGFX->isFullscreen());
	RECT window = {};
	if (!AdjustWindowRect(&window, style, FALSE))
	{
		throw WNDEXCPT_NOTE("Failed to adjust window dimensions!");
	}
	SetWindowPosition(window.left, window.top);
	HMONITOR hMon = MonitorFromWindow(hWnd, MONITOR_DEFAULTTONEAREST);
	MONITORINFO mInfo = { sizeof(mInfo) };
	if (!GetMonitorInfo(hMon, &mInfo))
	{
		throw WNDEXCPT_NOTE("Failed retriving display monitor info!");
	}
	SetWindowDimensions(mInfo.rcMonitor.right - mInfo.rcMonitor.left, mInfo.rcMonitor.bottom - mInfo.rcMonitor.top);
	pseudoFullscreen = true;
}

bool Window::isPseudoFullscreen() const
{
	return pseudoFullscreen;
}

Window::~Window()
{
	DestroyWindow(hWnd);
}

std::optional<int> Window::ProcessMessages()
{
	MSG msg;
	if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
	{
		if (msg.message == WM_QUIT)
		{
			return (int)msg.wParam;
		}
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	return std::optional<int>();
}