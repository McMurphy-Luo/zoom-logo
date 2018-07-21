/**
 * @author McMurphy.Luo
 * @date 2018-07-21
 * @brief
 */

#ifndef ZOOM_LOGO_DEFINE_H_
#define ZOOM_LOGO_DEFINE_H_

#ifdef _DEBUG
#define ZOOM_LOGO_DEBUG
#endif

#define NAMESPACE zoom_logo
#define NAMESPACE_BEGIN namespace NAMESPACE {
#define NAMESPACE_END }

#ifdef ZOOM_LOGO_DEBUG

#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>

#ifndef DBG_NEW
#define DBG_NEW new ( _NORMAL_BLOCK , __FILE__ , __LINE__ )
#define new DBG_NEW
#endif // DBG_NEW

#endif // ZOOM_LOGO_DEBUG

#endif // ZOOM_LOGO_DEFINE_H_