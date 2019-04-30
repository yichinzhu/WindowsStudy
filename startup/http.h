#pragma once

#include <winhttp.h>
BOOL Request(LPCWSTR host, INTERNET_PORT port, LPCWSTR method, LPCWSTR uri, LPCWSTR headers, LPVOID data, DWORD dataLength, LPVOID dataReceived);