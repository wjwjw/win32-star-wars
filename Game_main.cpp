#define WIN32_LEAN_AND_MEAN  // just say no to MFC

#include <windows.h>   // include all the windows headers
#include <windowsx.h>  // include useful macros
#include <mmsystem.h>  // very important and include WINMM.LIB too!
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "Sprite.h"
#include "Background.h"
#include "Bitmap.h"
#include "ScrollingBackground.h"
#include "SpriteManger.h"
#include "MySpriteEX.h"


#pragma comment(lib, "winmm.lib")
#pragma comment(lib, "msimg32.lib")

#define KEYDOWN(vk_code) ((GetAsyncKeyState(vk_code) & 0x8000) ? 1 : 0)
#define KEYUP(vk_code)   ((GetAsyncKeyState(vk_code) & 0x8000) ? 0 : 1)
//////////////////////////////////////////////////////////////////////


#define WINDOW_CLASS_NAME "WINCLASS1"

#define WINDOW_WIDTH  810
#define WINDOW_HEIGHT 600


// GLOBALS ////////////////////////////////////////////////
HWND      main_window_handle = NULL; // globally track main window
HINSTANCE hinstance_app      = NULL; // globally track hinstance


HDC         g_hOffscreenDC;
HBITMAP     g_hOffscreenBitmap;

BackgroundLayer*      g_pBG0Layer;
BackgroundLayer*      g_pBG01Layer;

Bitmap* g_ball_bitmap; //棒子位图
Bitmap* g_bullet_bitmap; //子弹位图
Bitmap*   g_pGameOverBitmap;//游戏结束位图
Bitmap*   g_pGameWiner;//游戏胜利


BOOL g_IsCan_Move_blue;
BOOL g_IsCan_Move_red;
MySprite* g_ball_blue_sprite;
MySprite* g_ball_red_sprite;

//子弹创建
MySprite* g_bullet_sprite;

bool Game_Over=false;


DWORD start_blue = GetTickCount();
DWORD start_red = GetTickCount();

int left_score=0;  //得分
int right_score=0;


int ball_frames[2][4] = {
	{0,1,2,3},
	{4,5,6,7}
};
int bullet_frames[][4]={
	{0,1,2,3},
	{4,5,6,7},
	{8,9,10,11},
	{12,13,14,15}
};

//???
//int g_y1 = 350;
//int g_y2 = 50;

