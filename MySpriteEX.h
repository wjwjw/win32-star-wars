#include"Sprite.h"
#include <math.h>

class MySprite : public Sprite
{
private:
	POINT m_center;
	int m_radius;
	int m_lever;

public:

	MySprite(Bitmap* pBitmap);
	MySprite(Bitmap* pBitmap, RECT& rcBounds, BOUNDSACTION baBoundsAction);
	MySprite(Bitmap* pBitmap, POINT ptPosition, POINT ptVelocity, int iZOrder,
		RECT& rcBounds, BOUNDSACTION baBoundsAction = BA_STOP);

	void SetCenter(POINT c);
	void SetRadius(int r);
	void SetLever(int l);
	POINT GetCenter();
	int GetRadius();
	int GetLever();

	bool TestCollision(MySprite* pTestSprite);
};

//-----------------------------------------------------------------
// Sprite Inline General Methods
//-----------------------------------------------------------------


//-----------------------------------------------------------------
// Sprite Inline Helper Methods
//-----------------------------------------------------------------

///////////////////////////////（add）///////////////////////////////////////
inline void MySprite :: SetCenter(POINT c) //设置圆心
{
	m_center = c;
}

inline void MySprite :: SetRadius(int r) //半径
{
	m_radius = r;
}

inline void MySprite :: SetLever(int l) //类级别
{
	m_lever = l;
}

inline POINT MySprite :: GetCenter()  //得圆心
{
	return m_center;
}

inline int MySprite :: GetRadius()  //得半径
{
	return m_radius;
}

inline int MySprite :: GetLever()//得级别
{
	return m_lever;
}

inline bool MySprite :: TestCollision(MySprite* pTestSprite) //判断碰撞
{
	POINT pt2 = pTestSprite->GetCenter();
	double dist = sqrt((double)(m_center.x - pt2.x)*(m_center.x - pt2.x) + (double)(m_center.y - pt2.y)*(m_center.y - pt2.y));
	return (dist <= (m_radius + pTestSprite->GetRadius())) ? 1 : 0;
}
