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


//v1.9 copyright Comine.com 20160428R0819
#include <windows.h>
#include "MStdLib.h"
#include "MBuffer.h"
#include "MFileOps.h"
#include "MProcessJob.h"


//******************************************************
//**  MProcessJob class
//******************************************************
void MProcessJob::ClearObject(void)
	{
	MStdMemSet(&mProcInfo,0,sizeof(mProcInfo) );
	mTopWindow=NULL;
	}


/////////////////////////////////////////////////
BOOL CALLBACK MProcessJob::GGetTopWindow(HWND hwnd,LPARAM lparam)
	{
	MProcessJob *ref=(MProcessJob *)lparam;
	if(ref==NULL)
		{
		return FALSE;
		}

	DWORD procid;
	DWORD threadid;
	threadid=GetWindowThreadProcessId(hwnd,&procid);
	
	if(procid==ref->GetProcessID())
		{
		ref->mTopWindow=hwnd;
		return FALSE;
		}

	return TRUE;
	}


/////////////////////////////////////////////////
MProcessJob::MProcessJob(void)
	{  ClearObject();  }


////////////////////////////////////////////////
MProcessJob::~MProcessJob(void)
	{  Destroy();  }


////////////////////////////////////////////////
bool MProcessJob::Create(const char *commandline,HANDLE hstdin,HANDLE hstdout,HANDLE hstderr)
	{
	Destroy();
	STARTUPINFOA startinfo;
	MStdMemSet(&startinfo,0,sizeof(startinfo) );

	// Set Inherit handles to false
	BOOL inherithandles=FALSE;

	// Dumplicate stdin
	HANDLE hchildstdin=INVALID_HANDLE_VALUE;
	if(hstdin!=INVALID_HANDLE_VALUE)
		{
		// 
		if(DuplicateHandle(GetCurrentProcess()
				,hstdin,GetCurrentProcess(),&hchildstdin,0
				,TRUE,DUPLICATE_SAME_ACCESS) ==FALSE)
			{
			Destroy();
			return false;
			}
		}

	// Duplicate Stdout
	HANDLE hchildstdout=INVALID_HANDLE_VALUE;
	if(hstdout!=INVALID_HANDLE_VALUE)
		{
		// duplicate the handle
		if(DuplicateHandle(GetCurrentProcess()
				,hstdout,GetCurrentProcess(),&hchildstdout,0
				,TRUE,DUPLICATE_SAME_ACCESS) ==FALSE)
			{
			Destroy();
			CloseHandle(hchildstdin);
			return false;
			}
		}

	// Duplicate StdError
	HANDLE hchildstderr=INVALID_HANDLE_VALUE;
	if(hstderr!=INVALID_HANDLE_VALUE)
		{
		// duplicate the handle
		if(DuplicateHandle(GetCurrentProcess()
				,hstderr,GetCurrentProcess(),&hchildstderr,0
				,TRUE,DUPLICATE_SAME_ACCESS) ==FALSE)
			{
			Destroy();
			CloseHandle(hchildstdin);
			CloseHandle(hchildstderr);
			return false;
			}
		}


	if(hchildstdin!=INVALID_HANDLE_VALUE 
			|| hchildstdout!=INVALID_HANDLE_VALUE
			|| hchildstderr!=INVALID_HANDLE_VALUE )
		{
		startinfo.hStdInput=hchildstdin;
		startinfo.hStdOutput=hchildstdout;
		startinfo.hStdError=hchildstderr;

		startinfo.dwFlags |= STARTF_USESTDHANDLES;
		inherithandles=TRUE;
		}
	
	startinfo.cb=sizeof(startinfo);

	MBuffer bufcommandline(2000);
	bufcommandline.SetString(commandline);

	if(CreateProcessA(NULL,bufcommandline,NULL,NULL
			,inherithandles,NORMAL_PRIORITY_CLASS
			,NULL,NULL,&startinfo,&mProcInfo)==FALSE)
		{
		Destroy();
		CloseHandle(hchildstdin);
		CloseHandle(hchildstdout);
		CloseHandle(hchildstderr);
		return false;
		}

	// Close extra handles if necessary
	CloseHandle(hchildstdin);
	CloseHandle(hchildstdout);
	CloseHandle(hchildstderr);
	return true;
	}


////////////////////////////////////////////////
bool MProcessJob::Create(DWORD processid,bool fullaccess)
	{
	Destroy();
	HANDLE hprocess;
	if(fullaccess==true)
		{
		hprocess=OpenProcess(PROCESS_ALL_ACCESS,FALSE,processid);
		}
	else
		{
		hprocess=OpenProcess(PROCESS_TERMINATE|PROCESS_SUSPEND_RESUME
					|PROCESS_DUP_HANDLE|PROCESS_QUERY_INFORMATION
				,FALSE,processid);
		}

	if(hprocess==NULL)
		{
		Destroy();  return false;
		}

	mProcInfo.hProcess=hprocess;
	mProcInfo.dwProcessId=processid;

	return true;
	}


////////////////////////////////////////////////
bool MProcessJob::Destroy(void)
	{
	if(mProcInfo.hThread!=NULL) { CloseHandle(mProcInfo.hThread);  }
	if(mProcInfo.hProcess!=NULL) { CloseHandle(mProcInfo.hProcess);  }
	ClearObject();  return true;
	}


/////////////////////////////////////////////////
HANDLE MProcessJob::GetProcessHandle(void)
	{
	return mProcInfo.hProcess;
	}

/////////////////////////////////////////////////
HANDLE MProcessJob::GetThreadHandle(void)
	{
	return mProcInfo.hThread;
	}


/////////////////////////////////////////////////
DWORD MProcessJob::GetProcessID(void)
	{
	return mProcInfo.dwProcessId;
	}


/////////////////////////////////////////////////
DWORD MProcessJob::GetThreadID(void)
	{
	return mProcInfo.dwThreadId;
	}


/////////////////////////////////////////////////
int MProcessJob::Wait(DWORD timeout)// timeout is in ms
	{
	if(mProcInfo.hProcess==NULL)
		{
		return false;
		}

	DWORD dwret=WaitForSingleObject(mProcInfo.hProcess,timeout);
	if(dwret==WAIT_OBJECT_0) { return 1; }
	else if(dwret==WAIT_TIMEOUT) { return 0; }
	else  {  return -1;  }
	}


//////////////////////////////////////////////////
bool MProcessJob::Kill(void) // Kills Process
	{
	if(mProcInfo.hProcess==NULL)
		{
		return false;
		}

	if(TerminateProcess(mProcInfo.hProcess,255)==FALSE)
		{
		return false;
		}

	return true;
	}


////////////////////////////////////////////////////
bool MProcessJob::IsAlive(void)		//=true if process is alive
	{
	DWORD exitcode=0;
	if(GetExitCodeProcess(mProcInfo.hProcess,&exitcode)==FALSE)
		{
		return false;
		}

	if(exitcode==STILL_ACTIVE) { return true; }

	return false;
	}


////////////////////////////////////////////////////////
int MProcessJob::GetExitValue(void)	//=-1 if still running
	{
	DWORD exitcode=0;
	if(GetExitCodeProcess(mProcInfo.hProcess,&exitcode)==FALSE)
		{
		return -1;
		}

	if(exitcode==STILL_ACTIVE) { return -1; }

	return (int)exitcode;
	}


/////////////////////////////////////////////////////////
HWND MProcessJob::GetTopWindow(void)
	{
	if(mTopWindow==NULL)
		{  EnumWindows(GGetTopWindow,(LPARAM)this);  }

	return mTopWindow;
	}

