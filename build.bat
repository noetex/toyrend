@echo off

if /i "%1" equ "clean" (
	del *.obj *.exe *.pdb>nul
	exit /b 0
)

set msvc_flags=/nologo /W3 /D TOYREND_SYSTEM_WINDOWS /D TOYREND_COMPILER_MSVC /Zi /D TOYREND_BUILD_DEBUG
set link_flags=/WX /subsystem:windows /incremental:no /opt:ref,icf
set lib_files=kernel32.lib user32.lib gdi32.lib
call cl toyrend.c /Fe:toyrend.exe %msvc_flags% /link %link_flags% %lib_files%
