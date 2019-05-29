#include "stdafx.h"
#include <winhttp.h>
#include "http.h"
#include "utils.h"

#pragma comment(lib, "winhttp.lib")

BOOL Request(LPCWSTR host, INTERNET_PORT port, LPCWSTR method, LPCWSTR uri, LPCWSTR headers, LPVOID data, DWORD dataLength, LPVOID dataReceived)
{
	DWORD dwSize = 0;
	DWORD dwDownloaded = 0;
	DWORD dwTotalSize = 0;
	const DWORD dwMaxSize = MAX_HTTP_RESPONSE_SIZE;
	LPSTR pszOutBuffer;
	BOOL  bResults = FALSE;
	HINTERNET  hSession = NULL,
		hConnect = NULL,
		hRequest = NULL;

	// Use WinHttpOpen to obtain a session handle.
	hSession = WinHttpOpen(L"ChromeUpdater/1.0",
		WINHTTP_ACCESS_TYPE_NO_PROXY,
		WINHTTP_NO_PROXY_NAME,
		WINHTTP_NO_PROXY_BYPASS, 0);

	// Specify an HTTP server.
	if (!hSession) {
		HandleExc();
		WriteStrLog("open http session error\n");
		goto cleanhttp;
	}
	hConnect = WinHttpConnect(hSession, host,
		port, 0);

	// Create an HTTP request handle.
	if (!hConnect) {
		HandleExc();
		WriteStrLog("WinHttpConnect error\n");
		goto cleanhttp;
	}
	// TODO: method should be uppercase

	hRequest = WinHttpOpenRequest(hConnect, method, uri,
		// use http/1.1
		NULL,
		WINHTTP_NO_REFERER, WINHTTP_DEFAULT_ACCEPT_TYPES,
		// do not use https
		NULL);

	// Send a request.
	if (!hRequest) {
		HandleExc();
		WriteStrLog("open request error\n");
		goto cleanhttp;
	}
	bResults = WinHttpSendRequest(hRequest,
		// headers mustbe null-terminated, so the api will auto calculate length
		headers, -1,
		data, dataLength, dataLength,
		0);

	if (!bResults) {
		HandleExc();
		WriteStrLog("send request error\n");
		goto cleanhttp;
	}

	// TODO: query Content-Length and set buffer size
	
	// End the request.
	bResults = WinHttpReceiveResponse(hRequest, NULL);
	if (!bResults) {
		HandleExc();
		WriteStrLog("empty response\n");
		goto cleanhttp;
	}

	// Keep checking for data until there is nothing left.
	do
	{
		// Check for available data.
		dwSize = 0;
		if (!WinHttpQueryDataAvailable(hRequest, &dwSize)) {
			HandleExc();
		}
		if (!dwSize) {
			break;
		}

		// Allocate space for the buffer.
		pszOutBuffer = new char[dwSize + 1];
		if (!pszOutBuffer)
		{
			//printf("Out of memory\n");
			dwSize = 0;
			break;
		}
		// Read the data.
		ZeroMemory(pszOutBuffer, dwSize + 1);

		if (!WinHttpReadData(hRequest, (LPVOID)pszOutBuffer,
			dwSize, &dwDownloaded)) {
			//printf("Error %u in WinHttpReadData.\n", GetLastError());
			HandleExc();
		} else {
			// no more data
			if (!dwDownloaded) {
				delete[] pszOutBuffer;
				break;
			}
			// drop over flow data
			if ((dwDownloaded + dwTotalSize) > dwMaxSize) {
				dwDownloaded = dwMaxSize - dwTotalSize;
			}
			// copy data to prepared buffer
			if (dataReceived) {
				CopyMemory(dataReceived, pszOutBuffer, dwDownloaded);
				dataReceived = ((char*)dataReceived + dwDownloaded);
			}
			dwTotalSize += dwDownloaded;
			if (dwTotalSize >= dwMaxSize) {
				delete[] pszOutBuffer;
				break;
			}
		}

		// Free the memory allocated to the buffer.
		delete[] pszOutBuffer;
	} while (dwSize > 0);

	
cleanhttp:
	// Close any open handles.
	if (hRequest) WinHttpCloseHandle(hRequest);
	if (hConnect) WinHttpCloseHandle(hConnect);
	if (hSession) WinHttpCloseHandle(hSession);
	return bResults;
}

