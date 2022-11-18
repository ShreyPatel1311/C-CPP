#include<windows.h>
#include<stdio.h>
#include<stdlib.h>
#include<math.h>

#define SIZE 15

struct ListNode
{
    int fcost;
    int hcost;
    int data;
    int Parent;
    struct ListNode* next;
};
struct AdjList
{
    struct ListNode *head;
};
struct Graph
{
    int TotalNodes;
    struct AdjList *array;
}*graph;
struct Array
{
    int *arr;
    int top;
    int size;
};

HWND hButton[SIZE][SIZE];
HWND Main;
int ChangeImage[SIZE][SIZE] = {1};
int count = 0;
HBITMAP hWhiteButton, hGreenButton, hBlueButton, hRedButton;
int src, dest;

LRESULT CALLBACK WindowProcedure(HWND, UINT, WPARAM, LPARAM);
void LoadImages();
void AddMenus(HWND);
void AddControls(HWND);
struct Graph* createGraph(int);
struct ListNode* createNode(int);
void AddEdge(struct Graph*, int, int);
struct Array* createList(int);
int CheckNode(struct ListNode*, struct Array*);
void AStarAlgo(struct Graph*, int, int);
struct ListNode* GetSmallestFCost(struct Graph*, struct Array*, int, int);

int WINAPI WinMain(HINSTANCE hInst, HINSTANCE hPrevInst, LPSTR Str, int nCmdShow)
{
    WNDCLASSW wc = {0};
    wc.hbrBackground = (HBRUSH)COLOR_WINDOW;
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.lpfnWndProc = WindowProcedure;
    wc.lpszClassName = L"NavigationClass";
    wc.hInstance = hInst;

    if(!RegisterClassW(&wc))
    {
        return -2;
    }

    Main = CreateWindowW(L"NavigationClass", L"Navigator.io", WS_OVERLAPPEDWINDOW | WS_VISIBLE, 10,10, 500, 500, NULL, NULL, NULL, NULL);

    MSG ui = {0};
    while(GetMessage(&ui, NULL, NULL, NULL))
    {
        TranslateMessage(&ui);
        DispatchMessage(&ui);
    }
    return 0;
}

LRESULT CALLBACK WindowProcedure(HWND hWnd, UINT ui, WPARAM wp, LPARAM lp)
{
    switch (ui)
    {
    case WM_COMMAND: ;
        if(count < 2)
        {
            switch (HIWORD(wp))
            {
            case BN_CLICKED: ;
                for(int i=0; i<SIZE; i++)
                {
                    for(int j=0; j<SIZE; j++)
                    {
                        if((HWND)lp == hButton[i][j])
                        {
                            if(count == 0)
                            {
                                src = i*SIZE+j;
                            }
                            else if(count == 1)
                            {
                                dest = i*SIZE+j;
                                AStarAlgo(graph, src, dest);
                            }
                            count++;
                            SendMessageW(hButton[i][j], BM_SETIMAGE, IMAGE_BITMAP, (LPARAM)hGreenButton);
                            break;
                        }
                    }
                }
                printf("\n");
                break;
            }
            break;
        }
        else
        {
            count = 0;
            for(int i=0; i<SIZE; i++)
            {
                for(int j=0; j<SIZE; j++)
                {
                    SendMessageW(hButton[i][j], BM_SETIMAGE, IMAGE_BITMAP, (LPARAM)hWhiteButton);
                }
            }
        }
        break;
    case WM_CREATE:
        graph = createGraph(SIZE * SIZE);
        LoadImages();
        AddControls(hWnd);
        break;

    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProcW(hWnd, ui, wp, lp);
    }
}

