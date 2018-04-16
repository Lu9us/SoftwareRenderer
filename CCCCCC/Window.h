#pragma once
#include <Windows.h>
#include "Definitions.h"
#include "Runtime.h"


struct Buffer
{
	 void * bitmapMemory;
	 HBITMAP bitmapHandel;
	 HDC  deviceContext;
	 BITMAPINFO bitInfo;
	 int bitmapHeight;
	 int bitmapWidth;
     int bytesPerPixel = 4;
	 void AllocateBuffer(int height,int width );
};





class Window
{
public:
	 Window(HINSTANCE hInstance,bool doubleBuffer)
	{
		GetModuleHandle(0);
		instance = this;
		//MessageBox(0, "This is a message box ", "Screw you", 0);
		this->doubleBuffer = doubleBuffer;
		if (doubleBuffer)
		{
			buffer = new Buffer[2];
			buffer[0] = Buffer();
			buffer[1] = Buffer();
			currentBuffer = &buffer[0];
		}
		else
		{
			buffer = new Buffer();
			currentBuffer = buffer;
		}

		 windowClass = {};

		 windowClass.style = CS_OWNDC | CS_HREDRAW | CS_VREDRAW;
		 windowClass.lpfnWndProc = MainCB;
		 windowClass.hInstance = hInstance;
		 windowClass.lpszClassName = "WINC";


		if (RegisterClass(&windowClass))
		{

			handle = CreateWindow("WINC", "this is a window", WS_OVERLAPPEDWINDOW | WS_VISIBLE, 0, 0, 1280, 1080, (HWND)NULL, (HMENU)NULL, hInstance,this);
		}
	
		
	}
	void run();
	Runtime* runtime;
	void setPixel(int x, int y, uint8 * color);
	int getBufferWidth();
	int getBufferHeight();
private:
	
	HWND handle;
	WNDCLASS windowClass;
	Buffer * currentBuffer;
	Buffer * buffer;
	bool doubleBuffer;
	bool running = false;
	int currentBufferInt = 0;
	int frameCounter;
	void Draw();
	void Win32UpdateWindow(HDC context, RECT* windowRect, int x, int y, int width, int height);
	void ResizeDIBSection(int width, int height);
	internal LRESULT CALLBACK MainCB(HWND Window, UINT Message, WPARAM Wparam, LPARAM LParam);

protected:
	internal Window* instance;

};


