///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//Copyright 2007 David Martin. All Rights Reserved.
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include "windows.h"
#include "resource.h"
#include "WMFile_demo.h"
#include "wingdi.h"
#include "info.h"
#define WINDOW_CLASS_NAME "WINCLASS1"
#define MAX_LOADSTRING 100
//////////////////////////////////////////
#include <stdio.h>
//#include <iphlpapi.h>
#include <string.h>
#include <Assert.h>
#include "func.h"
#include <dos.h>
//////////////////////////////////////////

#include <string>
#define BUFSIZE MAX_PATH

//#define IDC_EDITBOX_TEXT 1000

//#define "windowsX.h"

// Global Variables:
HINSTANCE hInst;								// current instance
TCHAR szTitle[MAX_LOADSTRING];								// The title bar text
TCHAR szWindowClass[MAX_LOADSTRING];								// The title bar text

// Foward declarations of functions included in this code module:
ATOM				MyRegisterClass(HINSTANCE hInstance);
BOOL				InitInstance(HINSTANCE, int);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK	About(HWND, UINT, WPARAM, LPARAM);

//dialog callback procedure declaration
INT_PTR CALLBACK DialogProc(
  HWND hwndDlg,  
  UINT uMsg,     
  WPARAM wParam, 
  LPARAM lParam  
);

//function to create region based on the bitmap
void createRegion(HWND);
void splashRegion(HWND);
//function to paint the region window
void paintRegion();

//global variables
//handle to region
HRGN hRgn = NULL;
//dest and memory DCs
HDC hdcDest, hdcMem;
//buffer to hold bitmap data
BITMAP bmpInfo;
int ApplicationStartUpStatus=1;
int anchor=0;


int startup=0;
HINSTANCE Hinstance;
//int Ncmdshow;
HWND take;
//handle to main application window
HWND ghWnd = NULL;

