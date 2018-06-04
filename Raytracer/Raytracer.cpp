// Raytracer.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "Raytracer.h"

#include "math.h"

#define MAX_LOADSTRING 100


//Some variables
float playerX = 3.0;
float playerY = 3.0;

float fov = 1;
float moveSpeed = 1.0;
float turnSpeed = 1.0;

float dirX = 0;
float dirY = 1;

double rayX = 0.0;
double rayY = 0.0;

float rays = 0; //How many rays have been drawn

float width = 500;
int height = 500;
int grid[6][6] =  { { 1, 1, 1, 1, 1, 1 },
					{ 1, 0, 0, 0, 0, 1 },
					{ 1, 0, 0, 0, 0, 1 },
					{ 1, 0, 0, 0, 0, 1 },
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

	switch (message)
	{
	case WM_KEYDOWN:
	{
		switch (wParam){
		case 0x57:	//W
			playerY -= 1;
			InvalidateRect(hWnd, NULL, TRUE);
			break;
		case 0x53:	//S
			playerY += 1;
			InvalidateRect(hWnd, NULL, TRUE);
			break;
		case 0x41:	//A
			playerX -= 1;
			InvalidateRect(hWnd, NULL, TRUE);
			break;
		case 0x44:	//D
			playerX += 1;
			InvalidateRect(hWnd, NULL, TRUE);
			break;
		case 0x25:	//Left
			InvalidateRect(hWnd, NULL, TRUE);
			break;
		case 0x27:	//Right
			InvalidateRect(hWnd, NULL, TRUE);
			break;
		}
	}
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
		hdc = BeginPaint(hWnd, &ps);
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
		Ellipse(hdc, mapOffsetX + (playerX * 10), mapOffsetY + (playerY * 10), (mapOffsetX + (playerX * 10)) + 10, (mapOffsetY+ (playerY * 10)+10));	//draw player

		//Raytracing:
		rayX = playerX; //- fov;
		rayY = playerY;
		rays = 0;
		while (rays < width){
			float raysNormalized = (rays - (width / 2)) / (width / 2);
			int newRayX = static_cast<int>(floor(rayX));
			int newRayY = static_cast<int>(floor(rayY));
			while (grid[newRayY][newRayX] == 0){
				double oldRay[2] = { rayX, rayY };
				rayY -= 0.0001;
				rayX = (((rayX - oldRay[0]) * cos(fov*raysNormalized)) - ((oldRay[1] - rayY) * sin(fov*raysNormalized)));
				rayY = (((oldRay[1] - rayY) * cos(fov*raysNormalized)) + ((rayX - oldRay[0]) * sin(fov*raysNormalized)));
				newRayY = static_cast<int>(floor(rayY));
				newRayX = static_cast<int>(floor(rayX));

				drawLine(mapOffsetX + (oldRay[0]*10), mapOffsetY + (oldRay[1]*10), mapOffsetX + (rayX*10), mapOffsetY + (rayY*10), hWnd);	//Debug Rays
			}

			//float dist = sqrt(pow(abs(rayX - oldRay[0]), 2) + pow(abs(rayY - oldRay[1]), 2));
			float dist = sqrt(pow(abs(rayX - playerX), 2) + pow(abs(rayY - playerY), 2));
			float size = (dist / 6)*height;
			drawLine(rays, size/2, rays, height - (size/2), hWnd);
			//create_line(rays, size / 2, rays, height - (size / 2));
			//rayX += interval;
			rayY = playerY;
			rayX = playerX;
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