int Is_run;
BOOL SpriteCollision(Sprite* pSpriteHitter, Sprite* pSpriteHittee)
{
	POINT  pVelocity = pSpriteHitter->GetVelocity();
	POINT  sVelocity = pSpriteHittee->GetVelocity();
	RECT  pPosition = pSpriteHitter->GetPosition();
	RECT  sPosition = pSpriteHittee->GetPosition();

	//if((pVelocity.x != 0 && pVelocity.y != 0) && (sVelocity.x != 0 && sVelocity.y != 0)) //子弹v子弹
	if(pSpriteHitter->GetLevel()&&pSpriteHittee->GetLevel())//子弹v子弹
	{
		//POINT ptSwapVelocity = pSpriteHitter->GetVelocity();
		//pSpriteHitter->SetVelocity(pSpriteHittee->GetVelocity());
		//pSpriteHittee->SetVelocity(ptSwapVelocity);
		RECT temp_Position = pPosition ;
		POINT temp_Velocity = pVelocity;
		int max_Level = pSpriteHitter->GetLevel()>pSpriteHittee->GetLevel()?pSpriteHitter->GetLevel():pSpriteHittee->GetLevel();
		if(max_Level<4)  //记录碰撞后等级
			max_Level++;
		else
			max_Level=4;
		if(pSpriteHittee->GetLevel()>pSpriteHitter->GetLevel())//删除等级小的精灵
		{
			Sprite* temp=pSpriteHitter;
			pSpriteHitter=pSpriteHittee;
			pSpriteHittee=temp;
		}
	    pSpriteHittee->Kill();

		if(pVelocity.x+sVelocity.x<max_Level  || pVelocity.y+sVelocity.y<max_Level )
		pSpriteHitter->SetVelocity((pVelocity.x+sVelocity.x)%4,(pVelocity.y+sVelocity.y)%4);      //初始化速度
		else
		pSpriteHitter->SetVelocity((pVelocity.x+sVelocity.x)/max_Level,(pVelocity.y+sVelocity.y)/max_Level);      //初始化速度
		pSpriteHitter->SetPosition((pPosition.left+sPosition.left)/2,(pPosition.top+sPosition.top)/2);  //初始坐标

		pSpriteHitter->SetLevel(max_Level); //初始化级别
		pSpriteHitter->SetFrameList(bullet_frames[max_Level-1],4); //行指针
	}
	else
	{
		sVelocity.x = sVelocity.x * (-1);
		//if( (pSpriteHittee->GetPosition().top-pSpriteHittee->GetHeight() == pSpriteHitter->GetPosition().top) ||
		//	(pSpriteHitter->GetPosition().top-pSpriteHitter->GetHeight() == pSpriteHittee->GetPosition().top)  )
		sVelocity.y = sVelocity.y*(-1);

		POINT temp = pSpriteHitter->GetVelocity();
		temp.x*=(-1);
		//if( (pSpriteHittee->GetPosition().top-pSpriteHittee->GetHeight() == pSpriteHitter->GetPosition().top) ||
		//	(pSpriteHitter->GetPosition().top-pSpriteHitter->GetHeight() == pSpriteHittee->GetPosition().top)  )
		temp.y*=(-1);
		pSpriteHitter->SetVelocity(temp);
     	pSpriteHitter->SetPosition(pPosition);
		pSpriteHittee->SetPosition(sPosition);
	}
	return true;
}
//BOOL SpriteCollision(Sprite* pSpriteHitter, Sprite* pSpriteHittee)
//{
// // if (pSpriteHitter->GetBitmap() == pSpriteHittee->GetBitmap() ) //子弹相撞
// //｛
// // ｝
//
//  if ((pSpriteHitter->GetBitmap() == g_ball_bitmap   && pSpriteHittee->GetBitmap() == g_bullet_bitmap) ||
// (pSpriteHitter->GetBitmap() == g_bullet_bitmap && pSpriteHittee->GetBitmap() == g_ball_bitmap))      //子弹撞棒子
//  {
//		POINT temp=pSpriteHittee->GetVelocity();
//		temp.x*=(-1);
//		pSpriteHittee->SetVelocity(temp);
//		temp=pSpriteHitter->GetVelocity();
//		temp.x*=(-1);
//		pSpriteHitter->SetVelocity(temp);	
//  }
//	return 0;
//}
 //FUNCTIONS //////////////////////////////////////////////
void SpriteDying(Sprite* pSpriteDying)
{
	//TODO
}


void GamePrint(HDC hDC)
{

	g_pBG0Layer->Draw(g_hOffscreenDC,0,0,false);
	DrawSprites(g_hOffscreenDC);


	// Draw the score
	char szText[64];
	RECT  rect = { 700, 0, 800, 50 };
	sprintf(szText, "life：%d", 50-left_score);
	SetBkMode(g_hOffscreenDC, TRANSPARENT);
	SetTextColor(g_hOffscreenDC, RGB(255, 255, 255));
	DrawText(g_hOffscreenDC, szText, -1, &rect, DT_SINGLELINE | DT_CENTER | DT_VCENTER);
	RECT  rect2 = { 10, 0, 100, 50 };
	sprintf(szText, "life：%d", 50-right_score);
	SetBkMode(g_hOffscreenDC, TRANSPARENT);
	SetTextColor(g_hOffscreenDC, RGB(255, 255, 255));
	DrawText(g_hOffscreenDC, szText, -1, &rect2, DT_SINGLELINE | DT_CENTER | DT_VCENTER);
	if((50-left_score)<1||(50-right_score)<1)
		Game_Over=true;


	// Blit the offscreen bitmap to the game screen
	BitBlt(hDC, 0, 0, WINDOW_WIDTH,WINDOW_HEIGHT, g_hOffscreenDC, 0, 0, SRCCOPY);

	// Cleanup
	ReleaseDC(main_window_handle, hDC);

	if(Game_Over)
	{
		g_pGameOverBitmap->Draw(hDC, 170, 150, TRUE, RGB(0,0,0));
		Sleep(1000);
		if((50-left_score)<1)
			g_pGameWiner->Draw(hDC, g_ball_blue_sprite->GetPosition().left-20,g_ball_blue_sprite->GetPosition().top-20, TRUE, RGB(0,0,0));
		else
			g_pGameWiner->Draw(hDC, g_ball_red_sprite->GetPosition().left-20,g_ball_red_sprite->GetPosition().top-20, TRUE, RGB(0,0,0));

	}

}

