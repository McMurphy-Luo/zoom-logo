#include "./ZoomLogo.h"
#include <cassert>
#include <functional>
#include <dwrite.h>
#include <tchar.h>
#include "./MainWindow.h"

using std::chrono::high_resolution_clock;
using std::chrono::time_point;
using std::chrono::duration;
using std::chrono::duration_cast;
using std::chrono::seconds;
using std::mem_fn;
using std::bind;
using std::double_t;
using std::int_least32_t;
using std::wstring;
using Gdiplus::Graphics;
using Gdiplus::SolidBrush;
using Gdiplus::FontStyle;
using Gdiplus::FontFamily;
using Gdiplus::Font;
using Gdiplus::Unit;
using Gdiplus::Color;
using Gdiplus::Rect;
using Gdiplus::Status;
using Gdiplus::Pen;
using Gdiplus::LinearGradientBrush;
using Gdiplus::GraphicsPath;

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
  const double_t kRectangleRightTopRadius = 22.0 / 240.0;
  const double_t kRectangleLeftBottomRadius = 22.0 / 240.0;
  const double_t kTrapezoidLeftTopLeft = 156.0 / 240.0;
  const double_t kTrapezoidLeftTopTop = 103.0 / 240.0;
  const double_t kTrapezoidLeftBottomLeft = 156.0 / 240.0;
  const double_t kTrapezoidLeftBottomBottom = 136.0 / 240.0;
  const double_t kTrapezoidRightTopRight = 191.0 / 240.0;
  const double_t kTrapezoidRightTopTop = 80.0 / 240.0;
  const double_t kTrapezoidRightBottomRight = 191.0 / 240.0;
  const double_t kTrapezoidRightBottomBottom = 159.0 / 240.0;
  const int32_t kBackgroundColor = 0x777777FF;
  const int32_t kPrimaryColorRGBA = 0xffffffff;
  const int32_t kRectangleTopColorRGBA = 0x2cbdffff;
  const int32_t kRectangleBottomColorRGBA = 0x2c63ffff;

  Color IntColorToGdiColor(int32_t int_color_value)
  {
    return Color(
      int_color_value >> 0 & 0x000000FF,
      int_color_value >> 24 & 0x000000FF,
      int_color_value >> 16 & 0x000000FF,
      int_color_value >> 8 & 0x000000FF
    );
  }

  D2D1_COLOR_F IntColorToD2DColor(int32_t int_color_value)
  {
    return D2D1::ColorF(
      (int_color_value >> 8) & 0x00FFFFFF,
      static_cast<float>((int_color_value & 0x000000FF)) / 0xFF
    );
  }
}

NAMESPACE_BEGIN

ZoomLogo::ZoomLogo(MainWindow* parent_window)
  : parent_window_(parent_window)
  , last_render_time_consume_(duration_cast<high_resolution_clock::duration>(seconds(1)))
  , factory_()
  , render_target_()
  , paint_connection_()
  , size_connection_()
{
  HRESULT result = D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, &factory_);
  assert(SUCCEEDED(result));
  RECT client_rectangle = parent_window->ClientRectangle();
  result = factory_->CreateHwndRenderTarget(D2D1::RenderTargetProperties(), D2D1::HwndRenderTargetProperties(parent_window->WindowHandler(), D2D1::SizeU(client_rectangle.right - client_rectangle.left, client_rectangle.bottom - client_rectangle.top)), &render_target_);
  assert(SUCCEEDED(result));
  paint_connection_ = parent_window_->Connect(WM_PAINT, bind(mem_fn(&ZoomLogo::Paint), this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));
  size_connection_ = parent_window_->Connect(WM_SIZE, bind(mem_fn(&ZoomLogo::Size), this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));
}

ZoomLogo::~ZoomLogo()
{
  paint_connection_.disconnect();
}

