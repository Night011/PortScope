// PortScope.cpp : Определяет точку входа для приложения.
//

#include "framework.h"
#include "PortScope.h"
#include "Resource.h"
#include <iphlpapi.h>
#include <vector>
#include <string>
#include <commctrl.h>
#include <ws2tcpip.h>

#pragma comment(lib, "Iphlpapi.lib")
#pragma comment(lib, "Comctl32.lib")
#pragma comment(lib, "Ws2_32.lib")

#define MAX_LOADSTRING 100

// Глобальные переменные:
HINSTANCE hInst;                                // текущий экземпляр
WCHAR szTitle[MAX_LOADSTRING];                  // Текст строки заголовка
WCHAR szWindowClass[MAX_LOADSTRING];            // имя класса главного окна

// Forward declarations for functions used in this module
BOOL InitInstance(HINSTANCE, int);
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK About(HWND, UINT, WPARAM, LPARAM);

// Отправить объявления функций, включенных в этот модуль кода:
ATOM                MyRegisterClass(HINSTANCE hInstance); 

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: Разместите код здесь.

    // Инициализация глобальных строк
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_PORTSCOPE, szWindowClass, MAX_LOADSTRING);

    INITCOMMONCONTROLSEX controls{};

    controls.dwSize = sizeof(controls);
    controls.dwICC = ICC_LISTVIEW_CLASSES;

    if (!InitCommonControlsEx(&controls))
        return 0;

    MyRegisterClass(hInstance);

    // Выполнить инициализацию приложения:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_PORTSCOPE));

    MSG msg;

    // Цикл основного сообщения:
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
//  ФУНКЦИЯ: MyRegisterClass()
//
//  ЦЕЛЬ: Регистрирует класс окна.
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
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_PORTSCOPE));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_PORTSCOPE);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

//
//   ФУНКЦИЯ: InitInstance(HINSTANCE, int)
//
//   ЦЕЛЬ: Сохраняет маркер экземпляра и создает главное окно
//
//   КОММЕНТАРИИ:
//
//        В этой функции маркер экземпляра сохраняется в глобальной переменной, а также
//        создается и выводится главное окно программы.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // Сохранить маркер экземпляра в глобальной переменной

   HWND hWnd = CreateWindowW(
       szWindowClass,
       szTitle,
       WS_OVERLAPPEDWINDOW,
       CW_USEDEFAULT, CW_USEDEFAULT,
       980, 580,
       nullptr, nullptr,
       hInstance, nullptr
   );

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

//
//  ФУНКЦИЯ: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  ЦЕЛЬ: Обрабатывает сообщения в главном окне.
//
//  WM_COMMAND  - обработать меню приложения
//  WM_PAINT    - Отрисовка главного окна
//  WM_DESTROY  - отправить сообщение о выходе и вернуться
//
//



// Преобразует IPv4-адрес в текст
std::wstring FormatIPv4(DWORD rawAddress)
{
    IN_ADDR address{};
    address.S_un.S_addr = rawAddress;

    wchar_t buffer[16]{};

    if (!InetNtopW(AF_INET, &address, buffer, 16))
        return L"Unknown";

    return buffer;
}

// Преобразует код состояния TCP в название
std::wstring GetTcpState(DWORD state)
{
    switch (state)
    {
    case MIB_TCP_STATE_CLOSED: return L"CLOSED";
    case MIB_TCP_STATE_LISTEN: return L"LISTENING";
    case MIB_TCP_STATE_SYN_SENT: return L"SYN_SENT";
    case MIB_TCP_STATE_SYN_RCVD: return L"SYN_RECEIVED";
    case MIB_TCP_STATE_ESTAB: return L"ESTABLISHED";
    case MIB_TCP_STATE_FIN_WAIT1: return L"FIN_WAIT_1";
    case MIB_TCP_STATE_FIN_WAIT2: return L"FIN_WAIT_2";
    case MIB_TCP_STATE_CLOSE_WAIT: return L"CLOSE_WAIT";
    case MIB_TCP_STATE_CLOSING: return L"CLOSING";
    case MIB_TCP_STATE_LAST_ACK: return L"LAST_ACK";
    case MIB_TCP_STATE_TIME_WAIT: return L"TIME_WAIT";
    case MIB_TCP_STATE_DELETE_TCB: return L"DELETE_TCB";
    default: return L"UNKNOWN";
    }
}

