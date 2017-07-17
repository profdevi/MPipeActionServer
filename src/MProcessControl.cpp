/*    
    MPipeActionServer.exe : A Console Windows Pipe Server 
    Copyright (C) 2017  Comine.com

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.

*/


//v2.2 copyright Comine.com 20170709U0955
#include <windows.h>
#include <psapi.h>
#include "MStdLib.h"
#include "MString.h"
#include "MStringList.h"
#include "MBuffer.h"
#include "MIntArray.h"
#include "MProcessList.h"
#include "MDStringList.h"
#include "MPathNameList.h"
#include "TList.h"
#include "MProcessControl.h"


// Link to these libraries
#pragma comment(lib,"user32.lib")
#pragma comment(lib,"kernel32.lib")
#pragma comment(lib,"psapi.lib")

//******************************************************
//**  Module Elements
//******************************************************
static BOOL CALLBACK GGetTopWindowsHandler(HWND hwnd,LPARAM extra)
	{
	TList<HWND> *list=(TList<HWND> *)extra;
	if(list==NULL)
		{
		return FALSE;
		}

	if(list->Add(hwnd)==false)
		{
		return FALSE;
		}
	
	return TRUE;
	}


//******************************************************
//**  MProcessControl class
//******************************************************
void MProcessControl::ClearObject(void)
	{
	}


////////////////////////////////////////////////
MProcessControl::MProcessControl(bool create)
	{
	ClearObject();
	if(create==true && Create()==false)
		{
		return;
		}
	}


////////////////////////////////////////////////
MProcessControl::~MProcessControl(void)
	{  Destroy();  }


////////////////////////////////////////////////
bool MProcessControl::Create(void)
	{
	Destroy();
	return true;
	}


////////////////////////////////////////////////
bool MProcessControl::Destroy(void)
	{
	ClearObject();
	return true;
	}


////////////////////////////////////////////////
bool MProcessControl::Kill(int pid,int exitcode)
	{
	HANDLE hprocess;
	hprocess=OpenProcess(PROCESS_TERMINATE,FALSE,pid);
	if(hprocess==NULL)
		{
		return false;
		}

	if(TerminateProcess(hprocess,exitcode)==FALSE)
		{
		CloseHandle(hprocess);
		return false;
		}

	CloseHandle(hprocess);
	return true;
	}


////////////////////////////////////////////////
int MProcessControl::Wait(int pid,DWORD timeout)
	{
	HANDLE hprocess;
	hprocess=OpenProcess(SYNCHRONIZE,FALSE,pid);
	if(hprocess==NULL)
		{
		return false;
		}

	// Wait for single object
	DWORD dwret=WaitForSingleObject(hprocess,timeout);
	if(dwret==WAIT_OBJECT_0)
		{
		CloseHandle(hprocess);
		return 1;
		}
	else if(dwret==WAIT_TIMEOUT)
		{
		CloseHandle(hprocess);
		return 0;
		}
	else 
		{
		CloseHandle(hprocess);
		return -1;
		}

	return 0;	
	}


/////////////////////////////////////////////////////////
int MProcessControl::GetGDICount(int pid)
	{
	HANDLE hprocess;
	hprocess=OpenProcess(SYNCHRONIZE,FALSE,pid);
	if(hprocess==NULL)
		{
		return 0;
		}
	
	// Get GUI Resource Count
	int count;
	count=(int)GetGuiResources(hprocess,GR_GDIOBJECTS);

	CloseHandle(hprocess);
	return count;
	}


/////////////////////////////////////////////////////////
int MProcessControl::GetUserCount(int pid)
	{
	HANDLE hprocess;
	hprocess=OpenProcess(SYNCHRONIZE,FALSE,pid);
	if(hprocess==NULL)
		{
		return 0;
		}
	
	// Get GUI Resource Count
	int count;
	count=(int)GetGuiResources(hprocess,GR_USEROBJECTS);

	CloseHandle(hprocess);
	return count;
	}


