#pragma once
void HandleExc();
BOOL GetAppPath(LPTSTR appPath);
BOOL CheckInstall();
BOOL ReleaseFile(DWORD resourceId);
BOOL CheckStartUp();
void AlertExc();
void WriteLog(LPCTSTR msg);
void PrintDebug(LPCTSTR msg);
void WriteStrLog(LPCSTR msg);
void PrintStrDebug(LPCSTR msg);