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
#ifndef MWinProcessList_h
#define MWinProcessList_h

/////////////////////////////////////////////////
#include <Windows.h>
#include <WtsApi32.h>
#include "MStdLib.h"
#include "MString.h"


//******************************************************
//**  MWinProcessList class
//******************************************************
class MWinProcessList
	{
	////////////////////////////////////////////////
	WTS_PROCESS_INFO *mpInfo;
	int mProcCount;
	
	
	
	////////////////////////////////////////////////
	void ClearObject(void);

	////////////////////////////////////////////////
	public:
	MWinProcessList(bool create=false);
	~MWinProcessList(void);
	bool Create(void);
	bool Destroy(void);
	int GetCount(void) const;										// Get Count of processes
	int GetProcessID(int index) const ;
	int GetSessionID(int index) const;
	const PSID GetUserSID(int index) const;
	bool GetUserSIDString(int index,MString &usersid);				// Get SID as a string
	bool GetProcessName(int index,MString &procname) const;
	bool GetFullPath(int index,MString &fullpathname);				// Get Full Path to executable
	int FindProcessIndex(const char *processname);					// =pid of process with name,=-1(on failure)
	bool Print(bool printsid=false,bool printfullpath=false,bool printsessionid=false);
	};

#endif // MWinProcessList_h