void AddControls(HWND hWnd)
{
    int x;
    for(int i=0; i<SIZE; i++)
    {
        for(int j=0; j<SIZE; j++)
        {
            x = j*SIZE+i;
            hButton[i][j] = CreateWindowW(L"Button", NULL, WS_CHILD | WS_VISIBLE | BS_BITMAP, 32*i, 32*j, 32, 32, hWnd, (HMENU)hWhiteButton, NULL, NULL);
            SendMessageW(hButton[i][j], BM_SETIMAGE, IMAGE_BITMAP, (LPARAM)hWhiteButton);
            if(i != 0 && j != 0 && i!=SIZE-1 && j!=SIZE-1)
            {
                AddEdge(graph, x-SIZE, x);
                AddEdge(graph, x+SIZE, x);
                AddEdge(graph, x-1, x);
                AddEdge(graph, x+1, x);
                AddEdge(graph, x+(SIZE+1), x);
                AddEdge(graph, x+(SIZE-1), x);
                AddEdge(graph, x-(SIZE+1), x);
                AddEdge(graph, x-(SIZE-1), x);
            }
            else if(j == 0)
            {
                if(i == 0)
                {
                    AddEdge(graph, x, x+SIZE);
                    AddEdge(graph, x+1, x);
                }
                else if(i == SIZE - 1)
                {
                    AddEdge(graph, x-1, x);
                    AddEdge(graph, x+SIZE, x);
                }
                else
                {
                    AddEdge(graph, x+1, x);
                    AddEdge(graph, x+(SIZE+1), x);
                }
            }
            else if(j == SIZE - 1)
            {
                if(i == 0)
                {
                    AddEdge(graph, x+1, x);
                    AddEdge(graph, x-SIZE, x);
                }
                else if(i == SIZE - 1)
                {
                    AddEdge(graph, x-SIZE, x);
                    AddEdge(graph, x-1, x);
                }
                else
                {
                    AddEdge(graph, x-1, x);
                    AddEdge(graph, x-(SIZE+1), x);
                    AddEdge(graph, x-(SIZE-1), x);
                }
            }
            else
            {
                if(i == 0)
                {
                    AddEdge(graph, x-SIZE, x);
                    AddEdge(graph, x-(SIZE-1), x);
                }
                else if( i == SIZE - 1)
                {
                    AddEdge(graph, x + SIZE, x);
                }
            }
        }
    }
}

void LoadImages()
{
    hWhiteButton =  (HBITMAP)LoadImageW(NULL, L"WhiteButton.bmp", IMAGE_BITMAP, 30, 30, LR_LOADFROMFILE);
    hGreenButton =  (HBITMAP)LoadImageW(NULL, L"GreenButton.bmp", IMAGE_BITMAP, 30, 30, LR_LOADFROMFILE);
    hBlueButton =  (HBITMAP)LoadImageW(NULL, L"BlueButton.bmp", IMAGE_BITMAP, 30, 30, LR_LOADFROMFILE);
    hRedButton = (HBITMAP)LoadImageW(NULL, L"RedButton.bmp", IMAGE_BITMAP, 30, 30, LR_LOADFROMFILE);
}

struct Graph* createGraph(int size)
{
    struct Graph* graph = (struct Graph*)malloc(sizeof(struct Graph));
    graph->TotalNodes = size;
    graph->array = (struct AdjList*)malloc(graph->TotalNodes * sizeof(struct AdjList));
    int i;
    for(int i=0; i<graph->TotalNodes; i++)
    {
        graph->array[i].head = NULL;
    }
    return graph;
}

struct ListNode* createNode(int dest)
{
    struct ListNode* newNode = (struct ListNode*)malloc(sizeof(struct ListNode));
    newNode->data = dest;
    newNode->fcost = 0;
    newNode->hcost = 0;
    newNode->next = NULL;
    return newNode;
}

struct Array* createList(int size)
{
    struct Array* arr = (struct Array*)malloc(sizeof(struct Array));
    arr->size = size;
    arr->arr = (int*)malloc(sizeof(int) * size);
    arr->top = -1;
    return arr;
}

float Distance(float dest, float src)
{
    float dist = 0;
    if(dest - src > 0)
    {
        float x = (dest/SIZE - src/SIZE) * (dest/SIZE - src/SIZE);
        float y = (fmod(dest,SIZE) - fmod(src,SIZE)) * (fmod(dest,SIZE) - fmod(src,SIZE));
        dist = sqrt(x + y);
    }
    else
    {
        float x = (src/SIZE - dest/SIZE) * (src/SIZE - dest/SIZE);
        float y = (fmod(src,SIZE)- fmod(dest,SIZE)) * (fmod(src,SIZE)- fmod(dest,SIZE));
        dist = sqrt(x + y);
    }
    return dist * 100;
}

