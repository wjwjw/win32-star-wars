#pragma once

//-----------------------------------------------------------------
// Include Files
//-----------------------------------------------------------------
#include "Bitmap.h"

class Background
{

protected :
	int m_iWidth, m_iHeight;//�����Ŀ��
	COLORREF m_crColor;//��������ɫ��ֻ�����ڴ�ɫ����
	Bitmap* m_pBitmap;//��������ͼ�񱳾���λͼͼ��

public:
	//���촿ɫ����
	Background(int iWidth, int iHeight, COLORREF crColor);
	//����ͼ�񱳾�
	Background(Bitmap* pBitmap);
	virtual ~Background();

	//���±�������ۣ����ڴ�ɫ������ͼ�񱳾����������κ�����
	//�ú�����Ŀ����ʹ�����Ķ��������ܹ�ʹ��������������
	virtual void Update();
	virtual void Draw(HDC hDC);

	int GetWidth(){return m_iWidth;}
	int GetHeight(){return m_iHeight;}
};