LRESULT ZoomLogo::Paint(UINT msg, WPARAM w_param, LPARAM l_param)
{
  OutputDebugString(_T("D2DPaint is called;"));
  PAINTSTRUCT paint_struct;
  HDC dc = BeginPaint(parent_window_->WindowHandler(), &paint_struct);

  GdiPlusRender(dc);

  /*
  HRESULT result;
  CComPtr<ID2D1DCRenderTarget> dc_render_target;
  D2D1_RENDER_TARGET_PROPERTIES dc_render_target_properties = D2D1::RenderTargetProperties(
    D2D1_RENDER_TARGET_TYPE_DEFAULT,
    D2D1::PixelFormat(
      DXGI_FORMAT_B8G8R8A8_UNORM,
      D2D1_ALPHA_MODE_IGNORE),
    0,
    0,
    D2D1_RENDER_TARGET_USAGE_NONE,
    D2D1_FEATURE_LEVEL_DEFAULT
  );
  result = factory_->CreateDCRenderTarget(&dc_render_target_properties, &dc_render_target);
  assert(SUCCEEDED(result));
  RECT window_rect = parent_window_->ClientRectangle();
  dc_render_target->BindDC(dc, &window_rect);
  D2DRender(dc_render_target);
  */

  /*
  D2DRender(render_target_);
  */

  BOOL succeeded = EndPaint(parent_window_->WindowHandler(), &paint_struct);
  assert(succeeded);
  return 0;
}

LRESULT ZoomLogo::Size(UINT msg, WPARAM w_param, LPARAM l_param)
{
  OutputDebugString(_T("ZoomLogo::Size is called;"));
  RECT client_rectangle = parent_window_->ClientRectangle();
  render_target_->Resize(D2D1::SizeU(client_rectangle.right - client_rectangle.left, client_rectangle.bottom - client_rectangle.top));
  return 0;
}