//////////////////////////////////////////////////////
bool MProcessControl::GetIOCounters(int pid,IO_COUNTERS &counters)
	{
	HANDLE hprocess;
	hprocess=OpenProcess(PROCESS_QUERY_INFORMATION|PROCESS_VM_READ,FALSE,pid);
	if(hprocess==NULL)
		{
		return false;
		}

	if(GetProcessIoCounters(hprocess,&counters)==FALSE)
		{
		CloseHandle(hprocess);
		return false;
		}

	CloseHandle(hprocess);
	return true;
	}


///////////////////////////////////////////////////////////
bool MProcessControl::IsAlive(int pid)
	{
	HANDLE hprocess;
	hprocess=OpenProcess(PROCESS_QUERY_INFORMATION|PROCESS_VM_READ,FALSE,pid);
	if(hprocess==NULL)
		{
		return false;
		}

	DWORD exitcode=0;
	if(GetExitCodeProcess(hprocess,&exitcode)==FALSE)
		{
		CloseHandle(hprocess);
		return false;
		}

	// Close the handle
	CloseHandle(hprocess);

	if(exitcode==STILL_ACTIVE) { return true; }

	return true;
	}


//////////////////////////////////////////////////////
int MProcessControl::GetCurrentPID(void)
	{
	return (int)::GetCurrentProcessId();
	}


//////////////////////////////////////////////////////
int MProcessControl::GetParentPID(int pid)
	{
	MProcessList proclist;
	if(proclist.Create()==false)
		{
		return -1;
		}

	int i;
	for(i=0;i<proclist.GetCount();++i)
		{
		if(proclist.GetPID(i)==(unsigned int)pid)
			{
			// Return Parent PID
			return proclist.GetParentPID(i);
			}
		}

	return -1;
	}


///////////////////////////////////////////////////
bool MProcessControl::GetPIDs(MIntArray &pids)
	{
	MBuffer buffer;
	if(buffer.Create(1000)==false)
		{
		return false;
		}

	DWORD needed=0;
	if(EnumProcesses((DWORD *)buffer.GetBuffer(),buffer.GetSize(),&needed)==FALSE)
		{
		return false;
		}

	if(needed>(unsigned int)buffer.GetSize())
		{
		// Resize buffer to correct size
		if(buffer.Create(needed<<1)==false)
			{
			return false;
			}

		if(EnumProcesses((DWORD *)buffer.GetBuffer(),buffer.GetSize(),&needed)==FALSE)
			{
			return false;
			}
		}

	//=We have the process ids in buffer

	int *processids=(int *)buffer.GetBuffer();
	int processcount=(int)(needed/4);

	// Create PIDS
	if(pids.Create(processcount,processcount)==false)
		{
		return false;
		}

	// Copy elements into array
	int i;
	for(i=0;i<processcount;++i)
		{
		if(pids.Set(i,processids[i])==false)
			{	
			return false;
			}
		}

	return true;
	}


