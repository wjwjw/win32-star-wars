#include "MySpriteEX.h"

MySprite::MySprite(Bitmap* pBitmap) : Sprite(pBitmap)
{

}

MySprite::MySprite(Bitmap* pBitmap, RECT& rcBounds, BOUNDSACTION baBoundsAction):Sprite(pBitmap, rcBounds, baBoundsAction)
{

}

MySprite::MySprite(Bitmap* pBitmap, POINT ptPosition, POINT ptVelocity, int iZOrder,
				   RECT& rcBounds, BOUNDSACTION baBoundsAction) :Sprite(pBitmap,ptPosition,ptVelocity,iZOrder,
				   rcBounds,baBoundsAction = BA_STOP)
{

}