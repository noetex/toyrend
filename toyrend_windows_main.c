static void
enable_dpi_awareness(void)
{
	HMODULE Winuser = LoadLibraryA("user32.dll");
	void* Function = GetProcAddress(Winuser, "SetProcessDpiAwarenessContext");
	if(Function)
	{
		BOOL (*SetProcessDpiAwarenessContext)(DPI_AWARENESS_CONTEXT) = Function;
		if(!SetProcessDpiAwarenessContext(DPI_AWARENESS_CONTEXT_PER_MONITOR_AWARE_V2))
		{
			SetProcessDpiAwarenessContext(DPI_AWARENESS_CONTEXT_PER_MONITOR_AWARE);
		}
		return;
	}
	HMODULE ShellCore = LoadLibraryA("shcore.dll");
	Function = GetProcAddress(ShellCore, "SetProcessDpiAwareness");
	if(Function)
	{
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

void WinMainCRTStartup(void)
{
	enable_dpi_awareness();
	HWND Window = create_the_window();
	HDC WindowDC = GetDC(Window);
	HDC MemoryDC = CreateCompatibleDC(WindowDC);

	RECT WindowRect;
	GetClientRect(Window, &WindowRect);
	LONG WindowWidth = WindowRect.right - WindowRect.left;
	LONG WindowHeight = WindowRect.bottom - WindowRect.top;
	BITMAPINFO BitmapInfo = {0};
	BitmapInfo.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	BitmapInfo.bmiHeader.biWidth = WindowWidth;
	BitmapInfo.bmiHeader.biHeight = WindowHeight;
	BitmapInfo.bmiHeader.biPlanes = 1;
	BitmapInfo.bmiHeader.biBitCount = 32;
	BitmapInfo.bmiHeader.biCompression = BI_RGB;
	uint32_t* Pixels = 0;
	size_t NumPixels = WindowWidth * WindowHeight;
	HBITMAP BitmapHandle = CreateDIBSection(MemoryDC, &BitmapInfo, DIB_RGB_COLORS, &Pixels, 0, 0);
	SelectObject(MemoryDC, BitmapHandle);
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
		for(int Index = 0; Index < NumPixels; Index += 1)
		{
			Pixels[Index] = 0x000000ff;
		}
		BitBlt(WindowDC, 0, 0, WindowWidth, WindowHeight, MemoryDC, 0, 0, SRCCOPY);
	}
label_main_loop_exit:
	ExitProcess(0);
}
