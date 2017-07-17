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


//v1.5 copyright Comine.com 20170703M1825
#include <windows.h>
#include <tlhelp32.h>
#include <psapi.h>
#include "MStdLib.h"
#include "MDynaBlock.h"
#include "MString.h"
#include "MBuffer.h"
#include "MProcessList.h"


#pragma comment(lib,"Psapi.lib")

//******************************************************
//**  MProcessList class
//******************************************************
void MProcessList::ClearObject(void)
	{
	mProcessCount=0;
	}


////////////////////////////////////////////////
MProcessList::MProcessList(bool create)
	{
	ClearObject();
	if(create==true && Create()==false)
		{
		return;
		}
	}


////////////////////////////////////////////////
MProcessList::~MProcessList(void)
	{  Destroy();  }


////////////////////////////////////////////////
bool MProcessList::Create(void)
	{
	Destroy();

	// Allocate initial array
	if(mArray.Create(0)==false)
		{
		Destroy();  return false;
		}

	// Get Snap Shot
	HANDLE hsnapshot=CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS,0);
	if(hsnapshot==INVALID_HANDLE_VALUE)
		{
		Destroy();  return false;
		}

	// Init Proc Entry structure
	PROCESSENTRY32W procentry;
	MStdMemSet(&procentry,0,sizeof(procentry) );
	procentry.dwSize=sizeof(procentry);

	if(Process32First(hsnapshot,&procentry)==TRUE)
		{
		for(;;)
			{
			// Add Info into table
			if(mArray.ChangeSize(sizeof(procentry))==false)
				{
				CloseHandle(hsnapshot);  Destroy();  return false;
				}

			PROCESSENTRY32 *array=(PROCESSENTRY32 *)mArray.GetBuffer();
			if(array==NULL)
				{
				CloseHandle(hsnapshot);  Destroy();  return false;
				}

			MStdMemCpy(array+mProcessCount,&procentry,sizeof(procentry) );

			// Increate Process COunt
			mProcessCount = mProcessCount + 1;

			if(Process32Next(hsnapshot,&procentry)==FALSE)
				{ break; }
			}
		}
	else
		{
		CloseHandle(hsnapshot);
		Destroy();
		return false;
		}
	
	// Release Snapshot
	CloseHandle(hsnapshot);

	return true;
	}


////////////////////////////////////////////////
bool MProcessList::Destroy(void)
	{
	mArray.Destroy();
	ClearObject();
	return true;
	}


////////////////////////////////////////////////
int MProcessList::GetCount(void)			// return # of processes running
	{
	return mProcessCount;
	}


////////////////////////////////////////////////
unsigned int MProcessList::GetPID(int index)	// Get ProcessId  of Process
	{
	if(index<0 || index>=mProcessCount)
		{
		return 0;
		}

	PROCESSENTRY32 *proctable=(PROCESSENTRY32 *)mArray.GetBuffer();

	return proctable[index].th32ProcessID;
	}


////////////////////////////////////////////////
unsigned int MProcessList::GetParentPID(int index)	// Get Parent Process ID
	{
	if(index<0 || index>=mProcessCount)
		{
		return 0;
		}

	PROCESSENTRY32 *proctable=(PROCESSENTRY32 *)mArray.GetBuffer();

	return proctable[index].th32ParentProcessID;
	}


////////////////////////////////////////////////
int MProcessList::GetIndex(unsigned int pid)			// Get Index of PID
	{

	PROCESSENTRY32 *proctable=(PROCESSENTRY32 *)mArray.GetBuffer();

	for(int i=0;i<mProcessCount;++i)
		{
		if(proctable[i].th32ProcessID==pid) { return i; }
		}

	return -1;
	}


////////////////////////////////////////////////
int MProcessList::GetThreadCount(int index)			// Total Number of threads in process
	{
	if(index<0 || index>=mProcessCount)
		{
		return 0;
		}

	PROCESSENTRY32 *proctable=(PROCESSENTRY32 *)mArray.GetBuffer();

	return proctable[index].cntThreads;
	}


/////////////////////////////////////////////
bool MProcessList::GetExeName(int index,MString &exename)	// Get Full Path of Process
	{
	if(index<0 || index>=mProcessCount)
		{
		return false;
		}

	PROCESSENTRY32 *proctable=(PROCESSENTRY32 *)mArray.GetBuffer();

	if(exename.Create(proctable[index].szExeFile)==false)
		{
		return false;
		}

	return true;
	}


////////////////////////////////////////////////////////
bool MProcessList::GetFullPath(int index,MString &fullpath)
	{
	if(index<0 || index>=mProcessCount)
		{
		return false;
		}

	unsigned int pid=GetPID(index);

	// Now We Open the process
	HANDLE hprocess=OpenProcess(PROCESS_QUERY_INFORMATION|PROCESS_VM_READ,FALSE,pid);
	if(hprocess==NULL)
		{
		// Unable to open System Process
		fullpath.Create("System Process");
		return false;
		}

	// Get Module Path
	MBuffer buffer(1000);
	if(GetModuleFileNameEx(hprocess,NULL,(LPWSTR)buffer.GetBuffer(),buffer.GetSize() )==0)
		{
		CloseHandle(hprocess);
		return false;
		}

	if(fullpath.Create(buffer.GetWideBuffer())==false)
		{
		CloseHandle(hprocess);
		return false;
		}

	CloseHandle(hprocess);
	return true;
	}

