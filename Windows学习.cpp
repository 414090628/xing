// Windows学习.cpp : 定义应用程序的入口点。
//

#include "framework.h"
#include "Windows学习.h"

/*
 特性:
	1. 遇到不懂的,没有讲过的类型, 转定义看看
	2. H开头, 在windows中一般是句柄的标识
	3. P, LP开头, 在windows中一般是指针的标识
	4. windows里面的类型都是大写

  只有 exe 是可执行文件
   png, txt, jpg, c, cpp 都是资源文件
*/

#define MAX_LOADSTRING 100

/*
 win32的框架
	1. 入口函数			WinMain(){}	函数
	2. 窗口注册类信息	WNDCLASSEXW 类型
	3. 窗口创建			HWND		类型
	4. 显示窗口			ShowWindow(hWnd, nCmdShow);	函数
	5. 更新窗口			UpdateWindow(hWnd);			函数
	6. 信息循环			MSG 类型
	7. 入口函数结束
 */

// 全局变量:
HINSTANCE hInst;                                // 当前实例
//HINSTANCE		实例句柄类型
WCHAR szTitle[MAX_LOADSTRING];                  // 标题栏文本
WCHAR szWindowClass[MAX_LOADSTRING];            // 主窗口类名
HWND g_hWnd;
// 此代码模块中包含的函数的前向声明:
ATOM                MyRegisterClass(HINSTANCE hInstance);//注册窗口类
BOOL                InitInstance(HINSTANCE, int);		 //保存实例句柄并创建主窗口
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM); //处理主窗口的消息
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);	 //“关于”框的消息处理程序
INT_PTR CALLBACK    AboutS(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);
INT_PTR CALLBACK	About_S(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);
void				createControl(HWND hWndParent);
//菜单, 从编程的角度来分类: 静态菜单(初始化改变), 动态菜单(程序运行中改变), 快捷菜单
//静态菜单: 在菜单资源编辑器预先编辑好自己加载
//动态菜单: 在程序运行的过程中通过代码生成
//快捷菜单: 是前两种菜单的组合, 在菜单资源编辑器预先编辑好, 然后在程序运行的过程中动态显示
//
//对于菜单而言, 可以理解为一个二维数组
//菜单里面的每一个元素叫菜单项, 菜单项包括两个最基本的要素: 1. 菜单项名; 2. 该菜单的唯一标识ID
//每一个元素可以是嵌套的子菜单数组
//
//右键快捷菜单
//全局变量
HMENU g_hRMenu;

int g_HeroX = 0, g_HeroY = 0;
int g_HeroDir = 0;

//GDI 图形设备接口, 在窗口上输出图形和图像
//GDI 函数的分类
//1. 取得和释放 设备上下文(设备环境) 的函数
//2. 取设备内容消息的函数
//3. 绘图函数
//4. 设定和取得设备上下文参数的函数
//5. 调用GDI对象的函数
//
//设备上下文: 设备环境( DC ), 包含有关某个设备的绘制属性的消息的windows数据结构
// 4类
// 1. 直线和曲线;  2. 填充区域;  3. 文字;  4. 位图;
//
//在窗口绘制图形. 图像, 4个步骤
//1. 得到设备环境句柄
//2. 修改设备属性
//3. 绘图
//3. 释放设备环境
//
/*在windows里面得到设备环境句柄有两种情况
	1. 在WM_PAINT消息里面去得到设备环境句柄
		//什么情况下窗口响应PAINT消息: 1. 窗口最初创建; 2. 窗口出现无效区域
				//窗口出现无效区域的4中情况: 1. 窗口位置或大小改变	
											 2. 窗口隐藏之后重新显示或被其它窗口遮掩部分重新可见
											 3. 调用InvalidateRect()和InvalidateRgn()函数, 使窗口主动出现无效区域
													Invalidate 失效
											 4. 调用ScrollDC()和ScrollWindows()函数, 滚动客户区或窗口, 出现无效区域
	
	2. 在WM_PAINT消息之外得到设备环境句柄
		1. 通过GetDC()得到设备环境句柄			格式: HDC hdc = GetDC(当前窗口句柄);
		2. 没有设置第二步也可以画GDI的对象, DC里面默认设置了一只黑色的画笔, 用来画线
				但是要注意dc里面要有默认的数据
			修改:定义一只画笔		HPEN hpen = nullptr;  画笔句柄

			得到系统预设的画笔,改它颜色
									hpen = (HPEN)GetStockObject(WHITE_PEN);	
			预设画笔不满足需求, 创建一个画笔
									hpen = CreatePen(		Create 创建  Pen 笔
											PS_DASHDOT,		画笔的风格(实线, 虚线之类)
											1,				笔的宽度
											RGB(255,0,0));	
	PS_SOLID 实线	PS_DASHDOT 虚线
			给窗口句柄关联画笔		SelectObject(hdc, hpen); 给hdc关联一个新的画笔对象
			A(透明) R(红色) G(绿色) B(蓝色)
			电脑中的三基色: R(红色) G(绿色) B(蓝色)


	3. 绘图	画线(HPEN)	HPEN hpen = nullpte;
				如	LineTo(hdc, x, y);				画直线函数 
					MoveToEx(hdc, x, y, nullptr)	移动线的起始位置函数
					Arc(hdc,		//画圆弧
						x, y,		//圆的外接矩形左上角
						x, y,		//圆的外接矩形右下角
						x, y,		//圆弧的起点
						x, y)		//圆弧的终点

			画点	SetPixel(	//画点
						hdc,	//设备环境句柄
						100+i,  //坐标x轴
						100,	//坐标y轴
						RGB(0,0,255));	//颜色RGB(红, 绿, 蓝)
			获得当前坐标点的RGB
					GetPixel(hdc, 100 + i, 100));
			可以和画点函数搭配 
					SetPixel(hdc, 100+i, 400, GetPixel(hdc, 100 + i, 100));

			画刷(HBRUSH)	HBRUSH hbrush = nullptr;
				hbrush = CreateSolidBrush(RGB(0,0,0));	//创建一个纯色的画刷
				hbrush = CreateHatchBrush(		//创建一个风格画刷
							HS_BDIAGONAL,		//画刷风格
							RGB(0,0,0));		//颜色
					SelectObject(hdc, hbrush);	//修改hdc的画刷
					Rectangle(hdc, 500, 500, 800, 800);	//用画刷填充矩形

				hbrush = Create
		在绘图中一般不用画刷绘图
					Pie(		//画饼图
						hdc,	//
						x, y,	//圆的外接矩形左上角
						x, y,	//圆的外接矩形右下角
						x, y,	//饼弧的起点
						x, y)	//饼弧的终点

					Chord(		//画弦弧
						hdc,	//
						x, y,	//圆的外接矩形左上角
						x, y,	//圆的外接矩形右下角
						x, y,	//饼弧的起点
						x, y)	//饼弧的终点
					
			字体句柄(HFONT)	HFONT hfont = nullptr;
				hfont = CreateFont(		//创建字体
						字体的高度,
						字体的宽度,		//为0会自动匹配高度的比例
						字体的倾斜角度, //以头字为中心点, 逆时针旋转
						基线的倾斜角度,	//比如3d三维中的x,y,z 中的z轴
						字体的粗细,		//400以下为细体, 700以上为粗体
						是否为斜体,		//bool类型
						是否有下划线,   //bool类型
						是否有水平线,	//bool类型
						字符集,			//GB2312_CHARSET 中文字符集, ANST_CHARSET 英文字符集
						输出的精度,		//不管 为 0
						采取的精度,		//不管 为 0
						输出的质量,		//不管 为 0
						不知道,			//不管 为 0
						字样名);		//比如 楷体 之类的 与字符集搭配, 字样名要在字符集中有的
				锚点 默认左上角, 作用是改变对齐方式
					SetTextAlign(hdc, TA_RIGHT | TA_TOP);	//对齐方式

				SetTextColor(hdc, RGB(0,0,0));	//设置文本的前景色
				SetBkColor(hdc, RGB(0,0,0));	//设置文本的背景色
				SetBkMode(hdc, TRANSPARENT);	//设置文本背景的模式
				TRANSPARENT  用挡住的背景色来填充背景, 效果:背景透明,  
				OPAQUE		 默认, 用自身的背景色来填充

				TextOut(hdc, x, y, 文本, 文本占几字节);
				
			//位图, GDI中只支持位图(.bmp格式图片)
			//	1.	位图无法预设
			//		 HDC hdc = GetDC(hWnd);	//创建一个设备环境句柄
			//	2.	创建一个兼容DC
			//		 HDC bitDc = CreateCompatibleDC(hdc);	//根据hdc创建一个兼容的bitDc
			//		创建一个HBITMAP加载资源中的图片
			//		 HBITMAP hBitmap = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_BITMAP1));
			//	3. 绘图
			//		绘图函数
			//			BitBlt(hdc, 0,0,1024,768,bitDc,0,0,SRCCOPY);	//
			//	4. 释放
			//		ReleaseDC(hWnd, hdc);
			//		DeleteDC(bitDc);
			//		DeleteObject(hBitmap);

	4. 通过ReleaseDC()释放设备环境句柄		格式: ReleaseDC(当前窗口句柄, hdc);
			如果有关联新的画笔要有释放		格式: DeleteObject(关联的画笔句柄);	释放画笔句柄
			如果有关联新的画笔要有释放		格式: DeleteObject(关联的画刷句柄);
		GDI中, 同一时刻只能使用同一类的一个GDI对象
*/


