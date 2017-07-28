///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//Copyright 2007 David Martin. All Rights Reserved.
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//#include <windows.h>
//#include "resource.h"
//#include <stdio.h>
#include <iphlpapi.h>
//#include <string.h>
//#include <Assert.h>
//#include "info.h"

const char ClassName[] = "MainWindowClass";
HWND hWndButton;
HWND hWndButton1;
HWND hWndStatic;
void refill(HWND hwnd);






void release_ip()
{
	PIP_INTERFACE_INFO pInfo;
	pInfo = (IP_INTERFACE_INFO *) malloc( sizeof(IP_INTERFACE_INFO) );
	ULONG ulOutBufLen = 0;
	DWORD dwRetVal = 0;

// Make an initial call to GetInterfaceInfo to get
// the necessary size into the ulOutBufLen variable
	if ( GetInterfaceInfo(pInfo, &ulOutBufLen) == ERROR_INSUFFICIENT_BUFFER) 
	{
		//GlobalFree(pInfo);
		pInfo = (IP_INTERFACE_INFO *) malloc (ulOutBufLen);
	}

// Make a second call to GetInterfaceInfo to get the
// actual data we want
	if ((dwRetVal = GetInterfaceInfo(pInfo, &ulOutBufLen)) == NO_ERROR )
		{
			//printf("\tAdapter Name: %ws\n", pInfo->Adapter[0].Name);
			//printf("\tAdapter Index: %ld\n", pInfo->Adapter[0].Index);
			//printf("\tNum Adapters: %ld\n", pInfo->NumAdapters);
		}
	else
		{
			//printf("GetInterfaceInfo failed.\n");
			LPVOID lpMsgBuf;
			
	if (FormatMessage( 
		FORMAT_MESSAGE_ALLOCATE_BUFFER | 
		FORMAT_MESSAGE_FROM_SYSTEM | 
		FORMAT_MESSAGE_IGNORE_INSERTS,
		NULL,
		dwRetVal,
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), // Default language
		(LPTSTR) &lpMsgBuf,
		0,
		NULL )) 
		{
			//printf("\tError: %s", lpMsgBuf);
		}
  LocalFree( lpMsgBuf );
		}

// Call IpReleaseAddress and IpRenewAddress to release and renew
// the IP address on the specified adapter.
		if ((dwRetVal = IpReleaseAddress(&pInfo->Adapter[0])) == NO_ERROR)
		{
			//printf("IP release succeeded.\n");
		}
		else
		{
			//printf("IP release failed\n");
		}

		
}
///////////////////////////////////////////////////////////////////////////////////////
void renew_ip()
{
	PIP_INTERFACE_INFO pInfo;
	pInfo = (IP_INTERFACE_INFO *) malloc( sizeof(IP_INTERFACE_INFO) );
	ULONG ulOutBufLen = 0;
	DWORD dwRetVal = 0;

// Make an initial call to GetInterfaceInfo to get
// the necessary size into the ulOutBufLen variable
	if ( GetInterfaceInfo(pInfo, &ulOutBufLen) == ERROR_INSUFFICIENT_BUFFER) 
	{
		//GlobalFree(pInfo);
		pInfo = (IP_INTERFACE_INFO *) malloc (ulOutBufLen);
	}

// Make a second call to GetInterfaceInfo to get the
// actual data we want
	if ((dwRetVal = GetInterfaceInfo(pInfo, &ulOutBufLen)) == NO_ERROR )
		{
			//printf("\tAdapter Name: %ws\n", pInfo->Adapter[0].Name);
			//printf("\tAdapter Index: %ld\n", pInfo->Adapter[0].Index);
			//printf("\tNum Adapters: %ld\n", pInfo->NumAdapters);
		}
	else
		{
			//printf("GetInterfaceInfo failed.\n");
			LPVOID lpMsgBuf;
			
	if (FormatMessage( 
		FORMAT_MESSAGE_ALLOCATE_BUFFER | 
		FORMAT_MESSAGE_FROM_SYSTEM | 
		FORMAT_MESSAGE_IGNORE_INSERTS,
		NULL,
		dwRetVal,
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), // Default language
		(LPTSTR) &lpMsgBuf,
		0,
		NULL )) 
		{
			//printf("\tError: %s", lpMsgBuf);
		}
  LocalFree( lpMsgBuf );
		}

