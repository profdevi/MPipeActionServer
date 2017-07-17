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


//v1.3 copyright Comine.com 20170703M1821
#ifndef MProcessInfo_h
#define MProcessInfo_h

/////////////////////////////////////////////////////
#undef UNICODE
#include <windows.h>
#include "MString.h"

//******************************************************
//**  MProcessInfo class
//******************************************************
class MProcessInfo
	{
	////////////////////////////////////////////////
	MString mCurrentDir;				// Holds Current Directory
	MString mExePath;					// Holds Exe Path
	
	////////////////////////////////////////////////
	void ClearObject(void);
	
	////////////////////////////////////////////////
	public:
	MProcessInfo(void);
	~MProcessInfo(void);
	bool Create(void);
	bool Destroy(void);
	const char *GetCurrentDir(void);
	const char *GetExePath(void);
	bool SetCurrentDir(const char *dir);
	};

#endif // MProcessInfo_h

