/**
 * @author McMurphy.Luo
 * @date 2018-07-21
 * @brief the declaration of the main window of app
*/

#ifndef ZOOM_LOGO_MAIN_WINDOW_H_
#define ZOOM_LOGO_MAIN_WINDOW_H_

#include "./Define.h"
#include <Windows.h>
#include <boost/signals2.hpp>
#include "./Utf8String.h"

NAMESPACE_BEGIN

const char* const kMainWindowClass = u8"MainWindow.zoom-logo.zoom.us";

typedef LRESULT MessageHandler(UINT msg, WPARAM w_param, LPARAM l_param);

class MainWindow : boost::noncopyable
{
public:
  MainWindow(const Utf8String& window_name, HINSTANCE module_handle);

  void Show(int show_flags);

  inline HWND WindowHandler() const;

  inline RECT ClientRectangle() const;

  inline MSG LastMessage() const;

  LRESULT Trigger(UINT msg, WPARAM w_param, LPARAM l_param);

  boost::signals2::connection Connect(UINT msg, std::function<MessageHandler> handler);

private:
  HWND window_handler_;
  MSG last_message_;
  boost::signals2::signal<MessageHandler> on_paint_;
  boost::signals2::signal<MessageHandler> on_size_;
  boost::signals2::signal<MessageHandler> on_other_;
};

NAMESPACE_END

#endif // ZOOM_LOGO_MAIN_WINDOW_H_