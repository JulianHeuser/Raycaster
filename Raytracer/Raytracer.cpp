// Raytracer.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "Raytracer.h"

#include "math.h"

#define MAX_LOADSTRING 100


//Some variables
float playerX = 3.0;
float playerY = 3.0;
float playerRadius = 0.25;

float planeX = 0;
float planeY = .66;

float fov = 1;
float moveSpeed = 0.1;
float turnSpeed = .1;

float dirX = -1;
float dirY = 0;

float xChange = 0;
float yChange = 0;

//double rayX = 0.0;
//double rayY = 0.0;

float rays = 0; //How many rays have been drawn

float width = 500;
int height = 500;
int grid[6][6] =  { { 1, 1, 1, 1, 1, 1 },
					{ 1, 0, 0, 1, 0, 1 },
					{ 1, 0, 0, 0, 0, 1 },
					{ 1, 1, 0, 0, 0, 1 },
					{ 1, 0, 0, 0, 0, 1 },
					{ 1, 1, 1, 1, 1, 1 } };

//calculate how many segments to use
float interval = ((2 * fov) + 1) / width;

// Global Variables:
HINSTANCE hInst;								// current instance
TCHAR szTitle[MAX_LOADSTRING];					// The title bar text
TCHAR szWindowClass[MAX_LOADSTRING];			// the main window class name

// Forward declarations of functions included in this code module:
ATOM				MyRegisterClass(HINSTANCE hInstance);
BOOL				InitInstance(HINSTANCE, int);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK	About(HWND, UINT, WPARAM, LPARAM);

int APIENTRY _tWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPTSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

 	// MAIN CODE

	//END MAIN CODE

	MSG msg;
	HACCEL hAccelTable;

	// Initialize global strings
	LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadString(hInstance, IDC_RAYTRACER, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// Perform application initialization:
	if (!InitInstance (hInstance, nCmdShow))
	{
		return FALSE;
	}

	hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_RAYTRACER));

	// Main message loop:
	while (GetMessage(&msg, NULL, 0, 0))
	{
		if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	return (int) msg.wParam;
}



//
//  FUNCTION: MyRegisterClass()
//
//  PURPOSE: Registers the window class.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style			= CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc	= WndProc;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= hInstance;
	wcex.hIcon			= LoadIcon(hInstance, MAKEINTRESOURCE(IDI_RAYTRACER));
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
	wcex.lpszMenuName	= MAKEINTRESOURCE(IDC_RAYTRACER);
	wcex.lpszClassName	= szWindowClass;
	wcex.hIconSm		= LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	return RegisterClassEx(&wcex);
}

//
//   FUNCTION: InitInstance(HINSTANCE, int)
//
//   PURPOSE: Saves instance handle and creates main window
//
//   COMMENTS:
//
//        In this function, we save the instance handle in a global variable and
//        create and display the main program window.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   HWND hWnd;

   hInst = hInstance; // Store instance handle in our global variable

   hWnd = CreateWindow(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, NULL, NULL, hInstance, NULL);

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

//
//  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE:  Processes messages for the main window.
//
//  WM_COMMAND	- process the application menu
//  WM_PAINT	- Paint the main window
//  WM_DESTROY	- post a quit message and return
//
//

