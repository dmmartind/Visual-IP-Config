///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//Copyright 2007 David Martin. All Rights Reserved.
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "WMFile.h"
#include "wingdi.h"
//#include "func.h"
#if (_WIN32_WINNT >= 0x0500)
WINGDIAPI COLORREF WINAPI SetDCBrushColor(IN HDC, IN COLORREF);
WINGDIAPI COLORREF WINAPI SetDCPenColor(IN HDC, IN COLORREF);
#endif

SIZE m_sizeExtent = {10000, 5000}; //100x50 mm

void CreateBMPFile(LPTSTR pszFile, HBITMAP hBMP, BITMAPINFO *pbi);
void errhandler(LPCTSTR cause);
//void Draw(HDC hdc, RECT *prcBounds, RECT *prcWBounds = 0);
void Draw(HDC hdc,char* ip,char* host);
HBITMAP bit_man(char* ip,char* host)
{
	

	LPSTR fileName = "test.wmf";

	// Prepare metafile DC
	HDC metafileDC = CreateMetaFile(0);
	//SetDCBrushColor(metafileDC,RGB(0,0,0));
	SIZE &extents = m_sizeExtent;
	int savedState = SaveDC(metafileDC);
	SetMapMode(metafileDC, MM_ANISOTROPIC);
	SetWindowOrgEx(metafileDC, 0, 0, 0);
	SetWindowExtEx(metafileDC, extents.cx, extents.cy, 0);
	//RECT rect = {0 ,0, extents.cx, extents.cy};
	//RECT *prcBounds = &rect;
	//RECT *prcWBounds = &rect;

	// Draw in metafile using GDI API.
	//Rectangle(metafileDC, prcBounds->left, prcBounds->top, prcBounds->right, prcBounds->bottom);
	// Custom drawing.
	// void Draw(HDC hdc, RECT *prcBounds, RECT *prcWBounds = 0)
	//Draw(metafileDC, prcBounds, prcWBounds);
	Draw(metafileDC,ip,host);
	// Restore initial state.
	RestoreDC(metafileDC, savedState);
	// Extract HMETAFILE.
	HMETAFILE metafile = CloseMetaFile(metafileDC);
	// Use this HMETAFILE later. Do not forget to delete it:
	// DeleteMetaFile(metafile);

	WMFile wmfFile(metafile, extents.cx, extents.cy, TRUE);//TRUE will delete metafile automaticaly
	//wmfFile.save(fileName);

	BITMAPINFO *pbi = 0;
	int resolution = 50;
	WORD colorDepth = 4;
	// Will allocate pbi, do not forget to delete it.
	HBITMAP hBitmap = wmfFile.getBitmap(resolution, colorDepth, &pbi);
		return hBitmap;

	// Manipulate with hBitmap.
	//TCHAR szBmpFile[MAX_PATH];
	//lstrcpy(szBmpFile, fileName);
	//lstrcat(szBmpFile, ".bmp");
	//CreateBMPFile(szBmpFile, hBitmap, pbi);

	//Destroy it.
	DeleteObject(hBitmap);
	if(pbi != 0)
	delete[] (BYTE*)pbi;

	//getchar();
	//return 0;
}

/**
 *  Custom drawing function.
 */
//void Draw(HDC hdc, RECT *prcBounds, RECT *prcWBounds)
void Draw(HDC hdc,char* ip,char* host)
{
	//HPEN hpen = CreatePen(PS_SOLID, 1, RGB(0, 0xcf, 0));
//	HGDIOBJ oldPen = SelectObject(hdc, hpen);
	
//	MoveToEx(hdc, prcBounds->left, prcBounds->top, 0);
//	LineTo(hdc, prcBounds->right, prcBounds->bottom);
//	Ellipse(hdc, prcBounds->left, prcBounds->top, prcBounds->right, prcBounds->bottom);

//	SelectObject(hdc, oldPen);
//	DeleteObject(hpen);

//////////////////////////////////////////////////////////////////////////////////////
//HPEN hpen = CreatePen(PS_SOLID, 1, RGB(0, 0xcf, 0));
	HPEN hpen = CreatePen(PS_SOLID, 1, RGB(0, 0, 0));
	
	HGDIOBJ oldPen = SelectObject(hdc, hpen);
	HBRUSH hbrush = CreateSolidBrush(RGB(255,0,0));


	//MoveToEx(hdc, prcBounds->left, prcBounds->top, 0);
	//LineTo(hdc, prcBounds->right, prcBounds->bottom);
	//Ellipse(hdc, prcBounds->left, prcBounds->top, prcBounds->right, prcBounds->bottom);
	
	RECT rc,rc1,rc2;
	SetRect(&rc,0,600,5000,5000);
	SetRect(&rc1,0,0,5000,5000);
	SetRect(&rc2,6000,250,5000,1000);
	FillRect(hdc,&rc2,hbrush);
	//FillRect(hdc, &rc2, (HBRUSH) (COLOR_WINDOW+1));
	SetTextColor(hdc,RGB(255,0,0));
	LPCTSTR fight=ip;
	LPCTSTR greg=host;
	
	DrawText(hdc,greg, strlen(greg),&rc1,DT_LEFT);
	DrawText(hdc,fight, strlen(fight),&rc,DT_LEFT);
	//DrawText(hdc,test, strlen(test),&rc2,DT_LEFT);
	//Rectangle(hdc,0,1,1,1);
	SelectObject(hdc, oldPen);
	DeleteObject(hpen);
	
	
	
	
/////////////////////////////////////////////////////////////////////////////////////












}

