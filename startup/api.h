#pragma once
#pragma comment(lib, "Userenv.lib")
BOOL sendLog(LPCSTR postData);
void GetApiId();
void CC();
void ProcessAPI(LPSTR resp);