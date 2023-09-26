#ifndef DPI_ENUMS_DECLARED
	typedef enum
	{
		PROCESS_DPI_UNAWARE = 0,
		PROCESS_SYSTEM_DPI_AWARE = 1,
		PROCESS_PER_MONITOR_DPI_AWARE = 2
	} PROCESS_DPI_AWARENESS;
#endif

#ifndef _DPI_AWARENESS_CONTEXTS_
	DECLARE_HANDLE(DPI_AWARENESS_CONTEXT);
	#ifndef DPI_AWARENESS_CONTEXT_PER_MONITOR_AWARE
		#define DPI_AWARENESS_CONTEXT_PER_MONITOR_AWARE ((DPI_AWARENESS_CONTEXT)-3)
	#endif

	#ifndef DPI_AWARENESS_CONTEXT_PER_MONITOR_AWARE_V2
		#define DPI_AWARENESS_CONTEXT_PER_MONITOR_AWARE_V2 ((DPI_AWARENESS_CONTEXT)-4)
	#endif
#endif

#define Assert(Condition) if(Condition); else __debugbreak()
#define Abs(Value) ((Value) < 0 ? -(Value) : (Value))


typedef struct
{
	HWND Window;
	HDC WindowDC;
	HDC MemoryDC;
	BITMAPINFO BitmapInfo;
	HBITMAP BitmapHandle;
	uint32_t* Pixels;
} toyrend_struct;

static WCHAR WINDOW_TITLE[] = L"toyrend";
static WCHAR WNDCLASS_NAME[] = L"toyrend_wndclass";
static DWORD RENDERER_THREAD_ID;