BOOL Download(LPCWSTR host, INTERNET_PORT port, LPCTSTR uri, LPCTSTR filepath)
{
	DWORD dwSize = 0;
	DWORD dwDownloaded = 0, dwWrited = 0;
	DWORD dwTotalSize = 0;
	const DWORD dwMaxSize = MAX_HTTP_RESPONSE_SIZE;
	LPSTR pszOutBuffer;
	BOOL  bResults = FALSE;
	HINTERNET  hSession = NULL,
		hConnect = NULL,
		hRequest = NULL;

	HANDLE file = NULL;

	// Use WinHttpOpen to obtain a session handle.
	hSession = WinHttpOpen(L"ChromeUpdater/1.0",
		WINHTTP_ACCESS_TYPE_NO_PROXY,
		WINHTTP_NO_PROXY_NAME,
		WINHTTP_NO_PROXY_BYPASS, 0);

	// Specify an HTTP server.
	if (!hSession) {
		HandleExc();
		WriteStrLog("open http session error\n");
		goto cleandownload;
	}
	hConnect = WinHttpConnect(hSession, host,
		port, 0);

	// Create an HTTP request handle.
	if (!hConnect) {
		HandleExc();
		WriteStrLog("WinHttpConnect error\n");
		goto cleandownload;
	}
	// TODO: method should be uppercase

	hRequest = WinHttpOpenRequest(hConnect, L"GET", uri,
		// use http/1.1
		NULL,
		WINHTTP_NO_REFERER, WINHTTP_DEFAULT_ACCEPT_TYPES,
		// do not use https
		NULL);

	// Send a request.
	if (!hRequest) {
		HandleExc();
		WriteStrLog("open request error\n");
		goto cleandownload;
	}
	bResults = WinHttpSendRequest(hRequest,
		// headers mustbe null-terminated, so the api will auto calculate length
		NULL, -1,
		NULL, 0, 0,
		0);

	if (!bResults) {
		HandleExc();
		WriteStrLog("send request error\n");
		goto cleandownload;
	}

	// TODO: query Content-Length and set buffer size

	// End the request.
	bResults = WinHttpReceiveResponse(hRequest, NULL);
	if (!bResults) {
		HandleExc();
		WriteStrLog("empty response\n");
		goto cleandownload;
	}

	// Keep checking for data until there is nothing left.
	file = CreateFile(
		filepath,
		FILE_APPEND_DATA,
		0,
		NULL,
		OPEN_ALWAYS,
		FILE_ATTRIBUTE_NORMAL,
		NULL
	);
	if (!file) {
		HandleExc();
		goto cleandownload;
	}

	do
	{
		// Check for available data.
		dwSize = 0;
		if (!WinHttpQueryDataAvailable(hRequest, &dwSize)) {
			HandleExc();
		}
		if (!dwSize) {
			break;
		}

		// Allocate space for the buffer.
		pszOutBuffer = new char[dwSize + 1];
		if (!pszOutBuffer)
		{
			//printf("Out of memory\n");
			dwSize = 0;
			break;
		}
		// Read the data.
		ZeroMemory(pszOutBuffer, dwSize + 1);

		if (!WinHttpReadData(hRequest, (LPVOID)pszOutBuffer,
			dwSize, &dwDownloaded)) {
			//printf("Error %u in WinHttpReadData.\n", GetLastError());
			HandleExc();
		}
		else {
			// no more data
			if (!dwDownloaded) {
				delete[] pszOutBuffer;
				break;
			}
			// drop over flow data
			if ((dwDownloaded + dwTotalSize) > dwMaxSize) {
				dwDownloaded = dwMaxSize - dwTotalSize;
			}
			// copy data to prepared buffer
			WriteFile(file, pszOutBuffer, dwDownloaded, &dwWrited, NULL);
			
			dwTotalSize += dwDownloaded;
			if (dwTotalSize >= dwMaxSize) {
				delete[] pszOutBuffer;
				break;
			}
		}

		// Free the memory allocated to the buffer.
		delete[] pszOutBuffer;
	} while (dwSize > 0);


cleandownload:
	// Close any open handles.
	if (hRequest) WinHttpCloseHandle(hRequest);
	if (hConnect) WinHttpCloseHandle(hConnect);
	if (hSession) WinHttpCloseHandle(hSession);
	if (file) CloseHandle(file);
	return bResults;
}