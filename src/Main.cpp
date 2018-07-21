#include <Windows.h>
#include <tchar.h>


int APIENTRY _tWinMain(HINSTANCE instance, HINSTANCE previous_instance, LPTSTR command_line, int show) {
	MessageBoxW(nullptr, L"Hello World", L"Hello World!", 0);
	return 0;
}