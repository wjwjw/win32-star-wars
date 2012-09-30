/*精灵管理器*/

#include "Sprite.h"
#include <vector>
using namespace std;

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////

extern vector<Sprite*>     m_vSprites;

//精灵碰撞响应函数接口，由特定的游戏实现
BOOL SpriteCollision(Sprite* pSpriteHitter, Sprite* pSpriteHittee);

//精灵被删除时的事件响应函数
void SpriteDying(Sprite* pSprite);


BOOL CheckSpriteCollision(Sprite* pTestSprite);

void AddSprite(Sprite* pSprite);

void DrawSprites(HDC hDC);

void UpdateSprites(int&,int&);

void CleanupSprites();

Sprite* IsPointInSprite(int x, int y);