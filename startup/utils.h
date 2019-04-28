#pragma once
void handleExc();
BOOL GetAppPath(LPTSTR appPath);
BOOL CheckInstall();
BOOL ReleaseFile(DWORD resourceId);
BOOL CheckStartUp();
void AlertExc();
void WriteLog(LPCTSTR msg);
void PrintDebug(LPTSTR msg);
void WriteStrLog(LPSTR msg);
void PrintStrDebug(LPSTR msg);