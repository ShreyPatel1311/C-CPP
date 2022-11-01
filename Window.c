#include<windows.h>
#include<string.h>
#include<stdlib.h>
#include<stdio.h>

#define FILE_EXIT 1
#define FILE_NEW 2
#define FILE_OPEN 3
#define GenerateButton 4

LRESULT CALLBACK WindowProcedure(HWND, UINT, WPARAM, LPARAM);
void AddMenus(HWND);
void AddControls(HWND);
void LoadImages();

HWND hName, hAge, hOut, hLogo;
HMENU hMenu;
HBITMAP hLogoImage, hGenerateImage;

int WINAPI WinMain(HINSTANCE hInst, HINSTANCE hPrevInst, LPSTR args, int nCmdShow)
{
    WNDCLASSW wc = {0};
    wc.hbrBackground = (HBRUSH)COLOR_WINDOW;
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.hInstance = hInst;
    wc.lpszClassName = L"MyWindowClass";
    wc.lpfnWndProc = WindowProcedure;

    if(!RegisterClassW(&wc))
        return -1;

    CreateWindowW(L"MyWindowClass", L"My Window", WS_OVERLAPPEDWINDOW | WS_VISIBLE, 100, 100, 500, 500, NULL, NULL, NULL, NULL);

    MSG msg = {0};

    while(GetMessage(&msg, NULL, NULL, NULL))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return 0;
}

LRESULT CALLBACK WindowProcedure(HWND hWnd, UINT msg, WPARAM wp, LPARAM lp)
{
    switch(msg)
    {
    case WM_COMMAND:
        switch(wp)
        {
        case FILE_NEW:
            break;
        case FILE_EXIT:
            MessageBeep(MB_OK);
            DestroyWindow(hWnd);
            break;
        case GenerateButton: ;
            char Name[30];
            char Age[10];
            char Out[70];

            GetWindowText(hName, Name, 30);
            GetWindowText(hAge, Age, 10);

            strcpy(Out, "My Name is ");
            strcat(Out, Name);
            strcat(Out, "\n And I'm ");
            strcat(Out, Age);
            strcat(Out, " years Old!");

            SetWindowText(hOut, Out);
            break;
        }
        break;
    case WM_CREATE:
        LoadImages();
        AddMenus(hWnd);
        AddControls(hWnd);
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProcW(hWnd, msg, wp, lp);
    }
}

void AddMenus(HWND hWnd)
{
    hMenu = CreateMenu();
    HMENU hFileMenu = CreateMenu();
    HMENU hSubMenu = CreateMenu();

    AppendMenu(hSubMenu, MF_SEPARATOR, NULL, NULL);
    AppendMenu(hFileMenu, MF_STRING, FILE_NEW, "New");
    AppendMenu(hFileMenu, MF_POPUP, (UINT_PTR)hSubMenu, "Open");
    AppendMenu(hFileMenu, MF_STRING, FILE_EXIT, "Exit");
    AppendMenu(hFileMenu, MF_SEPARATOR, NULL, NULL);

    AppendMenu(hMenu, MF_POPUP, (UINT_PTR)hFileMenu, "File");
    AppendMenu(hMenu, MF_STRING, NULL, "Help");
    SetMenu(hWnd, hMenu);
}

void AddControls(HWND hWnd)
{
    CreateWindowW(L"static", L"Name :", WS_VISIBLE | WS_CHILD, 125, 0, 38, 20, hWnd, NULL, NULL, NULL);
    hName = CreateWindowW(L"edit", L"", WS_VISIBLE | WS_CHILD | WS_BORDER, 175, 0, 100, 20, hWnd, NULL, NULL, NULL);

    CreateWindowW(L"static", L"Age :", WS_VISIBLE | WS_CHILD, 125, 25, 38, 20, hWnd, NULL, NULL, NULL);
    hAge = CreateWindowW(L"edit", L"", WS_VISIBLE | WS_CHILD | WS_BORDER, 175, 25, 100, 20, hWnd, NULL, NULL, NULL);

    HWND hButton= CreateWindowW(L"Button", NULL, WS_VISIBLE | WS_CHILD | BS_BITMAP, 150, 50, 55, 55, hWnd, (HMENU)GenerateButton, NULL, NULL);
    SendMessageW(hButton, BM_SETIMAGE, IMAGE_BITMAP, (LPARAM)hGenerateImage);

    hOut = CreateWindowW(L"edit", L"", WS_VISIBLE | WS_CHILD | WS_BORDER, 75, 100, 300, 300, hWnd, NULL, NULL, NULL);
    hLogo = CreateWindowW(L"static", NULL, WS_VISIBLE | WS_CHILD | SS_BITMAP, 0, 0, 100, 100, hWnd, NULL, NULL, NULL);
    SendMessageW(hLogo, STM_SETIMAGE, IMAGE_BITMAP, (LPARAM)hLogoImage);
}

void LoadImages()
{
    hLogoImage = (HBITMAP)LoadImageW(NULL, L"Button_Off.bmp", IMAGE_BITMAP, 50, 50, LR_LOADFROMFILE);
    hGenerateImage = (HBITMAP)LoadImageW(NULL, L"Button_On.bmp", IMAGE_BITMAP, 50, 50, LR_LOADFROMFILE);
}