/////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////
void start_funct() 
{
	

			
		char test[BUFSIZE];
		GetModuleFileName(NULL,test,BUFSIZE);
	
		//MessageBox(NULL,test,"HELLO",MB_OK);
		HKEY hk;
		char* hold;
		//DWORD dw;
		

		LONG lResult4=RegOpenKey(HKEY_CURRENT_USER,"Software\\Microsoft\\Windows\\CurrentVersion\\Run\\IPCONFIG",&hk);
		//if( (ERROR_SUCCESS!=lResult3)) || //(ERROR_SUCCESS!=lResult4))
		if((ERROR_SUCCESS!=lResult4))
			 {
				 //MessageBox(NULL,"HELLO","HELLO",MB_OK);
				 startup=0;
			 }
		else
			startup=1;		
			
			//RegCloseKey(hk);

}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// FUNCTIONS //////////////////////////////////////////////
LRESULT CALLBACK WindowProc(HWND hwnd, 
						    UINT msg, 
                            WPARAM wparam, 
                            LPARAM lparam)
{
// this is the main message handler of the system
PAINTSTRUCT		ps;		// used in WM_PAINT
HDC				hdc;	// handle to a device context

// what is the message 
switch(msg)
	{	
		
	case WM_CREATE: 
        {

			HINSTANCE hInstance=(HINSTANCE)GetWindowLong(hwnd,GWL_HINSTANCE);

			HWND hWndgroup1=CreateWindowEx(0,"BUTTON","Host Information",WS_VISIBLE | WS_CHILD | BS_GROUPBOX,5,5,380,175,hwnd,NULL,hInstance,NULL);

			//HWND hWndgroup2=CreateWindowEx(0,"BUTTON","Ethernet Adapter Information",WS_VISIBLE | WS_CHILD | BS_GROUPBOX,5,175,380,335,hwnd,NULL,hInstance,NULL);

			HWND host_name=CreateWindowEx(0,"EDIT",NULL,WS_CHILD | WS_VISIBLE, 175,25,200,20,hwnd,(HMENU)IDC_host_name,hInstance,NULL);
			//left,down,height,length//(HMENU)IDC_EDITBOX_TEXT
			HWND dns_server=CreateWindowEx(0,"EDIT", NULL,WS_VISIBLE | WS_CHILD, 175,60,200,20,hwnd,(HMENU)IDC_dns_server,hInstance,NULL);
			//left,down
			//HWND node_type=CreateWindowEx(0,"EDIT", NULL,WS_VISIBLE | WS_CHILD, 175,95,200,20,hwnd,(HMENU)IDC_node_type,hInstance,NULL);
			
			//HWND net_bios=CreateWindowEx(0,"EDIT", NULL,WS_VISIBLE | WS_CHILD, 175,130,200,20,hwnd,(HMENU)IDC_net_bios,hInstance,NULL);
			
			//HWND adapter=CreateWindowEx(0,"EDIT", NULL,WS_VISIBLE | WS_CHILD, 175,200,200,20,hwnd,(HMENU)IDC_adapter,hInstance,NULL);

			HWND adapter_addess=CreateWindowEx(0,"EDIT", NULL,WS_VISIBLE | WS_CHILD , 100,150,214,20,hwnd,(HMENU)IDC_adapter_addess,hInstance,NULL);

			HWND ip_address=CreateWindowEx(0,"EDIT", NULL,WS_VISIBLE | WS_CHILD, 175,95,200,20,hwnd,(HMENU)IDC_ip_address,hInstance,NULL);

			HWND subnetmask=CreateWindowEx(0,"EDIT", NULL,WS_VISIBLE | WS_CHILD, 175,125,200,20,hwnd,(HMENU)IDC_subnetmask,hInstance,NULL);

			
			
			//HWND default_gateway=CreateWindowEx(0,"EDIT", NULL,WS_VISIBLE | WS_CHILD, 175,300,200,20,hwnd,(HMENU)IDC_default_gateway,hInstance,NULL);

			//HWND type=CreateWindowEx(0,"EDIT", NULL,WS_VISIBLE | WS_CHILD, 175,325,200,20,hwnd,(HMENU)IDC_type,hInstance,NULL);

			//HWND dhcp_server=CreateWindowEx(0,"EDIT", NULL,WS_VISIBLE | WS_CHILD, 175,350,200,20,hwnd,(HMENU)IDC_dhcp_server,hInstance,NULL);

			//HWND primary_wins=CreateWindowEx(0,"EDIT", NULL,WS_VISIBLE | WS_CHILD, 175,375,200,20,hwnd,(HMENU)IDC_primary_wins,hInstance,NULL);

			//HWND secondary_wins=CreateWindowEx(0,"EDIT", NULL,WS_VISIBLE | WS_CHILD, 175,400,200,20,hwnd,(HMENU)IDC_secondary_wins,hInstance,NULL);

			//HWND lease_obtained=CreateWindowEx(0,"EDIT", NULL,WS_VISIBLE | WS_CHILD, 175,425,200,20,hwnd,(HMENU)IDC_lease_obtained,hInstance,NULL);

			//HWND lease_expires=CreateWindowEx(0,"EDIT", NULL,WS_VISIBLE | WS_CHILD, 175,450,200,20,hwnd,(HMENU)IDC_lease_expires,hInstance,NULL);

			//HWND hWndtext15=CreateWindowEx(0,"EDIT", NULL,WS_VISIBLE | WS_CHILD, 175,450,200,20,hwnd,NULL,hInstance,NULL);

			//HWND hWndlabel1=CreateWindowEx(0,"EDIT", NULL,WS_VISIBLE | WS_CHILD, 175,475,200,20,hwnd,NULL,hInstance,NULL);

	HWND hWndStatic1 = CreateWindow("STATIC","Host Name ",WS_VISIBLE | WS_CHILD | SS_CENTER,50,25,100,20,hwnd,NULL,(HINSTANCE)GetWindowLong(hwnd, GWL_HINSTANCE),NULL);
																				//left,down,down,down
	HWND hWndStatic12 = CreateWindow("STATIC","DNS Servers",WS_VISIBLE | WS_CHILD | SS_CENTER,50,60,100,20,hwnd,NULL,(HINSTANCE)GetWindowLong(hwnd, GWL_HINSTANCE),NULL);
	//HWND hWndStatic2 = CreateWindow("STATIC","Node Type",WS_VISIBLE | WS_CHILD | SS_CENTER,50,95,100,20,hwnd,NULL,(HINSTANCE)GetWindowLong(hwnd, GWL_HINSTANCE),NULL);
	//HWND hWndStatic3 = CreateWindow("STATIC","NetBIOS Scope ID ",WS_VISIBLE | WS_CHILD | SS_CENTER,35,130,120,30,hwnd,NULL,(HINSTANCE)GetWindowLong(hwnd, GWL_HINSTANCE),NULL);
	//HWND hWndStatic4 = CreateWindow("STATIC","Hello",WS_VISIBLE | WS_CHILD | SS_CENTER,125,125,50,20,hwnd,NULL,(HINSTANCE)GetWindowLong(hwnd, GWL_HINSTANCE),NULL);
	//HWND hWndStatic5 = CreateWindow("STATIC","Hello",WS_VISIBLE | WS_CHILD | SS_CENTER,125,150,50,20,hwnd,NULL,(HINSTANCE)GetWindowLong(hwnd, GWL_HINSTANCE),NULL);
	//HWND hWndStatic6 = CreateWindow("STATIC","Hello",WS_VISIBLE | WS_CHILD | SS_CENTER,125,175,50,20,hwnd,NULL,(HINSTANCE)GetWindowLong(hwnd, GWL_HINSTANCE),NULL);
	//HWND hWndStatic7 = CreateWindow("STATIC","Adapter",WS_VISIBLE | WS_CHILD | SS_CENTER,50,200,100,20,hwnd,NULL,(HINSTANCE)GetWindowLong(hwnd, GWL_HINSTANCE),NULL);
	//HWND hWndStatic8 = CreateWindow("STATIC","Adapter Address",WS_VISIBLE | WS_CHILD | SS_CENTER,25,225,150,20,hwnd,NULL,(HINSTANCE)GetWindowLong(hwnd, GWL_HINSTANCE),NULL);
	HWND hWndStatic9 = CreateWindow("STATIC","IP Address",WS_VISIBLE | WS_CHILD | SS_CENTER,25,95,150,20,hwnd,NULL,(HINSTANCE)GetWindowLong(hwnd, GWL_HINSTANCE),NULL);
	HWND hWndStatic10 = CreateWindow("STATIC","Subnet Mask",WS_VISIBLE | WS_CHILD | SS_CENTER,25,125,150,20,hwnd,NULL,(HINSTANCE)GetWindowLong(hwnd, GWL_HINSTANCE),NULL);
	//HWND hWndStatic11= CreateWindow("STATIC","Default Gateway",WS_VISIBLE | WS_CHILD | SS_CENTER,25,300,150,20,hwnd,NULL,(HINSTANCE)GetWindowLong(hwnd, GWL_HINSTANCE),NULL);
	//HWND hWndStatic13 = CreateWindow("STATIC","Type",WS_VISIBLE | WS_CHILD | SS_CENTER,75,325,50,20,hwnd,NULL,(HINSTANCE)GetWindowLong(hwnd, GWL_HINSTANCE),NULL);
	//HWND hWndStatic14= CreateWindow("STATIC","DHCP Server",WS_VISIBLE | WS_CHILD | SS_CENTER,25,350,150,20,hwnd,NULL,(HINSTANCE)GetWindowLong(hwnd, GWL_HINSTANCE),NULL);
	//HWND hWndStatic15= CreateWindow("STATIC","Primary WINS Server",WS_VISIBLE | WS_CHILD | SS_CENTER,25,375,150,20,hwnd,NULL,(HINSTANCE)GetWindowLong(hwnd, GWL_HINSTANCE),NULL);
	//HWND hWndStatic16= CreateWindow("STATIC","Secondary WINS Server",WS_VISIBLE | WS_CHILD | SS_CENTER,5,400,170,20,hwnd,NULL,(HINSTANCE)GetWindowLong(hwnd, GWL_HINSTANCE),NULL);
	//HWND hWndStatic17= CreateWindow("STATIC","Lease Obtained",WS_VISIBLE | WS_CHILD | SS_CENTER,25,425,150,20,hwnd,NULL,(HINSTANCE)GetWindowLong(hwnd, GWL_HINSTANCE),NULL);
	//HWND hWndStatic18= CreateWindow("STATIC","Lease Expires",WS_VISIBLE | WS_CHILD | SS_CENTER,25,450,150,20,hwnd,NULL,(HINSTANCE)GetWindowLong(hwnd, GWL_HINSTANCE),NULL);
	//HWND hWndStatic19= CreateWindow("STATIC","Hello",WS_VISIBLE | WS_CHILD | SS_CENTER,125,475,50,20,hwnd,NULL,(HINSTANCE)GetWindowLong(hwnd, GWL_HINSTANCE),NULL);
	//HWND hWndStatic20= CreateWindow("STATIC","Hello",WS_VISIBLE | WS_CHILD | SS_CENTER,125,500,50,20,hwnd,NULL,(HINSTANCE)GetWindowLong(hwnd, GWL_HINSTANCE),NULL);
	//HWND hWndStatic21 = CreateWindow("STATIC","Hello",WS_VISIBLE | WS_CHILD | SS_CENTER,125,525,50,20,hwnd,NULL,(HINSTANCE)GetWindowLong(hwnd, GWL_HINSTANCE),NULL);
	//HWND hWndStatic22= CreateWindow("STATIC","Hello",WS_VISIBLE | WS_CHILD | SS_CENTER,125,550,300,20,hwnd,NULL,(HINSTANCE)GetWindowLong(hwnd, GWL_HINSTANCE),NULL);
	HWND hWndbutton1= CreateWindow("BUTTON","RELEASE",WS_VISIBLE | WS_CHILD | SS_CENTER,25,200,150,20,hwnd,(HMENU)IDC_Release,(HINSTANCE)GetWindowLong(hwnd, GWL_HINSTANCE),NULL);
	HWND hWndbutton2= CreateWindow("BUTTON","RENEW",WS_VISIBLE | WS_CHILD | SS_CENTER,200,200,150,20,hwnd,(HMENU)IDC_Renew,(HINSTANCE)GetWindowLong(hwnd, GWL_HINSTANCE),NULL);
	
			
			
			
			
			
			
			//do initialization stuff here

        // return success
		return(0);
		} break;
case WM_COMMAND: 
        {
            switch (LOWORD(wparam))
            {
                case IDC_Release:
                {
                    switch (HIWORD(wparam))
                    {
                        case BN_CLICKED:
                            //MessageBox(NULL, "Release", "Success", MB_OK | MB_ICONINFORMATION);
							release_ip();
							refill(hwnd);
							DestroyWindow(take);
					HWND hwndDlg = CreateDialog(hInst, MAKEINTRESOURCE(IDD_DIALOG1), ghWnd, (DLGPROC)DialogProc);
					//create region
					createRegion(hwndDlg);//get_info
							break; 
                    }
                }break;
				case IDC_Renew:
					{
						switch (HIWORD(wparam))
				        {
                        case BN_CLICKED:
                            //MessageBox(NULL, "Renew", "Success", MB_OK | MB_ICONINFORMATION);
							renew_ip();
							refill(hwnd);
							DestroyWindow(take);
					HWND hwndDlg = CreateDialog(hInst, MAKEINTRESOURCE(IDD_DIALOG1), ghWnd, (DLGPROC)DialogProc);
					//create region
					createRegion(hwndDlg);//get_info
							break; 
					    }
					}
            return 0;
        } 
		}break; 	
	




	case WM_PAINT: 
		{
		// simply validate the window
		hdc = BeginPaint(hwnd,&ps);	 
		// Background
		HBRUSH hbrBkGnd = CreateSolidBrush(RGB(212,207,201));
		RECT rc;

	SetRect(&rc,0,0,390,517);
	FillRect(hdc, &rc, hbrBkGnd);

	refill(hwnd);
	EndPaint(hwnd,&ps);
    // return success
		return(0);
   		} break;
 
	case WM_QUIT: 
		{
		// kill the application, this sends a WM_QUIT message 
		//PostQuitMessage(0);
SendMessage(hwnd,WM_QUIT,NULL,NULL);
        // return success
		return(0);
		} break;

	default:break;

    } // end switch

// process any messages that we didn't take care of 
return (DefWindowProc(hwnd, msg, wparam, lparam));

} // end WinProc
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////

