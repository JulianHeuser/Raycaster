#pragma once

#include <Windows.h>
#include <d2d1.h>
using namespace std;

class Graphics{
	ID2D1Factory* factory;
	ID2D1HwndRenderTarget* renderTarget;
	ID2D1SolidColorBrush* brush;
public:
	Graphics();
	~Graphics();

	bool Init(HWND windowHandle);

	void BeginDraw(){ renderTarget->BeginDraw(); }
	void EndDraw(){ renderTarget->EndDraw(); }

	void clearScreen(float r, float g, float b);
	void drawCircle(float x, float y, float radius, float r, float g, float b, float a);
	void drawLine(float x, float y, float x2, float y2, float r, float g, float b, float a);
	void drawRect(float x, float y, float x2, float y2, float r, float g, float b, float a);
};