#include<windows.h>

LRESULT CALLBACK WindowProcedure(HWND, UINT, WPARAM, LPARAM);
void AddControls(HWND);
void LoadImages();

int ChangeImage = 0;

HWND hButton;
HBITMAP hButtonOn, hButtonOff;

int WINAPI WinMain(HINSTANCE hInst, HINSTANCE hPrevInst, LPSTR args, int nCmdShow)
{
    WNDCLASSW wc = {0};
    wc.hbrBackground = (HBRUSH)COLOR_WINDOW;
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.lpfnWndProc = WindowProcedure;
    wc.lpszClassName = L"MyWindowClass";
    wc.hInstance = hInst;

    if(!RegisterClassW(&wc))
    {
        return -1;
    }

    CreateWindowW(L"MyWindowClass", "My Window", WS_OVERLAPPEDWINDOW | WS_VISIBLE, 100, 100, 500, 500, NULL, NULL, NULL, NULL);

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
        if(ChangeImage == 1)
        {
            SendMessageW(hButton, BM_SETIMAGE, IMAGE_BITMAP, (LPARAM)hButtonOn);
            ChangeImage = 0;
        }
        else
        {
            SendMessageW(hButton, BM_SETIMAGE, IMAGE_BITMAP, (LPARAM)hButtonOff);
            ChangeImage = 1;
        }
        break;
    case WM_CREATE:
        LoadImages();
        AddControls(hWnd);
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProcW(hWnd, msg, wp, lp);
    }
}

void AddControls(HWND hWnd)
{
    hButton = CreateWindowW(L"Button", NULL, WS_CHILD | WS_VISIBLE | BS_BITMAP, 100, 100, 50, 50, hWnd, (HMENU)hButtonOff, NULL, NULL);
    SendMessageW(hButton, BM_SETIMAGE, IMAGE_BITMAP, (LPARAM)hButtonOff);
}

void LoadImages()
{
    hButtonOn = (HBITMAP)LoadImageW(NULL, L"Button_On.bmp", IMAGE_BITMAP, 25, 25, LR_LOADFROMFILE);
    hButtonOff = (HBITMAP)LoadImageW(NULL, L"Button_Off.bmp", IMAGE_BITMAP, 25, 25, LR_LOADFROMFILE);
}
