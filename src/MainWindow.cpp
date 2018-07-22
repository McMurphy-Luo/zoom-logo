#include "./MainWindow.h"
#include <cassert>

using namespace zoom_logo;

namespace
{
  LRESULT CALLBACK WindowProc(HWND window_handler, UINT msg, WPARAM l_param, LPARAM p_param) {
    return 1;
  }

  WNDCLASSEX MainWindowClass(HINSTANCE instance) {
    static WNDCLASSEX the_window_class;
    the_window_class.cbSize = sizeof(the_window_class);
    the_window_class.style = CS_DBLCLKS;
    the_window_class.lpfnWndProc = WindowProc;
    the_window_class.cbClsExtra = 0;
    the_window_class.cbWndExtra = 0;
    the_window_class.hInstance = instance;
    the_window_class.hIcon = nullptr;
    the_window_class.hCursor = LoadCursorW(nullptr, MAKEINTRESOURCEW(32512));
    the_window_class.hbrBackground = nullptr;
    the_window_class.lpszMenuName = nullptr;
    the_window_class.lpszClassName = Utf8StringToWString(kMainWindowClass).c_str();
    the_window_class.hIconSm = nullptr;
    return the_window_class;
  }

  ATOM RegisterMainWindow(HINSTANCE module_handle) {
    static bool is_class_registered = false;
    ATOM register_result;
    if (!is_class_registered) {
      WNDCLASSEX main_window_class = MainWindowClass(module_handle);
      register_result = RegisterClassEx(&main_window_class);
      is_class_registered = true;
    }
    return register_result;
  }
}



MainWindow::MainWindow(const Utf8String& window_name, HINSTANCE module_handle)
  : window_handler_(0)
  , last_message_()
{
  RegisterMainWindow(module_handle);
  CreateWindowEx()
}

void MainWindow::Show(int show_flags) {
  assert(IsWindow(window_handler_));
  ShowWindow(window_handler_, show_flags);
}

HWND MainWindow::WindowHandler() const {
  return window_handler_;
}

MSG MainWindow::LastMessage() const {
  return last_message_;
}

LRESULT MainWindow::OnPaint(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
  return 1;
}

LRESULT MainWindow::OnSize(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
  return 1;
}