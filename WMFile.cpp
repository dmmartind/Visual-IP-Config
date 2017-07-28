///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//Copyright 2007 David Martin. All Rights Reserved.
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	
#include "StdAfx.h"
#include "WMFile.h"
#include "AutoArray.h"
#include "wingdi.h"

/// Required in save process.
const DWORD DOTS_PER_INCH = 1270;
const DWORD APMHEADER_KEY = 0x9AC6CDD7;

/**
 *  Copy constructor.
 */
WMFile::WMFile(WMFile &metafile) throw(_com_error)
{
	m_bOwner = TRUE; //this is new owner
	hMetaFile_ = CopyMetaFile(metafile.hMetaFile_, 0);
	xExtent_ = metafile.xExtent_;
	yExtent_ = metafile.yExtent_;
	xOrg_ = metafile.xOrg_;
	yOrg_ = metafile.yOrg_;
}

/**
 *  Constructor.
 */
WMFile::WMFile(HMETAFILE metafile, int xExt, int yExt, 
				   BOOL bOwner) throw(_com_error) :
hMetaFile_(metafile), xExtent_(xExt), yExtent_(yExt), 
xOrg_(0), yOrg_(0), m_bOwner(bOwner)
{
	if(hMetaFile_ == 0)
		//invalid parameter
		throw _com_error(E_INVALIDARG);
}

/**
 *  Construct new metafile and extract required attributes automaticaly.
 *  @param metafileDC device context of windows metafile.
 */
WMFile::WMFile(HDC metafileDC) throw(_com_error)
{
	if(metafileDC == 0)
		// invalid parameter
		throw _com_error(E_INVALIDARG);
	// extract required attributes automaticaly
	// extract origin
	POINT org;
	BOOL b = GetWindowOrgEx(metafileDC, &org);
	if(!b)
		throw _com_error(HRESULT_FROM_WIN32(GetLastError()));
	xOrg_ = org.x;
	yOrg_ = org.y;

	// extract extents
	SIZE ext;
	b = GetWindowExtEx(metafileDC, &ext);
	if(!b)
		throw _com_error(HRESULT_FROM_WIN32(GetLastError()));
	xExtent_ = ext.cx;
	yExtent_ = ext.cy;
	// extract metafile
	hMetaFile_ = CloseMetaFile(metafileDC);
	if(hMetaFile_ == 0)
		throw _com_error(HRESULT_FROM_WIN32(GetLastError()));
	m_bOwner = TRUE;
}

/**
 *  Destructor.
 */
WMFile::~WMFile()
{
	if(hMetaFile_ && m_bOwner)
	{
		DeleteMetaFile(hMetaFile_);
		hMetaFile_ = 0;
	}
}

/**
 *  Write a metafile in placeable format.
 *  Fills out and writes a placeable header, followed by the
 * metafile bits, to the file.
 *  @param fileName filename to write to.
 *  @return operation completeness: TRUE is Success, FALSE is failure.
 */
BOOL WMFile::save(LPCTSTR fileName) throw()
{
	try
	{
		saveChecked(fileName);
		return TRUE;
	}
	catch(...)
	{
		return FALSE;
	}
}

/**
 *  Write a metafile in placeable format.
 *  Fills out and writes a placeable header, followed by the
 * metafile bits, to the file. Same as save, but throws error if occurred.
 *  @param fileName filename to write to.
 */
