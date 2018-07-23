#include "./Define.h"
#include <Windows.h>
#include <tchar.h>
#include <cassert>
#include <gdiplus.h>
#include "./MainWindow.h"
#include "./ZoomLogo.h"

using zoom_logo::MainWindow;
using zoom_logo::ZoomLogo;

int APIENTRY _tWinMain(HINSTANCE instance, HINSTANCE previous_instance, LPTSTR command_line, int show) {
  CoInitialize(nullptr);
  ULONG_PTR gdi_plus_token;
  GdiplusStartup(&gdi_plus_token, nullptr, nullptr);

  MainWindow* the_main_window = new MainWindow("Hello World", instance);
  ZoomLogo* zoom_logo = new ZoomLogo(the_main_window);

  the_main_window->Show(show);

  MSG message;
  while (GetMessage(&message, nullptr, 0, 0)) {
    TranslateMessage(&message);
    DispatchMessage(&message);
  }
  delete zoom_logo;
  delete the_main_window;

  GdiplusShutdown(gdi_plus_token);
  CoUninitialize();
#ifdef _CRTDBG_MAP_ALLOC
  _CrtDumpMemoryLeaks();
#endif
	return 0;
}