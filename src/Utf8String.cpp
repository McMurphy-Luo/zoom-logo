#include "./Utf8String.h"
#include <Windows.h>

NAMESPACE_BEGIN

std::wstring Utf8StringToWString(const Utf8String& source)
{
  MultiByteToWideChar(CP_UTF8, 0, nullptr, )
}

Utf8String WStringToUtf8String(const std::wstring& source)
{

}

NAMESPACE_END