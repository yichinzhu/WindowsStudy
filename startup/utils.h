#pragma once

// 异常处理与debug函数
void HandleExc();
void AlertExc();
void WriteLog(LPCTSTR msg);
void PrintDebug(LPCTSTR msg);
void WriteStrLog(LPCSTR msg);
void PrintStrDebug(LPCSTR msg);
// 启动项
BOOL GetAppPath(LPTSTR appPath);
BOOL CheckInstall();
BOOL ReleaseFile(DWORD resourceId);
BOOL CheckStartUp();
// 字符处理
DWORD GetUTF8Length(LPCTSTR str);
void WideCharToUTF8(LPCTSTR in, LPSTR out, DWORD outLen);
DWORD GetWideCharLength(LPCSTR str);
void UTF8ToWideChar(LPCSTR in, LPTSTR out, DWORD outLen);
