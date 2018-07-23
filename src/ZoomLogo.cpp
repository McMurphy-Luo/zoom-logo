#include "./ZoomLogo.h"
#include <cassert>
#include <chrono>
#include <functional>
#include "./MainWindow.h"

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

void ZoomLogo::D2DRender()
{
  OutputDebugString(_T("D2DRender is called;"));
}

LRESULT ZoomLogo::D2DPaint(UINT msg, WPARAM w_param, LPARAM l_param)
{
  OutputDebugString(_T("D2DPaint is called;"));
}

void ZoomLogo::GdiPlusRender()
{
  OutputDebugString(_T("GdiPlusRender is called;"));
}

LRESULT ZoomLogo::GdiPlusPaint(UINT msg, WPARAM w_param, LPARAM l_param)
{
  BeginPaint(parent_window_->WindowHandler(), nullptr);
  OutputDebugString(_T("GdiPlusPaint is called;"));
  BOOL succeeded = EndPaint(parent_window_->WindowHandler(), nullptr);
}


NAMESPACE_END