void ZoomLogo::D2DRender(ID2D1RenderTarget* render_target)
{
  OutputDebugString(_T("D2DRender is called;"));
  time_point<high_resolution_clock> time_before_render = high_resolution_clock::now();
  HRESULT result;
  render_target->BeginDraw();
  D2D1_COLOR_F the_color = IntColorToD2DColor(kBackgroundColor);
  render_target->Clear(the_color);

  int client_width = render_target->GetSize().width;
  int client_height = render_target->GetSize().height;

  CComPtr<ID2D1SolidColorBrush> primary_brush;
  result = render_target->CreateSolidColorBrush(IntColorToD2DColor(kPrimaryColorRGBA), &primary_brush);
  assert(SUCCEEDED(result));

  int circle_diameter = min(client_width, client_height);
  int circle_radius = circle_diameter * kCircleRadius;
  int circle_border_width = circle_radius * kCircleBorderWidth;

  render_target->FillEllipse(D2D1::Ellipse(D2D1::Point2( client_width / 2, client_height / 2 ), circle_diameter / 2, circle_diameter / 2), primary_brush);

  D2D1_GRADIENT_STOP brush_descriptor[2];
  brush_descriptor[0].color = IntColorToD2DColor(kRectangleTopColorRGBA);
  brush_descriptor[0].position = 0.0;
  brush_descriptor[1].color = IntColorToD2DColor(kRectangleBottomColorRGBA);
  brush_descriptor[1].position = 1.0;
  CComPtr<ID2D1GradientStopCollection> brush_description;
  result = render_target->CreateGradientStopCollection(brush_descriptor, 2, &brush_description);
  assert(SUCCEEDED(result));

  D2D1_ELLIPSE inner_ellipse = D2D1::Ellipse(D2D1::Point2(client_width / 2, client_height / 2), circle_diameter / 2 - circle_diameter * kCircleBorderWidth, circle_diameter / 2 - circle_diameter * kCircleBorderWidth);
  CComPtr<ID2D1LinearGradientBrush> cirlce_bg_brush;
  result = render_target->CreateLinearGradientBrush(D2D1::LinearGradientBrushProperties(
    D2D1::Point2F(client_width / 2, client_height / 2 - circle_diameter / 2 + circle_diameter * kCircleBorderWidth)
    ,
    D2D1::Point2F(client_width / 2, client_height / 2 + circle_diameter / 2 - circle_diameter * kCircleBorderWidth)
  ), brush_description, &cirlce_bg_brush);
  assert(SUCCEEDED(result));
  render_target->FillEllipse(inner_ellipse, cirlce_bg_brush);

  D2D1_RECT_L out_ellipse_bounding_rect;
  out_ellipse_bounding_rect.left = client_width / 2 - circle_diameter / 2;
  out_ellipse_bounding_rect.right = client_width / 2 + circle_diameter / 2;
  out_ellipse_bounding_rect.top = client_height / 2 - circle_diameter / 2;
  out_ellipse_bounding_rect.bottom = client_height / 2 + circle_diameter / 2;

  int rectangle_line_left_left = out_ellipse_bounding_rect.left + kRectangleLeft * circle_diameter;
  int rectangle_line_left_top = out_ellipse_bounding_rect.top + kRectangleTop * circle_diameter + kRectangleLeftTopBorderRadius * circle_diameter;
  int rectangle_line_left_bottom = out_ellipse_bounding_rect.top + kRectangleTop * circle_diameter + kRectangleHeight * circle_diameter - kRectangleLeftBottomRadius * circle_diameter;

  int rectangle_line_top_left = rectangle_line_left_left + kRectangleLeftTopBorderRadius * circle_diameter;
  int rectangle_line_top_top = out_ellipse_bounding_rect.top + kRectangleTop * circle_diameter;
  int rectangle_line_top_right = rectangle_line_left_left + kRectangleWidth * circle_diameter - kRectangleRightTopRadius * circle_diameter;

  int rectangle_line_right_left = rectangle_line_left_left + kRectangleWidth * circle_diameter;
  int rectangle_line_right_top = rectangle_line_top_top + kRectangleRightTopRadius * circle_diameter;
  int rectangle_line_right_bottom = rectangle_line_top_top + kRectangleHeight * circle_diameter - kRectangleRightBottomRadius * circle_diameter;

  int rectangle_line_bottom_left = rectangle_line_left_left + kRectangleLeftBottomRadius * circle_diameter;
  int rectangle_line_bottom_bottom = rectangle_line_top_top + kRectangleHeight * circle_diameter;
  int rectangle_line_bottom_right = rectangle_line_left_left + kRectangleWidth * circle_diameter - kRectangleRightBottomRadius * circle_diameter;

  CComPtr<ID2D1PathGeometry> zoom_logo_path;
  result = factory_->CreatePathGeometry(&zoom_logo_path);
  assert(SUCCEEDED(result));

  CComPtr<ID2D1GeometrySink> zoom_logo_sink;
  result = zoom_logo_path->Open(&zoom_logo_sink);
  assert(SUCCEEDED(result));

  zoom_logo_sink->BeginFigure(D2D1::Point2(rectangle_line_left_left, rectangle_line_left_bottom), D2D1_FIGURE_BEGIN_FILLED);
  zoom_logo_sink->AddLine(D2D1::Point2(rectangle_line_left_left, rectangle_line_left_top));
  zoom_logo_sink->AddArc(D2D1::ArcSegment(
    D2D1::Point2(rectangle_line_top_left, rectangle_line_top_top),
    D2D1::Size(kRectangleLeftTopBorderRadius * circle_diameter, kRectangleLeftTopBorderRadius * circle_diameter),
    0,
    D2D1_SWEEP_DIRECTION_CLOCKWISE,
    D2D1_ARC_SIZE_SMALL
  ));
  zoom_logo_sink->AddLine(D2D1::Point2(
    rectangle_line_top_right,
    rectangle_line_top_top
  ));
  
  zoom_logo_sink->AddArc(D2D1::ArcSegment(
    D2D1::Point2(rectangle_line_right_left, rectangle_line_right_top),
    D2D1::Size(kRectangleRightTopRadius * circle_diameter, kRectangleRightTopRadius * circle_diameter),
    0,
    D2D1_SWEEP_DIRECTION_CLOCKWISE,
    D2D1_ARC_SIZE_SMALL
  ));
  zoom_logo_sink->AddLine(D2D1::Point2(
    rectangle_line_right_left,
    rectangle_line_right_bottom
  ));
  zoom_logo_sink->AddArc(D2D1::ArcSegment(
    D2D1::Point2(rectangle_line_bottom_right, rectangle_line_bottom_bottom),
    D2D1::Size(kRectangleRightBottomRadius * circle_diameter, kRectangleRightBottomRadius * circle_diameter),
    0,
    D2D1_SWEEP_DIRECTION_CLOCKWISE,
    D2D1_ARC_SIZE_SMALL
  ));
  zoom_logo_sink->AddLine(D2D1::Point2(rectangle_line_bottom_left, rectangle_line_bottom_bottom));
  zoom_logo_sink->AddArc(D2D1::ArcSegment(
    D2D1::Point2(rectangle_line_left_left, rectangle_line_left_bottom),
    D2D1::Size(kRectangleLeftBottomRadius * circle_diameter, kRectangleLeftBottomRadius * circle_diameter),
    0,
    D2D1_SWEEP_DIRECTION_CLOCKWISE,
    D2D1_ARC_SIZE_SMALL
  ));
  
  zoom_logo_sink->EndFigure(D2D1_FIGURE_END_CLOSED);

  zoom_logo_sink->BeginFigure(
    D2D1::Point2(
    kTrapezoidLeftTopLeft * circle_diameter + out_ellipse_bounding_rect.left,
    kTrapezoidLeftTopTop * circle_diameter + out_ellipse_bounding_rect.top
    ),
    D2D1_FIGURE_BEGIN_FILLED
  );
  zoom_logo_sink->AddLine(
    D2D1::Point2(
      kTrapezoidRightTopRight * circle_diameter + out_ellipse_bounding_rect.left,
      kTrapezoidRightTopTop * circle_diameter + out_ellipse_bounding_rect.top
    )
  );
  zoom_logo_sink->AddLine(
    D2D1::Point2(
      kTrapezoidRightBottomRight * circle_diameter + out_ellipse_bounding_rect.left,
      kTrapezoidRightBottomBottom * circle_diameter + out_ellipse_bounding_rect.top
    )
  );
  zoom_logo_sink->AddLine(
    D2D1::Point2(
      kTrapezoidLeftBottomLeft * circle_diameter + out_ellipse_bounding_rect.left,
      kTrapezoidLeftBottomBottom * circle_diameter + out_ellipse_bounding_rect.top
    )
  );
  zoom_logo_sink->AddLine(
    D2D1::Point2(
      kTrapezoidLeftTopLeft * circle_diameter + out_ellipse_bounding_rect.left,
      kTrapezoidLeftTopTop * circle_diameter + out_ellipse_bounding_rect.top
    )
  );
  zoom_logo_sink->EndFigure(D2D1_FIGURE_END_CLOSED);
  result = zoom_logo_sink->Close();
  assert(SUCCEEDED(result));
  render_target->FillGeometry(zoom_logo_path, primary_brush);

  seconds one_second(1);
  int frame_count =  duration_cast<high_resolution_clock::duration>(one_second) / last_render_time_consume_;
  
  wchar_t buf[10];
  memset(buf, 0, 10);
  
  int itow_success = _itow_s(frame_count, buf, sizeof(buf) / sizeof(wchar_t), 10);
  assert(itow_success == 0);
  
  IDWriteFactory* factory_pointer;
  CComPtr<IDWriteFactory> write_factory;
  result = DWriteCreateFactory(DWRITE_FACTORY_TYPE_SHARED, __uuidof(IDWriteFactory), (IUnknown**)&factory_pointer);
  assert(SUCCEEDED(result));
  write_factory = factory_pointer;
  CComPtr<IDWriteTextFormat> text_format;
  CComPtr<IDWriteTextLayout> text_layout;
  result = write_factory->CreateTextFormat(
    L"Times New Roman",
    nullptr,
    DWRITE_FONT_WEIGHT::DWRITE_FONT_WEIGHT_NORMAL,
    DWRITE_FONT_STYLE::DWRITE_FONT_STYLE_NORMAL,
    DWRITE_FONT_STRETCH::DWRITE_FONT_STRETCH_NORMAL,
    32,
    L"en-US",
    &text_format
  );
  assert(SUCCEEDED(result));
  result = write_factory->CreateTextLayout(buf, wcslen(buf), text_format, client_width, client_height, &text_layout);
  assert(SUCCEEDED(result));

  render_target->DrawTextLayout(D2D1::Point2(10, 10), text_layout, primary_brush);
  
  result = render_target->EndDraw();
  assert(SUCCEEDED(result));

  last_render_time_consume_ = high_resolution_clock::now() - time_before_render;
}