void GameStart(HDC hDC)
{
	//创建屏幕外设备环境和位图
	g_hOffscreenDC = CreateCompatibleDC(hDC);
	g_hOffscreenBitmap = CreateCompatibleBitmap(hDC,WINDOW_WIDTH, WINDOW_HEIGHT);
	SelectObject(g_hOffscreenDC, g_hOffscreenBitmap);

	RECT rcViewport1 = { 0, 0, 800, 600 };	 	  //左上角坐标，宽和高
	//g_pBG01Layer = new BackgroundLayer(hDC, "Res/clogo32.bmp");
	//g_pBG01Layer->SetViewport(rcViewport1);
	//g_pBG01Layer->SetDirection(SD_DOWN);
	//g_pBG01Layer->SetSpeed(2);
	//g_pBG01Layer->Draw(hDC,0,0,false);
	//Sleep(3000);

	g_pGameOverBitmap = new Bitmap(hDC, "Res/GameOver.bmp");
	g_pGameWiner = new Bitmap(hDC, "Res/win.bmp");

 	g_pBG0Layer = new BackgroundLayer(hDC, "Res/stars.bmp");
	g_pBG0Layer->SetViewport(rcViewport1);
	g_pBG0Layer->SetDirection(SD_DOWN);
	g_pBG0Layer->SetSpeed(2);

	//创建并加载位图
	g_ball_bitmap = new Bitmap(hDC, "Res/ball4x2(480x240).bmp");

	//创建精灵
	RECT    rcBounds = { 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT };
	g_ball_blue_sprite = new MySprite(g_ball_bitmap,rcBounds,BA_BOUNCE);
	g_ball_blue_sprite->SetPosition(25,WINDOW_HEIGHT/3);  //初始坐标

	g_ball_blue_sprite->SetRowAndCol(4,2); //位图按几列几行分
	g_ball_blue_sprite->SetDelayFrame(40);   //每40帧换一张
	g_ball_blue_sprite->SetFrameList(ball_frames[0],4); //行指针
	g_ball_blue_sprite->SetBShowAnim(true);  //带帧切换
	g_ball_blue_sprite->SetLevel(0);
	AddSprite(g_ball_blue_sprite);

	g_ball_red_sprite = new MySprite(g_ball_bitmap,rcBounds,BA_BOUNCE);
	g_ball_red_sprite->SetPosition(645,WINDOW_HEIGHT/3);
	g_ball_red_sprite->SetRowAndCol(4,2);
	g_ball_red_sprite->SetDelayFrame(40);
	g_ball_red_sprite->SetFrameList(ball_frames[1],4);
	g_ball_red_sprite->SetBShowAnim(true);
	g_ball_red_sprite->SetLevel(0);
	AddSprite(g_ball_red_sprite);

	GamePrint(hDC);

	g_IsCan_Move_blue = g_IsCan_Move_red = 1;
	Is_run = 0;
}

void Gamelogic(HDC hdc)
{
	UpdateSprites(left_score,right_score);
}

