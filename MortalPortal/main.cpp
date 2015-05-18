#include <iostream>
#include <windows.h>

#include "Application.h"

int main()
{
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	try
	{
		Application app(false, true, 1280, 720, L"Mortal Portal");
		app.Run();
		
	}
	catch (std::exception& e)
	{
		std::cout << e.what() << std::endl;
	}
	return 0;
}