void WMFile::saveChecked(LPCTSTR fileName) throw(_com_error)
{
	HANDLE			hOutFile;
	APMFILEHEADER	apmFileHeader;
	HMETAFILE		hMetaCopy;	
	
	hMetaCopy = hMetaFile_;
	
	// Fill out the placeable header
	apmFileHeader.key = APMHEADER_KEY;
	apmFileHeader.hmf = 0;
	apmFileHeader.inch = DOTS_PER_INCH;
	apmFileHeader.reserved = 0;
	apmFileHeader.bbox.left = himetricToDots(xOrg_);
	apmFileHeader.bbox.top = himetricToDots(yOrg_);
	apmFileHeader.bbox.right = himetricToDots(xOrg_ + xExtent_);
	apmFileHeader.bbox.bottom = himetricToDots(yOrg_ + yExtent_);
	apmFileHeader.checksum = calculateAPMCheckSum(apmFileHeader);

	// Get the bits
	UINT requiredSize = GetMetaFileBitsEx(hMetaCopy, 0, 0);
	if(requiredSize == 0)
		throw _com_error(HRESULT_FROM_WIN32(GetLastError()));
	LPVOID lpBuffer = new BYTE[requiredSize];
	if(lpBuffer == 0)
		throw _com_error(E_OUTOFMEMORY);
	AutoArray<BYTE> deleteOnComplete((BYTE*)lpBuffer);
	UINT result = GetMetaFileBitsEx(hMetaCopy, requiredSize, lpBuffer);
	if(result == 0)
	{
		throw _com_error(HRESULT_FROM_WIN32(GetLastError()));
	}

	// Open the file
	if((hOutFile = openFile(fileName)) == INVALID_HANDLE_VALUE)
	{
		throw _com_error(HRESULT_FROM_WIN32(GetLastError()));
	}
	WindowHandle fileRelease(hOutFile);
	DWORD bytesWritten = 0;
	// Write the header
	if(!WriteFile(hOutFile, &apmFileHeader, 22, &bytesWritten, 0))
	{
		throw _com_error(HRESULT_FROM_WIN32(GetLastError()));
	}
	// Write the bits
	if(!WriteFile(hOutFile, lpBuffer, requiredSize, &bytesWritten, 0))
	{
		throw _com_error(HRESULT_FROM_WIN32(GetLastError()));
	}
}

/**
 *  Calculates the checksum for a placeable header
 *  The checksum is derived by XORing each word.
 *  @param apmfh A placeable header
 *  @return the checksum
 */
WORD WMFile::calculateAPMCheckSum( APMFILEHEADER apmfh )
{
	LPWORD	lpWord;
	WORD	wResult, i;
	
	// Start with the first word
	wResult = *(lpWord = (LPWORD)(&apmfh));
	// XOR in each of the other 9 words
	for(i=1;i<=9;i++)
	{
		wResult ^= lpWord[i];
	}
	return wResult;
}

/**
 *  Opens handle for specified file.
 *	@param fileName path to file.
 *  @param forRead indication of file usage (for read of for write).
 *  @return opened file handle.
 */
HANDLE WMFile::openFile(LPCTSTR fileName, BOOL forRead) throw(_com_error)
{
	HANDLE hFile; 

	if(forRead)
	{
		hFile = CreateFile(fileName,
			GENERIC_READ,                // open for reading 
			0,                           // do not share 
			NULL,                        // no security 
			OPEN_EXISTING,               // open existing 
			FILE_ATTRIBUTE_NORMAL,       // normal file
			NULL);                       // no attr. template 
	}
	else
	{// default
		hFile = CreateFile(fileName,
			GENERIC_WRITE,               // open for writing 
			0,                           // do not share 
			NULL,                        // no security 
			CREATE_ALWAYS,               // overwrite existing 
			FILE_ATTRIBUTE_NORMAL,       // normal file
			NULL);                       // no attr. template 
	}
	if(hFile == 0)
		throw _com_error(HRESULT_FROM_WIN32(GetLastError()));
	return hFile;
}

/**
 *  Convert incapsulated metafile to bitmap.
 *  @param resolution required resolution.
 *  @param bitsPerSample required color depth in bits.
 *  @param pbi bitmap info header used in bitmap creation.
 *  @return newly created bitmap.
 */
