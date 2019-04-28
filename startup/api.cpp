#include "stdafx.h"
#include "http.h"
#include <Lmcons.h>

void baseInfo()
{
	TCHAR username[UNLEN + 1];
	DWORD usernameLength = 0;
	ZeroMemory(username, sizeof(username));
	GetUserName(username, &usernameLength);

}

void sendLog()
{
	char postData[] = "{\"foo\": \"bar\"}";
	LPSTR dataReceived = new char[MAX_HTTP_RESPONSE_SIZE + 1];
	ZeroMemory(dataReceived, MAX_HTTP_RESPONSE_SIZE + 1);
	Request(L"10.16.45.143", 8081, L"POST", L"app/test", L"Connection: close\r\nContent-Type: application/json\r\n", postData, strlen(postData), dataReceived);
}