void HandleKeys(HDC hDC)
{
if(KEYDOWN('W') && g_IsCan_Move_blue)
	{
		g_ball_blue_sprite->SetPosition(g_ball_blue_sprite->GetPosition().left,g_ball_blue_sprite->GetPosition().top - 1);
		if(CheckSpriteCollision(g_ball_blue_sprite))
		g_ball_blue_sprite->SetPosition(g_ball_blue_sprite->GetPosition().left,g_ball_blue_sprite->GetPosition().top + 2);
	}
	if(KEYDOWN('S') && g_IsCan_Move_blue)
	{
		g_ball_blue_sprite->SetPosition(g_ball_blue_sprite->GetPosition().left,g_ball_blue_sprite->GetPosition().top + 1);
		if(CheckSpriteCollision(g_ball_blue_sprite))
		g_ball_blue_sprite->SetPosition(g_ball_blue_sprite->GetPosition().left,g_ball_blue_sprite->GetPosition().top - 2);

	}
	if( KEYDOWN('D') )
	{
		//TODO:new a blue ball in this point with v

		int x = g_ball_blue_sprite->GetPosition().left;
		int y = g_ball_blue_sprite->GetPosition().top;
		
		DWORD delay = (GetTickCount()-start_blue);
		if(delay>600 && m_vSprites.size()<10)
		{		
			g_bullet_bitmap = new Bitmap(hDC, "Res/bullet4x4(180x180).bmp"); //加载位图
			//创建精灵
			RECT    rcBounds = { 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT };
			g_bullet_sprite = new MySprite(g_bullet_bitmap,rcBounds,BA_BOUNCE);
			g_bullet_sprite->SetVelocity(-1,1);      //初始化速度
			g_bullet_sprite->SetPosition(x+100,y);  //初始坐标

			g_bullet_sprite->SetRowAndCol(4,4); //位图按几列几行分
			g_bullet_sprite->SetDelayFrame(40);   //每40帧换一张
			g_bullet_sprite->SetFrameList(bullet_frames[0],4); //行指针
			g_bullet_sprite->SetBShowAnim(true);  //带帧切换
			g_bullet_sprite->SetLevel(1); //初始化级别
			start_blue = GetTickCount();
			AddSprite(g_bullet_sprite);
		}
	}

	if(KEYDOWN(VK_UP) && g_IsCan_Move_red)
	{
		g_ball_red_sprite->SetPosition(g_ball_red_sprite->GetPosition().left,g_ball_red_sprite->GetPosition().top - 1);
		if(CheckSpriteCollision(g_ball_red_sprite))
		g_ball_red_sprite->SetPosition(g_ball_red_sprite->GetPosition().left,g_ball_red_sprite->GetPosition().top + 2);

	}
	if(KEYDOWN(VK_DOWN) && g_IsCan_Move_red)
	{
		g_ball_red_sprite->SetPosition(g_ball_red_sprite->GetPosition().left,g_ball_red_sprite->GetPosition().top + 1);
		if(CheckSpriteCollision(g_ball_red_sprite))
		g_ball_red_sprite->SetPosition(g_ball_red_sprite->GetPosition().left,g_ball_red_sprite->GetPosition().top - 2);
	}
	if( KEYDOWN(VK_LEFT) )
	{
		//TODO:new a red ball in this point with v
		int x = g_ball_red_sprite->GetPosition().left;
		int y = g_ball_red_sprite->GetPosition().top;

		DWORD delay = (GetTickCount()-start_red);
		if(delay>600 && m_vSprites.size()<10)
		{

		g_bullet_bitmap = new Bitmap(hDC, "Res/bullet4x4(180x180).bmp"); //加载子弹位图
		//创建精灵
		RECT    rcBounds = { 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT };
		g_bullet_sprite = new MySprite(g_bullet_bitmap,rcBounds,BA_BOUNCE);
		g_bullet_sprite->SetVelocity(1,-1);      //初始化速度
		g_bullet_sprite->SetPosition(x-40,y);  //初始坐标

		g_bullet_sprite->SetRowAndCol(4,4); //位图按几列几行分
		g_bullet_sprite->SetDelayFrame(40);   //每40帧换一张
		g_bullet_sprite->SetFrameList(bullet_frames[0],4); //行指针
		g_bullet_sprite->SetBShowAnim(true);  //带帧切换
		g_bullet_sprite->SetLevel(1); //初始化级别

		start_red = GetTickCount();
		AddSprite(g_bullet_sprite);

		}
	}

}

void Gameend()
{

}
void MouseMove(int x, int y)
{
}

void MouseButtonDown(int x, int y, BOOL bLeft)
{
}

void MouseButtonUp(int x, int y, BOOL bLeft)
{
}
/////////////////////////////////////////////////////////////////////