bool shell(HWND hwnd)
{
// create the window
if (!(hwnd = CreateWindowEx(NULL, // extended style
                            WINDOW_CLASS_NAME,   // class
						    "Visual IPCONFIG", // title
							    WS_SYSMENU  | WS_VISIBLE,
					 	    0,0,	    // initial x,y
						    395,300,  // initial width, height
					    NULL,	    // handle to parent 
						    NULL,	    // handle to menu
						    hInst,// instance of this application
						    NULL)))	// extra creation parms
return(0);

 
}
////////////////////////////////////////////////////////////////////////////////////////////////right-click menu
HMENU SystemTrayMenu(HWND hwnd)
{
	HMENU hMenuPopup;
	POINT pt;

	//directly create a pop up which will be displayed on right click. 
	//no main menu is required
	hMenuPopup=CreatePopupMenu();
	AppendMenu (hMenuPopup, MF_STRING,    IDM_SYS_VISUAL_IPCONFIG, TEXT("Visual IPCONFIG")) ;
	AppendMenu (hMenuPopup, MF_STRING,    IDM_Release,		TEXT("Release")) ;
	AppendMenu (hMenuPopup, MF_STRING,    IDM_Renew,		TEXT("Renew")) ;
	AppendMenu (hMenuPopup, MF_STRING,    IDM_APP_STARTUP,		TEXT("Set the App to start")) ;
	AppendMenu (hMenuPopup, MF_STRING,    IDM_APP_EXIT,			TEXT("Exit Application")) ;
	AppendMenu (hMenuPopup, MF_SEPARATOR, 0, NULL) ;
	AppendMenu (hMenuPopup, MF_STRING,    IDM_TASK,		TEXT("About Application")) ;
	AppendMenu (hMenuPopup, MF_STRING,    IDM_Anchor,		TEXT("Anchor")) ;
	//AppendMenu (hMenuPopup, MF_STRING,    IDM_options,		TEXT("Options")) ;
	//for check to be displayed on system tray menu also
		if(ApplicationStartUpStatus)
			CheckMenuItem(hMenuPopup,7,MF_BYPOSITION|MF_CHECKED);
		else
			CheckMenuItem(hMenuPopup,7,MF_BYPOSITION|MF_UNCHECKED);

		if(startup==1)
			CheckMenuItem(hMenuPopup,3,MF_BYPOSITION|MF_CHECKED);
		else
			CheckMenuItem(hMenuPopup,3,MF_BYPOSITION|MF_UNCHECKED);

	//capture the point where mouse click has happened and display menu there.
	GetCursorPos(&pt);
	SetForegroundWindow(hwnd);
	TrackPopupMenu(hMenuPopup, TPM_LEFTALIGN | TPM_RIGHTBUTTON, pt.x, pt.y, 0, hwnd, NULL);
	//on right click on icon the system menu of windows use to pop up
	//so had send a null msg to maintain control after showing menu.
	PostMessage(hwnd, WM_NULL, 0, 0);
	return hMenuPopup;
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

int APIENTRY WinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPSTR     lpCmdLine,
                     int       nCmdShow)
{
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
WNDCLASSEX winclass; // this will hold the class we create
HWND	   hwnd;	 // generic window handle
MSG		   msg;		 // generic message

// first fill in the window class stucture
winclass.cbSize         = sizeof(WNDCLASSEX);
winclass.style			= CS_CLASSDC;//CS_DBLCLKS | CS_OWNDC | 
                          //CS_HREDRAW | CS_VREDRAW;
winclass.lpfnWndProc	= WindowProc;
winclass.cbClsExtra		= 0;
winclass.cbWndExtra		= 0;
winclass.hInstance		= hInst;
winclass.hIcon			= LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ICON1));
winclass.hIconSm		= LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ICON1));
winclass.hCursor		= LoadCursor(NULL, IDC_ARROW);
winclass.hbrBackground	= (HBRUSH)GetStockObject(BLACK_BRUSH);
winclass.lpszMenuName	= NULL;
winclass.lpszClassName	= WINDOW_CLASS_NAME;
winclass.hIconSm        = LoadIcon(NULL, IDI_APPLICATION);