void ZoomLogo::GdiPlusRender(HDC dc)
{
  OutputDebugString(_T("GdiPlusRender is called;"));  
  time_point<high_resolution_clock> time_before_render = high_resolution_clock::now();
  Status result;
  Graphics frame(dc);
  result = frame.SetSmoothingMode(Gdiplus::SmoothingModeAntiAlias);
  assert(result == Status::Ok);
  result = frame.Clear(IntColorToGdiColor(kBackgroundColor));
  assert(result == Status::Ok);

  RECT client_rect = parent_window_->ClientRectangle();
  int client_width = client_rect.right - client_rect.left;
  int client_height = client_rect.bottom - client_rect.top;
  SolidBrush out_circle_brush(IntColorToGdiColor(kPrimaryColorRGBA));
  result = out_circle_brush.GetLastStatus();
  assert(result == Status::Ok);
  int circle_diameter = min(client_width, client_height);
  int circle_radius = circle_diameter * kCircleRadius;
  int circle_border_width = circle_radius * kCircleBorderWidth;
  Rect out_ellipse_bounding_rect(
    client_width > client_height ? (client_width / 2 - circle_radius) : 0,
    client_width > client_height ? 0 : (client_height / 2 - circle_radius),
    circle_radius * 2,
    circle_radius * 2
  );
  result = frame.FillEllipse(&out_circle_brush, out_ellipse_bounding_rect);
  assert(result == Status::Ok);

  Rect inner_ellise_bounding_rect(
    out_ellipse_bounding_rect.GetLeft() + circle_border_width,
    out_ellipse_bounding_rect.GetTop() + circle_border_width,
    out_ellipse_bounding_rect.Width - 2 * circle_border_width,
    out_ellipse_bounding_rect.Height - 2 * circle_border_width
  );
  LinearGradientBrush inner_circle_brush(
    inner_ellise_bounding_rect,
    IntColorToGdiColor(kRectangleTopColorRGBA),
    IntColorToGdiColor(kRectangleBottomColorRGBA),
    Gdiplus::LinearGradientMode::LinearGradientModeVertical
  );
  result = inner_circle_brush.GetLastStatus();
  assert(result == Status::Ok);
  result = frame.FillEllipse(&inner_circle_brush, inner_ellise_bounding_rect);
  assert(result == Status::Ok);


  int rectangle_line_left_left = out_ellipse_bounding_rect.GetLeft() + kRectangleLeft * circle_diameter;
  int rectangle_line_left_top = out_ellipse_bounding_rect.GetTop() + kRectangleTop * circle_diameter + kRectangleLeftTopBorderRadius * circle_diameter;
  int rectangle_line_left_bottom = out_ellipse_bounding_rect.GetTop() + kRectangleTop * circle_diameter + kRectangleHeight * circle_diameter - kRectangleLeftBottomRadius * circle_diameter;
  

  int rectangle_line_top_left = rectangle_line_left_left + kRectangleLeftTopBorderRadius * circle_diameter;
  int rectangle_line_top_top = out_ellipse_bounding_rect.GetTop() + kRectangleTop * circle_diameter;
  int rectangle_line_top_right = rectangle_line_left_left + kRectangleWidth * circle_diameter - kRectangleRightTopRadius * circle_diameter;
  
  int rectangle_line_right_left = rectangle_line_left_left + kRectangleWidth * circle_diameter;
  int rectangle_line_right_top = rectangle_line_top_top + kRectangleRightTopRadius * circle_diameter;
  int rectangle_line_right_bottom = rectangle_line_top_top + kRectangleHeight * circle_diameter - kRectangleRightBottomRadius * circle_diameter;

  int rectangle_line_bottom_left = rectangle_line_left_left + kRectangleLeftBottomRadius * circle_diameter;
  int rectangle_line_bottom_bottom = rectangle_line_top_top + kRectangleHeight * circle_diameter;
  int rectangle_line_bottom_right = rectangle_line_left_left + kRectangleWidth * circle_diameter - kRectangleRightBottomRadius * circle_diameter;

  GraphicsPath zoom_rect;
  
  zoom_rect.StartFigure();

  zoom_rect.AddLine(
    rectangle_line_left_left,
    rectangle_line_left_bottom,
    rectangle_line_left_left,
    rectangle_line_left_top
  );
  zoom_rect.AddArc(
    Rect(rectangle_line_left_left,
      rectangle_line_top_top,
      kRectangleLeftTopBorderRadius * circle_diameter * 2 + 1,
      kRectangleLeftTopBorderRadius * circle_diameter * 2 + 1),
    180,
    90
  );
  zoom_rect.AddLine(
    rectangle_line_top_left,
    rectangle_line_top_top,
    rectangle_line_top_right,
    rectangle_line_top_top
  );
  zoom_rect.AddArc(
    Rect(rectangle_line_right_left - kRectangleRightTopRadius * circle_diameter * 2,
         rectangle_line_top_top,
         kRectangleRightTopRadius * circle_diameter * 2 + 1, kRectangleRightTopRadius * circle_diameter * 2 + 1),
    270,
    90
  );
  zoom_rect.AddLine(
    rectangle_line_right_left,
    rectangle_line_right_top,
    rectangle_line_right_left,
    rectangle_line_right_bottom
  );
  zoom_rect.AddArc(
    Rect(
      rectangle_line_right_left - 2 * kRectangleRightBottomRadius * circle_diameter,
      rectangle_line_bottom_bottom - 2* kRectangleRightBottomRadius * circle_diameter,
      2 * kRectangleRightBottomRadius * circle_diameter + 1,
      2 * kRectangleRightBottomRadius * circle_diameter + 1
    ),
    0,
    90
  );
  zoom_rect.AddLine(
    rectangle_line_bottom_right,
    rectangle_line_bottom_bottom,
    rectangle_line_bottom_left,
    rectangle_line_bottom_bottom
  );
  zoom_rect.AddArc(
    Rect(
      rectangle_line_left_left,
      rectangle_line_bottom_bottom - 2 * circle_diameter * kRectangleLeftBottomRadius,
      2 * circle_diameter * kRectangleLeftBottomRadius + 1,
      2 * circle_diameter * kRectangleLeftBottomRadius + 1
    ),
    90,
    90
  );
  zoom_rect.CloseFigure();

  zoom_rect.StartFigure();
  zoom_rect.AddLine(
    static_cast<int>(kTrapezoidLeftTopLeft * circle_diameter) + out_ellipse_bounding_rect.GetLeft(),
    kTrapezoidLeftTopTop * circle_diameter + out_ellipse_bounding_rect.GetTop(),
    kTrapezoidRightTopRight * circle_diameter + out_ellipse_bounding_rect.GetLeft(),
    kTrapezoidRightTopTop * circle_diameter + out_ellipse_bounding_rect.GetTop()
  );
  zoom_rect.AddLine(
    static_cast<int>(kTrapezoidRightTopRight * circle_diameter) + out_ellipse_bounding_rect.GetLeft(),
    kTrapezoidRightTopTop * circle_diameter + out_ellipse_bounding_rect.GetTop(),
    kTrapezoidRightBottomRight * circle_diameter + out_ellipse_bounding_rect.GetLeft(),
    kTrapezoidRightBottomBottom * circle_diameter + out_ellipse_bounding_rect.GetTop()
  );
  zoom_rect.AddLine(
    static_cast<int>(kTrapezoidRightBottomRight * circle_diameter) + out_ellipse_bounding_rect.GetLeft(),
    kTrapezoidRightBottomBottom * circle_diameter + out_ellipse_bounding_rect.GetTop(),
    kTrapezoidLeftBottomLeft * circle_diameter + out_ellipse_bounding_rect.GetLeft(),
    kTrapezoidLeftBottomBottom * circle_diameter + out_ellipse_bounding_rect.GetTop()
  );
  zoom_rect.AddLine(
    static_cast<int>(kTrapezoidLeftBottomLeft * circle_diameter) + out_ellipse_bounding_rect.GetLeft(),
    kTrapezoidLeftBottomBottom * circle_diameter + out_ellipse_bounding_rect.GetTop(),
    kTrapezoidLeftTopLeft * circle_diameter + out_ellipse_bounding_rect.GetLeft(),
    kTrapezoidLeftTopTop * circle_diameter + out_ellipse_bounding_rect.GetTop()
  );
  zoom_rect.CloseFigure();

  Pen primary_color_pen(kPrimaryColorRGBA);
  frame.FillPath(&out_circle_brush, &zoom_rect);
  assert(result == Status::Ok);

  FontFamily fontFamily(L"Times New Roman");
  Font font(&fontFamily, 32, FontStyle::FontStyleRegular, Unit::UnitPixel);
  seconds one_second(1);
  int frame_count = duration_cast<high_resolution_clock::duration>(one_second) / last_render_time_consume_;
  wchar_t buf[10];
  memset(buf, 0, 10);
  int itow_success = _itow_s(frame_count, buf, sizeof(buf) / sizeof(wchar_t), 10);
  assert(itow_success == 0);

  result = frame.DrawString(buf, -1, &font, Gdiplus::PointF(10, 10), &out_circle_brush);
  assert(result == Status::Ok);

  last_render_time_consume_ = high_resolution_clock::now() - time_before_render;
}

NAMESPACE_END