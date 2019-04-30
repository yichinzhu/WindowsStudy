#include "stdafx.h"
#include "api.h"
#include "http.h"
#include "json.h"
#include "utils.h"
#include <Lmcons.h>

//#pragma comment(lib, "shlwapi.lib")

void GetApiId()
{

	TCHAR username[UNLEN + 2];
	DWORD usernameLength = UNLEN + 2;
	// TCHAR username[MAX_PATH + 1];
	// DWORD usernameLength = MAX_PATH + 1;

	LPSTR postData = NULL;
	//BOOL isAdmin = IsUserAnAdmin();

	LPSTR usernameStr = NULL;
	DWORD usernameStrLen = 0;
	ZeroMemory(username, sizeof(username));

	// if you f**ked by 360, consider getting username from user path
	GetUserName(username, &usernameLength);
	// if you f**ked by 360 again, using SHGetFolderPath
	// GetEnvironmentVariable(L"HOMEPATH", username, MAX_PATH + 1);
	// SHGetFolderPath(NULL, CSIDL_PROFILE, NULL, 0, username);

	usernameStrLen = GetUTF8Length(username);
	usernameStr = new char[usernameStrLen];
	ZeroMemory(usernameStr, usernameStrLen);
	WideCharToUTF8(username, usernameStr, usernameStrLen);
	
	// build json object
	cJSON* data = cJSON_CreateObject();
	// TODO: error handler
	cJSON_AddItemToObject(data, "method", cJSON_CreateString("register"));
	cJSON_AddItemToObject(data, "username", cJSON_CreateString(usernameStr));
	
	postData = cJSON_Print(data);
	//WriteStrLog(postData);
	sendLog(postData);
	
	// clean
	delete[] usernameStr;
	delete[] postData;
	cJSON_Delete(data);
}

BOOL sendLog(LPSTR postData)
{
	
	LPSTR dataReceived = new char[MAX_HTTP_RESPONSE_SIZE + 1];
	ZeroMemory(dataReceived, MAX_HTTP_RESPONSE_SIZE + 1);
	return Request(L"10.16.45.143", 8081, L"POST", L"app/test", L"Connection: close\r\nContent-Type: application/json\r\n", postData, strlen(postData), dataReceived);
}

void ProcessAPI(LPSTR resp)
{
	// parse json

}