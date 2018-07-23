#include "./ZoomLogo.h"
#include <cassert>
#include <chrono>
#include <functional>
#include "./MainWindow.h"

namespace
{
  const double_t kCircleRadius = 0.5;
  const double_t kCircleBorderWidth = 8 / 240;
  const double_t kRectangleLeft = 48 / 240;
  const double_t kRectangleWidth = 104 / 240;
  const double_t kRectangleTop = 80 / 240;
  const double_t kRectangleHeight = 80 / 240;
  const double_t kRectangleLeftTopBorderRadius = 2 / 240;
  const double_t kRectangleRightBottomRadius = 2 / 240;
  const double_t kRectangleRightTopRadius = 11 / 240;
  const double_t kRectangleLeftBottomRadius = 11 / 240;
  const double_t kTrapezoidLeftTopLeft = 156 / 240;
  const double_t kTrapezoidLeftTopTop = 103 / 240;
  const double_t kTrapezoidLeftBottomLeft = 156 / 240;
  const double_t kTrapezoidLeftBottomBottom = 136 / 240;
  const double_t kTrapezoidRightTopRight = 191 / 240;
  const double_t kTrapezoidRightTopTop = 80 / 240;
  const double_t kTrapezoidRightBottomRight = 191 / 240;
  const double_t kTrapezoidRightBottomBottom = 159 / 240;
  const std::int_least32_t kPrimaryColorRGBA = 0xffffffff;
  const std::int_least32_t kRectangleTopColorRGBA = 0x2cbdffff;
  const std::int_least32_t kRectangleBottomColorRGBA = 0x2c63ffff;
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
  parent_window_->Connect(WM_PAINT, std::bind(std::mem_fn(&ZoomLogo::GdiPlusPaint), this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));
}

ZoomLogo::~ZoomLogo()
{

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

void ZoomLogo::GdiPlusRender()
{
  OutputDebugString(_T("GdiPlusRender is called;"));
}

LRESULT ZoomLogo::GdiPlusPaint(UINT msg, WPARAM w_param, LPARAM l_param)
{
  PAINTSTRUCT paint_struct;
  BeginPaint(parent_window_->WindowHandler(), &paint_struct);
  OutputDebugString(_T("GdiPlusPaint is called;"));
  BOOL succeeded = EndPaint(parent_window_->WindowHandler(), &paint_struct);
  assert(succeeded);
  return 0;
}


NAMESPACE_END