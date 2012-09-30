//-----------------------------------------------------------------
// Sprite Object
// C++ Header - Sprite.h
//-----------------------------------------------------------------

#pragma once

//-----------------------------------------------------------------
// Include Files
//-----------------------------------------------------------------
#include <windows.h>
#include "Bitmap.h"

//-----------------------------------------------------------------
// Custom Data Types
//-----------------------------------------------------------------
typedef WORD        BOUNDSACTION;//边界动作
const BOUNDSACTION  BA_STOP   = 0,//停止
					BA_WRAP   = 1,//环绕
					BA_BOUNCE = 2,//弹开
					BA_DIE    = 3;//删除

typedef WORD        SPRITEACTION;  //碰撞动作（add）
const SPRITEACTION  SA_NONE   = 0x0000L,
                    SA_KILL   = 0x0001L;


//-----------------------------------------------------------------
// Sprite Class
//-----------------------------------------------------------------
class Sprite
{
protected:
	// Member Variables
	Bitmap*       m_pBitmap;
	RECT          m_rcPosition;
	POINT         m_ptVelocity;
	int           m_iZOrder;
	RECT          m_rcBounds;
	BOUNDSACTION  m_baBoundsAction;
	BOOL          m_bHidden;

	RECT	      m_rcCollision;  //碰撞矩阵（add）

	int m_iRow,m_iCol;//精灵图片的长宽，即每行帧数和每列帧数
	bool m_bShowAnim;//是否播放动画帧，停止时可以不播放动画帧
	int* pFrameList; //帧序列数组，即当前播放的是哪个帧序列

	/*帧动画属性*/
	int m_iNumFrame;//总帧数
	//int m_iCurFrame;//当前帧索引
	int m_iDelayFrame;//帧切换速度
	int m_iTriggerFrame;//帧计数器

	void UpdateFrame();//切换帧
  
	virtual void  CalcCollisionRect();//计算碰撞矩阵（add）

	//添加精灵动画支持
	int m_iNumFrames, m_iCurFrame;
	int m_iFrameDelay, m_iFrameTrigger;


	//对于爆炸精灵，必须循环显示其动画帧，然后立即消失，为了提供支持，修改了Sprite类
	BOOL          m_bDying;//精灵是否被标记为将要被删除（普通精灵为false，将要删除的精灵为true）
	BOOL          m_bOneCycle;//是否只播放一个帧动画循环

	int Level;

public:
	void SetRowAndCol(int row, int col){m_iRow = row;m_iCol = col;}
	void SetFrameList(int array[], int iNumFrame);
	void SetDelayFrame(int iDelayFrame);
	void SetBShowAnim(bool show){m_bShowAnim = show;}

	int     GetWidth() { return m_pBitmap->GetWidth()/m_iRow; };
	int     GetHeight() { return m_pBitmap->GetHeight()/m_iCol; };
	
public:
	// Constructor(s)/Destructor
	Sprite(Bitmap* pBitmap);
	Sprite(Bitmap* pBitmap, RECT& rcBounds, BOUNDSACTION baBoundsAction = BA_STOP);
	Sprite(Bitmap* pBitmap, POINT ptPosition, POINT ptVelocity, int iZOrder,
		RECT& rcBounds, BOUNDSACTION baBoundsAction = BA_STOP);
	virtual ~Sprite();
	
	// General Methods
	//virtual void  Update();（update）
	void          Draw(HDC hDC);
	BOOL          IsPointInside(int x, int y);//击中测试，判断鼠标是否单击了精灵
	
	// General Methods
	virtual SPRITEACTION  Update(int&,int&);//（add）
	BOOL	TestCollision(Sprite* pTestSprite);//（add）

	// Accessor Methods
	RECT&   GetCollision() { return m_rcCollision; };//（add）

	RECT&   GetPosition(){ return m_rcPosition; };
	void    SetPosition(int x, int y);
	void    SetPosition(POINT ptPosition);
	void    SetPosition(RECT& rcPosition){ CopyRect(&m_rcPosition, &rcPosition); };
	
