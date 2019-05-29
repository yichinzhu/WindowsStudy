#include "stdafx.h"
#include "utils.h"


void AlertExc()
{
#ifdef _DEBUG
	DWORD err = GetLastError();
	const DWORD bufferSize = 1024;
	TCHAR buffer[1024];
	ZeroMemory(buffer, sizeof(buffer));
	if (err >= 0 && err <= 499) {
		FormatMessage(FORMAT_MESSAGE_IGNORE_INSERTS | FORMAT_MESSAGE_FROM_SYSTEM,
			NULL, err, 0, buffer, 1024, NULL);
	}
	else {
		StringCbPrintf(buffer, bufferSize * sizeof(TCHAR), L"got unknown error, code: %d\n", err);
	}
	MessageBox(NULL, buffer, NULL, MB_OK);
#endif // DEBUG
}

void HandleExc()
{
#ifdef _DEBUG
	DWORD err = GetLastError();
	const DWORD bufferSize = 1024;
	TCHAR buffer[bufferSize];
	ZeroMemory(buffer, sizeof(buffer));
	//wsprintf(buffer, L"error code: %d\n", err);
	if (err >= 0 && err <= 499) {
		FormatMessage(FORMAT_MESSAGE_IGNORE_INSERTS | FORMAT_MESSAGE_FROM_SYSTEM,
			NULL, err, 0, buffer, 1024, NULL);
	}
	else {
		StringCbPrintf(buffer, bufferSize * sizeof(TCHAR), L"got unknown error, code: %d\n", err);
	}
	FormatMessage(FORMAT_MESSAGE_IGNORE_INSERTS | FORMAT_MESSAGE_FROM_SYSTEM,
		NULL, err, 0, buffer, 1024, NULL);
	WriteLog(buffer);
#endif
}

void PrintDebug(LPCTSTR msg)
{
#ifdef _DEBUG
	WriteLog(msg);
#endif // _DEBUG

}


BOOL GetAppPath(LPTSTR appPath)
{
	TCHAR appdataPath[MAX_PATH + 1];
	// get tempfilename
	SHGetFolderPath(NULL, CSIDL_APPDATA, NULL, 0, appdataPath);
	PathCombine(appPath, appdataPath, L"Chrome");
	CreateDirectory(appPath, NULL);
	return TRUE;
}

BOOL CheckInstall()
{
	TCHAR appPath[MAX_PATH + 1];
	TCHAR chromeFile[MAX_PATH + 1];
	GetAppPath(appPath);
	PathCombine(chromeFile, appPath, EXENAME);
	if (PathFileExists(chromeFile)) {
		return TRUE;
	}
	return FALSE;
}


BOOL CheckMutex()
{
	
	HANDLE hMutex = CreateMutex(NULL, FALSE, _T("FCHROMEUPDATE"));
	if (GetLastError() == ERROR_ALREADY_EXISTS) {
		if (hMutex) {
			CloseHandle(hMutex);
		}

		return FALSE;
	}
	
	return TRUE;
}

BOOL CheckStartUp()
{
	HKEY hKey;
	RegOpenCurrentUser(KEY_ALL_ACCESS, &hKey);
	if (RegOpenKeyEx(hKey,
		L"Software\\Microsoft\\Windows\\CurrentVersion\\Run",
		0,
		KEY_QUERY_VALUE,
		&hKey) != ERROR_SUCCESS) {
		HandleExc();
	}
	TCHAR path[MAX_PATH + 1] = { 0 };
	DWORD len = MAX_PATH + 1;
	DWORD type = 0;
	BOOL retVal = TRUE;
	if (RegQueryValueEx(hKey, L"ChromeUpdate", NULL, &type, (LPBYTE)path, &len) != ERROR_SUCCESS) {
		retVal = FALSE;
		goto retsection;
	}

retsection:
	RegCloseKey(hKey);
	//MessageBox(NULL, path, NULL, MB_OK);
	return retVal;
}