/*
GDI是输出窗体的画面, 控件是输入

控件 用户输入(缩小用户的输入范围或者提示用户的操作)
	控件可以在窗口上显示, 但是一般不会在窗口上进行创建控件
		因为窗口是用来输出的, 控件是输入, 这两者不适合放在一起
	需要对话框来进行承载

对话框 是一个特殊的窗口, 主要用于用户的输入, 布局控件
对话框分为两类:
	模态对话框: 阻塞(同步)模式, 只有操作完当前对话框,才可以进行其它操作
	非模态对话框: 非阻塞(异步)模式,可以同时操作当前对话框,与其它操作

对话框的创建步骤:
 1. 添加对话框资源
 2. 准备一个专门对对话框的消息处理函数	
 3. 调用一个函数来创建一个对话框; 
		模态:		DialogBox(); 
		非模态:		CreateWindow(),非模态窗口创建完要用ShowWindow()来显示
 4. 释放对话框资源
		模态:		EndDialog();
		非模态:		DestroyWindow();

*/

/*
进程	通常被定义为一个正常运行的程序的实例
	组成:	操作系统用来管理进程的内核对象
			地址空间(所有的可执行模块, 所需要的数据, dll模块)

线程	线程是进程的一部分, 总是在进程的环境中被创建, 它是进程的某一个执行序列(函数)
	组成:	操作系统对线程实施管理的内核对象: 线程堆栈
	进程活动性不强, 只表示程序的所有权, 
	程序的工作都是由线程来完成的, 一个任务对应着一个线程, 多任务就是多线程

量程	操作系统为每一个线程安排了一定的cpu时间, 通过一种循环的方式为线程提供时间片

*/


//WinMain 是 win32窗体 的入口函数
int APIENTRY wWinMain(_In_ HINSTANCE hInstance,			//当前实例句柄
                     _In_opt_ HINSTANCE hPrevInstance,	//当前实例的父实例
                     _In_ LPWSTR    lpCmdLine,			//命令行
                     _In_ int       nCmdShow){			//窗口的显示方式
#if 0
	//字符集  vs中有两种字符集, 多字节字符集, 宽字节字符集
	//多字节
	char	c = 'a';			//char	表示多字节字符集里面的字符类型
	const char* pc = "ab中cd"	//多字节字符串的描述
	//pc = &c;
	
	
	//宽字节
	wchar_t w = 'a';		//wchar_t	表示宽字节字符型里面的字符类型
	const wchar_t* pw = &w;
	
	pc = "abc";				//多字节字符串的赋值
	pw = L"abc中";			//宽字节字符串的赋值

	int clen = strlen(pc);	//求多字节字符串的长度
	int wlen = wcslen(pw);	//求宽字符字符串的长度

	//在项目中用使用通配的字符类型
	const _TCHAR* str = _T("abc");	//_T()带参宏
	int strL = _tcslen(str);		//通配字符类型求大小
#endif

	//表示警告等级
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: 在此处放置代码。

    // 初始化全局字符串
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_WINDOWS, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);		//自定义的注册类函数

    // 执行应用程序初始化:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

	//定义句柄变量, 并初始化
    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_WINDOWS));
	//LoadAccelerators(实例句柄, 快捷键), 加载加速器(快捷键)操作
	//Load 加载 Accelerators 加速器

	HDC hdc = GetDC(g_hWnd);

	HDC hBuffDc = CreateCompatibleDC(hdc);
	HBITMAP hBuffBitmap = CreateCompatibleBitmap(hdc, 1024, 768); //创建一个声明图形都没有的兼容
	SelectObject(hBuffDc, hBuffBitmap);

	HDC hBkDc = CreateCompatibleDC(hdc);
	HBITMAP hBkBitmap = (HBITMAP)LoadImage(hInst, _T("bk.bmp"), IMAGE_BITMAP,900,900,LR_LOADFROMFILE);
	SelectObject(hBkDc, hBkBitmap);
	
	HDC hHeroDc = CreateCompatibleDC(hdc);
	HBITMAP hHeroBitmap = (HBITMAP)LoadImage(hInst, _T("hero.bmp"), IMAGE_BITMAP,0,0,LR_LOADFROMFILE);
	SelectObject(hHeroDc, hHeroBitmap);
	
	ReleaseDC(g_hWnd, hdc);