HBITMAP WMFile::getBitmap(int resolution, WORD bitsPerSample, BITMAPINFO **ppbi)
{
	long width = xExtent_;
	long height = yExtent_;
	long dotsWidth = calculateDotsForHimetric(resolution, width);
	long dotsHeight = calculateDotsForHimetric(resolution, height);

	int nInfoSize;
	nInfoSize = sizeof( BITMAPINFOHEADER );
	int nColorTableSize = 0;
	if( bitsPerSample <= 8 )
		nColorTableSize = sizeof(RGBQUAD) * (1 << bitsPerSample);
	nInfoSize += nColorTableSize;
	BITMAPINFO *bmInfo = (LPBITMAPINFO)new BYTE[nInfoSize];
	if(bmInfo == 0)
		throw _com_error(E_OUTOFMEMORY);
	AutoArray<BYTE> deleteAfterComplete((BYTE*)bmInfo);
	DWORD dwEffWidth = ((((bitsPerSample * dotsWidth) + 31) / 32) * 4);

	//prepare the bitmap attributes
	memset(&bmInfo->bmiHeader,0,sizeof(BITMAPINFOHEADER));
	bmInfo->bmiHeader.biSize=sizeof(BITMAPINFOHEADER);
	bmInfo->bmiHeader.biWidth=dotsWidth;
	bmInfo->bmiHeader.biHeight=dotsHeight;
	bmInfo->bmiHeader.biCompression=BI_RGB;
	bmInfo->bmiHeader.biXPelsPerMeter=static_cast<long>(resolution/2.54E-2);
	bmInfo->bmiHeader.biYPelsPerMeter=static_cast<long>(resolution/2.54E-2);
	bmInfo->bmiHeader.biPlanes=1;
	bmInfo->bmiHeader.biBitCount=bitsPerSample;
	bmInfo->bmiHeader.biSizeImage=dwEffWidth*dotsHeight;

	if(bitsPerSample <= 8)
	{
		SetStdPalette(bmInfo->bmiColors, bitsPerSample);
		bmInfo->bmiHeader.biClrUsed = 1 << bitsPerSample;
	}

	if(ppbi != 0)
	{
		*ppbi = (LPBITMAPINFO)new BYTE[nInfoSize];
		//copy BITMAPINFO
		memcpy(*ppbi, bmInfo, nInfoSize);
	}

	//create a temporary dc in memory.
	HDC screenDC = ::GetDC(0);
	if(screenDC == 0)
		throw _com_error(E_OUTOFMEMORY);
	HDC tempDC=CreateCompatibleDC(screenDC);
	if(tempDC == 0)
		throw _com_error(E_OUTOFMEMORY);
	//create a new bitmap and select it in the memory dc
	BYTE *pbase;
	HBITMAP tempBmp = CreateDIBSection(screenDC,bmInfo,DIB_RGB_COLORS,(void**)&pbase,0,0);
	if(tempBmp == 0)
		throw _com_error(E_OUTOFMEMORY);
	HGDIOBJ tempObj = SelectObject(tempDC, tempBmp);
	if(tempObj == 0)
		throw _com_error(E_OUTOFMEMORY);

	SaveDC(tempDC);

	int prevMode = SetMapMode(tempDC, MM_ANISOTROPIC);
	SetWindowOrgEx(tempDC, 0,0,0);
	SetWindowExtEx(tempDC, width, height, 0);
	SetViewportExtEx(tempDC, dotsWidth, dotsHeight, 0);

	// Erase the background.
    HBRUSH hbrBkGnd = CreateSolidBrush(PALETTERGB(0xff, 0xff, 0xff));
	RECT rc = {0, 0, width, height};
    FillRect(tempDC, &rc, hbrBkGnd);
    DeleteObject(hbrBkGnd);

	SetBkMode(tempDC, TRANSPARENT);

	PlayMetaFile(tempDC, hMetaFile_);

	SelectObject(tempDC, tempObj);

	RestoreDC(tempDC, -1);
	DeleteDC(tempDC);

	ReleaseDC(0, screenDC);
	return tempBmp;
}

