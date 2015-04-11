#include <iostream>
#include <windows.h>

#include "Application.h"

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow)
{
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	try
	{
		Application app(false, true, 1280, 720, L"Mortal Portal");
		app.Run();
		
	}
	catch (std::exception& e)
	{
		MessageBoxA(NULL, e.what(), "Exeption", MB_ICONERROR | MB_OK);
	}
	return 0;
}