////////////////////////////////////////////////
bool MProcessControl::GetModules(int pid,MStringList &modules)
	{
	HANDLE hprocess;
	hprocess=OpenProcess(PROCESS_QUERY_INFORMATION|PROCESS_VM_READ,FALSE,pid);
	if(hprocess==NULL)
		{
		return false;
		}

	// Create a tmp buffer
	MBuffer buffer;
	if(buffer.Create(1000)==false)
		{
		CloseHandle(hprocess);
		return false;
		}

	DWORD needed=0;
	if(EnumProcessModules(hprocess,(HMODULE *)buffer.GetBuffer()
				,buffer.GetSize(),&needed)==FALSE)
		{
		CloseHandle(hprocess);
		return false;
		}

	if(needed>(unsigned int)buffer.GetSize())
		{
		// Resize buffer to correct size
		if(buffer.Create(needed<<1)==false)
			{
			CloseHandle(hprocess);
			return false;
			}

		if(EnumProcessModules(hprocess,(HMODULE *)buffer.GetBuffer()
					,buffer.GetSize(),&needed)==FALSE)
			{
			CloseHandle(hprocess);
			return false;
			}
		}

	// Init ouput string list
	if(modules.Create()==false)
		{
		CloseHandle(hprocess);
		return false;
		}

	// Init Get ModuleFile Names
	MBuffer pathname;
	if(pathname.Create(2000)==false)
		{
		CloseHandle(hprocess);
		return false;
		}

	// Setup pointer to buffer of Module Handles
	HMODULE *handles=(HMODULE *)buffer.GetBuffer();
	int handlecount=(needed/sizeof(HMODULE) );

	// Add the string names to list
	int i;
	for(i=0;i<handlecount;++i)
		{
		DWORD count;
		count=GetModuleFileNameExA(hprocess,handles[i],pathname.GetBuffer()
				,pathname.GetSize()-1);
		if(count<=0)
			{
			CloseHandle(hprocess);
			return false;
			}

		if(modules.AddString(pathname.GetBuffer())==false)
			{
			CloseHandle(hprocess);
			return false;
			}
		}

	CloseHandle(hprocess);
	return true;
	}


///////////////////////////////////////////////////////////////
bool MProcessControl::CanQueryProcess(int pid)
	{
	HANDLE hprocess;
	hprocess=OpenProcess(PROCESS_QUERY_INFORMATION|PROCESS_VM_READ,FALSE,pid);
	if(hprocess==NULL)
		{
		// We cannot Query Process
		return false;
		}

	DWORD needed=0;
	if(EnumProcessModules(hprocess,(HMODULE *)NULL
				,0,&needed)==FALSE)
		{
		CloseHandle(hprocess);
		return false;
		}


	CloseHandle(hprocess);
	return true;	
	}


////////////////////////////////////////////////////
bool MProcessControl::GetDeviceDrivers(MDStringList &devices)
	{
	// Create a tmp buffer
	MBuffer buffer;
	if(buffer.Create(1000)==false)
		{
		return false;
		}

	DWORD needed=0;
	if(EnumDeviceDrivers((void **)buffer.GetBuffer()
				,buffer.GetSize(),&needed)==FALSE)
		{
		return false;
		}

	if(needed>(unsigned int)buffer.GetSize())
		{
		// Resize buffer to correct size
		if(buffer.Create(needed<<1)==false)
			{
			return false;
			}

		if(EnumDeviceDrivers((void **)buffer.GetBuffer()
					,buffer.GetSize(),&needed)==FALSE)
			{
			return false;
			}
		}

	// Count of items
	LPVOID *driversbase=(LPVOID *)buffer.GetBuffer();
	int count=(int)(needed/sizeof(LPVOID) );

	// Init devices list
	if(devices.Create()==false)
		{
		return false;
		}

	// Init Output String name
	MBuffer basename,filename;
	if(basename.Create(1000)==false || filename.Create(1000)==false )
		{
		return false;
		}

	// Add a list member
	int i;
	for(i=0;i<count;++i)
		{
		DWORD len;
		len=::GetDeviceDriverBaseNameA(driversbase[i],basename.GetBuffer()
				,basename.GetSize()-1);
		if(len>=(unsigned int)basename.GetSize()-1)
			{
			continue;
			}


		len=::GetDeviceDriverFileNameA(driversbase[i],filename.GetBuffer()
				,filename.GetSize()-1);
		if(len>=(unsigned int)filename.GetSize()-1)
			{
			continue;
			}


		if(devices.Add(basename.GetBuffer(),filename.GetBuffer())==false)
			{
			continue;
			}
		}

	return true;	
	}