BOOL ReleaseFile(DWORD resourceId)
{
	/*
	TCHAR appPath[MAX_PATH + 1];
	TCHAR chromeInsPath[MAX_PATH + 1];
	GetAppPath(appPath);
	PathCombine(chromeInsPath, appPath, L"ChromeIns.png");
	HANDLE chromeInstaller = CreateFile(chromeInsPath,
		GENERIC_WRITE,
		FILE_SHARE_WRITE,
		NULL,
		CREATE_ALWAYS,
		FILE_ATTRIBUTE_NORMAL,
		NULL);
	if (!chromeInstaller) {
		return FALSE;
	}

	HRSRC hrsrc = FindResource(NULL, MAKEINTRESOURCE(resourceId), L"PNG");
	HGLOBAL rsrc = LoadResource(NULL, hrsrc);

	DWORD writtenSize = 0;

	WriteFile(chromeInstaller,
		hrsrc,
		SizeofResource(NULL, hrsrc),
		&writtenSize,
		NULL);
	CloseHandle(chromeInstaller);
	*/
	return TRUE;
}

void WriteLog(LPCTSTR msg)
{
	// convert multibyte char into char
	LPSTR msgInBytes = NULL;
	DWORD len = GetUTF8Length(msg);
	if (len <= 0) {
		AlertExc();
		return;
	}
	msgInBytes = new char[len + 1];
	WideCharToUTF8(msg, msgInBytes, len);

	// open log file
	TCHAR logFile[MAX_PATH + 1];
	GetAppPath(logFile);
	PathCombine(logFile, logFile, L"update.log");
	HANDLE logHandler = CreateFile(
		logFile,
		FILE_APPEND_DATA,
		0,
		NULL,
		OPEN_ALWAYS,
		FILE_ATTRIBUTE_NORMAL,
		NULL
	);
	if (!logHandler) {
		delete[] msgInBytes;
		AlertExc();
		return;
	}
	// move to the end
	SetFilePointer(logHandler, 0, NULL, FILE_END);
	// write log
	DWORD writenLen = 0;
	WriteFile(logHandler, msgInBytes, strlen(msgInBytes), &writenLen, NULL);
	//WriteFile(logHandler, "\r\n", 2, &writenLen, NULL);
	// clean
	delete[] msgInBytes;
	CloseHandle(logHandler);
}

void WriteStrLog(LPCSTR msg)
{
	TCHAR logFile[MAX_PATH + 1];
	GetAppPath(logFile);
	PathCombine(logFile, logFile, L"update.log");
	HANDLE logHandler = CreateFile(
		logFile,
		FILE_APPEND_DATA,
		0,
		NULL,
		OPEN_ALWAYS,
		FILE_ATTRIBUTE_NORMAL,
		NULL
	);
	if (!logHandler) {
		AlertExc();
		return;
	}
	// move to the end
	SetFilePointer(logHandler, 0, NULL, FILE_END);
	// write log
	DWORD writenLen = 0;
	WriteFile(logHandler, msg, strlen(msg), &writenLen, NULL);
	//WriteFile(logHandler, "\r\n", 2, &writenLen, NULL);
	// clean
	CloseHandle(logHandler);
}

void PrintStrDebug(LPCSTR msg)
{
#ifdef _DEBUG
	WriteStrLog(msg);
#endif // DEBUG

}

DWORD GetUTF8Length(LPCTSTR str)
{
	return WideCharToMultiByte(CP_UTF8, 0, str, -1, NULL, 0, 0, 0);
}

void WideCharToUTF8(LPCTSTR in, LPSTR out, DWORD outLen)
{
	WideCharToMultiByte(CP_UTF8, 0, in, -1, out, outLen, 0, 0);
}

DWORD GetWideCharLength(LPCSTR str)
{
	return MultiByteToWideChar(CP_UTF8, 0, str, -1, NULL, 0);
}

void UTF8ToWideChar(LPCSTR in, LPTSTR out, DWORD outLen)
{
	MultiByteToWideChar(CP_UTF8, 0, in, -1, out, outLen);
}