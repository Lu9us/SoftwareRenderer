#include "Window.h"


void Buffer::AllocateBuffer(int height,int width)
{
	if (this->bitmapMemory)
	{
		VirtualFree(this->bitmapMemory, 0, MEM_RELEASE);
	}
	this->bitInfo.bmiHeader.biSize = sizeof(this->bitInfo.bmiHeader);
	this->bitInfo.bmiHeader.biHeight = -height;
	this->bitInfo.bmiHeader.biWidth = width;
	this->bitInfo.bmiHeader.biPlanes = 1;
	this->bitInfo.bmiHeader.biBitCount = 32;
	this->bitInfo.bmiHeader.biCompression = BI_RGB;
	this->bitmapHeight = height;
	this->bitmapWidth = width;

	int bitmapSize = (width*height)*this->bytesPerPixel;
	this->bitmapMemory = VirtualAlloc(0, bitmapSize, MEM_COMMIT, PAGE_READWRITE);
}


Window* Window::instance;
void Window::setPixel(int x, int y, uint8 * color)
{
	uint8 * row = (uint8 *)this->currentBuffer->bitmapMemory;
	int pitch = this->currentBuffer->bytesPerPixel * this->currentBuffer->bitmapWidth;
	row += (pitch * y);
	uint32 * pixel = (uint32*)row;
	pixel = pixel + x;
	uint8 colOffset = 0;
	uint8 red = *(color + colOffset);
	++colOffset;
	uint8 green = *(color + colOffset);
	++colOffset;
	uint8 blue = *(color + colOffset);
	*pixel = (red << 16 | green << 8 | blue);

}

LRESULT CALLBACK Window::MainCB(HWND Window, UINT Message, WPARAM Wparam, LPARAM LParam)
{
	LRESULT result;
	switch (Message)
	{
	case WM_SIZE:
	{
		RECT clientRect;
		GetClientRect(Window, &clientRect);
		int width = clientRect.right - clientRect.left;
		int height = clientRect.bottom - clientRect.top;
		Window::instance->ResizeDIBSection(height, width);
		result = 0;
		DeleteObject(&clientRect);
	}
	break;
	case WM_PAINT:
	{
		RECT clientRect;
		GetClientRect(Window, &clientRect);

		PAINTSTRUCT paint;
		HDC device = BeginPaint(Window, &paint);

		int x = paint.rcPaint.left;
		int y = paint.rcPaint.top;
		int width = paint.rcPaint.right - paint.rcPaint.left;
		int height = paint.rcPaint.bottom - paint.rcPaint.top;

		Window::instance->Win32UpdateWindow(device, &clientRect, x, y, width, height);
		EndPaint(Window, &paint);

		result = 0;
	} break;

	default:
		result = DefWindowProc(Window, Message, Wparam, LParam);
		break;
	}

	return (result);
}


void Window::ResizeDIBSection(int height, int width)
{
	if (this->currentBuffer->bitmapMemory)
	{
		VirtualFree(this->currentBuffer->bitmapMemory, 0, MEM_RELEASE);
	}

	this->currentBuffer->bitInfo.bmiHeader.biSize = sizeof(this->currentBuffer->bitInfo.bmiHeader);
	this->currentBuffer->bitInfo.bmiHeader.biHeight = -height;
	this->currentBuffer->bitInfo.bmiHeader.biWidth = width;
	this->currentBuffer->bitInfo.bmiHeader.biPlanes = 1;
	this->currentBuffer->bitInfo.bmiHeader.biBitCount = 32;
	this->currentBuffer->bitInfo.bmiHeader.biCompression = BI_RGB;
	this->currentBuffer->bitmapHeight = height;
	this->currentBuffer->bitmapWidth = width;

	int bitmapSize = (width*height)*this->currentBuffer->bytesPerPixel;
	this->currentBuffer->bitmapMemory = VirtualAlloc(0, bitmapSize, MEM_COMMIT, PAGE_READWRITE);



}

int Window::getBufferHeight()
{
	return currentBuffer->bitmapHeight;
}

int Window::getBufferWidth()
{
	return  currentBuffer->bitmapWidth;
}

void Window::Win32UpdateWindow(HDC context, RECT* windowRect, int x, int y, int width, int height)
{
	int windwidth = windowRect->right - windowRect->left;
	int windheight = windowRect->bottom - windowRect->top;

	StretchDIBits(context,/* x, y, width, height, x, y, width, height,*/
		0, 0, this->currentBuffer->bitmapWidth, this->currentBuffer->bitmapHeight,
		0, 0, windwidth, windheight,
		this->currentBuffer->bitmapMemory,
		&this->currentBuffer->bitInfo, DIB_RGB_COLORS, SRCCOPY);
}
void Window::Draw()
{
	if (this->handle)
	{
		HDC device = GetDC(this->handle);
		RECT clientRect;
		GetClientRect(this->handle, &clientRect);
		int x = clientRect.left;
		int y = clientRect.top;
		int width = clientRect.right - clientRect.left;
		int height = clientRect.bottom - clientRect.top;

		Win32UpdateWindow(device, &clientRect, x, y, width, height);
		ReleaseDC(this->handle, device);
		if (doubleBuffer)
		{
			if (currentBufferInt == 0)
			{
				currentBufferInt = 1;
				
			}
			else
			{
				currentBufferInt = 0;
			}
			this->currentBuffer = &this->buffer[currentBufferInt];
			this->currentBuffer->AllocateBuffer(height, width);
		}
	}

}



void Window::run()
{
	if (this->handle)
	{
		this->running = true;
		MSG msg;
		while (running)
		{
			frameCounter++;

			if (frameCounter > 60)
			{
				frameCounter = 0;
			}

			while (PeekMessage(&msg, 0, 0, 0, PM_REMOVE))
			{

				TranslateMessage(&msg);
				DispatchMessage(&msg);

			}
			this->runtime->Update(this->frameCounter);
			this->runtime->Draw(this->frameCounter);

			Draw();
			Sleep(10);




			//Draw(win);
		}
	}

}

