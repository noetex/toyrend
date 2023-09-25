#ifdef TOYREND_SYSTEM_WINDOWS
	#include<windows.h>
	#include"toyrend_windows.h"
#else
	#error unspecified or unsupported operating system
#endif

#include<stdint.h>

#ifdef TOYREND_SYSTEM_WINDOWS
	#include"toyrend_windows_main.c"
#endif