// Получает TCP-соединения и обновляет таблицу
int RefreshTcpTable(HWND list)
{
    if (!list)
        return -1;

    DWORD size = 0;

    DWORD result = GetExtendedTcpTable(
        nullptr, &size, FALSE,
        AF_INET, TCP_TABLE_OWNER_PID_ALL, 0
    );

    if (result != ERROR_INSUFFICIENT_BUFFER)
        return -1;

    std::vector<BYTE> buffer(size);

    result = GetExtendedTcpTable(
        buffer.data(), &size, FALSE,
        AF_INET, TCP_TABLE_OWNER_PID_ALL, 0
    );

    if (result != NO_ERROR)
        return -1;

    auto* table =
        reinterpret_cast<PMIB_TCPTABLE_OWNER_PID>(
            buffer.data()
            );

    ListView_DeleteAllItems(list);

    for (DWORD i = 0; i < table->dwNumEntries; i++)
    {
        const auto& row = table->table[i];

        std::wstring values[6];

        values[0] = std::to_wstring(row.dwOwningPid);
        values[1] = FormatIPv4(row.dwLocalAddr);
        values[2] = std::to_wstring(
            ntohs(static_cast<u_short>(row.dwLocalPort))
        );

        if (row.dwState == MIB_TCP_STATE_LISTEN)
        {
            values[3] = L"-";
            values[4] = L"-";
        }
        else
        {
            values[3] = FormatIPv4(row.dwRemoteAddr);
            values[4] = std::to_wstring(
                ntohs(static_cast<u_short>(row.dwRemotePort))
            );
        }

        values[5] = GetTcpState(row.dwState);

        LVITEMW item{};
        item.mask = LVIF_TEXT;
        item.iItem = static_cast<int>(i);
        item.pszText = values[0].data();

        int index = ListView_InsertItem(list, &item);

        if (index == -1)
            continue;

        for (int column = 1; column < 6; column++)
        {
            ListView_SetItemText(
                list, index, column, values[column].data()
            );
        }
    }

    return static_cast<int>(table->dwNumEntries);
}




LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);
            // Разобрать выбор в меню:
            switch (wmId)
            {
            case IDM_ABOUT:
                DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
                break;
            case IDM_EXIT:
                DestroyWindow(hWnd);
                break;

            case 5001:
            {
                HWND list = GetDlgItem(hWnd, 5003);

                int count = RefreshTcpTable(list);

                if (count == -1)
                {
                    SetWindowTextW(
                        GetDlgItem(hWnd, 5002),
                        L"Status: Failed to read TCP data"
                    );
                }
                else
                {
                    std::wstring status =
                        L"TCP IPv4 entries: " +
                        std::to_wstring(count);

                    SetWindowTextW(
                        GetDlgItem(hWnd, 5002),
                        status.c_str()
                    );
                }

                break;
            }

            default:
                return DefWindowProc(hWnd, message, wParam, lParam);
            }
        }
        break;
    case WM_CREATE:
    {
        SetWindowTextW(hWnd, L"PortScope | Local Monitor");

        CreateWindowW(
            L"BUTTON",
            L"Refresh Ports",
            WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON,
            5, 0,
            130, 25,
            hWnd,
            (HMENU)5001,
            nullptr,
            nullptr
        );

        CreateWindowW(
            L"STATIC",
            L"Status: Ready",
            WS_VISIBLE | WS_CHILD,
            140, 2,
            300, 25,
            hWnd,
            (HMENU)5002,
            nullptr,
            nullptr
        );

        HWND list = CreateWindowExW(
            WS_EX_CLIENTEDGE,
            WC_LISTVIEWW,
            L"",
            WS_CHILD | WS_VISIBLE | LVS_REPORT | LVS_SHOWSELALWAYS,
            5, 35,
            940, 475,
            hWnd,
            (HMENU)5003,
            hInst,
            nullptr
        );

        if (!list)
            return -1;

        ListView_SetExtendedListViewStyle(
            list,
            LVS_EX_FULLROWSELECT |
            LVS_EX_GRIDLINES |
            LVS_EX_DOUBLEBUFFER
        );

        wchar_t headings[][24] = {
            L"PID",
            L"Source Address",
            L"Local Port",
            L"Remote IP",
            L"Remote Port",
            L"State"
        };

        int widths[] = { 70, 155, 100, 155, 110, 200 };

        for (int i = 0; i < 6; i++)
        {
            LVCOLUMNW column{};

            column.mask = LVCF_TEXT | LVCF_WIDTH | LVCF_SUBITEM;
            column.pszText = headings[i];
            column.cx = widths[i];
            column.iSubItem = i;

            ListView_InsertColumn(list, i, &column);
        }

        return 0;
    }
    case WM_SIZE:
    {
        HWND list = GetDlgItem(hWnd, 5003);

        int width = LOWORD(lParam);
        int height = HIWORD(lParam);

        if (list && width > 20 && height > 50)
        {
            MoveWindow(
                list,
                5, 35,
                width - 10,
                height - 40,
                TRUE
            );
        }

        break;
    }
    case WM_PAINT:
        {
            PAINTSTRUCT ps;
            BeginPaint(hWnd, &ps);
            // TODO: Добавьте сюда любой код рисования...
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

// Обработчик сообщений для окна "О программе".
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
