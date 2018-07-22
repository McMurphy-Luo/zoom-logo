#include "./Define.h"
#include <Windows.h>
#include <tchar.h>
#include <cassert>
#include "./MainWindow.h"

using zoom_logo::MainWindow;

int APIENTRY _tWinMain(HINSTANCE instance, HINSTANCE previous_instance, LPTSTR command_line, int show) {
  CoInitialize(nullptr);

  MainWindow* the_main_window = new MainWindow("Hello World", instance);
  the_main_window->Show(show);

  MSG message;
  while (GetMessage(&message, nullptr, 0, 0)) {
    TranslateMessage(&message);
    DispatchMessage(&message);
  }

  delete the_main_window;
  CoUninitialize();
#ifdef _CRTDBG_MAP_ALLOC
  _CrtDumpMemoryLeaks();
#endif
	return 0;
}