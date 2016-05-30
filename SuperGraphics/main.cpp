#include "global_data.h"
#include "Game.h"
#include "Pipeline.h"
#include <wingdi.h>

//#define SSAA

extern const int		g_Width = 1000, g_Height = 800;	//client size

#ifdef SSAA
extern const int		g_ssaaWidth = g_Width * 3, g_ssaaHeight = g_Height * 3;	//client size
#endif

extern bool				g_bRendering = true;
CGame					g_Game;

static HDC				memDc;
static HBITMAP			hDibBitmap;
static HGDIOBJ			hPrevBitmap;
static tagRGBQUAD		(*pScreen)[g_Width];
static float			pDepth[g_Height][g_Width];

#ifdef SSAA
static HBITMAP			hSSAABitmap;
static tagRGBQUAD		(*pScreenSSAA)[g_ssaaWidth];
static float			pSSAADepth[g_ssaaHeight][g_ssaaWidth];
#endif

static HBITMAP			hTextureBmp;

static DWORD			last_frame = 0;
static RECT				rect_client;
const int				MILLISECONDS = 1000;

static const int		ID_TIMER_GAMELOOP = 1;
static const WCHAR		window_name[] = TEXT("Super Graphics v0.1");


inline void mySetPixel(int x, int y, float z, tagRGBQUAD color)
{

#ifdef SSAA
	pScreenSSAA[y][x] = color;
	pSSAADepth[y][x] = z;
#else
	pScreen[y][x] = color;
	pDepth[y][x] = z;
#endif
}
float getDepth(int x, int y)
{
#ifdef SSAA
	return pSSAADepth[y][x];
#else
	return pDepth[y][x];
#endif
}

void testAndSetPixel(int x, int y, float z, tagRGBQUAD color)
{
	//depth test before draw
#ifdef SSAA
	if (x<g_ssaaWidth && y<g_ssaaHeight && z < getDepth(x, y))
#else
	if (x<g_Width && y<g_Height && z < getDepth(x, y))
#endif
		mySetPixel(x, y, z, color);
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
void OnCreate(HWND hwnd);
//timer ID_TIMER_GAMELOOP 's callback function
void CALLBACK GameLoop(HDC hdc);
void Clear(HDC, int color);
void Release();

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	HWND		hWnd;
	MSG			msg; 

	//init window
	const WCHAR		class_name[] = TEXT("Super Graphics");
	WNDCLASSEX		WndCls;
	WndCls.cbSize = sizeof(WndCls);
	WndCls.style = CS_OWNDC | CS_VREDRAW | CS_HREDRAW;
	WndCls.lpfnWndProc = WndProc;
	WndCls.cbClsExtra = 0;
	WndCls.cbWndExtra = 0;
	WndCls.hInstance = hInstance;
	WndCls.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	WndCls.hCursor = LoadCursor(NULL, IDC_ARROW);
	WndCls.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	WndCls.lpszMenuName = NULL;
	WndCls.lpszClassName = class_name;
	WndCls.hIconSm = LoadIcon(hInstance, IDI_APPLICATION);
	if (!RegisterClassEx(&WndCls))
	{
		::MessageBox(NULL, TEXT("RegisterClassEx"), TEXT("will exit now..."), MB_OK);
		return false;
	}

	hWnd = CreateWindowEx(WS_EX_OVERLAPPEDWINDOW,
		class_name, window_name,
		WS_OVERLAPPEDWINDOW | WS_VISIBLE,
		CW_USEDEFAULT, CW_USEDEFAULT, g_Width, g_Height,
		NULL, NULL, hInstance, NULL);
	ShowWindow(hWnd, SW_SHOW);
	UpdateWindow(hWnd);

	//main loop
	last_frame = GetTickCount();
	while (g_bRendering)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			if (msg.message == WM_QUIT)
				break;

			//handle messages
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		HDC hdc = GetDC(hWnd);
		GameLoop(hdc);
		ReleaseDC(hWnd, hdc);
	}

	//release
	Release();

	return 0;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	static int lastx=0;
	static int lasty=0;
	PAINTSTRUCT		ps;
	HDC				hdc;
	switch (msg)
	{
	case WM_CREATE:
		{
			OnCreate(hWnd);

			RECT rectWindow;
			GetWindowRect(hWnd, &rectWindow);
			lastx = (rectWindow.right - rectWindow.left) / 2;
			lasty = (rectWindow.bottom - rectWindow.top) / 2;
		}
		break;
	case WM_KEYDOWN:
		switch (wParam)
		{
		case VK_ESCAPE:
			PostQuitMessage(0);
			g_bRendering = false;
			break;
		}
		break;
	case WM_CHAR:
		g_Game.keyDown(wParam);
		break;
	case WM_MOUSEMOVE:
	{
		CGame::eMouseType mt = CGame::EMT_NONE;
		switch (wParam)
		{
		case MK_LBUTTON:
			mt = CGame::EMT_LBUTTONDOWN;
			break;
		case MK_RBUTTON:
			mt = CGame::EMT_RBUTTONDOWN;
			break;
		case MK_MBUTTON:
			mt = CGame::EMT_MBUTTONDOWN;
			break;
		}
		int x = LOWORD(lParam);
		int y = HIWORD(lParam);
		g_Game.MouseMove(x - lastx, y - lasty, mt);
		lastx = x;
		lasty = y;
	}
		break;
	case WM_MOUSEWHEEL:
		g_Game.MouseWheel(GET_WHEEL_DELTA_WPARAM(wParam) / WHEEL_DELTA);
		break;
	case WM_DESTROY:
		g_bRendering = false;
		PostQuitMessage(0);//调用退出函数
		break;
	default:
		return DefWindowProc(hWnd, msg, wParam, lParam);
	}

	return 0;
}

