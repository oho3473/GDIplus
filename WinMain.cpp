#include<Windows.h>
#include<gdiplus.h>

#pragma comment(lib,"Gdiplus.lib")

using namespace Gdiplus;


const wchar_t gClassName[] = L"myClass";

LRESULT CALLBACK WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

int WINAPI WinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPSTR lpCmdLine,
	_In_ int nShowCmd)
{
	Gdiplus::GdiplusStartupInput gdiplusStartupInput;
	ULONG_PTR gdiplusToken;

	Gdiplus::GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, nullptr);

	HWND hWnd;
	WNDCLASSEX wc;

	ZeroMemory(&wc, sizeof(WNDCLASSEX));

	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpszClassName = gClassName;
	wc.hInstance = hInstance;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)COLOR_WINDOW;
	wc.lpfnWndProc = WindowProc;
	wc.cbSize = sizeof(WNDCLASSEX);

	if (!RegisterClassEx(&wc))
	{
		MessageBox(nullptr, L"Failed to register window class", L"Error", MB_ICONEXCLAMATION | MB_OK);
		return 0;
	}

	RECT wr = { 0,0,640,480 };
	AdjustWindowRect(&wr, WS_OVERLAPPEDWINDOW, FALSE);

	hWnd = CreateWindowEx(NULL,
		gClassName,
		L"GDI+",
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		wr.right-wr.left,
		wr.bottom-wr.top,
		NULL,
		NULL,
		hInstance,
		NULL);

	if (hWnd == nullptr)
	{
		MessageBox(nullptr, L"Failed to create winidow class", L"Error", MB_ICONEXCLAMATION | MB_OK);
		return 0;
	}

	ShowWindow(hWnd, nShowCmd);
	UpdateWindow(hWnd);

	MSG msg;
	while (GetMessage(&msg,NULL,0,0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	GdiplusShutdown(gdiplusToken);
	return static_cast<int>(msg.wParam);
}

void Onpaint(HWND hwnd)
{
	HDC hdc;
	PAINTSTRUCT ps;
	
	hdc = BeginPaint(hwnd, &ps);

	Graphics graphics(hdc);

	Image image(L"test.jpg");

	/*int x = ps.rcPaint.left;
	int y = ps.rcPaint.bottom;
	int w = ps.rcPaint.right - ps.rcPaint.left;
	int h = ps.rcPaint.bottom - ps.rcPaint.top;*/

	graphics.DrawImage(&image,0,0,image.GetWidth(),image.GetHeight());




	Pen pen(Color(255, 0, 255));
	SolidBrush brush(Color(255, 255, 0, 255));

	graphics.DrawLine(&pen, 0, 0, 100, 100);

	FontFamily fontFamily(L"¸¼Àº °íµñ");
	Font font(&fontFamily, 24, FontStyleRegular, UnitPixel);
	PointF pointF(0.0f, 110.0f);
	graphics.DrawString(L"Å×½ºÆ®",-1,&font,pointF,&brush);

	

	EndPaint(hwnd, &ps);
}


LRESULT CALLBACK WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
		case WM_PAINT:
			Onpaint(hWnd);
			break;
		case WM_CLOSE:
			DestroyWindow(hWnd);
			break;
		case WM_DESTROY:
			PostQuitMessage(0);
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
	}
}