/*
windows操作系统最大的特色是良好的用户交互性

消息机制: 产生消息, 传递消息并处理消息的过程
	1. 产生消息, 
		1). 不可控.因为windows是不开源的, 只能遵从windows的产生消息的过程
	2. 传递消息
		windows操作系统为每一个正在运行的应用程序维护一个消息队列
		1). 系统传递消息给应用程序的过程, 不可控的
		2). 应用程序给应用程序传递消息, 可控

	3. 处理消息(应用程序), 把消息当成条件, 怎么拿到这个条件
		1). 系统传递消息给应用程序, 是不可控的也就是说不可处理
		2). 而应用程序给应用程序传递消息, 是可控的, 可处理
*/

//消息结构变量
    MSG msg;
/*
消息结构体
	typedef struct tagMSG { 
    HWND        hwnd;		//窗口句柄(哪个窗口产生的消息), 句柄是个指针
    UINT        message;	//消息ID(是什么消息: 键盘消息丶鼠标消息等消息), 同一类消息共用一个ID
    WPARAM      wParam;		//消息的辅助参数	
    LPARAM      lParam;		//消息的辅助参数
    DWORD       time;		//消息的产生时间
    POINT       pt;			//消息产生时鼠标的坐标
	} MSG, *PMSG, NEAR *NPMSG, FAR *LPMSG;

GetMessage() 从消息队列中取出消息函数, 会去除消息队列中的消息
Get 获取 Message 消息
	GetMessage(
		_Out_ LPMSG lpMsg,		从消息队列中获取消息
		_In_opt_ HWND hWnd,		区分窗口消息
		_In_ UINT wMsgFilterMin, 过滤消息范围的开头ID
		_In_ UINT wMsgFilterMax  过滤消息范围的结尾ID
	);

	WM_QUIT 退出键宏定义
*/
	// 主消息循环:
//#if 1
  #if 0
	while (GetMessage(	//如果在消息队列中没有消息, GetMessage会一直等待, 不再往下执行,
			&msg,		//从消息队列中取出 的 消息 的 放置位
			nullptr,	//为空表示但凡是这个窗口的消息都获取
			0,			//消息过滤范围的开头ID, 0表示不过滤
			0))			//消息过滤范围的结尾ID, 0表示不过滤
    {	// 得到消息会立即处理, 如是退出消息返回false, 非退出消息返回true
		//Translate 处理翻译
		//TranslateAccelerator() 翻译快捷键, 如果是加速器消息返回true, 不是返回false
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))//取反, 是 不执行, 不是 执行
        {
            TranslateMessage(&msg);	//翻译消息, 主要做键消息的翻译
            DispatchMessage(&msg);	//投递消息, 把消息传递到回调函数内
        }
    
#else
	//PeekMessage()是从消息队列中拷贝一份, 不去除消息队列中的消息
	//不会等待获取消息, 在没有消息时也会往下执行
	/*  变量使用时要初始化, 
			GetMesage()会等待消息赋值, 
			但PeekMessage()不会等待, 这样在没有消息时PeekMessage()会未初始化
			*/
	//初始化消息变量
	ZeroMemory(&msg, sizeof(msg));		//内存清零函数
	//memset();		内存逐字节赋值
	while (msg.message != WM_QUIT)
	{
		if (PeekMessage(
			&msg,			//从消息队列中取出 的 消息 的 放置位
			nullptr,		//为空表示但凡是这个窗口的消息都获取
			0,				//消息过滤范围的开头ID, 0表示不过滤
			0,				//消息过滤范围的结尾ID, 0表示不过滤
			PM_REMOVE))		//是否移除消息队列中的消息. 一般移除, PM_REMOVE(移除)
		{
			if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))//取反, 是 不执行, 不是 执行
			{
				TranslateMessage(&msg);	//翻译消息, 主要做键消息的翻译
				DispatchMessage(&msg);	//投递消息, 把消息传递到回调函数内
			}
		}
#endif
		//不会堵塞 可以不断的进行 游戏的画面更新 和 数据更新
		static int index = 0;
		static float beginTimer = timeGetTime() / 1000.0f;
		float endTimer = timeGetTime() / 1000.0f;
		static float speed = 0.5f;

		hdc = GetDC(g_hWnd);
		//BitBlt(hdc, 0, 0, 700, 700, hBuffDc, 0, 0, SRCCOPY);
		BitBlt(hBuffDc, 0,0, 700, 700, hBkDc ,0,0, SRCCOPY);
		TransparentBlt(hBuffDc, g_HeroX,g_HeroY,90,88,hHeroDc, 90*index,88* g_HeroDir,90,88,RGB(255,255,255));
		if (endTimer - beginTimer >= speed)
		{
			++index;
			if (index > 3)
				index = 0;
			beginTimer = endTimer;
		}
		//BitBlt(hdc, 0, 0, 700, 700, hBuffDc, 0, 0, SRCCOPY);
		ReleaseDC(g_hWnd, hdc);
	}

	DeleteDC(hBkDc);
	DeleteObject(hBkBitmap);
	DeleteDC(hHeroDc);
	DeleteObject(hHeroBitmap);
	DeleteDC(hBuffDc);
	DeleteObject(hBuffBitmap);

    return (int) msg.wParam;
}

//
//  函数: MyRegisterClass()
//
//  目标: 注册窗口类。
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
	//窗口类的结构
    WNDCLASSEXW wcex;	
	//在末尾加 W 表示宽字节, 加 A 表示多字节, 加 EX 是扩展的意思
	//EXW 表示扩展宽字节

    wcex.cbSize = sizeof(WNDCLASSEX);	//结构占内存大小
	//在windows的结构中, 有cbSize就要给出sizeof(WNDCLASSEX), 为了更好的操作

	//类风格	CS_ 类风格的开头字母
    wcex.style          = CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS;
	//CS_HREDRAW 水平刷新		CS_VREDRAW 垂直刷新
	//CS_DBLCLKS 窗口响应鼠标双击事件
    wcex.lpfnWndProc    = WndProc;	//回调函数(设置回调函数, 并不直接调用)
    //wcex.lpfnWndProc=WndProc;	是告诉程序 事件消息 由 WndPreoc 来处理
	wcex.cbClsExtra     = 0;	//类的额外信息
    wcex.cbWndExtra     = 0;	//窗口的额外信息
    wcex.hInstance      = hInstance;	//窗口的实例句柄
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_WINDOWS));//应用程序的图标, 任务栏的图标
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);	//光标
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);			//窗体的背景颜色
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_WINDOWS);	//窗体的菜单
	//在菜单类型中 VIRTKEY 是 虚拟类型
    wcex.lpszClassName  = szWindowClass;	//注册窗口类名
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));	//窗体的标题图标
	//Load 加载	lcon 图标

    return RegisterClassExW(&wcex);		//系统函数, 返回一个系统的注册窗口类的函数调用
	//类的结构中所做的工作都是通过 RegisterClassExW 写入注册表中
	//只有在注册表中有这些数据, 才能在窗体中做实例化操作
}

