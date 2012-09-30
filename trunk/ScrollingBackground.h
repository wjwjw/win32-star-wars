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
//����ͼ��ʵ���Ͼ���һ�������λͼͼ������֮��������λͼ����˹������ӿڡ��ٶ��Լ��������Ϣ
class BackgroundLayer : Bitmap
{
protected:
	// Member Variables
	RECT      m_rcViewport;//�ӿھ����˽���������һ����ʵ�ʻ��Ƶ���Ϸ��Ļ��
	int       m_iSpeed;
	SCROLLDIR m_sdDirection;

	int m_ViewportWidth;//�ӿڵĿ��
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
	int               m_iNumLayers;//����ͼ�������
	BackgroundLayer*  m_pLayers[10];//��Ÿ���ͼ���ָ������
	
public:
	// Constructor(s)/Destructor
	ScrollingBackground(int iWidth, int iHeight);
	virtual ~ScrollingBackground();
	
	// General Methods
	virtual void  Update();//���¹��ɱ�����ͼ��
	virtual void  Draw(HDC hDC, bool bTrans = false, COLORREF crTransColor = RGB(0, 0, 0));
	void          AddLayer(BackgroundLayer* pLayer);//�򱳾������ͼ��
};