void AddEdge(struct Graph *graph, int src, int data)
{
    //putting directions from source to destination.
    struct ListNode* check = NULL;
    struct ListNode* NewNode = createNode(data);
    if(graph->array[src].head == NULL)
    {
        NewNode->next = graph->array[src].head;
        graph->array[src].head = NewNode;
    }
    else
    {
        check = graph->array[src].head;
        while(check->next != NULL)
        {
            check = check->next;
        }
        check->next = NewNode;
    }

    //putting directions from destination to source.

    NewNode = createNode(src);
    if(graph->array[data].head == NULL)
    {
        NewNode->next = graph->array[data].head;
        graph->array[data].head = NewNode;
    }
    else
    {
        check = graph->array[data].head;
        while(check->next != NULL)
        {
            check = check->next;
        }
        check->next = NewNode;
    }
}

void Neighbors(int Number, struct Graph* graph)
{
    struct ListNode* curr = graph->array[Number].head;
    while(curr != NULL)
    {
        SendMessageW(hButton[curr->data/SIZE][curr->data%SIZE], BM_SETIMAGE, IMAGE_BITMAP, (LPARAM)hRedButton);
        curr = curr->next;
    }
}

int CheckNode(struct ListNode* curr, struct Array* arr)
{
    for (int i = 0; i <= arr->top; i++)
    {
        if(arr->arr[i] == curr->data)
        {
            return 0;
        }
    }
    return 1;
}

int GetNearestNode(struct Array* arr, int dest, int src)
{
    int minNode = arr->arr[0];
    int f = (int)Distance(dest, minNode) + (int)Distance(minNode, src);
    int h = (int)Distance(dest, arr->arr[0]);
    for(int i=0; i<=arr->top; i++)
    {
        int currF = (int)Distance(dest, arr->arr[i]) + (int)Distance(arr->arr[i], src);
        if(f > currF)
        {
            f = currF;
            h = (int)Distance(dest, arr->arr[i]);
            minNode = arr->arr[i];
        }
        else if(f == currF)
        {
            int currH = (int)Distance(dest, arr->arr[i]);
            if(h > currH)
            {
                minNode =  arr->arr[i];
                h = currH;
            }
        }
    }
    return minNode;
}

void Shift(struct Array* arr, int start, int end)
{
    for(int i=start; i<end; i++)
    {
        arr->arr[i] = arr->arr[i+1];
    }
    arr->top--;
}

void RemoveNode(struct Array* arr, int data)
{
    for(int i=0; i<= arr->top; i++)
    {
        int x = arr->arr[i];
        if(x == data)
        {
            Shift(arr, i, arr->top);
            break;
        }
    }
}

void AStarAlgo(struct Graph* graph, int src, int dest)
{
    struct Array* open = createList(graph->TotalNodes);
    struct Array* closed = createList(graph->TotalNodes);
    struct ListNode* curr = graph->array[src].head;
    curr->fcost = (int)(Distance(dest, curr->data) + Distance(curr->data, src));
    curr->hcost = (int)Distance(dest, curr->data);
    struct ListNode* destN = graph->array[dest].head;
    while(curr != destN)
    {
        struct ListNode* adj = curr;
        while(adj != NULL)
        {
            if(CheckNode(adj, open) == 1 && CheckNode(adj, closed) == 1)
            {
                open->top++;
                open->arr[open->top] = adj->data;
                adj->Parent = curr->data;
            }
            adj = adj->next;
        }
        int min = GetNearestNode(open, dest, src);
        RemoveNode(open, min);
        curr = graph->array[min].head;
        closed->top++;
        closed->arr[closed->top] = min;
        if(src == min || dest == min)
        {
            SendMessageW(hButton[min/SIZE][min%SIZE], BM_SETIMAGE, IMAGE_BITMAP, (LPARAM)hGreenButton);
        }
        else
        {
            SendMessageW(hButton[min/SIZE][min%SIZE], BM_SETIMAGE, IMAGE_BITMAP, (LPARAM)hBlueButton);
        }
    }
    printf("Node Found!!");
}