//
//   函数: InitInstance(HINSTANCE, int)
//
//   目标: 保存实例句柄并创建主窗口
//
//   注释:
//
//        在此函数中，我们在全局变量中保存实例句柄并
//        创建和显示主程序窗口。
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // 将实例句柄存储在全局变量中

#if 1
//代码更改静态菜单
   //接收自定义菜单
   HMENU H1 = LoadMenu(hInst, MAKEINTRESOURCE(IDR_MENU1));
   //定义窗口句柄, 并创建窗口	HWND 句柄类型	hWnd 句柄类型变量
   HWND hWnd = CreateWindowW(	//Create  创建  Window 窗口  W 宽字节
	   szWindowClass,		//窗口类名, 与注册时保持一致
	   szTitle,				//窗口标题名
	   WS_OVERLAPPED | WS_SYSMENU | WS_MINIMIZEBOX,	//窗口风格
	   //CS 是类风格, WS 是窗口风格
	   CW_USEDEFAULT, 0,	//窗口的左上角x, y坐标
	   700, 700,	//窗口的高宽
	   //当x坐标给的是CW_USEDEFAULT, y坐标给0表示去匹配x坐标(单位: 像素点)
	   //CW_USEDEFAULT 给的是默认值, 是根据屏幕比来缩进窗口的打开位
	   nullptr,		//父窗口句柄
	   /*H1*/nullptr,		//菜单句柄, 如果值为 空, 表示默认使用注册时加载的菜单, 否则
	   hInstance,	//窗口实例句柄
	   nullptr);	//保留参数
#else
//动态菜单
   HMENU H2 = CreateMenu();	//创建一个空菜单
   HMENU H3 = CreateMenu();	//创建一个空菜单
   //尾部追加菜单函数
   AppendMenu(H3, 0, 10081, _T("新建(&N)"));
   AppendMenu(H3, 0, 10082, _T("打开(&O)"));
   AppendMenu(H3, MF_SEPARATOR, 0, _T("-"));
   // MF_SEPARATOR  分隔符风格, 分隔符不需要ID 为0
   AppendMenu(H3, 0, 10083, _T("保存(&M)\tCtrl+M"));
   AppendMenu(	//App 追加 end 尾部 Menu 菜单
	   H2,		//哪个菜单句柄
	   MF_POPUP,		//菜单风格, MF_POPUP 弹出式风格
	   (UINT_PTR)H3,	//创建菜单项的标识 ID
	   _T("文件(&F)"));	//菜单项名称
   //菜单是二维数组的方式进行存显
   //一级菜单不需要ID, 可以放置二级菜单的句柄, 因为参数是无符号int类型, 句柄要强转
//创建的菜单注册到窗口
   HWND hWnd = CreateWindowW(szWindowClass,szTitle,WS_OVERLAPPED | WS_SYSMENU | WS_MINIMIZEBOX,CW_USEDEFAULT, 0,700, 700, nullptr, 0,hInstance,nullptr);
   
   //菜单项的插入操作
   HMENU H4 = CreateMenu();	//创建一个空菜单

   InsertMenu(	 //菜单项的插入函数, Insert	插入  Menu 菜单
	   H4,	//哪个菜单
	   0,	//插入的方式(ID, 下标), 受第三个参数的控制, 当前是下标0的位置
	   MF_BYPOSITION,	//插入的方式风格	MF_BYPOSITION 以下标位插入
	   10091,	//创建菜单项的标识ID
	   _T("撤销(&Z)"));	//菜单项名
   InsertMenu(H4, 
	   10091,	//以ID的方式插入, 当前是ID 10091 的位置前的位置
	   MF_BYCOMMAND,//MF_BYCOMMAND 以ID的方式进行插入, 位置在此ID前一个位置
	   10092, _T("剪贴(&X)"));
   InsertMenu(H4, 1, MF_BYPOSITION, 10093, _T("复制(&C)")); //在下标1的位置进行插入
  
#if 1	//在尾部追加一级菜单
   AppendMenu(H2, MF_POPUP, (UINT_PTR)H4, _T("编辑(&E)"));
#else	//一级菜单的插入
   InsertMenu(H2, 0, MF_POPUP | MF_BYPOSITION, (UINT_PTR)H4, _T("编辑(&E)"));
#endif
   //菜单项的显示方式(激活, 未激活(变灰)之类的操作)
   EnableMenuItem(H2, 10083, MF_GRAYED);	//MF_GRAYED	  未激活(变灰,不可选)
   EnableMenuItem(H2, 10083, MF_ENABLED);	//MF_ENABLED  激活(可选)

   //当记不住Id号时, 可以通过一级菜单项的下标位来进行操作
   EnableMenuItem(H3, 0, MF_GRAYED | MF_BYPOSITION);

 //修改菜单项
   //以下标的方式
   ModifyMenu(H4, 1, MF_BYPOSITION, 20081, _T("aaa"));
   //以标识ID的方式
   ModifyMenu(H4, 0, MF_BYPOSITION, 20082, _T("bbb"));

//DeleteMenu();删除菜单函数, 但因为不删除加速器, 所以一般使用 变灰 操作, 不使用删除

//在UpdateWindow()函数后的菜单添加要 加上 SetMenu()更新菜单函数 刷新一下菜单
   SetMenu(hWnd, H2);
#endif
   if (!hWnd)
   {
      return FALSE;
   }
   g_hWnd = hWnd;
   //ShowCursor(false);	//显示光标函数, false不显示
   //Show 显示	Cursor 光标
   ShowWindow(hWnd, nCmdShow);	//显示窗口
   //Show 显示	Window 窗口
   UpdateWindow(hWnd);			//更新窗口

   return TRUE;
}

