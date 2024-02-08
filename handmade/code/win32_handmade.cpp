//This project was created by following along with Casey Muratory's Handmade
//Hero tutorials. For details on the project, go to https://handmadehero.org/
#include<windows.h>
#include<stdint.h>

#define internal static
#define local_persist static
#define global_variable static

typedef int8_t int8;
typedef int16_t int16;
typedef int32_t int32;
typedef int64_t int64;

typedef uint8_t uint8;
typedef uint16_t uint16;
typedef uint32_t uint32;
typedef uint64_t uint64;


struct win32_offscreen_buffer
{
	//NOTE(Casey): Pixels are always 32-bits wide, Memory Order BB GG RR XX
	BITMAPINFO Info;
	void *Memory;
	int Width;
	int Height;
	int Pitch;
};

struct win32_window_dimension
{
	int Width;
	int Height;
};

win32_window_dimension
Win32GetWindowDimension(HWND Window)
{
	win32_window_dimension Result;
	
	RECT ClientRect;
	GetClientRect(Window, &ClientRect);
	Result.Width = ClientRect.right - ClientRect.left;
	Result.Height = ClientRect.bottom - ClientRect.top;

	return(Result);
}

//TODO (Casey) This is a global for now.
global_variable bool Running; //statics always init to 0
global_variable win32_offscreen_buffer GlobalBackbuffer;


internal void
RenderWeirdGradient(win32_offscreen_buffer Buffer, int XOffset, int YOffset)
{
	//TODO(Casey) Let's see what the optimizer does.
	
	uint8 *Row = (uint8*)Buffer.Memory;
	for(int Y=0; 
		Y<Buffer.Height; 
		++Y)
	{
		uint32 *Pixel = (uint32 *)Row;
		for(int X=0; 
			X<Buffer.Width;
			++X)
		{
			uint8 Blue = (X+XOffset);
			uint8 Green = (Y+YOffset);

			*Pixel++ = ((Green << 8) | Blue);
		}
		
		Row += Buffer.Pitch;
	}
}

internal void
Win32ResizeDIBSection(win32_offscreen_buffer *Buffer, int Width, int Height)
{
	//TODO(casey): Bulletproof this
	//Maybe don't free first, free after, then free first if that fails

	if(Buffer->Memory)
	{
		VirtualFree(Buffer->Memory, 0, MEM_RELEASE);
	}

	Buffer->Width = Width;
	Buffer->Height = Height;
	int BytesPerPixel = 4;
	
	//NOTE: When the bitHeight field is negative, this is the clue to
	//Windows to treat this bitmap as top-down, not bottom-up, meaning that
	//the first three bytes of the image are the color for the top-left pixel
	//in the bitmap, not the bottom-left.
	Buffer->Info.bmiHeader.biSize = sizeof(Buffer->Info.bmiHeader);
	Buffer->Info.bmiHeader.biWidth = Buffer->Width;
	Buffer->Info.bmiHeader.biHeight = -Buffer->Height;
	Buffer->Info.bmiHeader.biPlanes = 1;
	Buffer->Info.bmiHeader.biBitCount = 32; //1B padding so px stay alignsed on 4B boundaries
	Buffer->Info.bmiHeader.biCompression = BI_RGB;

	//NOTE(Casey): Thank you to Chris Hecker of Spy Party fame
	//for clarifying the deal with StretchDIBits and BitBlt!
	//No more DC for us.
	int BitmapMemorySize = (Buffer->Width*Buffer->Height)*BytesPerPixel;
	Buffer->Memory = VirtualAlloc(0, BitmapMemorySize, MEM_COMMIT, PAGE_READWRITE);

	//TODO(casey): Probably clear this to black

	Buffer->Pitch = Width*BytesPerPixel;
}

internal void
Win32DisplayBufferInWindow(HDC DeviceContext,
						   int WindowWidth, int WindowHeight,
						   win32_offscreen_buffer Buffer)
{
	//TODO(Casey) Aspect ratio correction
	StretchDIBits(DeviceContext,
				  /*
				  X, Y, Width, Height,
				  X, Y, Width, Height,
				  */
				  0, 0, WindowWidth, WindowHeight,
				  0, 0, Buffer.Width, Buffer.Height,
				  Buffer.Memory,
				  &Buffer.Info,
				  DIB_RGB_COLORS, 
				  SRCCOPY);
}

LRESULT CALLBACK
Win32MainWindowCallBack(HWND Window,
						UINT Message,
						WPARAM WParam,
						LPARAM LParam)
{
	LRESULT Result = 0;
	switch(Message)
	{
		case WM_SIZE:
		{
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
		
		case WM_SETCURSOR:
		{
			SetCursor(0);
		} break;
		
		case WM_PAINT:
		{
			PAINTSTRUCT Paint;
			HDC DeviceContext = BeginPaint(Window, &Paint);
			int X = Paint.rcPaint.left;
			int Y = Paint.rcPaint.top;
			int Width = Paint.rcPaint.right - Paint.rcPaint.left;
			int	Height = Paint.rcPaint.bottom - Paint.rcPaint.top;

			win32_window_dimension Dimension = Win32GetWindowDimension(Window);
			Win32DisplayBufferInWindow(DeviceContext, Dimension.Width, Dimension.Height,
									   GlobalBackbuffer);
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

int CALLBACK
WinMain(HINSTANCE Instance,
		HINSTANCE PrevInstance,
		LPSTR CommandLine,
		int ShowCode)
{
	WNDCLASS WindowClass = {};

	Win32ResizeDIBSection(&GlobalBackbuffer, 1280, 720);

	WindowClass.style = CS_HREDRAW|CS_VREDRAW;
	WindowClass.lpfnWndProc = Win32MainWindowCallBack;
	WindowClass.hInstance = Instance;
//	  WindowClass.hIcon;
	WindowClass.lpszClassName = "HandmadeHeroWindowClass";
	
	if(RegisterClassA(&WindowClass))
	{
		HWND Window = 
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
		if(Window)
		{
			// NOTE(Casey): Since we specified CS_OWNDC, we can just
			// get one device context and use it forever because we
			// are not sharing it with anyone.
			HDC DeviceContext = GetDC(Window);
			
			int XOffset = 0;
			int YOffset = 0;

			Running = true;
			while(Running)
			{
				MSG Message;

				while(PeekMessage(&Message, 0, 0, 0, PM_REMOVE))
				{
					if(Message.message == WM_QUIT)
					{
						Running = false;
					}
					TranslateMessage(&Message);
					DispatchMessageA(&Message);
				}
				RenderWeirdGradient(GlobalBackbuffer, XOffset, YOffset);

				win32_window_dimension Dimension = Win32GetWindowDimension(Window);
				Win32DisplayBufferInWindow(DeviceContext, Dimension.Width,
										   Dimension.Height, GlobalBackbuffer);
				//ReleaseDC(Window, DeviceContext);

				++XOffset;
				YOffset += 2;
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