void drawLine(float StartX, float StartY, float EndX, float EndY, HWND hwnd)
{
	HDC hdc;

	hdc = GetDC(hwnd);
	MoveToEx(hdc, StartX, StartY, NULL);
	LineTo(hdc, EndX, EndY);

	ReleaseDC(hwnd, hdc);
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	int wmId, wmEvent;
	PAINTSTRUCT ps;
	HDC hdc;
	HPEN Hpen;


	//{
	//case WM_KEY:
	//{
		float oldDirX = dirX;
		float oldDirY = dirY;
		float oldPlaneX = planeX;
		int realPlayerX = floor(playerX);
		int realPlayerY = floor(playerY);
		//switch (wParam){
		//case 0x57:	//W
		if (GetAsyncKeyState(0x57)){
			yChange = moveSpeed * dirY;
			xChange = moveSpeed * dirX;
			InvalidateRect(hWnd, NULL, TRUE);
		}
		//case 0x53:	//S
		if (GetAsyncKeyState(0x53)){
			yChange = -moveSpeed * dirY;
			xChange = -moveSpeed * dirX;
			InvalidateRect(hWnd, NULL, TRUE);
		}
		//case 0x41:	//A
		if (GetAsyncKeyState(0x41)){
			xChange = moveSpeed * -dirY;
			yChange = moveSpeed * dirX;
			InvalidateRect(hWnd, NULL, TRUE);
		}
		//case 0x44:	//D
		if (GetAsyncKeyState(0x44)){
			xChange = moveSpeed * dirY;
			yChange = moveSpeed * -dirX;
			InvalidateRect(hWnd, NULL, TRUE);
		}
		//case 0x25:	//Left
		if (GetAsyncKeyState(0x25)){
			dirX = (oldDirX * cos(turnSpeed)) - (dirY * sin(turnSpeed));
			dirY = (dirY * cos(turnSpeed)) + (oldDirX * sin(turnSpeed));

			planeX = (planeX * cos(turnSpeed)) - (planeY * sin(turnSpeed));
			planeY = (planeY * cos(turnSpeed)) + (oldPlaneX * sin(turnSpeed));
			InvalidateRect(hWnd, NULL, TRUE);
		}
		//case 0x27:	//Right
		if (GetAsyncKeyState(0x27)){
			dirX = (oldDirX * cos(turnSpeed)) + (dirY * sin(turnSpeed));
			dirY = (dirY * cos(turnSpeed)) - (oldDirX * sin(turnSpeed));

			planeX = (planeX * cos(turnSpeed)) + (planeY * sin(turnSpeed));
			planeY = (planeY * cos(turnSpeed)) - (oldPlaneX * sin(turnSpeed));
			InvalidateRect(hWnd, NULL, TRUE);
		}
	switch (message)
	{
	case WM_COMMAND:
		wmId = LOWORD(wParam);
		wmEvent = HIWORD(wParam);
		// Parse the menu selections:
		switch (wmId)
		{
		case IDM_ABOUT:
			DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
			break;
		case IDM_EXIT:
			DestroyWindow(hWnd);
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
		break;
	case WM_PAINT:
	{

		//Move Character
		if (xChange < 0){
			playerRadius = -playerRadius;
		}
		else{
			playerRadius = abs(playerRadius);
		}
		if (xChange != 0 && grid[(int)floor(playerY)][(int)floor(playerX + xChange + playerRadius)] == 0){
			playerX += xChange;
			xChange = 0;
		}

		if (yChange < 0){
			playerRadius = -playerRadius;
		}
		else{
			playerRadius = abs(playerRadius);
		}
		if (yChange != 0 && grid[(int)floor(playerY + yChange + playerRadius)][(int)floor(playerX)] == 0){
			playerY += yChange;
			yChange = 0;
		}

		hdc = BeginPaint(hWnd, &ps);
		SelectObject(hdc, GetStockObject(DC_PEN));
		SelectObject(hdc, GetStockObject(DC_BRUSH));
		SetDCPenColor(hdc, RGB(0, 0, 255));
		SetDCBrushColor(hdc, RGB(0, 0, 255));
		// DRAWING
		//Map
		int mapOffsetX = 600;
		int mapOffsetY = 100;
		for (int y = 0; y < 6; y++){
			for (int x = 0; x < 6; x++){
				if (grid[y][x] > 0){
					Rectangle(hdc, mapOffsetX + (x * 10), mapOffsetY + (y * 10), (mapOffsetX + (x * 10)) + 10, (mapOffsetY + (y * 10)) + 10);
				}
			}
		}
		Ellipse(hdc, mapOffsetX + (playerX * 10)-5, mapOffsetY + (playerY * 10)-5, (mapOffsetX + (playerX * 10)) + 5, (mapOffsetY+ (playerY * 10)+5));	//draw player

		//Raytracing:
		//rayX = playerX; //- fov;
		//rayY = playerY;
		rays = 0;
		while (rays < width){
			float raysNormalized = (rays - (width / 2)) / (width / 2);

			float rayDirX = dirX + (raysNormalized * planeX);
			float rayDirY = dirY + (raysNormalized * planeY);

			//DDA
			float sideDistX;
			float sideDistY;

			float deltaDistX = abs(1 / rayDirX);
			float deltaDistY = abs(1 / rayDirY);
			
			int stepX;
			int stepY;

			int mapX = static_cast<int>(floor(playerX));
			int mapY = static_cast<int>(floor(playerY));

			int side;

			if (rayDirX < 0){
				stepX = -1;	
				sideDistX = (playerX - mapX) * deltaDistX;
			}
			else{
				stepX = 1;
				sideDistX = (mapX + 1 - playerX) * deltaDistX;
			}
			if (rayDirY < 0){
				stepY = -1;
				sideDistY = (playerY - mapY) * deltaDistY;
			}
			else{
				stepY = 1;
				sideDistY = (mapY + 1 - playerY) * deltaDistY;
			}

			//end DDA

			while (grid[mapY][mapX] == 0){
				float oldRay[2] = { mapX, mapY };

				if (sideDistX < sideDistY){
					sideDistX += deltaDistX;
					mapX += stepX;
					side = 0;
				}
				else{
					sideDistY += deltaDistY;
					mapY += stepY;
					side = 1;
				}

				drawLine(mapOffsetX + (oldRay[0]*10), mapOffsetY + (oldRay[1]*10), mapOffsetX + (mapX*10), mapOffsetY + (mapY*10), hWnd);	//Debug Rays
			}
			//float dist = sqrt(pow(abs(rayX - oldRay[0]), 2) + pow(abs(rayY - oldRay[1]), 2));
			float dist = sqrt(pow(abs(mapX - playerX), 2) + pow(abs(mapY - playerY), 2));
			float size = (dist / 6)*height;

			float perpWallDist;
			//drawLine(rays, size/2, rays, height - (size/2), hWnd);
			if (side == 0){
				perpWallDist = (mapX - playerX + (1 - stepX) / 2) / rayDirX;
			}
			else{
				perpWallDist = (mapY - playerY + (1 - stepY) / 2) / rayDirY;
			}
			int lineHeight = static_cast<int>(height / perpWallDist);
			int drawStart = -lineHeight / 2 + height / 2;
			int drawEnd = lineHeight / 2 + height / 2;
			if (side == 0){
			}
			else{
			}
			drawLine(rays, drawStart, rays, drawEnd, hWnd);
			

			//create_line(rays, size / 2, rays, height - (size / 2));
			//rayX += interval;
			rays += 1;
		}

		//TCHAR TextArray[] = { 'test' };
		//TextOut(hdc, 0, 0, TextArray, ARRAYSIZE(TextArray));

		
		//ENDDRAWING
		EndPaint(hWnd, &ps);
		break;
	}
	case WM_DESTROY:
		PostQuitMessage(0);
		break;

	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

// Message handler for about box.
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
		return (INT_PTR)TRUE;

	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
		{
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		break;
	}
	return (INT_PTR)FALSE;
}


/*
void castRays(float rayX, float rayY, float fov, float width, float height, float playerY, float playerX){
	int grid[6][6] = { { 1, 1, 1, 1, 1, 1 },
	{ 1, 0, 0, 0, 1, 1 },
	{ 1, 0, 1, 0, 0, 1 },
	{ 1, 0, 0, 0, 0, 1 },
	{ 1, 0, 0, 0, 0, 1 },
	{ 1, 1, 1, 1, 1, 1 } };
	int rays;
	float interval = ((2 * fov) + 1) / width;
	rayX = playerX - fov;
	rayY = playerY;
	rays = 0;
	while (rays < width){
		int oldRay[2] = { rayX, rayY };
		int newRayX = static_cast<int>(rayX);
		int newRayY = static_cast<int>(rayY);
		while (grid[newRayX][newRayY] == 0){
			rayY -= 0.1;
		}

		float dist = sqrt(pow(abs(rayX - oldRay[0]),2) + pow(abs(rayY - oldRay[1]),2));
		float size = (dist / 6)*height;

		//create_line(rays, size / 2, rays, height - (size / 2));
		rayX += interval;
		rayY = playerY;
		rays += 1;

	}
}
*/