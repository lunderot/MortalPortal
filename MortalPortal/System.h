#pragma once

#include <windows.h>
#include <stdexcept>

class System
{
protected:
	bool done;

	bool fullscreen;
	bool showCursor;
	int screenWidth;
	int screenHeight;
	LPCWSTR windowTitle;

	int realScreenWidth;
	int realScreenHeight;

	HINSTANCE hinstance;
	HWND hwnd;
private:
	virtual bool Update(float deltaTime) = 0; //Returns true on quit
	virtual void Render() = 0;
public:
	System(bool fullscreen, bool showCursor, int screenWidth, int screenHeight, LPCWSTR windowTitle);
	virtual ~System();

	void Run();

	LRESULT CALLBACK MessageHandler(HWND hwnd, UINT umsg, WPARAM wparam, LPARAM lparam);
};

LRESULT CALLBACK WndProc(HWND hwnd, UINT umessage, WPARAM wparam, LPARAM lparam);
static System* systemHandle = nullptr;