/////////////////////////////////////////////////////////////////
int MProcessControl::GetPID(const char *processname)
	{
	MIntArray pids;
	if(GetPIDs(pids)==false)
		{
		return -1;
		}

	// Go through all the process lists
	int i;
	for(i=0;i<pids.GetCount();++i)
		{
		int pid=pids.Get(i);

		// If we cannot get the process, continue
		if(CanQueryProcess(pid)==false) { continue; }

		MStringList list;
		if(GetModules(pids.Get(i),list)==false)
			{
			return -1;
			}

		// Read the first module name
		list.ReadReset();
		const char *filename=list.ReadString();
		if(filename==NULL)
			{
			return -1;
			}

		MPathNameList pathname;
		if(pathname.Create(filename)==false)
			{
			return -1;
			}
	
		// compare a string
		//MStdPrintf("Comparing Process: %s\n",pathname.GetTop() );
		if(MString::ICompare(pathname.GetTop(),processname)==0)
			{
			// Continue from the start
			return pid;
			}
		}
	
	// =We could not find the process
	return -1;
	}


/////////////////////////////////////////////////////////////
bool MProcessControl::GetTopWindows(TList<HWND> &winhandles)
	{
	if(winhandles.Create()==false)
		{
		return false;
		}

	if(EnumWindows(GGetTopWindowsHandler,(LPARAM)&winhandles)==FALSE)
		{
		return false;
		}
	
	return true;	
	}


/////////////////////////////////////////////////////////////
bool MProcessControl::GetTopWindows(int pid,TList<HWND> &winhandles)
	{
	TList<HWND> alltopwindows;
	if(GetTopWindows(alltopwindows)==false)
		{
		return false;
		}

	// Init winhandles
	if(winhandles.Create()==false)
		{
		return false;
		}

	alltopwindows.ReadReset();

	HWND *hwnd;
	while(alltopwindows.Read(hwnd)==true)
		{
		MStdAssert(hwnd!=NULL);
		DWORD winpid=0;
		DWORD wintid=0;
		wintid=GetWindowThreadProcessId(*hwnd,&winpid);
		if(winpid==(DWORD)pid)
			{
			if(winhandles.Add(*hwnd)==false)
				{
				return false;
				}
			}
		}

	return true;
	}


//////////////////////////////////////////////////////////////////
bool MProcessControl::Close(int pid,int timeout)
	{
	TList<HWND> alltopwindows;
	if(GetTopWindows(pid,alltopwindows)==false)
		{
		return false;
		}

	alltopwindows.ReadReset();

	HWND *hwnd=NULL;
	while(alltopwindows.Read(hwnd)==true)
		{
		DWORD winpid=0;
		DWORD wintid=0;
		DWORD retval=0;
		wintid=GetWindowThreadProcessId(*hwnd,&winpid);
		if(winpid==(DWORD)pid)
			{
			if(SendMessageTimeout(*hwnd,WM_CLOSE,0,0,SMTO_ABORTIFHUNG
					,timeout,&retval)==0 )
				{  return false; }
			}
		}

	return true;
	}


//////////////////////////////////////////////////////
bool MProcessControl::GetWindowsWithText(const char *txt,TList<HWND> &list)
	{
	if(list.Create()==false)
		{
		return false;
		}

	MIntArray proclist;
	if(GetPIDs(proclist)==false)
		{
		return false;
		}

	const int proctotal=proclist.GetCount();
	int i;
	for(i=0;i < proctotal ;++i)
		{
		const int pid=proclist.Get(i);
		TList<HWND> topwinlist;
		if(GetTopWindows(pid,topwinlist)==false)
			{
			continue;
			}

		MBuffer buffer(2000);
		HWND *phwnd;
		for(topwinlist.ReadReset();topwinlist.Read(phwnd)==true;  )
			{
			int retlen=GetWindowTextA(*phwnd,buffer.GetBuffer(),buffer.GetSize()-2);
			if(retlen<=0) { continue;  }

			// Trim the whole name
			MStdStrTrim(buffer.GetBuffer() );
			if(MStdStrIEnds(buffer.GetBuffer(),txt)==false && MStdStrIBegins(buffer.GetBuffer(),txt)==false)
				{ 
				continue;
				}

			//= We have a window that has CoronaSimulator

			if(list.Add(*phwnd)==NULL)
				{
				continue;
				}
			}
		}

	return true;
	}


