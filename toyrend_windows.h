typedef struct
{
	HWND Window;
	HDC WindowDC;
	HDC MemoryDC;
	BITMAPINFO BitmapInfo;
	HBITMAP BitmapHandle;
} toyrend_system_data;

static WCHAR WINDOW_TITLE[] = L"toyrend";
static WCHAR WNDCLASS_NAME[] = L"toyrend_wndclass";
static DWORD RENDERER_THREAD_ID;
