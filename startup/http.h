#pragma once

BOOL Request(LPCWSTR host, DWORD port, LPCWSTR method, LPCWSTR uri, LPCWSTR headers, LPVOID data, DWORD dataLength, LPVOID dataReceived);