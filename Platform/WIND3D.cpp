#include <windows.h>


#define WINDOW_LEFT		200				// 窗口位置
#define WINDOW_TOP		100				// 窗口位置
#define WINDOW_WIDTH	800				// 窗口宽度
#define WINDOW_HEIGHT	600				// 窗口高度
#define WINDOW_TITLE	 L"D3D游戏开发"	// 窗口标题
#define CLASS_NAME		L"D3D游戏开发"	// 窗口类名

// 声明窗口过程函数
LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);

int WINAPI WinMain(
    HINSTANCE  hInstance, HINSTANCE  hPrevInstance, LPSTR  lpCmdLine, int  nCmdShow)
{


    // 第一步，构造 WNDCLASSEX 结构体
    WNDCLASSEX wndClass = { 0 };
    wndClass.cbSize = sizeof(WNDCLASSEX);		                // 设置结构体的字节数大小
    wndClass.style = CS_HREDRAW | CS_VREDRAW;	                // 设置窗口的风格样式
    wndClass.lpfnWndProc = WndProc;				                // 设置窗口过程函数的指针
    wndClass.cbClsExtra = 0;						            // 设置窗口类的附加内存
    wndClass.cbWndExtra = 0;						            // 设置窗口的附加内存
    wndClass.hInstance = hInstance;					            // 设置程序实例句柄
    wndClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);			// 设置图标句柄
    wndClass.hCursor = LoadCursor(NULL, IDC_ARROW);				// 设置光标句柄
    wndClass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);// 设置背景画刷句柄
    wndClass.lpszMenuName = NULL;								// 设置菜单名称
    wndClass.lpszClassName = CLASS_NAME;						// 设置窗口类名称
    wndClass.hIconSm = NULL;

    if (!RegisterClassEx(&wndClass)) return -1;

    // 第三步：创建窗口
    HWND hwnd = CreateWindow(
        CLASS_NAME,				    // 窗口类名称
        WINDOW_TITLE,				// 窗口标题
        WS_OVERLAPPEDWINDOW,		// 窗口风格
        CW_USEDEFAULT,				// 窗口初始 x 坐标
        CW_USEDEFAULT,				// 窗口初始 y 坐标
        WINDOW_WIDTH,				// 窗口初始宽度
        WINDOW_HEIGHT,			    // 窗口初始高度
        NULL,						// 父窗口句柄
        NULL,						// 窗口菜单句柄
        hInstance,					// 程序实例句柄
        NULL);						// 附加参数

    // 第四步：显示窗口
    MoveWindow(hwnd, WINDOW_LEFT, WINDOW_TOP, WINDOW_WIDTH, WINDOW_HEIGHT, true);
    ShowWindow(hwnd, nCmdShow);
    UpdateWindow(hwnd);

    // 第五步：消息循环过程
    MSG msg = { 0 }; // 定义一个消息对象(msg)
    // 使用while循环消息队列，如果消息不是WM_QUIT消息，就继续循环
    while (msg.message != WM_QUIT)
    {
        // 获取消息并交给窗口过程函数
        if (PeekMessage(&msg, 0, 0, 0, PM_REMOVE)) {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    // 第六步：注销窗口
    UnregisterClass(CLASS_NAME, wndClass.hInstance);
    return 0;


}

// 定义窗口过程函数
LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam) {
    switch (message)
    {
    case WM_PAINT:
        ValidateRect(hwnd, NULL);	// 窗口重绘消息
        break;
    case WM_DESTROY:
        PostQuitMessage(0);			// 窗口销毁消息
        break;
    default:						// 调用缺省的窗口过程
        return DefWindowProc(hwnd, message, wParam, lParam);
    }
    return 0;
}