// register the window class
if (!RegisterClassEx(&winclass))
	return(0);
start_funct();

	



//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	
//	MSG msg;

	strcpy(szWindowClass,"mywindowclass");
	MyRegisterClass(hInstance);
//Hinstance= hInstance;
//Ncmdshow= nCmdShow;
	// Perform application initialization:
	if (!InitInstance (hInstance, nCmdShow)) 
	{
		return FALSE;
	}

	//message loop
//	while (GetMessage(&msg, NULL, 0, 0)) 
//	{
//		DispatchMessage(&msg);
//	}
//
//	return msg.wParam;

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// enter main event loop
while(GetMessage(&msg,NULL,0,0))
 	 { 
	 // translate any accelerator keys
	 TranslateMessage(&msg);

	 // send the message to the window proc
	 DispatchMessage(&msg);
	 } // end while

// return to Windows like this
return(msg.wParam);

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

}


















ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX); 

	wcex.style			= NULL;//CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc	= (WNDPROC)WndProc;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= hInstance;
	wcex.hIcon			= NULL;
	wcex.hIconSm		= NULL;
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
	wcex.lpszMenuName	= NULL;
	wcex.lpszClassName	= szWindowClass;
	wcex.hIconSm		= NULL;

	return RegisterClassEx(&wcex);
}



BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)//get_info
{
	int iRet = 0;

	hInst = hInstance; // Store instance handle in our global variable

	ghWnd = CreateWindow(
		szWindowClass, 
		szTitle, 
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, 
		0, 
		100, 
		150, 
		NULL, 
		NULL, 
		hInstance, 
		NULL);

	if (!ghWnd)
	{
		return FALSE;
	}
	//#include <dos.h>
	

	//create dialog box
	HWND hwndDlg = CreateDialog(hInstance, MAKEINTRESOURCE(IDD_DIALOG1), ghWnd, (DLGPROC)DialogProc);
	//create region
	//createRegion(hwndDlg);//get_info
	splashRegion(hwndDlg);
	Sleep(2000);
	//dont have to show or update the window
	//ShowWindow(hWnd, nCmdShow);
	//UpdateWindow(hWnd);
	DestroyWindow(hwndDlg);//open both of these commands to delete and create a new resource to show the ip region
	hwndDlg = CreateDialog(hInstance, MAKEINTRESOURCE(IDD_DIALOG1), ghWnd, (DLGPROC)DialogProc);
	createRegion(hwndDlg);

	return TRUE;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message) 
	{
	case WM_DESTROY:
		DeleteObject(hRgn);
		//Delete the memory DC
		DeleteDC(hdcMem);
		//delete the dest DC
		DeleteDC(hdcDest);
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
   }
   return 0;
}

