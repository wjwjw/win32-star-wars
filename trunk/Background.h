#pragma once

//-----------------------------------------------------------------
// Include Files
//-----------------------------------------------------------------
#include "Bitmap.h"

class Background
{

protected :
	int m_iWidth, m_iHeight;//背景的宽高
	COLORREF m_crColor;//背景的颜色，只适用于纯色背景
	Bitmap* m_pBitmap;//用作绘制图像背景的位图图像

public:
	//构造纯色背景
	Background(int iWidth, int iHeight, COLORREF crColor);
	//构造图像背景
	Background(Bitmap* pBitmap);
	virtual ~Background();

	//更新背景的外观，对于纯色背景和图像背景，它不做任何事情
	//该函数的目的是使派生的动画背景能够使用它来更新自身
	virtual void Update();
	virtual void Draw(HDC hDC);

	int GetWidth(){return m_iWidth;}
	int GetHeight(){return m_iHeight;}
};