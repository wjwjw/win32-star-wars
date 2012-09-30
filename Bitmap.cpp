#include "Bitmap.h"


Bitmap::Bitmap(HDC hDC, LPTSTR szFileName)
: m_hBitmap(NULL), m_iWidth(0), m_iHeight(0)
{
	LoadByAPI(hDC, szFileName);
}

// Create a bitmap from a resource
Bitmap::Bitmap(HDC hDC, UINT uiResID, HINSTANCE hInstance)
: m_hBitmap(NULL), m_iWidth(0), m_iHeight(0)
{
	Create(hDC, uiResID, hInstance);
}

Bitmap::Bitmap(HDC hDC, int iWidth, int iHeight, COLORREF crColor)
: m_hBitmap(NULL), m_iWidth(0), m_iHeight(0)
{
	Create(hDC, iWidth, iHeight, crColor);
}

Bitmap::~Bitmap()
{
	Free();
}


void Bitmap::Free()
{
	if (m_hBitmap != NULL)
	{
		DeleteObject(m_hBitmap);
		m_hBitmap = NULL;
	}
}


RGBQUAD palette[256];

BOOL Bitmap::Load8(HDC hDC, LPTSTR szFileName)
{
	Free();
	
	HANDLE hFile = CreateFile(szFileName, GENERIC_READ, FILE_SHARE_READ ,NULL,
		OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if(hFile == INVALID_HANDLE_VALUE)
		return false;
	
	BITMAPFILEHEADER bmfHeader;//位图文件头信息
	DWORD dwBytesRead;
	int bval = ReadFile(hFile, &bmfHeader,sizeof(BITMAPFILEHEADER),&dwBytesRead, NULL);
	if( (!bval) || (dwBytesRead!=sizeof(BITMAPFILEHEADER)) ||
		(bmfHeader.bfType!=0x4D42))
	{
		CloseHandle(hFile);
		return false;
	}
	
	BITMAPINFO bmi;	
	bval = ReadFile(hFile, &bmi, sizeof(BITMAPINFOHEADER), &dwBytesRead, NULL);
	if( (!bval) || (dwBytesRead!=sizeof(BITMAPINFOHEADER)))
	{
		CloseHandle(hFile);
		Free();
		return false;
	}
	
	int biheight = bmi.bmiHeader.biHeight;
	int bicount = bmi.bmiHeader.biBitCount;
	m_iWidth = (int)bmi.bmiHeader.biWidth;
	m_iHeight = (int)bmi.bmiHeader.biHeight;
	
	//读取调色板信息
	bval = ReadFile(hFile, palette, sizeof(RGBQUAD)*256, &dwBytesRead, NULL);
	if( (!bval) || (dwBytesRead!=sizeof(RGBQUAD)*256))
	{
		CloseHandle(hFile);
		Free();
		return false;
	}
	
	PBYTE pBitmapBits;
	m_hBitmap = CreateDIBSection(hDC, &bmi, DIB_PAL_COLORS,  (VOID**)&pBitmapBits, NULL, 0);
	if( (m_hBitmap!=NULL) && (pBitmapBits!=NULL))
	{
		bval = ReadFile(hFile, pBitmapBits, bmi.bmiHeader.biSizeImage, &dwBytesRead, NULL);
		if(bval)
			return true;
	}
	
	Free();  
	return false;
}


BOOL Bitmap::Load24(HDC hDC, LPTSTR szFileName)
{
	Free();
	
	HANDLE hFile = CreateFile(szFileName, GENERIC_READ, FILE_SHARE_READ ,NULL,
		OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if(hFile == INVALID_HANDLE_VALUE)
		return false;
	
	BITMAPFILEHEADER bmfHeader;//位图文件头信息
	DWORD dwBytesRead;
	int bval = ReadFile(hFile, &bmfHeader,sizeof(BITMAPFILEHEADER),&dwBytesRead, NULL);
	if( (!bval) || (dwBytesRead!=sizeof(BITMAPFILEHEADER)) ||
		(bmfHeader.bfType!=0x4D42))
	{
		CloseHandle(hFile);
		return false;
	}
	
	BITMAPINFO bmi;
	bval = ReadFile(hFile, &bmi, sizeof(BITMAPINFOHEADER), &dwBytesRead, NULL);
	if( (!bval) || (dwBytesRead!=sizeof(BITMAPINFOHEADER)))
	{
		CloseHandle(hFile);
		Free();
		return false;
	}
	
	m_iWidth = (int)bmi.bmiHeader.biWidth;
	m_iHeight = (int)bmi.bmiHeader.biHeight;
	
	
	PBYTE pBitmapBits;
	m_hBitmap = CreateDIBSection(hDC, &bmi, DIB_RGB_COLORS,  (VOID**)&pBitmapBits, NULL, 0);
	if( (m_hBitmap!=NULL) && (pBitmapBits!=NULL))
	{
		SetFilePointer(hFile, bmfHeader.bfOffBits, NULL, FILE_BEGIN);
		bval = ReadFile(hFile, pBitmapBits, bmi.bmiHeader.biSizeImage, &dwBytesRead, NULL);
		if(bval)
			return true;
	}
	
	Free();  
	return false;
}

BOOL Bitmap::LoadByAPI(HDC hDC, LPTSTR szFileName)
{
	Free();
	
	HANDLE hFile = CreateFile(szFileName, GENERIC_READ, FILE_SHARE_READ ,NULL,
		OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if(hFile == INVALID_HANDLE_VALUE)
		return false;
	
	BITMAPFILEHEADER bmfHeader;//位图文件头信息
	DWORD dwBytesRead;
	int bval = ReadFile(hFile, &bmfHeader,sizeof(BITMAPFILEHEADER),&dwBytesRead, NULL);
	if( (!bval) || (dwBytesRead!=sizeof(BITMAPFILEHEADER)) ||
		(bmfHeader.bfType!=0x4D42))
	{
		CloseHandle(hFile);
		return false;
	}
	
	BITMAPINFO bmi;
	bval = ReadFile(hFile, &bmi, sizeof(BITMAPINFOHEADER), &dwBytesRead, NULL);
	if( (!bval) || (dwBytesRead!=sizeof(BITMAPINFOHEADER)))
	{
		CloseHandle(hFile);
		Free();
		return false;
	}
	
	m_iWidth = (int)bmi.bmiHeader.biWidth;
	m_iHeight = (int)bmi.bmiHeader.biHeight;
	
	m_hBitmap = (HBITMAP)LoadImage(NULL, szFileName, IMAGE_BITMAP, 0,0,LR_LOADFROMFILE|LR_CREATEDIBSECTION);
	if(m_hBitmap == NULL)
		return false;
	
	return true;
}

BOOL Bitmap::Create(HDC hDC, int iWidth, int iHeight, COLORREF crColor)
{
	m_hBitmap = CreateCompatibleBitmap(hDC, iWidth, iHeight);
	if (m_hBitmap == NULL)
		return false;
	
	m_iWidth = iWidth;
	m_iHeight = iHeight;
	
	HDC hMemDC = CreateCompatibleDC(hDC);
	
	HBRUSH hBrush = CreateSolidBrush(crColor);
	
	HBITMAP hOldBitmap = (HBITMAP)SelectObject(hMemDC, m_hBitmap);
	
	RECT rcBitmap = { 0, 0, m_iWidth, m_iHeight };
	FillRect(hMemDC, &rcBitmap, hBrush);
	
	SelectObject(hMemDC, hOldBitmap);
	
	DeleteDC(hMemDC);
	DeleteObject(hBrush);
	
	return true;
}


BOOL Bitmap::Create(HDC hDC, UINT uiResID, HINSTANCE hInstance)
{
	// Free any previous DIB info
	Free();
	
	// Find the bitmap resource
	HRSRC hResInfo = FindResource(hInstance, MAKEINTRESOURCE(uiResID), RT_BITMAP);
	if (hResInfo == NULL)
		return FALSE;
	
	// Load the bitmap resource
	HGLOBAL hMemBitmap = LoadResource(hInstance, hResInfo);
	if (hMemBitmap == NULL)
		return FALSE;
	
	// Lock the resource and access the entire bitmap image
	PBYTE pBitmapImage = (BYTE*)LockResource(hMemBitmap);
	if (pBitmapImage == NULL)
	{
		FreeResource(hMemBitmap);
		return FALSE;
	}
	
	// Store the width and height of the bitmap
	BITMAPINFO* pBitmapInfo = (BITMAPINFO*)pBitmapImage;
	m_iWidth = (int)pBitmapInfo->bmiHeader.biWidth;
	m_iHeight = (int)pBitmapInfo->bmiHeader.biHeight;
	
	// Get a handle to the bitmap and copy the image bits
	PBYTE pBitmapBits;
	m_hBitmap = CreateDIBSection(hDC, pBitmapInfo, DIB_RGB_COLORS,
		(PVOID*)&pBitmapBits, NULL, 0);
	if ((m_hBitmap != NULL) && (pBitmapBits != NULL))
	{
		const PBYTE pTempBits = pBitmapImage + pBitmapInfo->bmiHeader.biSize +
			pBitmapInfo->bmiHeader.biClrUsed * sizeof(RGBQUAD);
		CopyMemory(pBitmapBits, pTempBits, pBitmapInfo->bmiHeader.biSizeImage);
		
		// Unlock and free the bitmap graphics object
		UnlockResource(hMemBitmap);
		FreeResource(hMemBitmap);
		return TRUE;
	}
	
	// Something went wrong, so cleanup everything
	UnlockResource(hMemBitmap);
	FreeResource(hMemBitmap);
	Free();
	return FALSE;
}


void Bitmap::Draw(HDC hDC, int x, int y)
{
	if (m_hBitmap != NULL)
	{
		HDC hMemDC = CreateCompatibleDC(hDC);
		
		HBITMAP hOldBitmap = (HBITMAP)SelectObject(hMemDC, m_hBitmap);
		
		//对于8位位图，需要设置逻辑调色板
		//		SetDIBColorTable(hMemDC, 0, 256, palette);
		
		BitBlt(hDC, x, y, m_iWidth, m_iHeight, hMemDC, 0, 0, SRCCOPY);
		
		SelectObject(hMemDC, hOldBitmap);
		DeleteDC(hMemDC);
	}
}

void Bitmap::Draw(HDC hDC, int x, int y, bool bTrans, COLORREF crTransColor)
{
/*
if (m_hBitmap != NULL)
{
HDC hMemDC = CreateCompatibleDC(hDC);

		HBITMAP hOldBitmap = (HBITMAP)SelectObject(hMemDC, m_hBitmap);
		
		  //对于8位位图，需要设置逻辑调色板
		  //		SetDIBColorTable(hMemDC, 0, 256, palette);
		  if(!bTrans)
		  BitBlt(hDC, x, y, m_iWidth, m_iHeight, hMemDC, 0, 0, SRCCOPY);
		  
			else
			TransparentBlt(hDC, x, y, m_iWidth, m_iHeight, hMemDC, 0, 0, m_iWidth, m_iHeight, crTransColor); 
			
			  SelectObject(hMemDC, hOldBitmap);
			  DeleteDC(hMemDC);
			  }
	*/
	//修改Draw方法
	DrawPart(hDC, x, y, 0,0,GetWidth(), GetHeight(), bTrans, crTransColor);
}


void Bitmap::DrawPart(HDC hDC, int x, int y, int xPart, int yPart,int wPart, int hPart, bool bTrans, COLORREF crTransColor)
{
	if (m_hBitmap != NULL)
	{
		HDC hMemDC = CreateCompatibleDC(hDC);
		
		HBITMAP hOldBitmap = (HBITMAP)SelectObject(hMemDC, m_hBitmap);
		
		//对于8位位图，需要设置逻辑调色板
		//		SetDIBColorTable(hMemDC, 0, 256, palette);
		if(!bTrans)
			BitBlt(hDC, x, y, wPart, hPart, hMemDC, xPart, yPart, SRCCOPY);
		
		else
			TransparentBlt(hDC, x, y, wPart, hPart, hMemDC, xPart, yPart, wPart, hPart, crTransColor); 
		
		SelectObject(hMemDC, hOldBitmap);
		DeleteDC(hMemDC);
	}
}
