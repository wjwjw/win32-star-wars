//-----------------------------------------------------------------
// Sprite Object
// C++ Source - Sprite.cpp
//-----------------------------------------------------------------

//-----------------------------------------------------------------
// Include Files
//-----------------------------------------------------------------
#include "Sprite.h"

//-----------------------------------------------------------------
// Sprite Constructor(s)/Destructor
//-----------------------------------------------------------------

Sprite::Sprite(Bitmap* pBitmap)
{
	// Initialize the member variables
	m_pBitmap = pBitmap;
	SetRect(&m_rcPosition, 0, 0, pBitmap->GetWidth(), pBitmap->GetHeight());

	CalcCollisionRect();//（add）
	
	m_ptVelocity.x = m_ptVelocity.y = 0;
	m_iZOrder = 0;
	SetRect(&m_rcBounds, 0, 0, 640, 480);
	m_baBoundsAction = BA_STOP;
	m_bHidden = FALSE;

	m_iRow = m_iCol = 1;
	m_iNumFrame = 1;
	m_iCurFrame = 0;
	m_iDelayFrame=0;
	m_iTriggerFrame=0;
	pFrameList = NULL;

	m_bShowAnim = false;

	m_bDying = FALSE;
	m_bOneCycle = FALSE;

}

Sprite::Sprite(Bitmap* pBitmap, RECT& rcBounds, BOUNDSACTION baBoundsAction)
{
	// Calculate a random position
	int iXPos = rand() % (rcBounds.right - rcBounds.left);
	int iYPos = rand() % (rcBounds.bottom - rcBounds.top);
	
	// Initialize the member variables
	m_pBitmap = pBitmap;
	SetRect(&m_rcPosition, iXPos, iYPos, iXPos + pBitmap->GetWidth(),
		iYPos + pBitmap->GetHeight());

	CalcCollisionRect();//（add）

	m_ptVelocity.x = m_ptVelocity.y = 0;
	m_iZOrder = 0;
	CopyRect(&m_rcBounds, &rcBounds);
	m_baBoundsAction = baBoundsAction;
	m_bHidden = FALSE;

	m_iRow = m_iCol = 1;
	m_iNumFrame = 1;
	m_iCurFrame = 0;
	m_iDelayFrame=0;
	m_iTriggerFrame=0;
	pFrameList = NULL;
	m_bShowAnim = false;

	m_bDying = FALSE;
	m_bOneCycle = FALSE;

}

Sprite::Sprite(Bitmap* pBitmap, POINT ptPosition, POINT ptVelocity, int iZOrder,
			   RECT& rcBounds, BOUNDSACTION baBoundsAction)
{
	// Initialize the member variables
	m_pBitmap = pBitmap;
	SetRect(&m_rcPosition, ptPosition.x, ptPosition.y,
		ptPosition.x + pBitmap->GetWidth(), ptPosition.y + pBitmap->GetHeight());

	CalcCollisionRect();//（add）

	m_ptVelocity = ptVelocity;
	m_iZOrder = iZOrder;
	CopyRect(&m_rcBounds, &rcBounds);
	m_baBoundsAction = baBoundsAction;
	m_bHidden = FALSE;

	m_iRow = m_iCol = 1;
	m_iNumFrame = 1;
	m_iCurFrame = 0;
	m_iDelayFrame=0;
	m_iTriggerFrame=0;
	pFrameList = NULL;
	m_bShowAnim = false;
	m_bDying = FALSE;
	m_bOneCycle = FALSE;
}

Sprite::~Sprite()
{
	if(pFrameList != NULL)
	{
		delete[] pFrameList;
		pFrameList = NULL;
	}
}


void Sprite::SetFrameList(int array[], int iNumFrame)
{
	m_iNumFrame = iNumFrame;
	m_iCurFrame = 0;
	if(pFrameList != NULL)
	{
		delete[] pFrameList;
		pFrameList = NULL;
	}
	pFrameList = new int[iNumFrame];
	memcpy(pFrameList, array,sizeof(int)*iNumFrame);

	//重新计算位置矩形
	RECT rcPos = GetPosition();
	rcPos.bottom = rcPos.top + GetHeight();
	rcPos.right = rcPos.left + GetWidth();
	SetPosition(rcPos);

}
void Sprite::SetDelayFrame(int iDelayFrame)
{
	m_iDelayFrame = iDelayFrame;
}


