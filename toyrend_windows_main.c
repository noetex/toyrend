static void
enable_dpi_awareness(void)
{
	HMODULE Winuser = LoadLibraryA("user32.dll");
	void* Function = GetProcAddress(Winuser, "SetProcessDpiAwarenessContext");
	if(Function)
	{
		DECLARE_HANDLE(my_dpi_awareness_context);
		BOOL (*SetProcessDpiAwarenessContext)(my_dpi_awareness_context) = Function;
		my_dpi_awareness_context PerMonitorAwareV2 = ((my_dpi_awareness_context)-4);
		if(!SetProcessDpiAwarenessContext(PerMonitorAwareV2))
		{
			my_dpi_awareness_context PerMonitorAware = ((my_dpi_awareness_context)-3);
			SetProcessDpiAwarenessContext(PerMonitorAware);
		}
		return;
	}
	HMODULE ShellCore = LoadLibraryA("shcore.dll");
	Function = GetProcAddress(ShellCore, "SetProcessDpiAwareness");
	if(Function)
	{
		typedef enum { PROCESS_PER_MONITOR_DPI_AWARE = 2 } DPI_PROCESS_AWARENESS;
		HRESULT (*SetProcessDpiAwareness)(PROCESS_DPI_AWARENESS) = Function;
		SetProcessDpiAwareness(PROCESS_PER_MONITOR_DPI_AWARE);
		return;
	}
	Function = GetProcAddress(Winuser, "SetProcessDPIAware");
	if(Function)
	{
		BOOL (*SetProcessDPIAware)(void) = Function;
		SetProcessDPIAware();
	}
}

static LRESULT CALLBACK
window_proc(HWND Window, UINT Message, WPARAM wParam, LPARAM lParam)
{
	LRESULT Result = 0;
	switch(Message)
	{
		case WM_CLOSE:
		case WM_DESTROY:
		{
			PostQuitMessage(0);
		} break;
		case WM_KEYDOWN:
		{
			if(wParam == VK_ESCAPE)
			{
				PostQuitMessage(0);
			}
		}
		default:
		{
			Result = DefWindowProcW(Window, Message, wParam, lParam);
		} break;
	}
	return Result;
}

static HWND
create_the_window(void)
{
	WNDCLASSEXW WindowClass = {0};
	WindowClass.cbSize = sizeof(WindowClass);
	WindowClass.style = CS_HREDRAW | CS_VREDRAW;
	WindowClass.lpfnWndProc = window_proc;
	WindowClass.lpszClassName = WNDCLASS_NAME;
	RegisterClassExW(&WindowClass);
	DWORD Style = WS_CAPTION | WS_SYSMENU | WS_VISIBLE;
	DWORD StyleEx = 0;
	HWND Result = CreateWindowExW(StyleEx, WNDCLASS_NAME, WINDOW_TITLE, Style, CW_USEDEFAULT, CW_USEDEFAULT, 1280, 720, 0, 0, 0, 0);
	return Result;
}

static void
toyrend_update(toyrend_struct* Renderer)
{
	LONG Width = Renderer->BitmapInfo.bmiHeader.biWidth;
	LONG Height = Renderer->BitmapInfo.bmiHeader.biHeight;
	BitBlt(Renderer->WindowDC, 0, 0, Width, Height, Renderer->MemoryDC, 0, 0, SRCCOPY);
}

static void
toyrend_resize(toyrend_struct* Renderer)
{
	RECT WindowRect;
	GetClientRect(Renderer->Window, &WindowRect);
	Renderer->BitmapInfo.bmiHeader.biWidth = WindowRect.right - WindowRect.left;
	Renderer->BitmapInfo.bmiHeader.biHeight = WindowRect.bottom - WindowRect.top;
	if(Renderer->BitmapHandle)
	{
		DeleteObject(Renderer->BitmapHandle);
		Renderer->BitmapHandle = 0;
	}
	Renderer->BitmapHandle = CreateDIBSection(Renderer->MemoryDC, &Renderer->BitmapInfo, DIB_RGB_COLORS, &Renderer->Pixels, 0, 0);
	SelectObject(Renderer->MemoryDC, Renderer->BitmapHandle);
}

static toyrend_struct
toyrend_create(HWND Window)
{
	toyrend_struct Result = {0};
	Result.Window = Window;
	Result.WindowDC = GetDC(Window);
	Result.MemoryDC = CreateCompatibleDC(Result.WindowDC);
	Result.BitmapInfo.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	Result.BitmapInfo.bmiHeader.biPlanes = 1;
	Result.BitmapInfo.bmiHeader.biBitCount = 32;
	Result.BitmapInfo.bmiHeader.biCompression = BI_RGB;
	toyrend_resize(&Result);
	return Result;
}

void WinMainCRTStartup(void)
{
	enable_dpi_awareness();
	HWND Window = create_the_window();
	toyrend_struct Renderer = toyrend_create(Window);
	line_t Line = { 50, 50, 800, 400 };
	circle_t Circle = {300, 300, 80};
	triangle_t Triangle =
	{
		(point_t){400, 100},
		(point_t){600, 300},
		(point_t){650, 150},
	};
	for(;;)
	{
		MSG Message;
		while(PeekMessageW(&Message, 0, 0, 0, PM_REMOVE))
		{
			if(Message.message == WM_QUIT)
			{
				goto label_main_loop_exit;
			}
			TranslateMessage(&Message);
			DispatchMessageW(&Message);
		}
		toyrend_clear(&Renderer, 0x000000ff);
		toyrend_circle(&Renderer, Circle, 0x0000ff00);
		toyrend_line(&Renderer, Line, 0x00ff0000);
		toyrend_triangle(&Renderer, Triangle, 0x00ffff00);
		toyrend_update(&Renderer);
	}
label_main_loop_exit:
	ExitProcess(0);
}
