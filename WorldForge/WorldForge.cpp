#include "Engine.h"
#include "Shaders.h"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
{
	try
	{
		PSS::Load(); VSS::Load();
		Engine WorldForge;
		while (true)
		{
			if (const auto exit_code = Window::ProcessMessages())
			{
				return *exit_code;
			}
			WorldForge.Go();
		}
	}
	catch (const BaseException& e)
	{
		MessageBox(nullptr, e.what(), e.GetType(), MB_ICONEXCLAMATION | MB_OK);
	}
	catch (const std::exception& e)
	{
		MessageBox(nullptr, e.what(), "Standard C++ Runtime Exception", MB_ICONEXCLAMATION | MB_OK);
	}
	catch ( ... )
	{
		MessageBox(nullptr, "Unknown error!\nNo details available!\nTry restarting application.", "Unknown Fatal Error", MB_ICONEXCLAMATION | MB_OK);
	}
	return -1;
}


