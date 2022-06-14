#include <windows.h>
#include <d3d9.h>
#include "d3dx9.h"

#pragma comment(lib,"d3d9.lib")
#pragma comment(lib,"d3dx9.lib")

#define WINDOW_LEFT		200				// 窗口位置
#define WINDOW_TOP		100				// 窗口位置
#define WINDOW_WIDTH	800				// 窗口宽度
#define WINDOW_HEIGHT	600				// 窗口高度
#define WINDOW_TITLE	 L"D3D游戏开发"	// 窗口标题
#define CLASS_NAME		L"D3D游戏开发"	// 窗口类名


// Direct3D设备指针对象
LPDIRECT3DDEVICE9 D3DDevice = NULL;

// 字体指针对象
LPD3DXFONT D3DFont = NULL;

bool startGame(HWND hwnd,HINSTANCE hInstance);

void endGame();

void Update(int type, WPARAM wParam);

void render(HWND hwnd);

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

    if (false == startGame(hwnd, hInstance)) return -1;

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
        else
        {
            render(hwnd);
        }
    }

    // 第六步：注销窗口
    UnregisterClass(CLASS_NAME, wndClass.hInstance);
    return 0;


}


bool startGame(HWND hwnd, HINSTANCE hInstance)
{
    LPDIRECT3D9 D3D9 = NULL;
    if (NULL == (D3D9 = Direct3DCreate9(D3D_SDK_VERSION))) return false;
    int vp = 0;
    D3DCAPS9 deviceCaps;

    if (FAILED(D3D9->GetDeviceCaps(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, &deviceCaps)))
    {
        return false;
    }
    if (deviceCaps.DevCaps & D3DDEVCAPS_HWTRANSFORMANDLIGHT)
        vp = D3DCREATE_HARDWARE_VERTEXPROCESSING;
    else
        vp = D3DCREATE_SOFTWARE_VERTEXPROCESSING;
    // 第三步：构造D3DPRESENT_PARAMETERS结构体
    D3DPRESENT_PARAMETERS D3DParameters;
    ZeroMemory(&D3DParameters, sizeof(D3DParameters));
    D3DParameters.BackBufferWidth = WINDOW_WIDTH;		// 指定后台缓冲区的宽度，窗口的宽度
    D3DParameters.BackBufferHeight = WINDOW_HEIGHT;		// 指定后台缓冲区的高度，窗口的高度
    D3DParameters.BackBufferFormat = D3DFMT_UNKNOWN;	// 指定后台缓冲区的像素格式
    D3DParameters.BackBufferCount = 1;					// 指定后台缓冲区的数量
    D3DParameters.MultiSampleType = D3DMULTISAMPLE_NONE;// 指定多重采样的类型
    D3DParameters.MultiSampleQuality = 0;				// 指定多重采样的格式
    D3DParameters.SwapEffect = D3DSWAPEFFECT_DISCARD;	// 指定Direct3D如何将后台缓冲区内容复制到前台的缓存中
    D3DParameters.hDeviceWindow = hwnd;					// 指定当前窗口句柄
    D3DParameters.Windowed = true;						// 指定窗口模式
    D3DParameters.EnableAutoDepthStencil = true;		// 是否开启深度缓存和模板缓存。
    D3DParameters.AutoDepthStencilFormat = D3DFMT_D24S8;// 指定深度缓冲及模板缓冲区的格式
    D3DParameters.Flags = 0;							// 表示锁定后备缓冲区
    D3DParameters.FullScreen_RefreshRateInHz = 0;		// 指定屏幕刷新率
    D3DParameters.PresentationInterval = D3DPRESENT_INTERVAL_DEFAULT;	// 指定后备缓冲区与前台缓冲区的交换频率
    

// 第四步：创建Direct3D设备指针对象
    if (FAILED(D3D9->CreateDevice(
        D3DADAPTER_DEFAULT,			// 指定将要使用的显卡适配器的标识号
        D3DDEVTYPE_HAL,				// 指定设备的类型
        NULL,						// 指定焦点窗口
        vp,							// 表示设备行为标示
        &D3DParameters,				// 指定到一个D3DPRESENT_PARAMETERS的结构体
        &D3DDevice)))				// 返回Direct3D设备指针对象
        return false;

    // 第五步：释放Direct3D接口对象
    D3D9->Release();
    D3D9 = NULL;


    // 创建一个字体对象
    D3DXCreateFont(D3DDevice, 24, 0, 1, 1, false, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, 0, L"微软雅黑", &D3DFont);
    return true;

}

void endGame()
{
}

void Update(int type, WPARAM wParam)
{
}

void render(HWND hwnd)
{

    // 第一步：清屏操作
    D3DDevice->Clear(
        0,							// 指定清除矩形的数量，一般设置0即可
        NULL,						// 指定清除矩形，设置为NULL表示视口所有矩形都要清除
        D3DCLEAR_STENCIL | D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, // 指定清除的缓冲区，包括模板/颜色/深度三种缓冲区，用|连接
        D3DCOLOR_XRGB(255, 255, 255),// 指定清除颜色缓冲区的颜色值
        1.0f,						// 指定清除深度缓冲区后的深度值，此参数取值从0.0到 1.0
        0);							// 指定清除模板缓冲区后的模板值，取默认值0即可

    // 第二步：开始绘制
    D3DDevice->BeginScene();

    // 第三步：绘制内容
    // 第三步：绘制文字
    RECT rect;
    GetClientRect(hwnd, &rect);
    D3DFont->DrawText(0, L"Hello，Direct3D游戏开发！", -1, &rect, DT_CENTER, D3DCOLOR_XRGB(255, 0, 0));
   
    // 第四步：结束绘制
    D3DDevice->EndScene();

    // 第五步：显示翻转
    D3DDevice->Present(
        NULL,		// 表示后台缓冲区，NULL值是全部的后台缓存矩形表面
        NULL,		// 表示前台缓冲区，NULL值指定整个前台缓存表面
        NULL,		// 表示窗口的句柄，NULL表示当前窗口
        NULL);		// 指定需要拷贝的后台缓存表面的大小和数据，一般是NULL
    
}

// 定义窗口过程函数
LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam) {
    switch (message)
    {
    case WM_PAINT:
        render(hwnd);
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