LRESULT CALLBACK WindowProc(HWND hwnd, 
							UINT msg, 
                            WPARAM wparam, 
                            LPARAM lparam)
{
	PAINTSTRUCT		ps;		// used in WM_PAINT
	HDC				hdc;	// handle to a device context

	switch(msg)
	{
	case WM_CREATE: 
		{			
			return(0);
		} break;
				
	case WM_PAINT: 
		{
			hdc = BeginPaint(hwnd,&ps);	 
			
			EndPaint(hwnd,&ps);
			return(0);
		} break;
		
	case WM_LBUTTONDOWN:
		MouseButtonDown(LOWORD(lparam), HIWORD(lparam), TRUE);
		return 0;
    case WM_LBUTTONUP:
		MouseButtonUp(LOWORD(lparam), HIWORD(lparam), TRUE);
		return 0;
    case WM_RBUTTONDOWN:
		MouseButtonDown(LOWORD(lparam), HIWORD(lparam), FALSE);
		return 0;
	case WM_RBUTTONUP:
		MouseButtonUp(LOWORD(lparam), HIWORD(lparam), FALSE);
		return 0;
    case WM_MOUSEMOVE:
		MouseMove(LOWORD(lparam), HIWORD(lparam));
		return 0;
		
	case WM_DESTROY: 
		{
			PostQuitMessage(0);
			return(0);
		} break;
		
	default:break;
		
    }
	return (DefWindowProc(hwnd, msg, wparam, lparam));
}
// WIMMAIN //////////////////////////////////////////////
int WINAPI WinMain(	HINSTANCE hinstance,
				   HINSTANCE hprevinstance,
				   LPSTR lpcmdline,
				   int ncmdshow)
{
	WNDCLASSEX winclass; // this will hold the class we create
	HWND	   hwnd;	 // generic window handle
	MSG		   msg;		 // generic message
	HDC        hdc;      // graphics device context
	
	winclass.cbSize         = sizeof(WNDCLASSEX);
	winclass.style			= CS_DBLCLKS | CS_OWNDC | 
		CS_HREDRAW | CS_VREDRAW;
	winclass.lpfnWndProc	= WindowProc;
	winclass.cbClsExtra		= 0;
	winclass.cbWndExtra		= 0;
	winclass.hInstance		= hinstance;
	winclass.hIcon			= LoadIcon(hinstance, IDI_APPLICATION);
	winclass.hCursor		= LoadCursor(NULL, IDC_ARROW); 
	winclass.hbrBackground	= (HBRUSH)GetStockObject(BLACK_BRUSH);
	winclass.lpszMenuName	= NULL;
	winclass.lpszClassName	= WINDOW_CLASS_NAME;
	winclass.hIconSm        = LoadIcon(hinstance, IDI_APPLICATION);
	
	hinstance_app = hinstance;
	
	if (!RegisterClassEx(&winclass))
		return(0);
	
	if (!(hwnd = CreateWindowEx(NULL,                // extended style
		WINDOW_CLASS_NAME,   // class
		"星球大战", // title
		WS_OVERLAPPEDWINDOW | WS_VISIBLE,
		0,0,	  // initial x,y
		WINDOW_WIDTH, // initial width
		WINDOW_HEIGHT,// initial height
		NULL,	  // handle to parent 
		NULL,	  // handle to menu
		hinstance,// instance of this application
		NULL)))	// extra creation parms
		return(0);
	
	main_window_handle = hwnd;
	
	hdc = GetDC(hwnd);
	

	GameStart(hdc);


	while(TRUE)
	{
		DWORD start = GetTickCount();
		if (PeekMessage(&msg,NULL,0,0,PM_REMOVE))
		{ 
			if (msg.message == WM_QUIT)
				break;
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		} 

		if(Game_Over)
			continue;
		if(Is_run == 0)
		{
			//f();
			Is_run = 1;
		}
		else
		{
			HandleKeys(hdc);
			Gamelogic(hdc);
			GamePrint(hdc);
		}

		DWORD delay = (GetTickCount()-start);
		if(delay <30)
			Sleep(delay- (GetTickCount()-start));		
	}
	
	return(msg.wParam);
}