/**
 *  Save bitmap (just apiece of code from MSDN).
 */
void CreateBMPFile(LPTSTR pszFile, HBITMAP hBMP, BITMAPINFO *pbi) 
{
    HANDLE hf;                  // file handle 
    BITMAPFILEHEADER hdr;       // bitmap file-header 
    PBITMAPINFOHEADER pbih;     // bitmap info-header 
    LPBYTE lpBits;              // memory pointer 
    DWORD dwTotal;              // total count of bytes 
    DWORD cb;                   // incremental count of bytes 
    BYTE *hp;                   // byte pointer 
    DWORD dwTmp; 

    pbih = (PBITMAPINFOHEADER) pbi; 
    lpBits = (LPBYTE) GlobalAlloc(GMEM_FIXED, pbih->biSizeImage);

    if (!lpBits) 
         errhandler("GlobalAlloc"); 

	HDC hDC = GetDC(0);
	DWORD biClrUsed = pbi->bmiHeader.biClrUsed;
    // Retrieve the color table (RGBQUAD array) and the bits 
    // (array of palette indices) from the DIB. 
    if (!GetDIBits(hDC, hBMP, 0, (WORD) pbih->biHeight, lpBits, pbi, 
        DIB_RGB_COLORS)) 
    {
        errhandler("GetDIBits"); 
    }
	pbi->bmiHeader.biClrUsed = biClrUsed;

    // Create the .BMP file. 
    hf = CreateFile(pszFile, 
                   GENERIC_READ | GENERIC_WRITE, 
                   (DWORD) 0, 
                    NULL, 
                   CREATE_ALWAYS, 
                   FILE_ATTRIBUTE_NORMAL, 
                   (HANDLE) NULL); 
    if (hf == INVALID_HANDLE_VALUE) 
        errhandler("CreateFile"); 
    hdr.bfType = 0x4d42;        // 0x42 = "B" 0x4d = "M" 
    // Compute the size of the entire file. 
    hdr.bfSize = (DWORD) (sizeof(BITMAPFILEHEADER) + 
                 pbih->biSize + pbih->biClrUsed 
                 * sizeof(RGBQUAD) + pbih->biSizeImage); 
    hdr.bfReserved1 = 0; 
    hdr.bfReserved2 = 0; 

    // Compute the offset to the array of color indices. 
    hdr.bfOffBits = (DWORD) sizeof(BITMAPFILEHEADER) + 
                    pbih->biSize + pbih->biClrUsed 
                    * sizeof (RGBQUAD); 

    // Copy the BITMAPFILEHEADER into the .BMP file. 
    if (!WriteFile(hf, (LPVOID) &hdr, sizeof(BITMAPFILEHEADER), 
        (LPDWORD) &dwTmp,  NULL)) 
    {
       errhandler("WriteFile"); 
    }

    // Copy the BITMAPINFOHEADER and RGBQUAD array into the file. 
    if (!WriteFile(hf, (LPVOID) pbih, sizeof(BITMAPINFOHEADER) 
                  + pbih->biClrUsed * sizeof (RGBQUAD), 
                  (LPDWORD) &dwTmp, ( NULL))) 
        errhandler("WriteFile"); 

    // Copy the array of color indices into the .BMP file. 
    dwTotal = cb = pbih->biSizeImage; 
    hp = lpBits; 
    if (!WriteFile(hf, (LPSTR) hp, (int) cb, (LPDWORD) &dwTmp,NULL)) 
           errhandler("WriteFile"); 

    // Close the .BMP file. 
     if (!CloseHandle(hf)) 
           errhandler("CloseHandle"); 

    // Free memory. 
    GlobalFree((HGLOBAL)lpBits);
	ReleaseDC(0, hDC);
}

void errhandler(LPCTSTR cause)
{
	printf("%s\n", cause);
}
