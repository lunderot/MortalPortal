#include "System.h"


System::System(bool fullscreen, bool showCursor, int screenWidth, int screenHeight, LPCWSTR windowTitle)
{
	this->done = false;

	this->fullscreen = fullscreen;
	this->showCursor = showCursor;
	this->screenWidth = screenWidth;
	this->screenHeight = screenHeight;
	this->windowTitle = windowTitle;

	WNDCLASSEX wc;
	DEVMODE dmScreenSettings;
	int posX, posY;

	if (systemHandle)
	{
		throw std::runtime_error("More than one system object detected");
	}
	systemHandle = this;

	hinstance = GetModuleHandle(NULL);

	//Setup the windows class with default settings.
	wc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	wc.lpfnWndProc = WndProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = hinstance;
	wc.hIcon = LoadIcon(NULL, IDI_WINLOGO);
	wc.hIconSm = wc.hIcon;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	wc.lpszMenuName = NULL;
	wc.lpszClassName = windowTitle;
	wc.cbSize = sizeof(WNDCLASSEX);

	RegisterClassEx(&wc);

	//Setup the screen settings depending on whether it is running in full screen or in windowed mode.
	if (fullscreen)
	{
		//Determine the resolution of the screen.
		screenWidth = GetSystemMetrics(SM_CXSCREEN);
		screenHeight = GetSystemMetrics(SM_CYSCREEN);

		//If full screen set the screen to maximum size of the users desktop and 32bit.
		memset(&dmScreenSettings, 0, sizeof(dmScreenSettings));
		dmScreenSettings.dmSize = sizeof(dmScreenSettings);
		dmScreenSettings.dmPelsWidth = (unsigned long)screenWidth;
		dmScreenSettings.dmPelsHeight = (unsigned long)screenHeight;
		dmScreenSettings.dmBitsPerPel = 32;
		dmScreenSettings.dmFields = DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;

		ChangeDisplaySettings(&dmScreenSettings, CDS_FULLSCREEN);

		posX = 0;
		posY = 0;
	}
	else //If windowed
	{
		//Place the window in the middle of the screen.
		posX = (GetSystemMetrics(SM_CXSCREEN) - screenWidth) / 2;
		posY = (GetSystemMetrics(SM_CYSCREEN) - screenHeight) / 2;

		realScreenHeight = GetSystemMetrics(SM_CYSCREEN);
		realScreenWidth = GetSystemMetrics(SM_CXSCREEN);
	}

	hwnd = CreateWindowEx(WS_EX_APPWINDOW, windowTitle, windowTitle,
		WS_OVERLAPPEDWINDOW | CW_USEDEFAULT | CW_USEDEFAULT,
		posX, posY, screenWidth, screenHeight, NULL, NULL, hinstance, NULL);


	ShowWindow(hwnd, SW_SHOW);
	SetForegroundWindow(hwnd);
	SetFocus(hwnd);

	ShowCursor(showCursor);
}


System::~System()
{
	ShowCursor(true);

	if (fullscreen)
	{
		ChangeDisplaySettings(NULL, 0);
	}

	DestroyWindow(hwnd);
	hwnd = NULL;

	UnregisterClass(windowTitle, hinstance);
	hinstance = NULL;

	systemHandle = nullptr;
}

void System::Run()
{
	float dt = 1 / 60.0f;
	float currentTime = GetTickCount() / 1000.0f;


	MSG windowMessage;
	ZeroMemory(&windowMessage, sizeof(windowMessage));

	while (!done)
	{
		//Handle window events
		if (PeekMessage(&windowMessage, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&windowMessage);
			DispatchMessage(&windowMessage);
		}
		if (windowMessage.message == WM_QUIT)
		{
			done = true;
		}
		//Update
		float newTime = GetTickCount() / 1000.0f;
		float frameTime = newTime - currentTime;
		currentTime = newTime;

		while (frameTime > 0.0f)
		{
			float deltaTime = min(frameTime, dt);
			done = Update(deltaTime);
			frameTime -= deltaTime;
		}
		//Render
		Render();
	}
}

LRESULT CALLBACK System::MessageHandler(HWND hwnd, UINT umsg, WPARAM wparam, LPARAM lparam)
{
	return DefWindowProc(hwnd, umsg, wparam, lparam);
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT umessage, WPARAM wparam, LPARAM lparam)
{
	switch (umessage)
	{
	case WM_DESTROY:
	{
		PostQuitMessage(0);
		return 0;
	}
	case WM_CLOSE:
	{
		PostQuitMessage(0);
		return 0;
	}
	default:
	{
		return systemHandle->MessageHandler(hwnd, umessage, wparam, lparam);
	}
	}
}
