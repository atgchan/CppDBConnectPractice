// cpp_dbConnect.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "cpp_dbConnect.h"

#include "sql.h"
#include "sqlext.h"

#include <string>
#include <iostream>

#pragma comment(lib, "odbc32.lib")

#define MAX_LOADSTRING 100

#define ID_ID 100
#define ID_PWD 101

#define ID_CREATE 200
#define ID_CHECK 201
#define ID_DELETE 202

SQLHENV hEnv;
SQLHDBC hDbc;
SQLHSTMT hStmt;

SQLWCHAR *ODBC_Name = (SQLWCHAR*)L"mysql_odbc";
SQLWCHAR *ODBC_ID = (SQLWCHAR*)L"root";
SQLWCHAR *ODBC_PW = (SQLWCHAR*)L"vudghk3";

bool DBConnect()
{
	if (SQLAllocHandle(SQL_HANDLE_ENV, SQL_NULL_HANDLE, &hEnv) != SQL_SUCCESS)
		return false;
	if (SQLSetEnvAttr(hEnv, SQL_ATTR_ODBC_VERSION, (SQLPOINTER)SQL_OV_ODBC3, SQL_IS_INTEGER) != SQL_SUCCESS)
		return false;
	if (SQLAllocHandle(SQL_HANDLE_DBC, hEnv, &hDbc) != SQL_SUCCESS)
		return false;
	if (SQLConnect(hDbc, ODBC_Name, SQL_NTS, ODBC_ID, SQL_NTS, ODBC_PW, SQL_NTS) != SQL_SUCCESS)
		return false;
	if (SQLAllocHandle(SQL_HANDLE_STMT, hDbc, &hStmt) != SQL_SUCCESS)
		return false;

	return true;
}

void DBDisConnect()
{
	if (hStmt) SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
	if (hDbc) SQLDisconnect(hDbc);
	if (hDbc) SQLFreeHandle(SQL_HANDLE_DBC, hDbc);
	if (hEnv) SQLFreeHandle(SQL_HANDLE_ENV, hEnv);
}

bool DBExcuteSQL(SQLWCHAR* qry)
{
	if (SQLExecDirect(hStmt, qry, SQL_NTS) != SQL_SUCCESS)
	{
		return false;
	}

	return true;
}

// Global Variables:
HINSTANCE hInst;                                // current instance
WCHAR szTitle[MAX_LOADSTRING];                  // The title bar text
WCHAR szWindowClass[MAX_LOADSTRING];            // the main window class name

// Forward declarations of functions included in this code module:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: Place code here.

    // Initialize global strings
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_CPP_DBCONNECT, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // Perform application initialization:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_CPP_DBCONNECT));

    MSG msg;

    // Main message loop:
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    return (int) msg.wParam;
}



//
//  FUNCTION: MyRegisterClass()
//
//  PURPOSE: Registers the window class.
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
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_CPP_DBCONNECT));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_CPP_DBCONNECT);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

//
//   FUNCTION: InitInstance(HINSTANCE, int)
//
//   PURPOSE: Saves instance handle and creates main window
//
//   COMMENTS:
//
//        In this function, we save the instance handle in a global variable and
//        create and display the main program window.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // Store instance handle in our global variable

   HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, 500, 200, nullptr, nullptr, hInstance, nullptr);

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

