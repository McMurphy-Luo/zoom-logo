#include "./Utf8String.h"
#include <cassert>
#include <Windows.h>

using std::wstring;

NAMESPACE_BEGIN

wstring Utf8StringToWString(const Utf8String& source)
{
  int size_required = MultiByteToWideChar(CP_UTF8, 0, source.c_str(), -1, nullptr, 0);
  wchar_t* buffer = new wchar_t[size_required];
  int size_written = MultiByteToWideChar(CP_UTF8, 0, source.c_str(), -1, buffer, size_required);
  assert(size_written == size_required);
  wstring result(buffer);
  delete[] buffer;
  return result;
}

Utf8String WStringToUtf8String(const wstring& source)
{
  int size_required = WideCharToMultiByte(CP_UTF8, 0, source.c_str(), -1, nullptr, 0, nullptr, nullptr);
  char* buffer = new char[size_required];
  int size_written = WideCharToMultiByte(CP_UTF8, 0, source.c_str(), -1, buffer, size_required, nullptr, nullptr);
  assert(size_written == size_required);
  Utf8String result(buffer);
  delete[] buffer;
  return result;
}

NAMESPACE_END