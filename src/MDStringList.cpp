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


//v2.3 copyright Comine.com 20170703M1828
#include "MStdLib.h"
#include "MString.h"
#include "TList.h"
#include "MDStringList.h"


//******************************************************
//**  MDStringList class
//******************************************************
void MDStringList::ClearObject(void)
	{
	}


////////////////////////////////////////////////
MDStringList::MDStringList(void)
	{  ClearObject();  }


////////////////////////////////////////////////
MDStringList::~MDStringList(void)
	{  Destroy();  }


////////////////////////////////////////////////
bool MDStringList::Create(void)
	{
	Destroy();
	
	if(mList.Create()==false)
		{
		Destroy();
		return false;
		}

	return true;
	}


////////////////////////////////////////////////
bool MDStringList::Destroy(void)
	{
	mList.Destroy();
	ClearObject();
	return true;
	}


///////////////////////////////////////////////
bool MDStringList::Add(const char *first,const char *second)
	{
	GNode *dataptr;
	if(mList.Add(dataptr)==false)
		{
		return false;
		}

	if(dataptr->First.Create(first)==false)
		{
		mList.Remove(dataptr);
		return false;
		}

	if(dataptr->Second.Create(second)==false)
		{
		mList.Remove(dataptr);
		return false;
		}

	return true;
	}


///////////////////////////////////////////////
bool MDStringList::ReadReset(void)
	{
	mReadNode=NULL;
	return mList.ReadReset();
	}


///////////////////////////////////////////////
bool MDStringList::Read(void)
	{
	return mList.Read(mReadNode);
	}


///////////////////////////////////////////////
const char *MDStringList::GetFirst(void)
	{
	return mReadNode->First.Get();
	}


///////////////////////////////////////////////
const char *MDStringList::GetSecond(void)
	{
	return mReadNode->Second.Get();
	}


//////////////////////////////////////////////
int MDStringList::GetCount(void)
	{
	return mList.GetCount();
	}