void OnCreate(HWND hWnd)
{
	RECT rectClient, rectWindow;
	GetClientRect(hWnd, &rectClient);
	GetWindowRect(hWnd, &rectWindow);
	//let client size to be(g_Width, g_Height);
	int width = (rectWindow.right - rectWindow.left) - (rectClient.right - rectClient.left) + g_Width;
	int height = (rectWindow.bottom - rectWindow.top) - (rectClient.bottom - rectClient.top) + g_Height;
	MoveWindow(hWnd, rectWindow.left, rectWindow.top, width, height, TRUE);
	GetClientRect(hWnd, &rect_client);

	//init game
#ifdef SSAA
		g_Game.init(g_ssaaWidth, g_ssaaHeight);
#else
		g_Game.init(g_Width, g_Height);
#endif

	//createdib and select it to memdc
	HDC hdc = GetDC(hWnd);
	memDc = CreateCompatibleDC(hdc);

	BITMAPINFOHEADER stBmpInfoHeader = { 0 };
	stBmpInfoHeader.biSize = sizeof(BITMAPINFOHEADER);
	stBmpInfoHeader.biWidth = g_Width;
	stBmpInfoHeader.biHeight = -g_Height;
	stBmpInfoHeader.biPlanes = 1;
	stBmpInfoHeader.biBitCount = 32;
	stBmpInfoHeader.biCompression = BI_RGB;
	stBmpInfoHeader.biClrUsed = 0;
	stBmpInfoHeader.biSizeImage = g_Width * 4 * g_Height;

	hDibBitmap = ::CreateDIBSection(NULL, (PBITMAPINFO)&stBmpInfoHeader, DIB_RGB_COLORS, reinterpret_cast<void**>(&pScreen), NULL, 0);
	assert(hDibBitmap != NULL);

#ifdef SSAA
	ZeroMemory(&stBmpInfoHeader, sizeof(stBmpInfoHeader));
	stBmpInfoHeader.biSize = sizeof(BITMAPINFOHEADER);
	stBmpInfoHeader.biWidth = g_ssaaWidth;
	stBmpInfoHeader.biHeight = -g_ssaaHeight;
	stBmpInfoHeader.biPlanes = 1;
	stBmpInfoHeader.biBitCount = 32;
	stBmpInfoHeader.biCompression = BI_RGB;
	stBmpInfoHeader.biClrUsed = 0;
	stBmpInfoHeader.biSizeImage = g_ssaaWidth * 4 * g_ssaaHeight;

	hSSAABitmap = ::CreateDIBSection(NULL, (PBITMAPINFO)&stBmpInfoHeader, DIB_RGB_COLORS, reinterpret_cast<void**>(&pScreenSSAA), NULL, 0);
	assert(hSSAABitmap != NULL);
#endif	
	hPrevBitmap = SelectObject(memDc, hDibBitmap);
	ReleaseDC(hWnd, hdc);

	g_Pipeline.hdc = memDc;
	g_Pipeline.drawType = EDT_FRAME;
	g_Pipeline.lineMode = ELM_DDA;
}