	void    OffsetPosition(int x, int y);
	POINT   GetVelocity() { return m_ptVelocity; };
	void    SetVelocity(int x, int y);
	void    SetVelocity(POINT ptVelocity);
	BOOL    GetZOrder() { return m_iZOrder; };
	void    SetZOrder(int iZOrder) { m_iZOrder = iZOrder; };
	void    SetBounds(RECT& rcBounds) { CopyRect(&m_rcBounds, &rcBounds); };
	void    SetBoundsAction(BOUNDSACTION ba) { m_baBoundsAction = ba; };
	BOOL    IsHidden() { return m_bHidden; };
	void    SetHidden(BOOL bHidden) { m_bHidden = bHidden; };
	void    Kill() { m_bDying = TRUE; };

	//添加精灵动画支持（要想将一个普通精灵转换为一个帧动画精灵，只需设置帧数以及帧延迟即可）
	void    SetNumFrames(int iNumFrames, BOOL bOneCycle = FALSE);
	void    SetFrameDelay(int iFrameDelay) { m_iFrameDelay = iFrameDelay; };

	Bitmap* GetBitmap()               { return m_pBitmap; };

	void SetLevel(int iLevel) { Level = iLevel; };
	int GetLevel() { return Level; };


};



//-----------------------------------------------------------------
// Sprite Inline General Methods
//-----------------------------------------------------------------


//-----------------------------------------------------------------
// Sprite Inline Helper Methods
//-----------------------------------------------------------------

///////////////////////////////（add）///////////////////////////////////////

inline void Sprite::CalcCollisionRect()
{
	int iXShrink = (m_rcPosition.left - m_rcPosition.right) / 12;
	int iYShrink = (m_rcPosition.top - m_rcPosition.bottom) / 12;
	CopyRect(&m_rcCollision, &m_rcPosition);
	InflateRect(&m_rcCollision, iXShrink, iYShrink);
}

//-----------------------------------------------------------------
// Sprite Inline General Methods
//-----------------------------------------------------------------
inline BOOL Sprite::TestCollision(Sprite* pTestSprite)
{
	RECT& rcTest = pTestSprite->GetCollision();
	//return m_rcCollision.left <= rcTest.right &&
	//	rcTest.left <= m_rcCollision.right &&
	//	m_rcCollision.top <= rcTest.bottom &&
	//	rcTest.top <= m_rcCollision.bottom;
	long x1,x2;
	long y1,y2;
	long r1,r2;
	x1=m_rcCollision.left+m_rcCollision.right;
	x1/=2;
	x2=rcTest.left+rcTest.right;
	x2/=2;
	y1=m_rcCollision.top+m_rcCollision.bottom;
	y1/=2;
	y2=rcTest.top+rcTest.bottom;
	y2/=2;
	r1=m_rcCollision.right-m_rcCollision.left;
	r1/=2;
	r2=rcTest.right-rcTest.left;
	r2/=2;
	return (x1-x2)*(x1-x2)+(y1-y2)*(y1-y2)<=(r1+r2)*(r1+r2);

}

/////////////////////////////（add）//////////////////////////////////////

inline BOOL Sprite::IsPointInside(int x, int y)
{
	POINT ptPoint;
	ptPoint.x = x;
	ptPoint.y = y;
	return PtInRect(&m_rcPosition, ptPoint);
}

//-----------------------------------------------------------------
// Sprite Inline Accessor Methods
//-----------------------------------------------------------------
inline void Sprite::SetNumFrames(int iNumFrames, BOOL bOneCycle)
{
	//设置帧数
	m_iNumFrames = iNumFrames;
	
	//重新计算位置
	RECT rect = GetPosition();
	rect.bottom = rect.top + ((rect.bottom - rect.top) / iNumFrames);
	SetPosition(rect);
	
	m_bOneCycle = bOneCycle;
}



inline void Sprite::SetPosition(int x, int y)
{
	OffsetRect(&m_rcPosition, x - m_rcPosition.left, y - m_rcPosition.top);
	CalcCollisionRect();//（add）
}

inline void Sprite::SetPosition(POINT ptPosition)
{
	OffsetRect(&m_rcPosition, ptPosition.x - m_rcPosition.left,
		ptPosition.y - m_rcPosition.top);
	CalcCollisionRect();//（add）
}

inline void Sprite::OffsetPosition(int x, int y)
{
	OffsetRect(&m_rcPosition, x, y);
	CalcCollisionRect();//（add）
}

inline void Sprite::SetVelocity(int x, int y)
{
	m_ptVelocity.x = x;
	m_ptVelocity.y = y;
}

inline void Sprite::SetVelocity(POINT ptVelocity)
{
	m_ptVelocity.x = ptVelocity.x;
	m_ptVelocity.y = ptVelocity.y;
}