//global variable for moving region using mouse
BOOL mouseState = 0;
POINT CursPt = {0};

//dialog callback procedure
INT_PTR CALLBACK DialogProc(
  HWND hwnd,  // handle to dialog box
  UINT uMsg,     // message
  WPARAM wParam, // first message parameter
  LPARAM lParam  // second message parameter
)
{
	take=hwnd;
	int ix = 0, iy = 0;

	switch (uMsg)
	{	
	case WM_PAINT:
		paintRegion();
		ValidateRect(hwnd,FALSE);
		return TRUE;
	case WM_MOVE:
		paintRegion();
		return TRUE;
	case WM_LBUTTONDOWN:
		if (anchor==1)
		{
			mouseState = 1;
			return TRUE;
		}
		else
			return TRUE;
	case WM_MOUSEMOVE:
		if(mouseState)
		{
			GetCursorPos(&CursPt);
			PostMessage(hwnd, WM_NCLBUTTONDOWN, HTCAPTION, MAKELPARAM( CursPt.x, CursPt.y));
			mouseState = 0;
		}
		return TRUE;
	case WM_RBUTTONDOWN:
		SystemTrayMenu(hwnd);
		//destroy the dialog box
		//EndDialog(hwnd,0);
		//send message to destroy the main application window and exit
		//SendMessage(ghWnd,WM_DESTROY,NULL,NULL);
		return TRUE;
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
case WM_COMMAND:
		 {
			 switch(LOWORD(wParam))
			 {
					////////////////////////////////////////////////////////
					case IDM_TASK: 
						MessageBox(NULL,"Programmer/GUI & GDI Design: David Martin. ©2007 All Rights Reserved.","ABOUT",MB_OK);
						//cursor();
						return TRUE;
					////////////////////////////////////////////////////////////
					//menu items
					case WM_QUIT:
						SendMessage(hwnd,WM_DESTROY,NULL,NULL);
					return true;
					case IDM_SYS_VISUAL_IPCONFIG:
				 		shell(hwnd);
					return TRUE;
					case IDM_Release:
					{
						 release_ip();
						 DestroyWindow(take);
						 HWND hwndDlg = CreateDialog(hInst, MAKEINTRESOURCE(IDD_DIALOG1), ghWnd, (DLGPROC)DialogProc);
						//create region
						createRegion(hwndDlg);//get_info
					 return TRUE;
					 }
					case IDM_Renew:
					{	
						renew_ip();
						DestroyWindow(take);
						HWND hwndDlg = CreateDialog(hInst, MAKEINTRESOURCE(IDD_DIALOG1), ghWnd, (DLGPROC)DialogProc);
						//create region
						createRegion(hwndDlg);//get_info
					return TRUE;
					}			 
			 
			 
					case IDM_Anchor:
						if(ApplicationStartUpStatus==1)
							ApplicationStartUpStatus=0;
						else
						ApplicationStartUpStatus=1;
						//ApplicationStartUpStatus
						if (anchor==0)
						{
							anchor=1;
						return TRUE;
						}
						else
						{
							anchor=0;
						return TRUE;
						}
/////////////////////////////////////////////////////////////////////////////////////
						case IDM_APP_STARTUP:
						{	//char* test=(char*)(dwRet);			 
							//MessageBox(NULL,"HOW","HOW",MB_OK);
							// Extract name

							HKEY hk,ok;
							if(startup==0)
							{
								char test[BUFSIZE];
								GetModuleFileName(NULL,test,BUFSIZE);
								//MessageBox(NULL,"startup=0","HELLO",MB_OK);
								DWORD dw;
								LONG lResult=RegCreateKey(HKEY_CURRENT_USER,"Software\\Microsoft\\Windows\\CurrentVersion\\Run",&hk);
								LONG lResult2=RegSetValueEx(hk,"IPCONFIG",0,REG_SZ,(BYTE*)test,lstrlen(test));
								LONG lResult3=RegCreateKey(hk,"IPCONFIG",&ok);
								
								
								
								if(( ERROR_SUCCESS!=lResult) || (ERROR_SUCCESS!=lResult2) || (ERROR_SUCCESS!=lResult3))
								{
									MessageBox(hwnd,"Error getting the startup information","StartUp Error",0);
									return 0;
   								}
								else
								{
									//MessageBox(NULL,test,"HELLO",MB_OK);
									RegCloseKey(hk);	
									startup=1;
								}
							}
							else if(startup==1)
							{
								char test[BUFSIZE];
								GetModuleFileName(NULL,test,BUFSIZE);
								//MessageBox(NULL,"startup=1","HELLO",MB_OK);
								DWORD dw;
								LONG lResult3=RegOpenKey(HKEY_CURRENT_USER,"Software\\Microsoft\\Windows\\CurrentVersion\\Run",&hk);
								
								if( ERROR_SUCCESS!=lResult3)
								{
									MessageBox(NULL,"Error getting the startup information1","StartUp Error",0);
									return 0;
								}
			
								 LONG lResult5 = RegDeleteValue(hk,"IPCONFIG");
								LONG lResult6=RegDeleteKey(HKEY_CURRENT_USER,"Software\\Microsoft\\Windows\\CurrentVersion\\Run\\IPCONFIG");
								if(( ERROR_SUCCESS!=lResult5) || ( ERROR_SUCCESS!=lResult5))
								{
									RegCloseKey(hk);
									MessageBox(hwnd,"Error getting the startup information2","StartUp Error",0);
									return 0;
								}
								RegCloseKey(hk);
								startup=0;
							}
				return TRUE;
				}

			

			 case IDM_APP_EXIT:
				//destroy the dialog box
				EndDialog(hwnd,0);
				//send message to destroy the main application window and exit
				SendMessage(ghWnd,WM_DESTROY,NULL,NULL);
				//MessageBox(hwnd,"Hello","Hello",MB_OKCANCEL);//test
				return TRUE;

				
			default:
				 return DefWindowProc (hwnd, uMsg, wParam, lParam) ;
			 }
		 }

			 
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	};
	return FALSE;
}

