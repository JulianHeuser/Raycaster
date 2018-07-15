#include <Windows.h>;
#include "Graphics.h";
#include <math.h>;
#include "Loader.h"
#include "GameLoader.h"
Graphics* graphics;




LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam){
	if (uMsg == WM_DESTROY){ PostQuitMessage(0); return 0; }
	
	//GameLoader::SwitchLevel(new Loader(), 2); -Switching level syntax

	/*float oldDirX = dirX;
	float oldDirY = dirY;
	float oldPlaneX = planeX;
	int realPlayerX = floor(playerX);
	int realPlayerY = floor(playerY);
	//switch (wParam){
	//case 0x57:	//W
	if (GetAsyncKeyState(0x57)){
		yChange = moveSpeed * dirY;
		xChange = moveSpeed * dirX;
	}
	//case 0x53:	//S
	if (GetAsyncKeyState(0x53)){
		yChange = -moveSpeed * dirY;
		xChange = -moveSpeed * dirX;
	}
	//case 0x41:	//A
	if (GetAsyncKeyState(0x41)){
		xChange = moveSpeed * -dirY;
		yChange = moveSpeed * dirX;
	}
	//case 0x44:	//D
	if (GetAsyncKeyState(0x44)){
		xChange = moveSpeed * dirY;
		yChange = moveSpeed * -dirX;
	}
	//case 0x25:	//Left
	if (GetAsyncKeyState(0x25)){
		dirX = (oldDirX * cos(turnSpeed)) - (dirY * sin(turnSpeed));
		dirY = (dirY * cos(turnSpeed)) + (oldDirX * sin(turnSpeed));

		planeX = (planeX * cos(turnSpeed)) - (planeY * sin(turnSpeed));
		planeY = (planeY * cos(turnSpeed)) + (oldPlaneX * sin(turnSpeed));
	}
	//case 0x27:	//Right
	if (GetAsyncKeyState(0x27)){
		dirX = (oldDirX * cos(turnSpeed)) + (dirY * sin(turnSpeed));
		dirY = (dirY * cos(turnSpeed)) - (oldDirX * sin(turnSpeed));

		planeX = (planeX * cos(turnSpeed)) + (planeY * sin(turnSpeed));
		planeY = (planeY * cos(turnSpeed)) - (oldPlaneX * sin(turnSpeed));
	}
	*/
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

	GameLoader::LoadInitialLevel(new Loader());

	MSG msg;
	msg.message = WM_NULL;

	while (msg.message != WM_QUIT){
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)){
			DispatchMessage(&msg);
		}
		else{
			GameLoader::Update();
			GameLoader::Render(graphics);

		}
	}


	delete graphics;

	return 0;
}