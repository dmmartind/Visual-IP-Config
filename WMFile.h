///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//Copyright 2007 David Martin. All Rights Reserved.
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


#pragma once
#pragma warning( disable : 4290 )

/**
 *  Rect for placeable metafile.
 */
struct RECTS
{
	WORD left;
	WORD top;
	WORD right;
	WORD bottom;
};
/**
 *  File header for metafile.
 */
struct APMFILEHEADER
{
	DWORD	key;
	WORD	hmf;
	RECTS	bbox;
	WORD	inch;
	DWORD	reserved;
	WORD	checksum;
};

/**
 *  Common manipulation on windows metafile, such 
 * as save, load and bitmap extraction.
 */
class WMFile
{
	WMFile() {};//restrict default constructor
public:
	WMFile(WMFile &metafile);
	WMFile(HMETAFILE metafile, int xExt, int yExt, BOOL bOwner = TRUE);
	WMFile(HDC metafileDC);
	~WMFile();
	BOOL save(LPCTSTR fileName) throw();
	void saveChecked(LPCTSTR fileName) throw(_com_error);
	HBITMAP getBitmap(int resolution, WORD bitsPerSample, BITMAPINFO **ppbi = 0);
	HMETAFILE getRawMetaFile();
	void getMetricExtent(int &xExt, int &yExt);
	int calculateDotsForHimetric(int resolution, int himetricUnits);
	int calculateDotsWidth(int resolution);
	int calculateDotsHeight(int resolution);
	bool getPaletteRGB(RGBQUAD *destPalette, WORD bitsPerSample);
protected:
	static void SetGrayPalette(RGBQUAD* pal, WORD bitsPerSample);
	static void SetStdPalette(LPVOID destinationPalette, WORD bitsPerSample);
	WORD calculateAPMCheckSum(APMFILEHEADER apmfh);
	HANDLE openFile(LPCTSTR fileName, BOOL forRead = FALSE) throw(_com_error);
	int himetricToDots(int extent);
	/// Incapsulated metafile
	HMETAFILE hMetaFile_;
	int xOrg_;
	int yOrg_;
	//in HI_METRIC
	int xExtent_;
	int yExtent_;
	BOOL m_bOwner;
};