void createRegion(HWND hwndDlg)//get_info
{
	//get the destination device context
	hdcDest = GetDC(hwndDlg);

	//create a memory DC
	hdcMem = CreateCompatibleDC(NULL);

	//image file name
	//char imageFile[] = "a.bmp";
	// Load the image
	//HANDLE hBitmap = (HBITMAP)LoadImage(GetModuleHandle(NULL), imageFile, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
	//if(!hBitmap)
	//{
		//handle error here
	//	return;
//	}
	HANDLE hBitmap=bit_man(get_info2(3),get_info(1));//get_info

	//Get information about the bitmap..
	GetObject(hBitmap, sizeof(bmpInfo), &bmpInfo);	// Get info about the bitmap 

	//Select the bitmap into the dc
	HGDIOBJ hGdiObj = SelectObject(hdcMem, hBitmap);

	//create an empty region
	hRgn = CreateRectRgn(0,0,0,0);

	//Create a region from a bitmap with transparency colour of white
	//change the pixel values for a different transparency color
	//ex - RGB(0,0,0) will mean a transparency color of black.. so the areas
	//of the bitmap not used to create the window will be black
	COLORREF crTransparent = RGB(255,255,255);

	int iX = 0;
	int iRet = 0;
	for (int iY = 0; iY < bmpInfo.bmHeight; iY++)
	{
		do
		{
			//skip over transparent pixels at start of lines.
			while (iX < bmpInfo.bmWidth && GetPixel(hdcMem, iX, iY) == crTransparent)
				iX++;
			//remember this pixel
			int iLeftX = iX;
			//now find first non transparent pixel
			while (iX < bmpInfo.bmWidth && GetPixel(hdcMem, iX, iY) != crTransparent)
				++iX;
			//create a temp region on this info
			HRGN hRgnTemp = CreateRectRgn(iLeftX, iY, iX, iY+1);
			//combine into main region.
			iRet = CombineRgn(hRgn, hRgn, hRgnTemp, RGN_OR);
			if(iRet == ERROR)
			{
				return;
			}
			//delete the temp region for next pass
			DeleteObject(hRgnTemp);
		}while(iX < bmpInfo.bmWidth);
		iX = 0;
	}

	//Centre it on current desktop
	iRet = SetWindowRgn(hwndDlg, hRgn, TRUE);
	if(!iRet)
	{
		return;
	}
int iY=0;
	iX = (GetSystemMetrics(SM_CXSCREEN)) / 2 - (bmpInfo.bmWidth / 2)+ 400;
	iY = (GetSystemMetrics(SM_CYSCREEN)) / 2 - (bmpInfo.bmHeight / 2)+ 300;
	//iRet = SetWindowPos(hwndDlg, HWND_TOPMOST, iX, iY, bmpInfo.bmWidth, bmpInfo.bmHeight, NULL);
	iRet = SetWindowPos(hwndDlg, HWND_TOPMOST, iX, iY, bmpInfo.bmWidth, bmpInfo.bmHeight, NULL);

	//Copy the memory dc into the screen dc
	paintRegion();

	//delete the bitmap
	DeleteObject(hBitmap);
}

