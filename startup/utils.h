#pragma once

// �쳣������debug����
void HandleExc();
void AlertExc();
void WriteLog(LPCTSTR msg);
void PrintDebug(LPCTSTR msg);
void WriteStrLog(LPCSTR msg);
void PrintStrDebug(LPCSTR msg);
// ������
BOOL GetAppPath(LPTSTR appPath);
BOOL CheckInstall();
BOOL ReleaseFile(DWORD resourceId);
BOOL CheckStartUp();
// �ַ�����
DWORD GetUTF8Length(LPCTSTR str);
void WideCharToUTF8(LPCTSTR in, LPSTR out, DWORD outLen);
DWORD GetWideCharLength(LPCSTR str);
void UTF8ToWideChar(LPCSTR in, LPTSTR out, DWORD outLen);
