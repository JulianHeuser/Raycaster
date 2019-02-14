#include "Graphics.h"
#include <math.h>
#include "vectors.h"

using namespace std;
#pragma comment(lib, "d2d1")

Graphics::Graphics(){
	factory = NULL;
	renderTarget = NULL;
	brush = NULL;
}

Graphics::~Graphics(){
	if (factory) factory->Release();
	if (renderTarget) renderTarget->Release();
	if (brush) brush->Release();
}

bool Graphics::Init(HWND windowHandle){
	HRESULT res = D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, &factory);
	if (res != S_OK) return false;

	RECT rect;
	GetClientRect(windowHandle, &rect);

	res = factory->CreateHwndRenderTarget(
		D2D1::RenderTargetProperties(), 
		D2D1::HwndRenderTargetProperties(windowHandle, D2D1::SizeU(rect.right, rect.bottom)),
		&renderTarget);

	if (res != S_OK) return false;
	res = renderTarget->CreateSolidColorBrush(D2D1::ColorF(0, 0, 0, 0), &brush);
	if (res != S_OK) return false;
	return true; 
}

void Graphics::clearScreen(float r, float g, float b){
	renderTarget->Clear(D2D1::ColorF(r, g, b));
}

void Graphics::drawCircle(float x, float y, float radius, float r, float g, float b, float a){
	brush->SetColor(D2D1::ColorF(r, g, b, a));
	renderTarget->DrawEllipse(D2D1::Ellipse(D2D1::Point2F(x,y),radius,radius),brush,3.0f);
}

void Graphics::drawLine(float x, float y, float x2, float y2, float r, float g, float b, float a){
	brush->SetColor(D2D1::ColorF(r, g, b, a));
	renderTarget->DrawLine(D2D1::Point2F(x, y), D2D1::Point2F(x2, y2), brush);
}

void Graphics::drawRect(float x, float y, float x2, float y2, float r, float g, float b, float a) {
	brush->SetColor(D2D1::ColorF(r, g, b, a));
	renderTarget->DrawRectangle(D2D1::RectF(x, y, x2, y2), brush);
}

void Graphics::drawTri(vec3D p1, vec3D p2, vec3D p3, float r, float g, float b, float a) {
	brush->SetColor(D2D1::ColorF(r, g, b, a));

	renderTarget->DrawLine(D2D1::Point2F(p1.x, p1.y), D2D1::Point2F(p2.x, p2.y), brush);
	renderTarget->DrawLine(D2D1::Point2F(p2.x, p2.y), D2D1::Point2F(p3.x, p3.y), brush);
	renderTarget->DrawLine(D2D1::Point2F(p3.x, p3.y), D2D1::Point2F(p1.x, p1.y), brush);

	//TODO: get a better fill algorithm

	/*
	vec3D avgP;
	avgP.x = (p1.x + p2.x + p3.x) / 3;
	avgP.y = (p1.y + p2.y + p3.y) / 3;
	float avgDistInitial = sqrtf((p1.x - avgP.x)*(p1.x - avgP.x) + (p1.y - avgP.y)*(p1.y - avgP.y));
	float avgDist = avgDistInitial - 1;

	vec3D np1 = p1;
	vec3D np2 = p2;
	vec3D np3 = p3;

	float factor = avgDistInitial / avgDist;

	while (avgDist >= 1) {
		renderTarget->DrawLine(D2D1::Point2F(np1.x, np1.y), D2D1::Point2F(np2.x, np2.y), brush);
		renderTarget->DrawLine(D2D1::Point2F(np2.x, np2.y), D2D1::Point2F(np3.x, np3.y), brush);
		renderTarget->DrawLine(D2D1::Point2F(np3.x, np3.y), D2D1::Point2F(np1.x, np1.y), brush);
		
		np1 = addVectors(divideVector(subtractVectors(np1, avgP), factor),avgP);
		np2 = addVectors(divideVector(subtractVectors(np2, avgP), factor),avgP);
		np3 = addVectors(divideVector(subtractVectors(np3, avgP), factor),avgP);
		avgDist = sqrtf((np1.x - avgP.x)*(np1.x - avgP.x) + (np1.y - avgP.y)*(np1.y - avgP.y));
	}
	*/

}