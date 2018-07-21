/**
 * @author McMurphy.Luo
 * @date 2018-07-21
 * @brief
 */

#ifndef ZOOM_LOGO_UTF8STRING_H_
#define ZOOM_LOGO_UTF8STRING_H_

#include "./Define.h"
#include <string>

NAMESPACE_BEGIN

typedef std::string Utf8String;

std::wstring Utf8StringToWString(const Utf8String& source);

Utf8String WStringToUtf8String(const std::wstring& source);

NAMESPACE_END

#endif // ZOOM_LOGO_UTF8STRING_H_