// Call IpReleaseAddress and IpRenewAddress to release and renew
// the IP address on the specified adapter.
		
		if ((dwRetVal = IpRenewAddress(&pInfo->Adapter[0])) == NO_ERROR) {
				//printf("IP renew succeeded.\n");
	}
else {
  //printf("IP renew failed.\n");
	}

}
//////////////////////////////////////////////////////////////////////////////////////

char* get_info(int msg)
{
		FIXED_INFO * FixedInfo;
		ULONG    ulOutBufLen;
		DWORD    dwRetVal;
		IP_ADDR_STRING * pIPAddr;

		FixedInfo = (FIXED_INFO *) GlobalAlloc( GPTR, sizeof( FIXED_INFO ) );
		ulOutBufLen = sizeof( FIXED_INFO );
   
		if( ERROR_BUFFER_OVERFLOW == GetNetworkParams( FixedInfo, &ulOutBufLen ) )
		{
			GlobalFree( FixedInfo );
			FixedInfo = (FIXED_INFO *) GlobalAlloc( GPTR, ulOutBufLen );
		}

		if ( dwRetVal = GetNetworkParams( FixedInfo, &ulOutBufLen ) )
		{
			MessageBox(NULL,"Call to GetNetworkParams failed. Return Value: %08x\n","WARNING!",MB_OK |  MB_ICONWARNING);
		}
		else
		{
			switch(msg)
			{
					case 1:
					{
						return (FixedInfo -> HostName);
					}
					break;
					case 2:
					{
						return (FixedInfo -> DomainName);
					}
					break;
					case 3:
					{
						return (FixedInfo -> DnsServerList.IpAddress.String);
					}
					break;
					case 4:
					{
						switch(FixedInfo->NodeType)
						{
							case BROADCAST_NODETYPE:
								{
									char* how="BROADCAST";
									return how;
								}break;
							
							case PEER_TO_PEER_NODETYPE:
								{
									char* how="PEER_TO_PEER";
									return how;
								}break;

							case MIXED_NODETYPE:
								{
									char* how="MIXED";
									return how;
								}break;

							case HYBRID_NODETYPE:
								{
									char* how="HYBRID";
									return how;
								}break;

							default:
								{
									char* how="NO";
									return how;
								}break;
						}
					}
					break;
					case 5:
					{
						return (FixedInfo->ScopeId);
					}
					break;
					default:
						return NULL;
					break;
			}
		
        }
}
//////////////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////
char* get_info2(int msg)

{
	PIP_ADAPTER_INFO pAdapterInfo;
	PIP_ADAPTER_INFO pAdapter = NULL;
	DWORD dwRetVal = 0;

	pAdapterInfo = (IP_ADAPTER_INFO *) malloc( sizeof(IP_ADAPTER_INFO) );
	ULONG ulOutBufLen = sizeof(IP_ADAPTER_INFO);

// Make an initial call to GetAdaptersInfo to get
// the necessary size into the ulOutBufLen variable
	if (GetAdaptersInfo( pAdapterInfo, &ulOutBufLen) == ERROR_BUFFER_OVERFLOW)
	{
		free(pAdapterInfo);
		pAdapterInfo = (IP_ADAPTER_INFO *) malloc (ulOutBufLen); 
	}

if ((dwRetVal = GetAdaptersInfo( pAdapterInfo, &ulOutBufLen)) == NO_ERROR)
	{
		pAdapter = pAdapterInfo;
		//while (pAdapter)
		//{

		switch(msg)
		{
		case 1:
			 {
				 return (pAdapter->AdapterName);
			 }
			 break;
		case 2:
			 {
				 return (pAdapter->Description);
			 }break;
		case 3:
			 {
				 return (pAdapter->IpAddressList.IpAddress.String);
			 }break;
		case 4:
			 {
				 return (pAdapter->IpAddressList.IpMask.String);
			 }break;
		case 5:
			 {
				 return (pAdapter->GatewayList.IpAddress.String);
			 }break;
		case 6:
			 {
				 if(pAdapter->DhcpEnabled)
				 {
					 return (pAdapter->DhcpServer.IpAddress.String);
				 }
				 else
				 { 
					 return("DHCP is disabled");
				 }
			 }break;
		default:
			return(NULL);
		};

}
}
/////////////////////////////////////////////////////////////////////////////////////////
char* PrintMACaddress(unsigned char MACData[])
{
	char buffer[200];

	sprintf(buffer,"MAC Address: %02X-%02X-%02X-%02X-%02X-%02X\n", 
		MACData[0], MACData[1], MACData[2], MACData[3], MACData[4], MACData[5]);

	//cout<<buffer<<endl;
	return buffer;


	//unsigned char* print_mac=MACData;
	//int count=-1;
	//while (count<=5)
	//{
	//	cout<<print_mac<<" - "<<endl;
	//	print_mac++;
	//	count++;
	//}
	
	}

