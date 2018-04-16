


#include <Windows.h>
#include <tchar.h>
#include "TestRuntime.h"













int CALLBACK WinMain(
	_In_ HINSTANCE hInstance,
	_In_ HINSTANCE hPrevInstance,
	_In_ LPSTR     lpCmdLine,
	_In_ int       nCmdShow)
{
	Window w = Window(hInstance, true);
	w.runtime = new TestRT(&w);
	w.run();
	
	
	
	return 0;
}