//
//  函数: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  目标: 处理主窗口的消息。
//
//  WM_COMMAND  - 处理应用程序菜单
//  WM_PAINT    - 绘制主窗口
//  WM_DESTROY  - 发送退出消息并返回
//
//
short x_qiDian, y_qiDian, isDian = false;
TCHAR strArr[1023] = {0};
int index = 0;
static int in = 2;
int g_hInst;
LRESULT CALLBACK WndProc(
	HWND hWnd,			//消息的窗口句柄
	UINT message,		//消息的类型(鼠标丶键盘等)
	WPARAM wParam,		//消息的辅助消息
	LPARAM lParam)		//消息的辅助消息, 保留坐标消息
{
	HDC hdc;
	HWND g_hDlgWnd;

	switch (message)
	{
//鼠标消息
	case WM_LBUTTONUP:
		isDian = false;
		break;
	//L 左 button 按键 down 按下
	case WM_LBUTTONDOWN:	//鼠标左键点击消息的宏定义
	{
		//MessageBox(0, _T("鼠标左键点击"), _T("鼠标事件"), 0);
#if 0
		SendMessage(	//传递消息函数, 投递消息到消息队列的 头部
			hWnd,		//接收消息的窗口
			WM_KEYDOWN,	//发送的消息ID
			0, 0);		//消息的辅助参数
#elif 0
		PostMessage(	//传递消息函数, 投递消息到消息队列的 尾部
			hWnd,		//接收消息的窗口
			WM_KEYDOWN,	//发送的消息ID
			0, 0);		//消息的辅助参数
#endif
		//isDian = true;
		//x_qiDian = LOWORD(lParam);
		//y_qiDian = HIWORD(lParam);
		//hdc = GetDC(hWnd);
		//TextOut(
		//	hdc,
		//	0, 0,				//开始位与结束位
		//	_T("鼠标左键点击"),	//输出文本
		//	6);					//输出文本的大小
		//ReleaseDC(hWnd, hdc);

		//进程的创建
		//STARTUPINFO si;
		//ZeroMemory(&si, sizeof(si));
		//si.cb = sizeof(si);
		//PROCESS_INFORMATION pi;
		//CreateProcess(	//创建进程函数
		//	_T("C:\\Windows/notepad.exe"),	//可执行的进程路径
		//	nullptr,	//命令行参数
		//	nullptr,	//是否被子进程所继承
		//	nullptr,	//是否被子线程所继承
		//	false,		//新创建的进程是否从调用线程处所继承了句柄
		//	0,			//创建标志
		//	nullptr,	//新进程的环境块
		//	nullptr,	//子进程的工作路径
		//	&si,
		//	&pi);
	}
	break;
	case WM_RBUTTONDOWN:
	{
		POINT pt{ LOWORD(lParam),HIWORD(lParam) };
		//需要客户区的坐标在屏幕坐标的哪
		ClientToScreen(hWnd, &pt);
		//把菜单的一级菜单中的单个拿出来
		HMENU h1 = GetSubMenu(g_hRMenu, 1);
		//弹出右键菜单
		//TrackPopupMenu(		//把客户区坐标转为屏幕坐标
		//	h1,		//弹出哪个菜单
		//	TPM_LEFTALIGN/*右显示*/ | TPM_TOPALIGN, //菜单风格
		//	pt.x, pt.y,	//弹出的坐标
		//	0,		//
		//	hWnd,	//哪个窗口
		//	nullptr); //在哪个区域显示

		//进程的结束方法
		//1. 主线程的入口函数返回(建议最好用这种)
		//2. 在主进程的某个线程处去调用 TerminateProcess() 去结束一个子进程
		//3. 在当前进程的某个位置调用 ExitProcess() 去结束自身这个进程
		//TerminateProcess(进程句柄, 退出码);
		ExitProcess(0);
	}
		break;
	case WM_RBUTTONDBLCLK:	//右键双击消息
		DialogBox(	//模态对话框的创建函数
			hInst,	//实例
			MAKEINTRESOURCE(IDD_DIALOG1),	//资源中对话框ID
			hWnd,	//父窗口句柄
			AboutS);	//框的消息处理函数
		break;
	case WM_LBUTTONDBLCLK:	//左键双击消息
		//非模态的创建函数
		g_hDlgWnd = CreateDialog(hInst, MAKEINTRESOURCE(IDD_DIALOG1), hWnd, About_S);
		ShowWindow(g_hDlgWnd, SW_SHOWNORMAL);
		break;
	case WM_MOUSEMOVE:	//鼠标移动消息
	if(isDian){
		short x_zhongDian = LOWORD(lParam);
		short y_zhongDian = HIWORD(lParam);
		hdc = GetDC(hWnd);
		//TextOut(
		//	hdc,
		//	0, 0,				//开始位与结束位
		//	_T("鼠标移动"),	//输出文本
		//	4);					//输出文本的大小
		MoveToEx(hdc, x_qiDian, y_qiDian, nullptr);			//设置绘图起点
		LineTo(hdc, x_zhongDian, y_zhongDian);	//画一条从起点到x,y的直线
		ReleaseDC(hWnd, hdc);
		x_qiDian = x_zhongDian;
		y_qiDian = y_zhongDian;
	}
	break;
	case WM_MOUSEWHEEL:	//鼠标滚轮消息
	{
		short y = HIWORD(wParam);		//鼠标滚轮滚动的数据在高两字节位,要注意数据类型
	}
	break;
	case WM_KEYDOWN:	//键盘按下消息的宏定义
		//MessageBox(0, _T("键盘按下"), _T("键盘事件"), 0);
		hdc = GetDC(hWnd);
		TextOut(hdc, 200, 0, _T("key"), 3);
		ReleaseDC(hWnd, hdc);
		
		switch (wParam)
		{
		case 'A':
			g_HeroX-= in;
			g_HeroY-= in;
			g_HeroDir = 3;
			break;
		case 'D':
			g_HeroX+= in;
			g_HeroY+= in;
			g_HeroDir = 0;
			break;
		case 'W':
			g_HeroX+= in;
			g_HeroY-= in;
			g_HeroDir = 2;
			break;
		case 'S':
			g_HeroX-= in;
			g_HeroY+= in;
			g_HeroDir = 1;
			break;
		}
		break;
	case WM_KEYUP:		//键盘弹起消息的宏定义
		break;
	case WM_CHAR:		//字符消息, 用户间接产生的消息
		
		break;
	//case WM_SYSCOMMAND:	//拦截退出消息
	//	switch (wParam)
	//	{
	//	case SC_CLOSE:	//退出消息
	//		if (IDYES == MessageBox(0, _T("确定退出?"), 0, MB_YESNO))
	//			DefWindowProc(hWnd, message, wParam, lParam);
	//		break;
	//	default:
	//		DefWindowProc(hWnd, message, wParam, lParam);
	//	}
	//	break;
	case WM_CREATE:		//重要消息, 创建窗口消息, 理解为类的构造, 在窗口出现前触发
		//MessageBox(0,0,_T("初始化"),0);
		RegisterHotKey(hWnd, 8001, MOD_CONTROL, 'M');	//热键注册
		//计时器消息: 1. 设置计时器   2. 响应计时器   3. 销毁计时器
		//设置计时器
		SetTimer(
			hWnd,	//窗口句柄
			5001,	//自定ID
			1000,	//间隔时间
			nullptr);	//计时器的回调函数, 如果为 nullptr 在 WM_TIMER 里响应, 否则在回调函数内响应
		g_hRMenu = LoadMenu(hInst, MAKEINTRESOURCE(IDR_MENU1));
		//CreateWindow 按钮控件在窗口上创建
		//CreateWindow(
		//	_T("button"),	//表示注册的类名, button是按钮
		//	_T("按钮"),		//按钮显示的文本
		//	WS_CHILD|WS_VISIBLE|BS_PUSHBUTTON,	//按钮的风格
		//	100,100,	//按钮的位置
		//	150,30,	//按钮的大小
		//	hWnd,	//父窗口
		//	(HMENU)5001,	//注册ID
		//	hInst,		//实例
		//	nullptr);	//
		////CreateWindow(_T("static"), _T("单选"), WS_CHILD|WS_VISIBLE,100,200,150,30,hWnd,(HMENU)5002,hInst,nullptr);
		break;
	case WM_TIMER:	//响应计时器
	{
		static int j = 0;
		hdc = GetDC(hWnd);
		TextOut(hdc, 500, 200+j, _T("timer"), 5);
		ReleaseDC(hWnd, hdc);
		j += 20;
		if (j == 100)
			KillTimer(	//销毁计时器
				hWnd,	//哪个窗口
				5001);	//哪个ID
	}
		break;
	case WM_ACTIVATE:	//激活消息
		switch (wParam)
		{
		case WA_CLICKACTIVE:	//鼠标激活
		{	
			static int j = 0;
			hdc = GetDC(hWnd);
			TextOut(hdc, 0, j, _T("鼠标激活"), 4);
			ReleaseDC(hWnd, hdc);
			j += 20;
		}
			break;
		case WA_ACTIVE:			//非鼠标激活
		{
			static int j = 0;
			hdc = GetDC(hWnd);
			TextOut(hdc, 100, j, _T("非鼠标激活"), 5);
			ReleaseDC(hWnd, hdc);
			j += 20; 
		}
			break;
		case WA_INACTIVE:		//非激活
		{	
			static int j = 0;
			hdc = GetDC(hWnd);
			TextOut(hdc, 200, j, _T("非激活"), 3);
			ReleaseDC(hWnd, hdc);
			j += 20; 
		}
			break;
		}
		break;
	case WM_SYSKEYDOWN:		//系统按键Alt按下消息
		switch (wParam)
		{
		case VK_MENU:
			hdc = GetDC(hWnd);
			TextOut(hdc, 0, 200, _T("系统按键Alt按下"), 9);
			ReleaseDC(hWnd, hdc);
			break;
		case 'M':
			hdc = GetDC(hWnd);
			TextOut(hdc, 200, 200, _T("系统按键Alt+M键按下"), 12);
			ReleaseDC(hWnd, hdc);
			break;
		case VK_F4:
			return 0;
			break;
		}
		break;
	case WM_SYSKEYUP:		//系统按键Alt弹起消息
		break;
	case VK_MENU:
		hdc = GetDC(hWnd);
		TextOut(hdc, 0, 200, _T("系统按键Alt弹起"), 9);
		ReleaseDC(hWnd, hdc);
		break;
	case WM_HOTKEY:		//热键消息
		switch (wParam)
		{
		case 8001:
			MessageBox(0, _T("ctrl + m"), 0, 0);
			break;
		}
    case WM_COMMAND:	//命令消息的宏定义
        {
            int wmId = LOWORD(wParam);	//LOWORD 高位
            // 分析菜单选择:
            switch (wmId)
            {
			case ID_SAVE:
				MessageBox(0, 0,0,0);
				break;
            case IDM_ABOUT:
                DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);	//
                break;
            case IDM_EXIT:
                DestroyWindow(hWnd);	//Destroy销毁, Window窗口, DestroyWindow销毁窗口
                break;
            default:
                return DefWindowProc(hWnd, message, wParam, lParam);//默认的窗口处理函数
            }
        }
        break;
    case WM_PAINT:	//绘制消息在两种情况下触发:1. 窗口最初创建; 2. 窗口出现无效区域
        {
            PAINTSTRUCT ps;			//设备环境句柄
			HPEN hPen = nullptr;	//定义一个画笔	
            HDC hdc = BeginPaint(hWnd, &ps);	//得到设备环境句柄
            // TODO: 在此处添加使用 hdc 的任何绘图代码...
			//画线
			MoveToEx(hdc, 100, 100, nullptr);
			LineTo(hdc, 400, 400);

			hPen = CreatePen(PS_DASHDOT, 1, RGB(255,0,0));
			SelectObject(hdc, hPen);

			Arc(hdc,100,100,400,400,0,400,250,0);

			//画点
			for (int i = 0; i < 100; ++i)
				SetPixel(hdc, 100+i, 100, RGB(0,0,255));
			for (int i = 0; i < 100; ++i)
				SetPixel(hdc, 100+i, 400, GetPixel(hdc, 100 + i, 100));
			DeleteObject(hPen);

			//位图, GDI中只支持位图(.bmp格式图片)
			//	1.	位图无法预设
			//		 HDC hdc = GetDC(hWnd);	//创建一个设备环境句柄
			//	2.	创建一个兼容DC
					 HDC bitDc = CreateCompatibleDC(hdc);	//根据hdc创建一个兼容的bitDc
					 HDC bitDC = CreateCompatibleDC(hdc);	//根据hdc创建一个兼容的bitDc
			//		创建一个HBITMAP加载资源中的图片
					 HBITMAP hBitmap = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_BITMAP1));		//这种发布是把图片打包进exe文件中
					 HBITMAP HBitmap = (HBITMAP)LoadImage(			//这种发布是把加载项目路径的图片
													hInst,			//当前实例
													_T("hero.bmp"),	//图片路径(绝对或相对, 一般都是相对路径)
													IMAGE_BITMAP,	//加载类型, IMAGE_BITMAP 表示是加载位图
													0, 0,			//高度和宽度, c开头的一般是宽度和高度, 给0是自行匹配图片大小
													LR_LOADFROMFILE);//加载方式, 从哪加载图片, LR_LOADFROMFILE 表示是从文件加载
					 SelectObject(bitDc, hBitmap);
					 SelectObject(bitDC, HBitmap);
			//	3. 绘图
			//		绘图函数
				//	BitBlt(hdc, 500,100,360,360,bitDc, 0, 0, SRCCOPY);	//
				//	BitBlt(
				//		hdc,		//目标DC
				//		900,100,	//在目标DC开始绘图的x,y坐标
				//		360,355,	//在目标DC绘图的大小
				//		bitDC,		//资源DC
				//		0, 0,		//资源DC的裁剪掉的坐标位置
				//		SRCCOPY);	//从资源DC怎么操作到目标DC, SRCCOPY 源拷贝
				////TransparentBlt 要加库文件 #pragma comment (lib, "Msimg32.lib")
					//TransparentBlt(	//图片背景透明化函数
					//	hdc,
					//	0,0,			//图片的绘制大小右下角
					//	360,355,		//图片的绘制大小右下角
					//	bitDC,			//保存图片的兼容DC
					//	0,0,			//填充区域左上角
					//	360,355,		//填充区域右下角
					//	RGB(255,255,255));
			//	4. 释放
					DeleteDC(bitDc);
					DeleteObject(hBitmap);

            EndPaint(hWnd, &ps);		//释放设备环境句柄
        }
        break;
    case WM_DESTROY:	//销毁消息的宏定义
        PostQuitMessage(0);	//投递退出消息, Post(投递)Quit(退出)Message(消息)
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

