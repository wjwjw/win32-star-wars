//-----------------------------------------------------------------
// ScrollingBackground Object
// C++ Source - ScrollingBackground.cpp
//-----------------------------------------------------------------

//-----------------------------------------------------------------
// Include Files
//-----------------------------------------------------------------
#include "ScrollingBackground.h"

//-----------------------------------------------------------------
// BackgroundLayer Constructor(s)/Destructor
//-----------------------------------------------------------------
// Create a background layer from a bitmap file
BackgroundLayer::BackgroundLayer(HDC hDC, LPTSTR szFileName,
								 int iSpeed, SCROLLDIR sdDirection)
								 : Bitmap(hDC, szFileName), m_iSpeed(iSpeed), m_sdDirection(sdDirection)
{
	//将视口默认设置为整个图层图像
	m_rcViewport.left = m_rcViewport.top = 0;
	m_rcViewport.right = m_iWidth;
	m_rcViewport.bottom = m_iHeight;

	m_ViewportWidth = m_iWidth;
	m_ViewportHeight = m_iHeight;
}

// Create a background layer from a bitmap resource
BackgroundLayer::BackgroundLayer(HDC hDC, UINT uiResID, HINSTANCE hInstance,
								 int iSpeed, SCROLLDIR sdDirection)
								 : Bitmap(hDC, uiResID, hInstance), m_iSpeed(iSpeed),
								 m_sdDirection(sdDirection)
{
	m_rcViewport.left = m_rcViewport.top = 0;
	m_rcViewport.right = m_iWidth;
	m_rcViewport.bottom = m_iHeight;

	m_ViewportWidth = m_iWidth;
	m_ViewportHeight = m_iHeight;
}

//负责绘制背景图层，需要考虑到视口
void BackgroundLayer::Draw(HDC hDC, int x, int y, bool bTrans, COLORREF crTransColor)
{	
	switch (m_sdDirection)
		{			
		case SD_DOWN:
			{
				if (m_ViewportHeight < m_iHeight)//视口高度 < 位图高度
				{
					if(m_rcViewport.top >= m_ViewportHeight-m_iHeight)//显示整张位图的一部分
					{
						DrawPart(hDC,x,y,0,(m_iHeight-m_ViewportHeight)+m_rcViewport.top,m_ViewportWidth,m_ViewportHeight,bTrans,crTransColor);
					}
					else
					{
						DrawPart(hDC, x, y,
							0,2*m_iHeight-m_ViewportHeight+m_rcViewport.top,
							m_ViewportWidth,m_ViewportHeight-m_iHeight-m_rcViewport.top,bTrans,crTransColor);
						DrawPart(hDC, x, y+m_ViewportHeight-m_iHeight-m_rcViewport.top,
							0,0,
							m_ViewportWidth,2*m_rcViewport.top+m_iHeight-m_ViewportHeight,bTrans,crTransColor);
					}
				}
				else //if(m_ViewportHeight == m_iHeight)//视口高度>=位图高度
				{
					DrawPart(hDC, x, y,	0,m_ViewportHeight+m_rcViewport.top,  m_ViewportWidth, -m_rcViewport.top, bTrans, crTransColor);
					DrawPart(hDC, x, y-m_rcViewport.top, 0, 0, m_ViewportWidth,m_ViewportHeight+m_rcViewport.top, bTrans, crTransColor);
				}
			}
			break;
		}

}

//负责移动视口以滚动图层的工作
//该方法使用图层的速度和方向来将视口滑动特定的像素值。
void BackgroundLayer::Update()
{
		switch (m_sdDirection)
		{			
		case SD_DOWN:
			// Move the layer left (slide the viewport right)
			m_rcViewport.top -= m_iSpeed;
			m_rcViewport.bottom -= m_iSpeed;
			if (m_ViewportHeight < m_iHeight)//视口高度 < 位图高度
			{
				if (m_rcViewport.top < -m_iHeight)		
				{
					m_rcViewport.top = 0 ;
					m_rcViewport.bottom = m_rcViewport.top+m_ViewportHeight;
				}
			}
			else //if(m_ViewportHeight == m_iHeight)
			{
				if (m_rcViewport.top < -m_ViewportHeight)		
				{
					m_rcViewport.top = 0 ;
					m_rcViewport.bottom = m_rcViewport.top+m_ViewportHeight;
				}
			}
			break;
		}
}




//-----------------------------------------------------------------
// ScrollingBackground Constructor(s)/Destructor
//-----------------------------------------------------------------
ScrollingBackground::ScrollingBackground(int iWidth, int iHeight)
: Background(iWidth, iHeight, 0), m_iNumLayers(0)
{
}

ScrollingBackground::~ScrollingBackground()
{
}

//-----------------------------------------------------------------
// ScrollingBackground General Methods
//-----------------------------------------------------------------
void ScrollingBackground::Update()
{
	// Update the layers
	for (int i = 0; i < m_iNumLayers; i++)
		m_pLayers[i]->Update();
}

void ScrollingBackground::Draw(HDC hDC, bool bTrans, COLORREF crTransColor)
{
	// Draw the layers
	for (int i = 0; i < m_iNumLayers; i++)
		m_pLayers[i]->Draw(hDC, 0, 0, bTrans, crTransColor);
}

void ScrollingBackground::AddLayer(BackgroundLayer* pLayer)
{
	// Add the new layer (maximum of 10)
	if (m_iNumLayers < 10)
		m_pLayers[m_iNumLayers++] = pLayer;
}
