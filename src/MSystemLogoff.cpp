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


//v1.3 copyright Comine.com 20170709U0655
#include <windows.h>
#include <string.h>
#include "MSystemLogoff.h"


//////////////////////////////////////////////////
#pragma comment(lib,"advapi32.lib")
#pragma comment(lib,"user32.lib")

//******************************************************
//**  MSystemLogoff class
//******************************************************
void MSystemLogoff::ClearObject(void)
	{
	}


////////////////////////////////////////////////
MSystemLogoff::MSystemLogoff(bool create)
	{
	ClearObject();
	if(create==true && Create()==false)
		{
		return;
		}
	}


////////////////////////////////////////////////
MSystemLogoff::~MSystemLogoff(void)
	{  Destroy();  }


////////////////////////////////////////////////
bool MSystemLogoff::Create(void)
	{
	Destroy();

	// Change Token Privledges
	HANDLE htoken=NULL; 	

	// Get a token for this process.
	if (OpenProcessToken(GetCurrentProcess(),
			TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, &htoken)==FALSE) 
		{
		Destroy();
		return false;
		}

	// Get the LUID for the shutdown privilege. 
	TOKEN_PRIVILEGES tokenprivleges;
	memset(&tokenprivleges,0,sizeof(tokenprivleges));

	if(LookupPrivilegeValue(NULL, SE_SHUTDOWN_NAME
				,&(tokenprivleges.Privileges[0].Luid) )==FALSE)
		{
		CloseHandle(htoken);
		Destroy();
		return false;
		}

	// Change Privledges
	tokenprivleges.PrivilegeCount = 1;
	tokenprivleges.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED; 

	// Get the shutdown privilege for this process. 

	if(AdjustTokenPrivileges(htoken, FALSE, &tokenprivleges, 0, 
			(PTOKEN_PRIVILEGES)NULL, 0)==FALSE)
		{
		CloseHandle(htoken);
		Destroy();
		return false;
		}

	// Close process token
	CloseHandle(htoken);
	
	return true;
	}


////////////////////////////////////////////////
bool MSystemLogoff::Destroy(void)
	{
	ClearObject();
	return true;
	}


///////////////////////////////////////////////
bool MSystemLogoff::LogOff(void)
	{
	// Shut down the system and force all applications to close. 
	if (ExitWindowsEx(EWX_LOGOFF, 0)==FALSE) 
		{
		return false;
		}

	return true;
	}


////////////////////////////////////////////////////////////////////////
bool MSystemLogoff::LogOffForced(void)
	{
	if (ExitWindowsEx(EWX_LOGOFF|EWX_FORCE, 0)==FALSE) 
		{
		return false;
		}
	
	return true;
	}


/////////////////////////////////////////////////////////////////////
bool MSystemLogoff::Reboot(void)
	{
	if (ExitWindowsEx(EWX_REBOOT, 0)==FALSE) 
		{
		return false;
		}

	return true;
	}


//////////////////////////////////////////////////////////////////////
bool MSystemLogoff::RebootForced(void)
	{
	if (ExitWindowsEx(EWX_REBOOT|EWX_FORCE, 0)==FALSE) 
		{
		return false;
		}

	return true;
	}


/////////////////////////////////////////////////////////////////////
bool MSystemLogoff::Shutdown(void)
	{
	if (ExitWindowsEx(EWX_SHUTDOWN, 0)==FALSE) 
		{
		return false;
		}

	return true;
	}


/////////////////////////////////////////////////////////////////////
bool MSystemLogoff::ShutdownForced(void)
	{
	if (ExitWindowsEx(EWX_SHUTDOWN|EWX_FORCE, 0)==FALSE)
		{
		return false;
		}

	return true;	
	}


//////////////////////////////////////////////////////////////////////
bool MSystemLogoff::PowerOff(void)
	{
	if (ExitWindowsEx(EWX_POWEROFF, 0)==FALSE)
		{
		return false;
		}

	return true;
	}


//////////////////////////////////////////////////////////////////////
bool MSystemLogoff::PowerOffForced(void)
	{
	if (ExitWindowsEx(EWX_POWEROFF|EWX_FORCE, 0)==FALSE)
		{
		return false;
		}

	return true;	
	}

