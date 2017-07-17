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
#ifndef MProcessControl_h
#define MProcessControl_h

////////////////////////////////////////////////
#include <windows.h>
#include "MString.h"
#include "MStringList.h"
#include "MIntArray.h"
#include "TList.h"
#include "MDStringList.h"

//******************************************************
//**  MProcessControl class
//******************************************************
class MProcessControl
	{
	////////////////////////////////////////////////
	void ClearObject(void);
	
	////////////////////////////////////////////////
	public:
	MProcessControl(bool create=false);
	~MProcessControl(void);
	bool Create(void);
	bool Destroy(void);
	bool Kill(int pid,int exitcode=0);						// Kill the given process. Very Dangerous
	int Wait(int pid,DWORD timeout=INFINITE);				// 0-timed out,1-process done,-1=error
	int GetGDICount(int pid);								// Get GDI object count for process
	int GetUserCount(int pid);								// Get User object count for process
	bool GetIOCounters(int pid,IO_COUNTERS &counters);		// Get IO Counters Info
	bool IsAlive(int pid);									// =true if process is alive
	int GetCurrentPID(void);								// Get Current Process ID
	int GetParentPID(int pid);								// Get Parent ProcessID
	bool GetPIDs(MIntArray &pids);							// Get a list of all processids
	bool GetModules(int pid,MStringList &modules);			// Get Modules(exe+dlls) for Process
	bool CanQueryProcess(int pid);							// Can we query info about process
	bool GetDeviceDrivers(MDStringList &devices);			// Get Devices
	int GetPID(const char *processname);					// Exe File Name
	bool GetTopWindows(TList<HWND> &winhandles);			// Get Topmost Windows list of HWNDS
	bool GetTopWindows(int pid,TList<HWND> &winhandles);	// Get Topmost Windows of Process
	bool Close(int pid,int timeout=3000);					// Send WM_CLOSE to top windows
	bool GetWindowsWithText(const char *txt,TList<HWND> &list);		// Get Window Handle with ends with text in title
	};

#endif // MProcessControl_h


