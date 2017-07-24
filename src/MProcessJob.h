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
#ifndef MProcessJob_h
#define MProcessJob_h

////////////////////////////////////////////////////
#include <windows.h>

//******************************************************
//**  MProcessJob class
//******************************************************
class MProcessJob
	{
	PROCESS_INFORMATION mProcInfo;
	HWND mTopWindow;

	////////////////////////////////////////////////
	void ClearObject(void);
	static BOOL CALLBACK GGetTopWindow(HWND hwnd,LPARAM lparam);

	////////////////////////////////////////////////
	public:
	MProcessJob(void);
	~MProcessJob(void);
	bool Create(const char *commandline
			,HANDLE hstdin=INVALID_HANDLE_VALUE
			,HANDLE hstdout=INVALID_HANDLE_VALUE
			,HANDLE hstderr=INVALID_HANDLE_VALUE);
	bool Create(DWORD processid,bool fullaccess=false);
	bool Destroy(void);
	HANDLE GetProcessHandle(void);
	HANDLE GetThreadHandle(void);
	DWORD GetProcessID(void);
	DWORD GetThreadID(void);
	int Wait(DWORD timeout=INFINITE);	// 0-timed out,1-process done,-1=error
	bool Kill(void);		// Kills Process Very SEVERE for global data in dlls
	bool IsAlive(void);		//=true if process is alive
	int GetExitValue(void);	//=-1 if still running
	HWND GetTopWindow(void);	// Returns the top level window
	};

#endif // MProcessJob_h