void paintRegion()
{
	//transfer color data from the source device context to the destination device context
	BitBlt(hdcDest, 0, 0, bmpInfo.bmWidth, bmpInfo.bmHeight, hdcMem, 0, 0, SRCCOPY);
}

void splashRegion(HWND hwndDlg)
{
	//get the destination device context
	hdcDest = GetDC(hwndDlg);

	//create a memory DC
	hdcMem = CreateCompatibleDC(NULL);

	//image file name:MAKEINTRESOURCE(IDD_DIALOG1)
	//char imageFile[] = "ipconfig.bmp";
	// Load the image
	HANDLE hBitmap = (HBITMAP)LoadImage(GetModuleHandle(NULL),MAKEINTRESOURCE(IDB_BITMAP1), IMAGE_BITMAP, 0, 0, NULL);
	if(!hBitmap)
	{
		//handle error here
		MessageBox(NULL,"Hello","Hello",MB_OK);
		return;
	}

	//Get information about the bitmap..
	GetObject(hBitmap, sizeof(bmpInfo), &bmpInfo);	// Get info about the bitmap 

	//Select the bitmap into the dc
	HGDIOBJ hGdiObj = SelectObject(hdcMem, hBitmap);

	//create an empty region
	hRgn = CreateRectRgn(0,0,0,0);

	//Create a region from a bitmap with transparency colour of white
	//change the pixel values for a different transparency color
	//ex - RGB(0,0,0) will mean a transparency color of black.. so the areas
	//of the bitmap not used to create the window will be black
	COLORREF crTransparent = RGB(255, 255, 255);

	int iX = 0;
	int iRet = 0;
	for (int iY = 0; iY < bmpInfo.bmHeight; iY++)
	{
		do
		{
			//skip over transparent pixels at start of lines.
			while (iX < bmpInfo.bmWidth && GetPixel(hdcMem, iX, iY) == crTransparent)
				iX++;
			//remember this pixel
			int iLeftX = iX;
			//now find first non transparent pixel
			while (iX < bmpInfo.bmWidth && GetPixel(hdcMem, iX, iY) != crTransparent)
				++iX;
			//create a temp region on this info
			HRGN hRgnTemp = CreateRectRgn(iLeftX, iY, iX, iY+1);
			//combine into main region.
			iRet = CombineRgn(hRgn, hRgn, hRgnTemp, RGN_OR);
			if(iRet == ERROR)
			{
				return;
			}
			//delete the temp region for next pass
			DeleteObject(hRgnTemp);
		}while(iX < bmpInfo.bmWidth);
		iX = 0;
	}

	//Centre it on current desktop
	iRet = SetWindowRgn(hwndDlg, hRgn, TRUE);
	if(!iRet)
	{
		return;
	}
int iY;
	iX = (GetSystemMetrics(SM_CXSCREEN)) / 2 - (bmpInfo.bmWidth / 2);
	iY = (GetSystemMetrics(SM_CYSCREEN)) / 2 - (bmpInfo.bmHeight / 2);
	iRet = SetWindowPos(hwndDlg, HWND_TOPMOST, iX, iY, bmpInfo.bmWidth, bmpInfo.bmHeight, NULL);

	//Copy the memory dc into the screen dc
	paintRegion();

	//delete the bitmap
	DeleteObject(hBitmap);
}