#include "./ZoomLogo.h"
#include <cassert>
#include <chrono>
#include <functional>
#include <gdiplus.h>
#include "./MainWindow.h"

using Gdiplus::Graphics;
using Gdiplus::SolidBrush;
using Gdiplus::Color;
using Gdiplus::Rect;
using Gdiplus::Status;
using Gdiplus::Pen;
using Gdiplus::LinearGradientBrush;
using std::double_t;
using std::int_least32_t;

namespace
{
  const double_t kCircleRadius = 0.5;
  const double_t kCircleBorderWidth = 8.0 / 240.0;
  const double_t kRectangleLeft = 48.0 / 240.0;
  const double_t kRectangleWidth = 104.0 / 240.0;
  const double_t kRectangleTop = 80.0 / 240.0;
  const double_t kRectangleHeight = 80.0 / 240.0;
  const double_t kRectangleLeftTopBorderRadius = 2.0 / 240.0;
  const double_t kRectangleRightBottomRadius = 2.0 / 240.0;
  const double_t kRectangleRightTopRadius = 11.0 / 240.0;
  const double_t kRectangleLeftBottomRadius = 11.0 / 240.0;
  const double_t kTrapezoidLeftTopLeft = 156.0 / 240.0;
  const double_t kTrapezoidLeftTopTop = 103.0 / 240.0;
  const double_t kTrapezoidLeftBottomLeft = 156.0 / 240.0;
  const double_t kTrapezoidLeftBottomBottom = 136.0 / 240.0;
  const double_t kTrapezoidRightTopRight = 191.0 / 240.0;
  const double_t kTrapezoidRightTopTop = 80.0 / 240.0;
  const double_t kTrapezoidRightBottomRight = 191.0 / 240.0;
  const double_t kTrapezoidRightBottomBottom = 159.0 / 240.0;
  const int_least32_t kBackgroundColor = 0x777777FF;
  const int_least32_t kPrimaryColorRGBA = 0xffffffff;
  const int_least32_t kRectangleTopColorRGBA = 0x2cbdffff;
  const int_least32_t kRectangleBottomColorRGBA = 0x2c63ffff;

  Color IntColorToGdiColor(int_least32_t int_color_value)
  {
    return Color(
      int_color_value >> 0 & 0x000000FF,
      int_color_value >> 24 & 0x000000FF,
      int_color_value >> 16 & 0x000000FF,
      int_color_value >> 8 & 0x000000FF
    );
  }
}


NAMESPACE_BEGIN

ZoomLogo::ZoomLogo(MainWindow* parent_window)
  : parent_window_(parent_window)
  , factory_()
  , render_target_()
{
  HRESULT result = D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, &factory_);
  assert(SUCCEEDED(result));
  result = factory_->CreateHwndRenderTarget(D2D1::RenderTargetProperties(), D2D1::HwndRenderTargetProperties(parent_window->WindowHandler()), &render_target_);
  assert(SUCCEEDED(result));
  gdiplus_paint_connection_ = parent_window_->Connect(WM_PAINT, std::bind(std::mem_fn(&ZoomLogo::GdiPlusPaint), this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));
}

ZoomLogo::~ZoomLogo()
{
  gdiplus_paint_connection_.disconnect();
}

void ZoomLogo::D2DRender()
{
  OutputDebugString(_T("D2DRender is called;"));
}

LRESULT ZoomLogo::D2DPaint(UINT msg, WPARAM w_param, LPARAM l_param)
{
  OutputDebugString(_T("D2DPaint is called;"));
  return 0;
}

void ZoomLogo::GdiPlusRender(HDC dc)
{
  OutputDebugString(_T("GdiPlusRender is called;"));  
  Status result;
  Graphics frame(dc);
  frame.SetSmoothingMode(Gdiplus::SmoothingModeAntiAlias);
  result = frame.GetLastStatus();
  assert(result == Status::Ok);
  RECT client_rect = parent_window_->ClientRectangle();
  int client_width = client_rect.right - client_rect.left;
  int client_height = client_rect.bottom - client_rect.top;
  SolidBrush background_brush(IntColorToGdiColor(kBackgroundColor));
  result = background_brush.GetLastStatus();
  assert(result == Status::Ok);
  Rect frame_rectangle(client_rect.left, client_rect.top, client_width, client_height);
  result = frame.FillRectangle(&background_brush, frame_rectangle);

  SolidBrush out_circle_brush(IntColorToGdiColor(kPrimaryColorRGBA));
  result = out_circle_brush.GetLastStatus();
  assert(result == Status::Ok);

  int circle_diameter = min(client_width, client_height);
  int circle_radius = circle_diameter * kCircleRadius;
  frame.FillEllipse(&out_circle_brush, Rect(
    client_width > client_height ? (client_width / 2 - circle_radius) : 0,
    client_width > client_height ? 0 : (client_height / 2 - circle_radius),
    circle_radius * 2,
    circle_radius * 2
  ));  

  assert(result == Status::Ok);
}

LRESULT ZoomLogo::GdiPlusPaint(UINT msg, WPARAM w_param, LPARAM l_param)
{
  OutputDebugString(_T("GdiPlusPaint is called;"));
  PAINTSTRUCT paint_struct;
  HDC dc = BeginPaint(parent_window_->WindowHandler(), &paint_struct);
  GdiPlusRender(dc);
  BOOL succeeded = EndPaint(parent_window_->WindowHandler(), &paint_struct);
  assert(succeeded);
  return 0;
}


NAMESPACE_END