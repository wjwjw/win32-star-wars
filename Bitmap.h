#pragma once


#include <windows.h>

class Bitmap
{
protected:
	HBITMAP m_hBitmap;
	int m_iWidth, m_iHeight;

	void Free();

public:
	Bitmap(HDC hDC, LPTSTR szFileName);
	Bitmap(HDC hDC, UINT uiResID, HINSTANCE hInstance);
	Bitmap(HDC hDC, int iWidth, int iHeight, COLORREF crColor = RGB(0, 0, 0));
	virtual ~Bitmap();

	BOOL LoadByAPI(HDC hDC, LPTSTR szFileName);
	BOOL Load8(HDC hDC, LPTSTR szFileName);
	BOOL Load24(HDC hDC, LPTSTR szFileName);
	BOOL Create(HDC hDC, int iWidth, int iHeight, COLORREF crColor);
	BOOL Create(HDC hDC, UINT uiResID, HINSTANCE hInstance);

	void Draw(HDC hDC, int x, int y);
	void Draw(HDC hDC, int x, int y, bool bTrans, COLORREF crTransColor);

	//对动画帧的支持
	void DrawPart(HDC hDC, int x, int y, int xPart, int yPart,int wPart, int hPart, bool bTrans, COLORREF crTransColor);
	
	int  GetWidth() { return m_iWidth; };
	int  GetHeight() { return m_iHeight; };
};
