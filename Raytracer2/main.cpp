#include <Windows.h>;
#include "Graphics.h";
#include <math.h>;
#include "Renderer.h"
Graphics* graphics;




LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam){
	if (uMsg == WM_DESTROY){ PostQuitMessage(0); return 0; }
	return DefWindowProc(hwnd,uMsg,wParam,lParam);
}

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE prevInstance, LPWSTR cmd, int cCmdShow){
	
	WNDCLASSEX windowclass;
	ZeroMemory(&windowclass, sizeof(WNDCLASSEX));
	windowclass.cbSize = sizeof(WNDCLASSEX);
	windowclass.hbrBackground = HBRUSH(COLOR_WINDOW);
	windowclass.hInstance = hInstance;
	windowclass.lpfnWndProc = WindowProc;
	windowclass.lpszClassName = "MainWindow";
	windowclass.style = CS_HREDRAW | CS_VREDRAW; //do i want this, or do i want to lock the window size?
	
	RegisterClassEx(&windowclass);
	RECT rect = {0,0,800,500};
	AdjustWindowRectEx(&rect, WS_OVERLAPPEDWINDOW, false, WS_EX_OVERLAPPEDWINDOW);
	HWND windowHandle = CreateWindowEx(WS_EX_OVERLAPPEDWINDOW, "MainWindow", "Raytracer", WS_OVERLAPPEDWINDOW, 100,100,rect.right - rect.left,rect.bottom-rect.top,NULL,NULL,hInstance,0);
	if (!windowHandle) return -1;

	graphics = new Graphics();
	if (!graphics->Init(windowHandle)){		//Graphics don't work, PANIC!!!
		delete graphics;
		return -1;
	}

	ShowWindow(windowHandle, cCmdShow);
	Renderer loader;
	loader.Load(1);

	MSG msg;
	msg.message = WM_NULL;

	while (msg.message != WM_QUIT){
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)){
			DispatchMessage(&msg);
		}
		else{
			loader.Update();
			loader.Render(graphics);

		}
	}


	delete graphics;

	return 0;
}