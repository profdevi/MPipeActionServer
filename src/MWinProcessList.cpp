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


//v1.1 copyright Comine.com 20170712W1452
#include <Windows.h>
#include <WtsApi32.h>
#include <sddl.h>
#include "MStdLib.h"
#include "TVector.h"
#include "MString.h"
#include "MWinProcessList.h"


//******************************************************
//**  Module elements
//******************************************************
#pragma comment(lib,"Wtsapi32.lib")
#pragma comment(lib,"Advapi32.lib")

//******************************************************
//**  MWinProcessList class
//******************************************************
void MWinProcessList::ClearObject(void)
	{
	mpInfo=0;
	}


////////////////////////////////////////////////
MWinProcessList::MWinProcessList(bool create)
	{
	ClearObject();
	if(create==true && Create()==false)
		{
		return;
		}
	}


////////////////////////////////////////////////
MWinProcessList::~MWinProcessList(void)
	{  Destroy();  }


////////////////////////////////////////////////
bool MWinProcessList::Create(void)
	{
	Destroy();

	if(WTSEnumerateProcesses(WTS_CURRENT_SERVER_HANDLE,0,1,&mpInfo,(DWORD *)&mProcCount)==FALSE)
		{
		Destroy();
		return false;
		}

	return true;
	}


////////////////////////////////////////////////
bool MWinProcessList::Destroy(void)
	{
	if(mpInfo!=0)
		{
		WTSFreeMemory(mpInfo);
		mpInfo=0;
		}

	ClearObject();
	return true;
	}


/////////////////////////////////////////////////
int MWinProcessList::GetCount(void) const
	{
	return (int)mProcCount;
	}


////////////////////////////////////////////////
int MWinProcessList::GetProcessID(int index) const
	{
	MStdAssert(index>=0 && index<mProcCount);
	return (int) mpInfo[index].ProcessId;
	}


//////////////////////////////////////////////////
int MWinProcessList::GetSessionID(int index) const
	{
	MStdAssert(index>=0 && index<mProcCount);
	return (int) mpInfo[index].SessionId;
	}


////////////////////////////////////////////////
const PSID MWinProcessList::GetUserSID(int index) const
	{
	MStdAssert(index>=0 && index<mProcCount);
	return mpInfo[index].pUserSid;
	}


/////////////////////////////////////////////////
bool MWinProcessList::GetUserSIDString(int index,MString &usersid)
	{
	MStdAssert(index>=0 && index<mProcCount);

	PSID pusersid=mpInfo[index].pUserSid;
	if(pusersid==0) {  return false; }

	LPWSTR strsid=0;
	if(ConvertSidToStringSid(mpInfo[index].pUserSid,&strsid)==FALSE)
		{
		return false;
		}

	if(usersid.Create(strsid)==false)
		{
		LocalFree(strsid);
		strsid=0;
		return false;
		}

	LocalFree(strsid);
	strsid=0;
	return true;
	}


/////////////////////////////////////////////////
bool MWinProcessList::GetProcessName(int index,MString &procname) const
	{
	MStdAssert(index>=0 && index<mProcCount);
	if(procname.Create(mpInfo[index].pProcessName)==false)
		{
		return false;
		}

	return true;
	}


//////////////////////////////////////////////////////////////
bool MWinProcessList::GetFullPath(int index,MString &fullpathname)
	{
	MStdAssert(index>=0 && index<mProcCount);
	const DWORD procid=mpInfo[index].ProcessId;
	HANDLE hprocess=OpenProcess(PROCESS_QUERY_LIMITED_INFORMATION,FALSE,procid);
	if(hprocess==INVALID_HANDLE_VALUE)
		{
		return false;
		}

	TVector<char> buffer(MStdPathMaxSize);
	DWORD exepathlen=buffer.GetCount();
	if(QueryFullProcessImageNameA(hprocess,0,buffer.Get(),&exepathlen)==FALSE)
		{
		CloseHandle(hprocess);
		return false;
		}

	CloseHandle(hprocess);
	if(fullpathname.Create(buffer.Get())==false)
		{
		return false;
		}

	return true;	
	}


//////////////////////////////////////////////////////////
int MWinProcessList::FindProcessIndex(const char *processname)
	{
	for(int i=0;i<mProcCount;++i)
		{
		MString name;
		if(GetProcessName(i,name)==false)
			{
			continue;
			}

		if(MStdStrICmp(name.Get(),processname)!=0) { continue; }
		return i;
		}

	return -1;
	}


/////////////////////////////////////////////////////////
bool MWinProcessList::Print(bool printsid,bool printfullpath,bool printsessionid)
	{
	for(int i=0;i<mProcCount;++i)
		{
		MString procname;
		MString usersid;
		MString fullpath;
		if(GetProcessName(i,procname)==false ) { continue; }
		if(GetUserSIDString(i,usersid)==false) { usersid.Create(""); }
		if(GetFullPath(i,fullpath)==false) { fullpath.Create("");  }
	
		MStdPrintf("%5d ",GetProcessID(i));
		
		if(printsessionid==true) { MStdPrintf("SessID=%d  ",GetSessionID(i) );  }

		if(printfullpath==true)
			{  MStdPrintf("%s  ",fullpath.Get());  }
		else
			{  MStdPrintf("%s  ",procname.Get() );  }


		if(printsid==true) { MStdPrintf("SID=%s  ",usersid.Get());  }

		MStdPrintf("\n");
		}


	return true;
	}


