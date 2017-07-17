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
#ifndef MProcessList_h
#define MProcessList_h

///////////////////////////////////////////////////
#include "MDynaBlock.h"
#include "MString.h"

//******************************************************
//**  MProcessList class
//******************************************************
class MProcessList
	{
	////////////////////////////////////////////////
	MDynaBlock mArray;									// Array Contains all info
	int mProcessCount;									// Total Count of Processes in mArray

	////////////////////////////////////////////////
	void ClearObject(void);
	
	////////////////////////////////////////////////
	public:
	MProcessList(bool create=false);
	~MProcessList(void);
	bool Create(void);
	bool Destroy(void);
	int GetCount(void);									// return # of processes running
	unsigned int GetPID(int index);						// Get ProcessId  of Parent
	unsigned int GetParentPID(int index);				// Get Parent Process ID
	int GetIndex(unsigned int pid);						// Get Index of PID
	int GetThreadCount(int index);						// Total Number of threads in process
	bool GetExeName(int index,MString &exename);		// Get Exe Name
	bool GetFullPath(int index,MString &fullpath);		// Get Full Path to Process
	};

#endif // MProcessList_h

