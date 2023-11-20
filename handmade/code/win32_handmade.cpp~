/*
	HEADER HEADER HEADER HEADER HEADER HEADER 
	HEADER HEADER HEADER HEADER HEADER HEADER 
	HEADER HEADER HEADER HEADER HEADER HEADER
	========================================= */
#include<windows.h>
#include<stdio.h>

#define internal        static
#define local_persist   static
#define global_variable static

//TODO (Casey) This is a global for now.
global_variable bool Running; //statics always init to 0

global_variable void *BitmapMemory;
global_variable HBITMAP BitmapHandle;
global_variable BITMAPINFO BitmapInfo;
global_variable HDC BitmapDeviceContext;

internal void Win32ResizeDIBSection(int Width, int Height)
{
	//TODO (casey): Bulletproof this
	//Maybe don't free first, free after, then free first if that fails
	
	if(BitmapHandle)
	{
		DeleteObject(BitmapHandle);
	}
	
	if(!BitmapDeviceContext)
	{
		//TODO (casey): Should we recreate these under certain special circumstances?
		HDC BitmapDeviceContext = CreateCompatibleDC(0);
	}
	BITMAPINFO BitmapInfo;
	BitmapInfo.bmiHeader.biSize = sizeof(BitmapInfo.bmiHeader.biSize);
	BitmapInfo.bmiHeader.biWidth = Width;
	BitmapInfo.bmiHeader.biHeight = Height;
	BitmapInfo.bmiHeader.biPlanes = 1;
	BitmapInfo.bmiHeader.biBitCount = 32;
	BitmapInfo.bmiHeader.biCompression = BI_RGB;
	BitmapInfo.bmiHeader.biSizeImage = 0;
	BitmapInfo.bmiHeader.biXPelsPerMeter = 0;
	BitmapInfo.bmiHeader.biYPelsPerMeter = 0;
	BitmapInfo.bmiHeader.biClrUsed = 0;
	BitmapInfo.bmiHeader.biClrImportant = 0;

	HDC BitmapDeviceContext = CreateCompatibleDC(0);
	HBITMAP BitmapHandle = CreateDIBSection
	(
		BitmapDeviceContext, &BitmapInfo,
		DIB_RGB_COLORS,
		&BitmapMemory,
		0,0
	);
}
internal void Win32UpdateWindow(HDC DeviceContext, int X, int Y, int Width, int Height)
{
	StretchDIBits
	(
		DeviceContext,
		X, Y, Width, Height,
		X, Y, Width, Height,
		BitmapMemory,
		&BitmapInfo,
		DIB_RGB_COLORS, 
		SRCCOPY
	);
}

LRESULT CALLBACK Win32MainWindowCallBack
(
	HWND Window, //Main window handle
	UINT Message,
	WPARAM WParam,
	LPARAM LParam
){
	LRESULT Result = 0;
	switch(Message)
	{
		case WM_SIZE:
		{
			RECT ClientRect;
			GetClientRect(Window, &ClientRect);
			int Width = ClientRect.bottom - ClientRect.top;
			int Height = ClientRect.bottom - ClientRect.top;
			Win32ResizeDIBSection(Width, Height);
			OutputDebugStringA("WM_SIZE\n");
		} break;
		
		case WM_CLOSE:
		{
			//TODO (Casey) Handle this with a message to the user?
			Running = false;
		} break;
		
		case WM_ACTIVATEAPP:
		{
			OutputDebugStringA("WM_ACTIVATEAPP\n");
		} break;
		
		case WM_DESTROY:
		{
			//TODO (Casey) Handle this as an error - recreate window?
			Running = false;
		} break;
		
		case WM_PAINT:
		{
			PAINTSTRUCT Paint;
			HDC DeviceContext = BeginPaint(Window, &Paint);
			int X = Paint.rcPaint.left;
			int Y = Paint.rcPaint.top;
			int Width = Paint.rcPaint.right - Paint.rcPaint.left;
			int	 Height = Paint.rcPaint.bottom - Paint.rcPaint.top;
			Win32UpdateWindow(DeviceContext, X, Y, Width, Height);
			EndPaint(Window, &Paint);
		} break;
		
		default :
		{
			//OutputDebugStringA("default\n");
			Result = DefWindowProc(Window, Message, WParam, LParam);
		} break;
	}
	return(Result);
};

int CALLBACK WinMain
(
	HINSTANCE Instance,
	HINSTANCE PrevInstance,	
	LPSTR     CommandLine,
	int       ShowCode
){
	WNDCLASS WindowClass = {};

	//TODO(casey): Check if HREDRAW/VREDRAW/OWNDC still matter
	WindowClass.lpfnWndProc = Win32MainWindowCallBack;
	WindowClass.hInstance = Instance;
	//WindowClass.hIcon;
	WindowClass.lpszClassName = "HandmadeHeroWindowClass";
	
	if(RegisterClassA(&WindowClass))
	{
		HWND WindowHandle = 
			CreateWindowExA
			(
				0,
				WindowClass.lpszClassName,
				"Handmade Hero",
				WS_OVERLAPPEDWINDOW|WS_VISIBLE,
				CW_USEDEFAULT,
				CW_USEDEFAULT,
				CW_USEDEFAULT,
				CW_USEDEFAULT,
				0,
				0,
				Instance,
				0
			);
		if(WindowHandle)
		{
			Running = true;
			while(Running)
			{
				MSG Message;
				BOOL MessageResult = GetMessageA(&Message, 0, 0, 0);
				if(MessageResult > 0)
				{
					TranslateMessage(&Message);
					DispatchMessageA(&Message);
					//static int YARP=0;
					//YARP++;
				}
				else
				{
					//static int NARP=0;
					//NARP++;
					break;
				}
			}
		}
		else
		{
			//TODO(casey): Logging
		}
	}
	else
	{
		//TODO(casey): Logging
	}
	return(0);
}
