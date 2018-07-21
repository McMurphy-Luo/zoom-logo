#include "./MainWindow.h"
#include <cassert>

namespace
{
  LRESULT CALLBACK WindowProc(HWND window_handler, UINT msg, WPARAM l_param, LPARAM p_param) {
    return 1;
  }

  WNDCLASS MainWindowClass(HINSTANCE instance) {
    static WNDCLASS the_window_class;
    the_window_class.hInstance = instance;
    the_window_class.cbClsExtra = 0;
    the_window_class.cbWndExtra = 0;
    the_window_class.hCursor = LoadCursor();
    the_window_class.hIcon = nullptr;
    the_window_class.hbrBackground = nullptr;
    the_window_class.lpfnWndProc = WindowProc;
    return the_window_class;
  }

  ATOM RegisterMainWindow(HINSTANCE module_handle) {
    static bool is_class_registered = false;
    ATOM register_result;
    if (!is_class_registered) {
      WNDCLASS main_window_class = MainWindowClass(module_handle);
      register_result = RegisterClass(&main_window_class);
      is_class_registered = true;
    }
    return register_result;
  }
}

NAMESPACE_BEGIN

MainWindow::MainWindow(const Utf8String& window_name, HINSTANCE module_handle)
  : window_handler_(0)
  , last_message_()
{
  RegisterMainWindow(module_handle);
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

NAMESPACE_END