INT_PTR CALLBACK AboutS(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:	//当对话框创建成功还没显示的时候就会响应这个消息
		//控件的动态添加
		//button 表示类名(已经注册好的), 表示按钮类, 包含很多按钮类型, 通过按钮的风格来决定是哪一类按钮
		//WS_CHILD 子窗口	WS_VISIBLE 可见的
		//BS_PUSHBUTTON	下压式按钮	BS_GROUPBOX 组框
		CreateWindow(_T("button"), _T("这是一个按钮"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
			350, 390, 150, 30, hDlg, (HMENU)5001, hInst, nullptr);
		//BS_GROUPBOX 组框
		CreateWindow(_T("button"), _T("这是一个调查问卷"), WS_CHILD | WS_VISIBLE | BS_GROUPBOX,
			10, 10, 500, 420, hDlg, (HMENU)5002, hInst, nullptr);
		//static静态文本
		CreateWindow(_T("static"), _T("今天吃了什么"), WS_CHILD | WS_VISIBLE,
			20, 40, 150, 30, hDlg, (HMENU)6001, hInst, nullptr);

		//BS_AUTORADIOBUTTON 单选框风格: 如果没有分组的风格, 所以的单选框默认一组
		//WS_GROUP 表达单选的分组, 从有WS_GROUP的控件到下一个有WS_GROUP的控件为一组
		CreateWindow(_T("button"), _T("米饭"), WS_CHILD | WS_VISIBLE | BS_AUTORADIOBUTTON | WS_GROUP,
			70, 70, 150, 30, hDlg, (HMENU)6002, hInst, nullptr);
		CreateWindow(_T("button"), _T("面食"), WS_CHILD | WS_VISIBLE | BS_AUTORADIOBUTTON,
			70,100,150,30,hDlg, (HMENU)6003, hInst, nullptr);
		CreateWindow(_T("button"), _T("随便"), WS_CHILD | WS_VISIBLE | BS_AUTORADIOBUTTON,
			70,130,150,30,hDlg, (HMENU)6004, hInst, nullptr);
		
		CreateWindow(_T("static"), _T("有什么菜"), WS_CHILD|WS_VISIBLE,
			20,170,150,30,hDlg, (HMENU)7001, hInst, nullptr);

		//BS_AUTOCHECKBOX 多选框风格
		CreateWindow(_T("button"), _T("肉"), WS_CHILD | WS_VISIBLE | BS_AUTOCHECKBOX,
			70,200,150,30,hDlg, (HMENU)7002, hInst, nullptr);
		CreateWindow(_T("button"), _T("还是肉"), WS_CHILD | WS_VISIBLE | BS_AUTOCHECKBOX,
			70,230,150,30,hDlg, (HMENU)7003, hInst, nullptr);
		CreateWindow(_T("button"), _T("依然是肉"), WS_CHILD | WS_VISIBLE | BS_AUTOCHECKBOX,
			70,260,150,30,hDlg, (HMENU)7004, hInst, nullptr);

		CreateWindow(_T("static"), _T("花了多少钱"), WS_CHILD|WS_VISIBLE,
			20,300,150,30,hDlg, (HMENU)8001, hInst, nullptr);
		CreateWindow(_T("button"), _T("10元之内"), WS_CHILD | WS_VISIBLE | BS_AUTORADIOBUTTON | WS_GROUP,
			70, 330, 150, 30, hDlg, (HMENU)8002, hInst, nullptr);
		CreateWindow(_T("button"), _T("100元之内"), WS_CHILD | WS_VISIBLE | BS_AUTORADIOBUTTON,
			70,360,150,30,hDlg, (HMENU)8003, hInst, nullptr);
		CreateWindow(_T("button"), _T("蹭的"), WS_CHILD | WS_VISIBLE | BS_AUTORADIOBUTTON,
			70,390,150,30,hDlg, (HMENU)8004, hInst, nullptr);
		
		//edit 编辑框	WS_BORDER 带边框
		CreateWindow(_T("edit"), _T("请输入"), WS_CHILD | WS_VISIBLE | WS_BORDER,
			250, 60, 200, 300, hDlg, (HMENU)9001, hInst, nullptr);

		//设置默认属性
		//GetDlgItem(hDlg,6001) 得到hDlg这个窗口中6001的控件元素的窗口句柄
		//BM_SETCHECK 设置选择
		SendMessage(
			GetDlgItem(hDlg, 6001),//控件的窗口句柄
			BM_SETCHECK,		   //设置风格
			1,					   //1 选中 0 没有选中
			0);
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

INT_PTR CALLBACK About_S(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
		createControl(hDlg);
        return (INT_PTR)TRUE;

    case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case 5021:
			GetDlgItemText(hDlg, 5012, strArr, 1023);	//获得编辑框当前文本
			//组合框分为两个区域, 一为显示区, 二为资源区(倒三角的区域便是)
			SetDlgItemText(hDlg, 5031, strArr);		//输出从编辑框获得的文本到组合框的显示区
			SendMessage(	//发送一个消息到组合框
				GetDlgItem(hDlg, 5031),		//从组合框的显示区获得消息
				CB_ADDSTRING,	//追加的方式, CB_ADDSTRING 尾部追加, CB_INSERTSTRING 插入追加
				//CB_INSERTSTRING,//插入位要有数值, 比如下标0位置没有数据时, 在下标1的位置插入会失败
				0,				//追加的开始位置, CB_INSERTSTRING的方式时可用
				(LPARAM)strArr);	//
			break;
		case 5022:
			GetDlgItemText(hDlg, 5012, strArr, 1023);
			SendMessage(GetDlgItem(hDlg, 5031), CB_INSERTSTRING, 2, (LPARAM)strArr);
			break;
		case 5023:
			//CB_DELETESTRING 删除, CB_RESETCONTENT 重置
			SendMessage(GetDlgItem(hDlg, 5031), CB_DELETESTRING, 2, (LPARAM)strArr);
			break;
		case 6021:
			GetCurrentDirectory(1023,strArr);//得到当前应用程序的绝对路径
			SetDlgItemText(hDlg, 6012, strArr);
			break;
		case 6022:
			GetTempPath(1023, strArr);	//得到当前应用程序的临时路径
			SetDlgItemText(hDlg, 6012, strArr);
			break;
		}
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hDlg, LOWORD(wParam));

			//win32文件操作
			//1. 定义用于文件操作的句柄(文件句柄) HANDLE 句柄
			HANDLE hFile = nullptr;
			//2. 创建文件
			hFile = CreateFile(		//创建文件函数
				_T("1.txt"),	//文件名
				GENERIC_ALL,	//文件的访问权限
				FILE_SHARE_READ,	//共享读模式
				nullptr,		//安全属性
				CREATE_ALWAYS,
				//OPEN_ALWAYS,	//文件创建方式, OPEN_ALWAYS文件没有就创建一个, OPEN_EXISTING文件没有就没有
				//OPEN_EXISTING,
				FILE_ATTRIBUTE_NORMAL,	//文件属性
				nullptr);		//文件模板句柄
			//3. 读写文件
			char s[1024] = { "Hello Wrold!"};
			//strcpy_s(s,(const char*)strArr);
			DWORD len = 0;
			//读文件句柄函数
			if (ReadFile(hFile, s, 1023, &len, nullptr))
				MessageBox(0,0,0,0);
			
			SetFilePointer(	//文件句柄指针偏移
				hFile,
				5,	//低32位的偏移量
				0,  //高32位的偏移量
				FILE_BEGIN);//从文件开始位置偏移

			WriteFile(	//写文件句柄函数
				hFile,	//操作的文件句柄
				s,		//写入的内容
				strlen(s),	//写入的内容字符个数
				&len,		//写入的具体个数(输出)
				nullptr);	//辅助信息
			//4. 关闭文件句柄
			CloseHandle(hFile);

			//win32文件夹遍历
			char fileStr[1024] = {0};	//准备一个辅助数组
			//准备一个文件
			HANDLE hfile = CreateFile(_T("1.txt"),GENERIC_WRITE, FILE_SHARE_READ, nullptr,OPEN_ALWAYS,FILE_ATTRIBUTE_NORMAL,nullptr);
			if (!hfile)
				return 0;
			//在windows里面得到文件夹下面的所有文件名  FindFirstFile  FindNextFile
			WIN32_FIND_DATAA fileDate;
			HANDLE hFindFile = FindFirstFileA(	//查找路径文件名下的第一个文件信息
				"Debug/*", //查找的路径文件名,  *.*: *表示任何, *.*点前面的*号表示任何文件名, 点后面的*表示任何后缀格式
				&fileDate);	//文件信息的保存位
			memcpy(fileStr, fileDate.cFileName, sizeof(char)* strlen(fileDate.cFileName));
			strcat_s(fileStr, "\n");

			WriteFile(hfile, fileStr, strlen(fileStr),&len, nullptr);
			/*		DWORD dwFileAttributes;		//文件属性
					FILETIME ftCreationTime;	//创建时间
					FILETIME ftLastAccessTime;	//访问时间
					FILETIME ftLastWriteTime;	//写入时间
					DWORD nFileSizeHigh;	//文件大小的高字节文件尺寸
					DWORD nFileSizeLow;		//文件大小的低字节
					DWORD dwReserved0;
					DWORD dwReserved1;
					_Field_z_ WCHAR  cFileName[ MAX_PATH ];
					_Field_z_ WCHAR  cAlternateFileName[ 14 ];	*/
			
			while (FindNextFileA(hFindFile, &fileDate))
			{
				ZeroMemory(fileStr, sizeof(fileStr));
				memcpy(fileStr, fileDate.cFileName, sizeof(char)* strlen(fileDate.cFileName));
				strcat_s(fileStr, "\n");
				WriteFile(hfile, fileStr, strlen(fileStr),&len,nullptr);
			}
			CloseHandle(hFindFile);
			CloseHandle(hfile);

            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}