/////////////////////////////////////////////////////////////////////////////////////////////

char* get_mac()
{
IP_ADAPTER_INFO AdapterInfo[16];			// Allocate information for up to 16 NICs
	DWORD dwBufLen = sizeof(AdapterInfo);		// Save the memory size of buffer

	DWORD dwStatus = GetAdaptersInfo(			// Call GetAdapterInfo
		AdapterInfo,							// [out] buffer to receive data
		&dwBufLen);								// [in] size of receive data buffer
	assert(dwStatus == ERROR_SUCCESS);			// Verify return value is valid, no buffer overflow

	PIP_ADAPTER_INFO pAdapterInfo = AdapterInfo;// Contains pointer to current adapter info
	//do {
	char* temp=PrintMACaddress(pAdapterInfo->Address);	// Print MAC address
		return temp;
		//pAdapterInfo = pAdapterInfo->Next;		// Progress through linked list
	//}
	//while(pAdapterInfo);						// Terminate if last adapter
}





/////////////////////////////////////////////////////////////////////////////////////////
void get_format(HWND hWnd,HDC hdc)
{
char* temp;
			RECT rt;
			GetClientRect(hWnd, &rt);
			
////////////////////////////////////////////////////////////////////////////////////////
			temp=NULL;
			temp=get_info(1);
			//MessageBox(NULL,temp,"HELP!!",MB_OK);
			char szHello[25]="HostName: ";
			strcat(szHello,temp);
			//MessageBox(NULL,szHello,"HELP!!",MB_OK);
			//DrawText(hdc, szHello, strlen(szHello), &rt, DT_VCENTER | DT_CENTER | DT_SINGLELINE |DT_WORDBREAK);
			TabbedTextOut(hdc,250,12,szHello, strlen(szHello),0,0,0);
/////////////////////////////////////////////////////////////////////////////////////////
			////////////////////////////////////////////////////////////////////////////////////////
			temp=NULL;
			temp=get_info(2);
			//MessageBox(NULL,temp,"HELP!!",MB_OK);
			char szHello2[25]="DomainName: ";
			strcat(szHello2,temp);
			//MessageBox(NULL,szHello,"HELP!!",MB_OK);
			//DrawText(hdc, szHello, strlen(szHello), &rt, DT_VCENTER | DT_CENTER | DT_SINGLELINE |DT_WORDBREAK);
			TabbedTextOut(hdc,250,35,szHello2, strlen(szHello2),0,0,0);
/////////////////////////////////////////////////////////////////////////////////////////
			////////////////////////////////////////////////////////////////////////////////////////
			temp=NULL;
			temp=get_info(3);
			//MessageBox(NULL,temp,"HELP!!",MB_OK);
			char szHello3[25]="DNS Address: ";
			strcat(szHello3,temp);
			//MessageBox(NULL,szHello,"HELP!!",MB_OK);
			//DrawText(hdc, szHello, strlen(szHello), &rt, DT_VCENTER | DT_CENTER | DT_SINGLELINE |DT_WORDBREAK);
			TabbedTextOut(hdc,250,60,szHello3, strlen(szHello3),0,0,0);
/////////////////////////////////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////////////////
			////////////////////////////////////////////////////////////////////////////////////////
			temp=NULL;
			temp=get_info2(1);
			//MessageBox(NULL,temp,"HELP!!",MB_OK);
			char szHello4[25]="AdapterName: ";
			strcat(szHello4,temp);
			//MessageBox(NULL,szHello,"HELP!!",MB_OK);
			//DrawText(hdc, szHello, strlen(szHello), &rt, DT_VCENTER | DT_CENTER | DT_SINGLELINE |DT_WORDBREAK);
			TabbedTextOut(hdc,100,80,szHello4, strlen(szHello4),0,0,0);
/////////////////////////////////////////////////////////////////////////////////////////
		////////////////////////////////////////////////////////////////////////////////////////
			temp=NULL;
			temp=get_info2(2);
			//MessageBox(NULL,temp,"HELP!!",MB_OK);
			char szHello5[25]="Description:  ";
			strcat(szHello5,temp);
			//MessageBox(NULL,szHello,"HELP!!",MB_OK);
			//DrawText(hdc, szHello, strlen(szHello), &rt, DT_VCENTER | DT_CENTER | DT_SINGLELINE |DT_WORDBREAK);
			TabbedTextOut(hdc,100,95,szHello5, strlen(szHello5),0,0,0);
/////////////////////////////////////////////////////////////////////////////////////////					
	////////////////////////////////////////////////////////////////////////////////////////
			temp=NULL;
			temp=get_info2(3);
			//MessageBox(NULL,temp,"HELP!!",MB_OK);
			char szHello6[25]="IP Address: ";
			strcat(szHello6,temp);
			//MessageBox(NULL,szHello,"HELP!!",MB_OK);
			//DrawText(hdc, szHello, strlen(szHello), &rt, DT_VCENTER | DT_CENTER | DT_SINGLELINE |DT_WORDBREAK);
			TabbedTextOut(hdc,250,110,szHello6, strlen(szHello6),0,0,0);
/////////////////////////////////////////////////////////////////////////////////////////					
	
//////////////////////////////////////////////////////////////////////////////////////////
			temp=NULL;
			temp=get_info2(4);
			//MessageBox(NULL,temp,"HELP!!",MB_OK);
			char szHello7[25]="IP Mask: ";
			strcat(szHello7,temp);
			//MessageBox(NULL,szHello,"HELP!!",MB_OK);
			//DrawText(hdc, szHello, strlen(szHello), &rt, DT_VCENTER | DT_CENTER | DT_SINGLELINE |DT_WORDBREAK);
			TabbedTextOut(hdc,250,130,szHello7, strlen(szHello7),0,0,0);
/////////////////////////////////////////////////////////////////////////////////////////		
///////////////////////////////////////////////////////////////////////////////////////
			temp=NULL;
			temp=get_info2(5);
			//MessageBox(NULL,temp,"HELP!!",MB_OK);
			char szHello8[25]="Gateway: ";
			strcat(szHello8,temp);
			//MessageBox(NULL,szHello,"HELP!!",MB_OK);
			//DrawText(hdc, szHello, strlen(szHello), &rt, DT_VCENTER | DT_CENTER | DT_SINGLELINE |DT_WORDBREAK);
			TabbedTextOut(hdc,250,150,szHello8, strlen(szHello8),0,0,0);
/////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////
			temp=NULL;
			temp=get_info2(5);
			//MessageBox(NULL,temp,"HELP!!",MB_OK);
			char szHello9[25]="DHCP Server: ";
			strcat(szHello9,temp);
			//MessageBox(NULL,szHello,"HELP!!",MB_OK);
			//DrawText(hdc, szHello, strlen(szHello), &rt, DT_VCENTER | DT_CENTER | DT_SINGLELINE |DT_WORDBREAK);
			TabbedTextOut(hdc,250,150,szHello9, strlen(szHello9),0,0,0);
/////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////
			temp=NULL;
			temp=get_mac();
			//MessageBox(NULL,temp,"HELP!!",MB_OK);
			char szHello10[25]=" ";
			strcat(szHello10,temp);
			//MessageBox(NULL,szHello,"HELP!!",MB_OK);
			//DrawText(hdc, szHello, strlen(szHello), &rt, DT_VCENTER | DT_CENTER | DT_SINGLELINE |DT_WORDBREAK);
			TabbedTextOut(hdc,200,170,szHello10, strlen(szHello10),0,0,0);
/////////////////////////////////////////////////////////////////////////////////////////


}

///////////////////////////////////////////////////////////////////////////////////////
void refill(HWND hwnd)
{
char* test=get_info(1);
	SetDlgItemText(hwnd,IDC_host_name,test);
	test=get_info(3);
	SetDlgItemText(hwnd,IDC_dns_server,test);
	test=get_info(4);
	SetDlgItemText(hwnd,IDC_node_type,test);
	test=get_info(5);
	SetDlgItemText(hwnd,IDC_net_bios,test);
	test=get_mac();
	SetDlgItemText(hwnd,IDC_adapter_addess,test);
	test=get_info2(3);
	SetDlgItemText(hwnd,IDC_ip_address,test);
	test=get_info2(4);
	SetDlgItemText(hwnd,IDC_subnetmask,test);
	//get_format(hwnd, hdc);
}
///////////////////////////////////////////////////////////////////////////////////////

