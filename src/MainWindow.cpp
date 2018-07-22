#include "./MainWindow.h"
#include <cassert>
#include <tchar.h>

using namespace zoom_logo;

namespace
{
  LRESULT CALLBACK WindowProc(HWND window_handler, UINT msg, WPARAM w_param, LPARAM l_param)
  {
    switch (msg) {
      case WM_CREATE:
      {
        CREATESTRUCT* p_create_struct = reinterpret_cast<CREATESTRUCT*>(l_param);
        MainWindow* p_created_wnd = reinterpret_cast<MainWindow*>(p_create_struct->lpCreateParams);
        SetWindowLongPtr(window_handler, GWLP_USERDATA, (LONG_PTR)p_created_wnd);
        return 0;
      }
      case WM_DESTROY:
      {
        PostQuitMessage(0);
        return 0;
      }
      case WM_SIZE:
      {
        MainWindow* theWindow = reinterpret_cast<MainWindow*>(GetWindowLongPtrW(window_handler, GWLP_USERDATA));
        return reinterpret_cast<MainWindow*>(GetWindowLongPtrW(window_handler, GWLP_USERDATA))->Trigger(msg, w_param, l_param);
      }
      case WM_LBUTTONDOWN:
      {
        MainWindow* theWindow = reinterpret_cast<MainWindow*>(GetWindowLongPtrW(window_handler, GWLP_USERDATA));
        return reinterpret_cast<MainWindow*>(GetWindowLongPtrW(window_handler, GWLP_USERDATA))->Trigger(msg, w_param, l_param);
      }
      case WM_PAINT:
      {
        MainWindow* theWindow = reinterpret_cast<MainWindow*>(GetWindowLongPtrW(window_handler, GWLP_USERDATA));
        return reinterpret_cast<MainWindow*>(GetWindowLongPtrW(window_handler, GWLP_USERDATA))->Trigger(msg, w_param, l_param);
      }
    }
    return DefWindowProc(window_handler, msg, w_param, l_param);
  }

  WNDCLASSEX MainWindowClass(HINSTANCE instance)
  {
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
    the_window_class.hIconSm = nullptr;
    return the_window_class;
  }

  ATOM RegisterMainWindow(HINSTANCE module_handle) {
    static bool is_class_registered = false;
    static ATOM register_result;
    if (!is_class_registered) {
      WNDCLASSEX main_window_class = MainWindowClass(module_handle);
      std::wstring class_name = Utf8StringToWString(kMainWindowClass);
      main_window_class.lpszClassName = class_name.c_str();
      register_result = RegisterClassEx(&main_window_class);
      assert(register_result);
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
  std::wstring class_name = Utf8StringToWString(kMainWindowClass);
  window_handler_ = CreateWindowEx(
    WS_EX_OVERLAPPEDWINDOW,
    class_name.c_str(),
    Utf8StringToWString(window_name).c_str(),
    WS_OVERLAPPEDWINDOW | WS_CLIPCHILDREN,
    CW_USEDEFAULT,
    CW_USEDEFAULT,
    CW_USEDEFAULT,
    CW_USEDEFAULT,
    nullptr,
    nullptr,
    module_handle,
    this
  );
  int error_code = GetLastError();
  TCHAR buffer[1024];
  _itot_s(error_code, buffer, 1024, 10);
  OutputDebugString(buffer);
  assert(IsWindow(window_handler_));
}

void MainWindow::Show(int show_flags)
{
  assert(IsWindow(window_handler_));
  ShowWindow(window_handler_, SW_SHOW);
}

HWND MainWindow::WindowHandler() const
{
  return window_handler_;
}

MSG MainWindow::LastMessage() const
{
  return last_message_;
}

LRESULT MainWindow::Trigger(UINT msg, WPARAM w_param, LPARAM l_param)
{
  switch (msg) {
    case WM_PAINT:
    {

    }
  }
  return DefWindowProc(window_handler_, msg, w_param, l_param);
}

RECT MainWindow::ClientRectangle() const
{
  RECT result;
  BOOL succeeded = GetClientRect(window_handler_, &result);
  assert(succeeded);
  return result;
}

boost::signals2::connection MainWindow::Connect(UINT msg, std::function<MessageHandler> handler)
{
  switch (msg) {
  case WM_PAINT:
    return on_paint_.connect(handler);
  case WM_SIZE:
    return on_size_.connect(handler);
  default:
    return on_other_.connect(handler);
  }
}