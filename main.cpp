#include <windows.h>
#include <tchar.h>
#include "nwpwin.h"
#include <list>


class SelObj {
	HDC hdc;
	HGDIOBJ h;

public:
	SelObj(HDC hdc,HGDIOBJ obj):hdc(hdc),h(::SelectObject(hdc,obj)){}
	~SelObj() { ::SelectObject(hdc, h); }
};
class MainWindow : public Window
{
	std::list<POINT>points;
protected:
	void OnPaint(HDC hdc)  
	{ 
		if (!points.empty()) {
			SelObj pen(hdc, GetStockObject(DC_PEN));
			SetDCPenColor(hdc, RGB(255, 0, 0));
			MoveToEx(hdc, points.front().x, points.front().y, 0);

			for (POINT p : points) {
				LineTo(hdc, p.x, p.y);
			}
	    }
	}
	void OnLButtonDown(POINT p) 
	{
		points.push_back(p);
		InvalidateRect(*this, NULL, true);
	}
	void OnKeyDown(int vk) 
	{
		switch (vk){
		case VK_BACK:
			if (!points.empty()) {
				points.pop_back();
				InvalidateRect(*this, NULL, true);
			}
			break;


		case VK_ESCAPE:
			if (!points.empty()) {
				points.clear();
				InvalidateRect(*this, NULL, true);
			}
		}

	}
	void OnDestroy()
	{
		::PostQuitMessage(0);
	}
};

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPTSTR, int)
{
	Application app;
	MainWindow wnd;
	wnd.Create(NULL, WS_OVERLAPPEDWINDOW | WS_VISIBLE, _T("NWP 5"));
	return app.Run();
}