void Clear(HDC hdc, int color)
{
#ifdef SSAA
	for (int i = 0; i < g_ssaaHeight; ++i)
	{
		for (int j = 0; j < g_ssaaWidth; ++j)
		{
			pSSAADepth[i][j] = 1.0f;
			pScreenSSAA[i][j] = { 0, 0, 0 };
		}
	}
#else
			FillRect(memDc, &rect_client, (HBRUSH)GetStockObject(color));
			for (int i = 0; i < g_Height; ++i)
			{
				for (int j = 0; j < g_Width; ++j)
					pDepth[i][j] = 1.0f;
			}
#endif
}

void CALLBACK GameLoop(HDC hdc)
{
	static int times = 0;
	static float delta_time = 0;
	//clear
	Clear(memDc, BLACK_BRUSH);

	WCHAR buff[128];
	RECT rct;
	//caculate fps
	if (times++ == 20)
	{
		DWORD current_frame = GetTickCount();
		int diff = (current_frame - last_frame);
		delta_time = 1.0f * MILLISECONDS*times / (diff == 0 ? 1.0f : diff);
		last_frame = current_frame;
		times = 0;
	}
	
	
	g_Game.update(0);
	g_Game.render(memDc); 	

#ifdef SSAA
	for (int j = 0; j < g_Height; ++j)
	{
		for (int i = 0; i < g_Width; ++i)
		{
			int m = 1 + j * 3;
			int n = 1 + i * 3;
			RGBQUAD color[9] = { pScreenSSAA[m - 1][n - 1], pScreenSSAA[m - 1][n], pScreenSSAA[m - 1][n + 1], pScreenSSAA[m][n - 1], pScreenSSAA[m][n], pScreenSSAA[m][n + 1], pScreenSSAA[m + 1][n - 1], pScreenSSAA[m + 1][n], pScreenSSAA[m + 1][n + 1] };
			BYTE b = static_cast<BYTE>((color[0].rgbBlue + 2 * color[1].rgbBlue + color[2].rgbBlue + 2 * color[3].rgbBlue + 4 * color[4].rgbBlue + 2 * color[5].rgbBlue + color[6].rgbBlue + 2 * color[7].rgbBlue + color[8].rgbBlue) >> 4);
			BYTE g = static_cast<BYTE>((color[0].rgbGreen + 2 * color[1].rgbGreen + color[2].rgbGreen + 2 * color[3].rgbGreen + 4 * color[4].rgbGreen + 2 * color[5].rgbGreen + color[6].rgbGreen + 2 * color[7].rgbGreen + color[8].rgbGreen) >> 4);
			BYTE r = static_cast<BYTE>((color[0].rgbRed + 2 * color[1].rgbRed + color[2].rgbRed + 2 * color[3].rgbRed + 4 * color[4].rgbRed + 2 * color[5].rgbRed + color[6].rgbRed + 2 * color[7].rgbRed + color[8].rgbRed) >> 4);
			pScreen[j][i] = { b, g, r };
		}
	}
#endif

	WCHAR *LineModes[] = {L"dda", L"Bresenham", L"AA", L"error"};
	WCHAR *DrawMode[] = {L"线框", L"纹理贴图", L"透视修正"};

	POINT pt;
	GetCursorPos(&pt);
	swprintf(buff, TEXT("帧率: %.2f \n鼠标:x=%d, y=%d \nPitch: %.2f° \nYaw: %.2f° \n画线算法:%s \n绘制模式:%s "), 
		delta_time, pt.x, pt.y, g_Game.getPitch(), g_Game.getYaw(), LineModes[g_Pipeline.lineMode], DrawMode[g_Pipeline.drawType]);
	rct = { 0, 0, 200, 150 };
	DrawText(memDc, buff, -1, &rct, DT_LEFT);
	swprintf(buff, TEXT("移动: w, a, s, d \n绕z轴: q,e \n模式:t,T \n光:l,L \n线模式:k,K "));
	rct = { 0, 150, 200, 300 };
	DrawText(memDc, buff, -1, &rct, DT_LEFT);
	
	BitBlt(hdc, rect_client.left, rect_client.top, g_Width, g_Height, memDc, 0, 0, SRCCOPY);
}

void Release()
{
	SelectObject(memDc, hPrevBitmap);
	DeleteDC(memDc);
	DeleteObject(hPrevBitmap);
	DeleteObject(hDibBitmap);
	DeleteObject(hTextureBmp);
}