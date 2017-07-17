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
#undef UNICODE
#include <windows.h>
#include "MStdLib.h"
#include "MBuffer.h"
#include "MString.h"
#include "MProcessInfo.h"


//******************************************************
//**  ModuleInfo
//******************************************************
static const int GBufferSize=2048;


//******************************************************
//**  MProcessInfo class
//******************************************************
void MProcessInfo::ClearObject(void)
	{
	}


////////////////////////////////////////////////
MProcessInfo::MProcessInfo(void)
	{  ClearObject();  }


////////////////////////////////////////////////
MProcessInfo::~MProcessInfo(void)
	{  Destroy();  }


////////////////////////////////////////////////
bool MProcessInfo::Create(void)
	{
	Destroy();

	MBuffer buffer;
	if(buffer.Create(GBufferSize)==false)
		{
		Destroy();  return false;
		}

	//*Get Current Directory
	DWORD len=::GetCurrentDirectoryA(buffer.GetSize()-1,buffer.GetBuffer());
	if(len==0 || len>=(DWORD)buffer.GetSize()-1)
		{
		Destroy();  return false;
		}

	if(mCurrentDir.Create(buffer.GetBuffer())==false)
		{
		Destroy();  return false;
		}

	//*Get Executable Path
	len=::GetModuleFileNameA(NULL,buffer.GetBuffer(),buffer.GetSize()-1);
	if(len==0 || len>=(DWORD)buffer.GetSize()-1)
		{
		Destroy();  return false;
		}

	if(mExePath.Create(buffer.GetBuffer())==false)
		{
		Destroy();  return false;
		}

	return true;
	}


////////////////////////////////////////////////
bool MProcessInfo::Destroy(void)
	{
	mCurrentDir.Destroy();
	mExePath.Destroy();
	ClearObject();
	return true;
	}


///////////////////////////////////////////////
const char *MProcessInfo::GetCurrentDir(void)
	{
	return mCurrentDir.Get();
	}


///////////////////////////////////////////////
const char *MProcessInfo::GetExePath(void)
	{
	return mExePath.Get();
	}


///////////////////////////////////////////////
bool MProcessInfo::SetCurrentDir(const char *dir)
	{
	if(dir==NULL || *dir==0)
		{
		return false;
		}

	BOOL ret=SetCurrentDirectoryA(dir);
	if(ret==FALSE)
		{
		//=Unable to set current directory
		return false;
		}

	return true;
	}

