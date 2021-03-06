/**
 * @author McMurphy.Luo
 * @date 2017-07-23
 * @brief define an abstract data type representing the zoom logo
 */

#ifndef ZOOM_LOGO_ZOOM_LOGO_H_
#define ZOOM_LOGO_ZOOM_LOGO_H_

#include "./Define.h"
#include <chrono>
#include <boost/signals2.hpp>
#include <boost/noncopyable.hpp>
#include <atlbase.h>
#include <gdiplus.h>
#include <d2d1.h>

NAMESPACE_BEGIN

class MainWindow; // forward declaration

class ZoomLogo : boost::noncopyable
{
public:
  explicit ZoomLogo(MainWindow* the_main_window);

  ~ZoomLogo();

  LRESULT Paint(UINT msg, WPARAM w_param, LPARAM l_param);

  LRESULT Size(UINT msg, WPARAM w_param, LPARAM l_param);

  void D2DRender(ID2D1RenderTarget* render_target);

  void GdiPlusRender(HDC dc);

private:
  MainWindow* parent_window_;
  std::chrono::high_resolution_clock::duration last_render_time_consume_;
  CComPtr<ID2D1Factory> factory_;
  CComPtr<ID2D1HwndRenderTarget> render_target_;
  boost::signals2::connection paint_connection_;
  boost::signals2::connection size_connection_;
};

NAMESPACE_END

#endif // ZOOM_LOGO_ZOOM_LOGO_H_