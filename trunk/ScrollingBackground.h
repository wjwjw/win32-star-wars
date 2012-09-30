//-----------------------------------------------------------------
// ScrollingBackground Object
// C++ Header - ScrollingBackground.h
//-----------------------------------------------------------------

#pragma once

//-----------------------------------------------------------------
// Include Files
//-----------------------------------------------------------------
#include <windows.h>
#include "Background.h"

//-----------------------------------------------------------------
// Custom Data Types
//-----------------------------------------------------------------
typedef WORD        SCROLLDIR;
const SCROLLDIR	SD_UP     = 0,
									SD_RIGHT  = 1,
									SD_DOWN   = 2,
									SD_LEFT   = 3;

//-----------------------------------------------------------------
// Background Layer Class
//-----------------------------------------------------------------
//背景图层实际上就是一个特殊的位图图像，特殊之处在于向位图添加了关于其视口、速度以及方向的信息
class BackgroundLayer : Bitmap
{
protected:
	// Member Variables
	RECT      m_rcViewport;//视口决定了将背景的哪一部分实际绘制到游戏屏幕上
	int       m_iSpeed;
	SCROLLDIR m_sdDirection;

	int m_ViewportWidth;//视口的宽高
	int m_ViewportHeight;
	
public:
	// Constructor(s)/Destructor
	BackgroundLayer(HDC hDC, LPTSTR szFileName, int iSpeed=0, SCROLLDIR sdDirection = SD_LEFT);
	BackgroundLayer(HDC hDC, UINT uiResID, HINSTANCE hInstance, int iSpeed = 0, SCROLLDIR sdDirection = SD_LEFT);
	
	// General Methods
	virtual void  Update();
	virtual void  Draw(HDC hDC, int x, int y, bool bTrans = false, COLORREF crTransColor = RGB(0, 0, 0));
	
	void          SetSpeed(int iSpeed)  { m_iSpeed = iSpeed; };
	void          SetDirection(SCROLLDIR sdDirection) { m_sdDirection = sdDirection; };
	void          SetViewport(RECT& rcViewport) 
	{ 
		CopyRect(&m_rcViewport, &rcViewport); 
		m_ViewportWidth = m_rcViewport.right-m_rcViewport.left;
		m_ViewportHeight = m_rcViewport.bottom-m_rcViewport.top;
	};
};

//-----------------------------------------------------------------
// Scrolling Background Class
//-----------------------------------------------------------------
class ScrollingBackground : Background
{
protected:
	// Member Variables
	int               m_iNumLayers;//背景图层的数量
	BackgroundLayer*  m_pLayers[10];//存放各个图层的指针数组
	
public:
	// Constructor(s)/Destructor
	ScrollingBackground(int iWidth, int iHeight);
	virtual ~ScrollingBackground();
	
	// General Methods
	virtual void  Update();//更新构成背景的图层
	virtual void  Draw(HDC hDC, bool bTrans = false, COLORREF crTransColor = RGB(0, 0, 0));
	void          AddLayer(BackgroundLayer* pLayer);//向背景添加新图层
};
