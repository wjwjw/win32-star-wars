#include "SpriteManger.h"

vector<Sprite*>     m_vSprites;

void AddSprite(Sprite* pSprite)
{
	// Add a sprite to the sprite vector
	if (pSprite != NULL)
	{
		// See if there are sprites already in the sprite vector
		if (m_vSprites.size() > 0)
		{
			// Find a spot in the sprite vector to insert the sprite by its z-order
			vector<Sprite*>::iterator siSprite;
			for (siSprite = m_vSprites.begin(); siSprite != m_vSprites.end(); siSprite++)
				if (pSprite->GetZOrder() < (*siSprite)->GetZOrder())
				{
					// Insert the sprite into the sprite vector
					if(m_vSprites.capacity()<10)
					m_vSprites.insert(siSprite, pSprite);
					return;
				}
		}

		// The sprite's z-order is highest, so add it to the end of the vector
		m_vSprites.push_back(pSprite);
	}
}

void DrawSprites(HDC hDC)
{
	// Draw the sprites in the sprite vector
	vector<Sprite*>::iterator siSprite;
	for (siSprite = m_vSprites.begin(); siSprite != m_vSprites.end(); siSprite++)
		(*siSprite)->Draw(hDC);
}

void UpdateSprites(int &left_score,int &right_score)
{
	// Check to see if the sprite vector needs to grow
	if (m_vSprites.size() >= (m_vSprites.capacity() / 2))
		m_vSprites.reserve(m_vSprites.capacity() * 2);

	// Update the sprites in the sprite vector
	RECT          rcOldSpritePos;
	SPRITEACTION  saSpriteAction;
	vector<Sprite*>::iterator siSprite;
	for (siSprite = m_vSprites.begin(); siSprite != m_vSprites.end(); siSprite++)
	{
		// Save the old sprite position in case we need to restore it
		rcOldSpritePos = (*siSprite)->GetPosition();

		// Update the sprite
		saSpriteAction = (*siSprite)->Update(left_score,right_score);

		// Handle the SA_KILL sprite action
		if (saSpriteAction & SA_KILL)
		{
			SpriteDying(*siSprite);
			delete (*siSprite);
			siSprite = m_vSprites.erase(siSprite);
			if(siSprite != m_vSprites.begin()) 
				siSprite--;
			continue;
		}

		bool Collision=CheckSpriteCollision(*siSprite);
		// See if the sprite collided with any others
		if (Collision)
			// Restore the old sprite position
			(*siSprite)->SetPosition(rcOldSpritePos);
	}
}

void CleanupSprites()
{
	// Delete and remove the sprites in the sprite vector
	vector<Sprite*>::iterator siSprite;
	for (siSprite = m_vSprites.begin(); siSprite != m_vSprites.end(); siSprite++)
	{
		delete (*siSprite);
		//	m_vSprites.erase(siSprite);
		//	siSprite--;
	}
	m_vSprites.clear();
}

Sprite* IsPointInSprite(int x, int y)
{
	// See if the point is in a sprite in the sprite vector
	vector<Sprite*>::reverse_iterator siSprite;
	for (siSprite = m_vSprites.rbegin(); siSprite != m_vSprites.rend(); siSprite++)
		if (!(*siSprite)->IsHidden() && (*siSprite)->IsPointInside(x, y))
			return (*siSprite);

	// The point is not in a sprite
	return NULL;
}


BOOL CheckSpriteCollision(Sprite* pTestSprite)
{
	// See if the sprite has collided with any other sprites
	vector<Sprite*>::iterator siSprite;
	for (siSprite = m_vSprites.begin(); siSprite != m_vSprites.end(); siSprite++)
	{
		// Make sure not to check for collision with itself
		if (pTestSprite == (*siSprite))
			continue;

		// Test the collision
		if (pTestSprite->TestCollision(*siSprite))
			// Collision detected
			return SpriteCollision((*siSprite), pTestSprite);
	}

	// No collision
	return FALSE;
}
