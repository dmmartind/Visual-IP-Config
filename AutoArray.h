///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//Copyright 2007 David Martin. All Rights Reserved.
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// AutoArray.h: interface for the AutoArray class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_AUTOARRAY_H__C090BDC4_1A33_420E_8F27_8221E6333E64__INCLUDED_)
#define AFX_AUTOARRAY_H__C090BDC4_1A33_420E_8F27_8221E6333E64__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/**
 *  Manage arrays deallocation.
 */
template <class T>
class AutoArray  
{
	AutoArray() {}
	AutoArray(AutoArray& ) {}
public:
	AutoArray(T *managed)
	{
		incapsulatedPtr_ = managed;
	}
	virtual ~AutoArray()
	{
		if(incapsulatedPtr_)
		{
			delete[] incapsulatedPtr_;
			incapsulatedPtr_ = 0;
		}
	}
protected:
	T *incapsulatedPtr_;
};

/**
 *  Manage handles closing.
 */
class WindowHandle
{
	WindowHandle() {}
	WindowHandle(WindowHandle& ) {}
public:
	WindowHandle(HANDLE handle)
	{
		incapsulatedHandle_ = handle;
	}
	~WindowHandle() 
	{
		if(incapsulatedHandle_ != 0)
		{
			CloseHandle(incapsulatedHandle_);
			incapsulatedHandle_ = 0;
		}
	}	
protected:
	HANDLE incapsulatedHandle_;

};

#endif // !defined(AFX_AUTOARRAY_H__C090BDC4_1A33_420E_8F27_8221E6333E64__INCLUDED_)