void	createControl(HWND hWndParent)
{
	//create 读音: kui ei ci, kui 读第一声, ei 读第四声, ci 读第三声
	CreateWindow(_T("static"), _T("请输入内容: "), WS_CHILD | WS_VISIBLE,
		18, 16, 100, 30, hWndParent, (HMENU)5011, hInst, nullptr);
	CreateWindow(_T("edit"), _T(""), WS_CHILD | WS_VISIBLE | WS_BORDER,
		110, 15, 300, 20, hWndParent, (HMENU)5012, hInst, nullptr);

	CreateWindow(_T("button"), _T("添加数据"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
		110, 50, 100, 30, hWndParent, (HMENU)5021, hInst, nullptr);
	CreateWindow(_T("button"), _T("追加数据"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
		210, 50, 100, 30, hWndParent, (HMENU)5022, hInst, nullptr);
	CreateWindow(_T("button"), _T("删除数据"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
		310, 50, 100, 30, hWndParent, (HMENU)5023, hInst, nullptr);

	//组合框 类名: combobox
	//分为3类: CBS_DROPDOWN 下拉式组合框(大小预留高度), CBS_SIMPLE 简单组合框, CBS_DROPDOWNLIST 下拉列表式组合框(有滚动条)
	CreateWindow(_T("combobox"), _T(""), WS_CHILD | WS_VISIBLE | CBS_DROPDOWN,
		110, 100, 300, 180, hWndParent, (HMENU)5031, hInst, nullptr);
	//组合框一定要有预留高度, 否则无法正常显示, 这个高度并不代表当里面只有一行的时候也要这个高度

	CreateWindow(_T("static"), _T("文件绝对路径: "), WS_CHILD | WS_VISIBLE,
		10, 150, 100, 30, hWndParent, (HMENU)6011, hInst, nullptr);
	CreateWindow(_T("edit"), _T(""), WS_CHILD | WS_VISIBLE | WS_BORDER,
		110, 150, 300, 20, hWndParent, (HMENU)6012, hInst, nullptr);

	CreateWindow(_T("button"), _T("当前绝对路径"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
		110, 185, 100, 30, hWndParent, (HMENU)6021, hInst, nullptr);
	CreateWindow(_T("button"), _T("当前临时路径"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
		210, 185, 100, 30, hWndParent, (HMENU)6022, hInst, nullptr);

}