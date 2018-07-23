/**
 * @author McMurphy.Luo
 * @date 2017-07-23
 * @brief define an abstract data type representing the zoom logo
 */

#ifndef ZOOM_LOGO_ZOOM_LOGO_H_
#define ZOOM_LOGO_ZOOM_LOGO_H_

#include "./Define.h"
#include <atlbase.h>
#include <d2d1.h>
#include <boost/signals2.hpp>
#include <boost/noncopyable.hpp>

NAMESPACE_BEGIN

class MainWindow; // forward declaration

class ZoomLogo : boost::noncopyable
{
public:
  explicit ZoomLogo(MainWindow* the_main_window);

  ~ZoomLogo();

  void D2DRender();

  LRESULT D2DPaint(UINT msg, WPARAM w_param, LPARAM l_param);

  void GdiPlusRender(HDC dc);

  LRESULT GdiPlusPaint(UINT msg, WPARAM w_param, LPARAM l_param);

private:
  MainWindow* parent_window_;
  CComPtr<ID2D1Factory> factory_;
  CComPtr<ID2D1HwndRenderTarget> render_target_;
  boost::signals2::connection d2d_paint_connection_;
  boost::signals2::connection gdiplus_paint_connection_;
};

NAMESPACE_END

#endif // ZOOM_LOGO_ZOOM_LOGO_H_