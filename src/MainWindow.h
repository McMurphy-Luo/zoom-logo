/**
 * @author McMurphy.Luo
 * @date 2018-07-21
 * @brief the declaration of the main window of app
*/

#ifndef ZOOM_LOGO_MAIN_WINDOW_H_
#define ZOOM_LOGO_MAIN_WINDOW_H_

#include "./Define.h"
#include <Windows.h>
#include "./Utf8String.h"
#include <boost/signals2.hpp>

NAMESPACE_BEGIN

const char* const kMainWindowClass = u8"MainWindow.zoom-logo.zoom.us";

class MainWindow
{
public:
  MainWindow(const Utf8String& window_name, HINSTANCE module_handle);

  inline void Show(int show_flags);

  inline HWND WindowHandler() const;

  inline MSG LastMessage() const;
  
protected:
  LRESULT OnPaint(UINT uMsg, WPARAM wParam, LPARAM lParam);
  LRESULT OnSize(UINT uMsg, WPARAM wParam, LPARAM lParam);

private:
  HWND window_handler_;
  MSG last_message_;
  boost::signals2::signal<LRESULT(UINT uMsg, WPARAM wParam, LPARAM lParam)> on_event_;
};

NAMESPACE_END

#endif // ZOOM_LOGO_MAIN_WINDOW_H_