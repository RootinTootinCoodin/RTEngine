#include "Globals.h"
#include <windows.h>  
#include <stdio.h>
#include "Application.h"

void log(const char file[], int line, const char* format, ...)
{
	static char tmp_string[4096];
	static char tmp_string2[4096];
	static va_list  ap;

	// Construct the string from variable arguments
	va_start(ap, format);
	vsprintf_s(tmp_string, 4096, format, ap);
	va_end(ap);
	sprintf_s(tmp_string2, 4096, "%s <<<>>> %s(%d)", tmp_string, file, line);
	OutputDebugString(tmp_string2);
	if (_app != nullptr)
		_app->app_log(tmp_string2);

}
