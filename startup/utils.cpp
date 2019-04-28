#include "stdafx.h"
#include "utils.h"


void AlertExc()
{
#ifdef _DEBUG
	DWORD err = GetLastError();
	TCHAR buffer[1024];
	FormatMessage(FORMAT_MESSAGE_IGNORE_INSERTS | FORMAT_MESSAGE_FROM_SYSTEM,
		NULL, err, 0, buffer, 1024, NULL);
	MessageBox(NULL, buffer, NULL, MB_OK);
#endif // DEBUG
}

void handleExc()
{
#ifdef _DEBUG
	DWORD err = GetLastError();
	TCHAR buffer[1024];
	FormatMessage(FORMAT_MESSAGE_IGNORE_INSERTS | FORMAT_MESSAGE_FROM_SYSTEM,
		NULL, err, 0, buffer, 1024, NULL);
	WriteLog(buffer);
#endif
}

void PrintDebug(LPTSTR msg)
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

BOOL CheckStartUp()
{
	HKEY hKey;
	RegOpenCurrentUser(KEY_ALL_ACCESS, &hKey);
	if (RegOpenKeyEx(hKey,
		L"Software\\Microsoft\\Windows\\CurrentVersion\\Run",
		0,
		KEY_QUERY_VALUE,
		&hKey) != ERROR_SUCCESS) {
		handleExc();
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
	DWORD len = WideCharToMultiByte(CP_UTF8, 0, msg, -1, NULL, 0, 0, 0);
	if (len <= 0) {
		AlertExc();
		return;
	}
	msgInBytes = new char[len + 1];
	WideCharToMultiByte(CP_UTF8, 0, msg, -1, msgInBytes, len, 0, 0);

	// open log file
	TCHAR logFile[MAX_PATH + 1];
	GetAppPath(logFile);
	PathCombine(logFile, logFile, L"application.log");
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
	WriteFile(logHandler, "\r\n", 2, &writenLen, NULL);
	// clean
	delete[] msgInBytes;
	CloseHandle(logHandler);
}

void WriteStrLog(LPSTR msg)
{
	TCHAR logFile[MAX_PATH + 1];
	GetAppPath(logFile);
	PathCombine(logFile, logFile, L"application.log");
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
	WriteFile(logHandler, "\r\n", 2, &writenLen, NULL);
	// clean
	CloseHandle(logHandler);
}

void PrintStrDebug(LPSTR msg)
{
#ifdef _DEBUG
	WriteStrLog(msg);
#endif // DEBUG

}