void ProccessCommand(HWND hWnd, int type, const TCHAR* id, const TCHAR* pwd)
{
	std::wstring wstr = L"";

	switch (type)
	{
		case ID_CREATE:
		{
			wstr = L"";
			wstr.append(L"CALL InsertUsers('");
			wstr.append(id);
			wstr.append(L"', '");
			wstr.append(pwd);
			wstr.append(L"')");
			if (true == DBExcuteSQL((SQLWCHAR*)wstr.c_str()))
			{
				MessageBox(hWnd, L"아이디 추가 성공!", L"아이디 추가 결과", MB_OK);
			}
			else
			{
				MessageBox(hWnd, L"아이디가 중복됩니다!", L"아이디 추가 결과", MB_OK);
			}
		}
		break;

		case ID_CHECK:
		{
			wstr = L"";
			wstr.append(L"CALL CheckUsers('");
			wstr.append(id);
			wstr.append(L"', '");
			wstr.append(pwd);
			wstr.append(L"')");
			if (true == DBExcuteSQL((SQLWCHAR*)wstr.c_str()))
			{
				MessageBox(hWnd, L"아이디와 비밀번호 일치!", L"비번 확인 결과", MB_OK);
			}
			else
			{
				MessageBox(hWnd, L"아이디 비밀번호 불일치!", L"비번 확인 결과", MB_OK);
			}
		}
		break;

		case ID_DELETE:
		{
			wstr = L"";
			wstr.append(L"CALL DeleteUsers('");
			wstr.append(id);
			wstr.append(L"', '");
			wstr.append(pwd);
			wstr.append(L"')");
			if (true == DBExcuteSQL((SQLWCHAR*)wstr.c_str()))
			{
				MessageBox(hWnd, L"삭제 성공!", L"아이디 삭제 결과", MB_OK);
			}
			else
			{
				MessageBox(hWnd, L"아이디 비밀번호 불일치!", L"아이디 삭제 결과", MB_OK);
			}
		}
		break;
	}
}

//
//  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE:  Processes messages for the main window.
//
//  WM_COMMAND  - process the application menu
//  WM_PAINT    - Paint the main window
//  WM_DESTROY  - post a quit message and return
//
//

HWND hId;
HWND hPwd;
TCHAR IdBuf[128];
TCHAR PwdBuf[128];

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
	case WM_CREATE:
		{
			DBConnect();
		}
		break;

    case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);
            // Parse the menu selections:
            switch (wmId)
            {
            case IDM_ABOUT:
                DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
                break;
            case IDM_EXIT:
                DestroyWindow(hWnd);
                break;

			case ID_CREATE:
				GetWindowText(hId, IdBuf, 128);
				GetWindowText(hPwd, PwdBuf, 128);
				ProccessCommand(hWnd, ID_CREATE, IdBuf, PwdBuf);
				break;

			case ID_CHECK:
				GetWindowText(hId, IdBuf, 128);
				GetWindowText(hPwd, PwdBuf, 128);
				ProccessCommand(hWnd, ID_CHECK, IdBuf, PwdBuf);
				break;

			case ID_DELETE:
				GetWindowText(hId, IdBuf, 128);
				GetWindowText(hPwd, PwdBuf, 128);
				ProccessCommand(hWnd, ID_DELETE, IdBuf, PwdBuf);
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

			// 아이디,  비밀번호 text
			TextOut(hdc, 10, 15, L"아이디: ", 5);
			TextOut(hdc, 10, 55, L"비밀번호: ", 6);
			//ReleaseDC(hWnd, hdc);

			// 아이디 input box
			hId = CreateWindow(L"edit", NULL, WS_CHILD | WS_VISIBLE | WS_BORDER |
				ES_AUTOHSCROLL, 90, 10, 200, 25, hWnd, (HMENU)ID_ID, hInst, NULL);


			// 아이디 input box
			hPwd = CreateWindow(L"edit", NULL, WS_CHILD | WS_VISIBLE | WS_BORDER |
				ES_AUTOHSCROLL, 90, 50, 200, 25, hWnd, (HMENU)ID_PWD, hInst, NULL);

			// 로그인 버튼
			CreateWindow(L"button", L"아이디 생성", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, 350, 10, 100, 25, hWnd, (HMENU)ID_CREATE, hInst, NULL);
			CreateWindow(L"button", L"비번 확인", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, 350, 50, 100, 25, hWnd, (HMENU)ID_CHECK, hInst, NULL);
			CreateWindow(L"button", L"아이디 삭제", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, 350, 90, 100, 25, hWnd, (HMENU)ID_DELETE, hInst, NULL);

            EndPaint(hWnd, &ps);
        }
        break;
    case WM_DESTROY:
		DBDisConnect();
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

// Message handler for about box.
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