//-----------------------------------------------------------------
// Sprite General Methods
//-----------------------------------------------------------------
SPRITEACTION Sprite::Update(int &left_score,int &right_score)//完成精灵移动和碰撞检测
{
	//查看是否需要删除精灵
	if(m_bDying)
		return SA_KILL;


	//更新帧
	UpdateFrame();

	//更新位置
	POINT ptNewPosition, ptSpriteSize, ptBoundsSize;
	ptNewPosition.x = m_rcPosition.left + m_ptVelocity.x;
	ptNewPosition.y = m_rcPosition.top + m_ptVelocity.y;
	ptSpriteSize.x = m_rcPosition.right - m_rcPosition.left;
	ptSpriteSize.y = m_rcPosition.bottom - m_rcPosition.top;
	ptBoundsSize.x = m_rcBounds.right - m_rcBounds.left;
	ptBoundsSize.y = m_rcBounds.bottom - m_rcBounds.top;
	
	// 检查边界动作
	// 环绕?
	if (m_baBoundsAction == BA_WRAP)
	{
		if ((ptNewPosition.x + ptSpriteSize.x) < m_rcBounds.left)
			ptNewPosition.x = m_rcBounds.right;//到了左边，就移动到右边边界
		else if (ptNewPosition.x > m_rcBounds.right)
			ptNewPosition.x = m_rcBounds.left - ptSpriteSize.x;
		if ((ptNewPosition.y + ptSpriteSize.y) < m_rcBounds.top)
			ptNewPosition.y = m_rcBounds.bottom;
		else if (ptNewPosition.y > m_rcBounds.bottom)
			ptNewPosition.y = m_rcBounds.top - ptSpriteSize.y;
	}
	// 弹开?（更改速度方向）
	else if (m_baBoundsAction == BA_BOUNCE)
	{
		BOOL bBounce = FALSE;
		POINT ptNewVelocity = m_ptVelocity;
		if (ptNewPosition.x < m_rcBounds.left)
		{
			bBounce = TRUE;
			ptNewPosition.x = m_rcBounds.left;
			ptNewVelocity.x = -ptNewVelocity.x;
		}
		else if ((ptNewPosition.x + ptSpriteSize.x) > m_rcBounds.right)
		{
			bBounce = TRUE;
			ptNewPosition.x = m_rcBounds.right - ptSpriteSize.x;
			ptNewVelocity.x = -ptNewVelocity.x;
		}
		if (ptNewPosition.y < m_rcBounds.top)
		{
			bBounce = TRUE;
			ptNewPosition.y = m_rcBounds.top;
			ptNewVelocity.y = -ptNewVelocity.y;
		}
		else if ((ptNewPosition.y + ptSpriteSize.y) > m_rcBounds.bottom)
		{
			bBounce = TRUE;
			ptNewPosition.y = m_rcBounds.bottom - ptSpriteSize.y;
			ptNewVelocity.y = -ptNewVelocity.y;
		}
		if (bBounce)//重设速度
			SetVelocity(ptNewVelocity);
	}

	//////////////////////（add）/////////////////////////////////
	// Die?
	else if (m_baBoundsAction == BA_DIE)
	{
		if ((ptNewPosition.x + ptSpriteSize.x) < m_rcBounds.left ||
			ptNewPosition.x > m_rcBounds.right ||
			(ptNewPosition.y + ptSpriteSize.y) < m_rcBounds.top ||
			ptNewPosition.y > m_rcBounds.bottom)
			return SA_KILL;
	}
	////////////////////////（add）/////////////////////////////////


	//停止(默认)
	else
	{
		//改变速度，以停止精灵
		if (ptNewPosition.x  < m_rcBounds.left ||
			ptNewPosition.x > (m_rcBounds.right - ptSpriteSize.x))
		{
			ptNewPosition.x = max(m_rcBounds.left, min(ptNewPosition.x,
				m_rcBounds.right - ptSpriteSize.x));
			SetVelocity(0, 0);
		}
		if (ptNewPosition.y  < m_rcBounds.top ||
			ptNewPosition.y > (m_rcBounds.bottom - ptSpriteSize.y))
		{
			ptNewPosition.y = max(m_rcBounds.top, min(ptNewPosition.y,
				m_rcBounds.bottom - ptSpriteSize.y));
			SetVelocity(0, 0);
		}
	}
	SetPosition(ptNewPosition);
	if(ptNewPosition.x>645+60 && this->GetHeight()<100)
	{
		left_score+=Level;
		this->Kill();
		return SA_KILL;
	}
	else if(ptNewPosition.x<25  && this->GetHeight()<100)
	{
		right_score+=Level;
		this->Kill();
		return SA_KILL;
	}

	return SA_NONE;//（add）
}

void Sprite::Draw(HDC hDC)
{
	// Draw the sprite if it isn't hidden
	if (m_pBitmap != NULL && !m_bHidden)
	{
		if(m_iNumFrame <= 1)
			m_pBitmap->Draw(hDC, m_rcPosition.left, m_rcPosition.top, TRUE, RGB(255,255,255));
		else 
			m_pBitmap->DrawPart(hDC,m_rcPosition.left,m_rcPosition.top,
			(pFrameList[m_iCurFrame]%m_iRow)*GetWidth(),
			(pFrameList[m_iCurFrame]/m_iRow)*GetHeight(),
			GetWidth(),GetHeight(),true,RGB(0,0,0));
	}
}


void Sprite::UpdateFrame()
{
	if(m_bShowAnim)
	{
		++m_iTriggerFrame;
		if(m_iTriggerFrame>=m_iDelayFrame)//要切换到下一帧了
		{

			++m_iCurFrame;
			if(m_iCurFrame>=m_iNumFrame)
				m_iCurFrame = 0;

			m_iTriggerFrame = 0;
		}
	}
	else //停止播放动画帧
	{
			if(m_bOneCycle)
				m_bDying = true;
			else
				m_iCurFrame = 0;
	}
	

}