/**
 *  Obtaine metafile handle as is.
 *  @return incapsulated metafile handle.
 */
HMETAFILE WMFile::getRawMetaFile()
{
	return hMetaFile_;
}

/**
 *  Obtaine horisontal and vertical extents.
 */
void WMFile::getMetricExtent(int &xExt, int &yExt)
{
	xExt = xExtent_;
	yExt = yExtent_;
}

int WMFile::himetricToDots(int extent)
{
	return extent*DOTS_PER_INCH/2540;
}

int WMFile::calculateDotsForHimetric(int resolution, int himetricUnits)
{
	return static_cast<int>(resolution*himetricUnits*0.01/25.4);
}

int WMFile::calculateDotsWidth(int resolution)
{
	return static_cast<int>(resolution*xExtent_*0.01/25.4);
}

int WMFile::calculateDotsHeight(int resolution)
{
	return static_cast<int>(resolution*yExtent_*0.01/25.4);
}

void WMFile::SetStdPalette(LPVOID destinationPalette, WORD bitsPerSample)
{
	if (destinationPalette == 0) return;
	switch (bitsPerSample){
	case 8:
		{
			const BYTE pal256[1024] = {0,0,0,0,0,0,128,0,0,128,0,0,0,128,128,0,128,0,0,0,128,0,128,0,128,128,0,0,192,192,192,0,
			192,220,192,0,240,202,166,0,212,240,255,0,177,226,255,0,142,212,255,0,107,198,255,0,
			72,184,255,0,37,170,255,0,0,170,255,0,0,146,220,0,0,122,185,0,0,98,150,0,0,74,115,0,0,
			50,80,0,212,227,255,0,177,199,255,0,142,171,255,0,107,143,255,0,72,115,255,0,37,87,255,0,0,
			85,255,0,0,73,220,0,0,61,185,0,0,49,150,0,0,37,115,0,0,25,80,0,212,212,255,0,177,177,255,0,
			142,142,255,0,107,107,255,0,72,72,255,0,37,37,255,0,0,0,254,0,0,0,220,0,0,0,185,0,0,0,150,0,
			0,0,115,0,0,0,80,0,227,212,255,0,199,177,255,0,171,142,255,0,143,107,255,0,115,72,255,0,
			87,37,255,0,85,0,255,0,73,0,220,0,61,0,185,0,49,0,150,0,37,0,115,0,25,0,80,0,240,212,255,0,
			226,177,255,0,212,142,255,0,198,107,255,0,184,72,255,0,170,37,255,0,170,0,255,0,146,0,220,0,
			122,0,185,0,98,0,150,0,74,0,115,0,50,0,80,0,255,212,255,0,255,177,255,0,255,142,255,0,255,107,255,0,
			255,72,255,0,255,37,255,0,254,0,254,0,220,0,220,0,185,0,185,0,150,0,150,0,115,0,115,0,80,0,80,0,
			255,212,240,0,255,177,226,0,255,142,212,0,255,107,198,0,255,72,184,0,255,37,170,0,255,0,170,0,
			220,0,146,0,185,0,122,0,150,0,98,0,115,0,74,0,80,0,50,0,255,212,227,0,255,177,199,0,255,142,171,0,
			255,107,143,0,255,72,115,0,255,37,87,0,255,0,85,0,220,0,73,0,185,0,61,0,150,0,49,0,115,0,37,0,
			80,0,25,0,255,212,212,0,255,177,177,0,255,142,142,0,255,107,107,0,255,72,72,0,255,37,37,0,254,0,
			0,0,220,0,0,0,185,0,0,0,150,0,0,0,115,0,0,0,80,0,0,0,255,227,212,0,255,199,177,0,255,171,142,0,
			255,143,107,0,255,115,72,0,255,87,37,0,255,85,0,0,220,73,0,0,185,61,0,0,150,49,0,0,115,37,0,
			0,80,25,0,0,255,240,212,0,255,226,177,0,255,212,142,0,255,198,107,0,255,184,72,0,255,170,37,0,
			255,170,0,0,220,146,0,0,185,122,0,0,150,98,0,0,115,74,0,0,80,50,0,0,255,255,212,0,255,255,177,0,
			255,255,142,0,255,255,107,0,255,255,72,0,255,255,37,0,254,254,0,0,220,220,0,0,185,185,0,0,150,150,0,
			0,115,115,0,0,80,80,0,0,240,255,212,0,226,255,177,0,212,255,142,0,198,255,107,0,184,255,72,0,
			170,255,37,0,170,255,0,0,146,220,0,0,122,185,0,0,98,150,0,0,74,115,0,0,50,80,0,0,227,255,212,0,
			199,255,177,0,171,255,142,0,143,255,107,0,115,255,72,0,87,255,37,0,85,255,0,0,73,220,0,0,61,185,0,
			0,49,150,0,0,37,115,0,0,25,80,0,0,212,255,212,0,177,255,177,0,142,255,142,0,107,255,107,0,72,255,72,0,
			37,255,37,0,0,254,0,0,0,220,0,0,0,185,0,0,0,150,0,0,0,115,0,0,0,80,0,0,212,255,227,0,177,255,199,0,
			142,255,171,0,107,255,143,0,72,255,115,0,37,255,87,0,0,255,85,0,0,220,73,0,0,185,61,0,0,150,49,0,0,
			115,37,0,0,80,25,0,212,255,240,0,177,255,226,0,142,255,212,0,107,255,198,0,72,255,184,0,37,255,170,0,
			0,255,170,0,0,220,146,0,0,185,122,0,0,150,98,0,0,115,74,0,0,80,50,0,212,255,255,0,177,255,255,0,
			142,255,255,0,107,255,255,0,72,255,255,0,37,255,255,0,0,254,254,0,0,220,220,0,0,185,185,0,0,
			150,150,0,0,115,115,0,0,80,80,0,242,242,242,0,230,230,230,0,218,218,218,0,206,206,206,0,194,194,194,0,
			182,182,182,0,170,170,170,0,158,158,158,0,146,146,146,0,134,134,134,0,122,122,122,0,110,110,110,0,
			98,98,98,0,86,86,86,0,74,74,74,0,62,62,62,0,50,50,50,0,38,38,38,0,26,26,26,0,14,14,14,0,240,251,255,0,
			164,160,160,0,128,128,128,0,0,0,255,0,0,255,0,0,0,255,255,0,255,0,0,0,255,0,255,0,255,255,0,0,255,255,255,0};
			memcpy(destinationPalette,pal256,1024);
			break;
		}
	case 4:
		{
			const BYTE pal16[64]={0,0,0,0,0,0,128,0,0,128,0,0,0,128,128,0,128,0,0,0,128,0,128,0,128,128,0,0,192,192,192,0,
								128,128,128,0,0,0,255,0,0,255,0,0,0,255,255,0,255,0,0,0,255,0,255,0,255,255,0,0,255,255,255,0};

			memcpy(destinationPalette,pal16,64);
			break;
		}
	case 2:
		{
			const BYTE pal2[]={0,0,0,0,255,255,255,0};
			memcpy(destinationPalette,pal2,8);
			break;
		}
	}
	return;
}

void WMFile::SetGrayPalette(RGBQUAD* pal, WORD bitsPerSample)
{
	if (bitsPerSample==0) 
		return;
	DWORD biClrUsed = (1<<bitsPerSample);
	for (ULONG ni=0;ni<biClrUsed;ni++)
		pal[ni].rgbBlue=pal[ni].rgbGreen = pal[ni].rgbRed = 
		(BYTE)(ni*(255/(biClrUsed-1)));
}
