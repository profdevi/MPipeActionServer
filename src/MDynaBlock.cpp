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


//v1.9 copyright Comine.com 20170703M1814
#include "MStdLib.h"
#include "MDynaBlock.h"


//***************************************************
//** Module Elements
//***************************************************
static const char GBlockInitChar=0;


//***************************************************
//** MDynaBlock
//***************************************************
void MDynaBlock::ClearObject(void)
	{
	mDynaBuffer=0;
	mDynaBlockSize=0;
	mMaxDynaBlockSize=0;
	mBlockSize=0;		
	}


///////////////////////////////////////////
int MDynaBlock::GetTotalBytesToHold(int databytes)
	{
	return ((databytes+mBlockSize-1)/mBlockSize)*mBlockSize;
	}


///////////////////////////////////////////
MDynaBlock::MDynaBlock(void)
	{  ClearObject();  }


///////////////////////////////////////////
MDynaBlock::MDynaBlock(int dynablocksize,int memblocksize)
	{
	ClearObject();
	if(Create(dynablocksize,memblocksize)==false)
		{
		return;
		}
	}


///////////////////////////////////////////
MDynaBlock::~MDynaBlock(void)
	{  Destroy();  }


///////////////////////////////////////////
bool MDynaBlock::Create(int dynablocksize,int memblocksize)
	{
	Destroy();

	if(memblocksize<=0)
		{
		Destroy(); return false;
		}

	mBlockSize=memblocksize; // Called first before SetSize()
	if(SetSize(dynablocksize)==false)
		{
		Destroy();  return false;
		}

	return true;
	}


///////////////////////////////////////////
bool MDynaBlock::Destroy(void)
	{
	if(mDynaBuffer!=0)
		{
		delete[] mDynaBuffer;
		mDynaBuffer=NULL;
		}

	ClearObject();
	return true;
	}


///////////////////////////////////////////
bool MDynaBlock::SetSize(int size)  // If Block Is smaller, extra memory is not released!
	{
	if(mBlockSize<=0)
		{
		return false;
		}

	if(size<0)
		{
		return false;
		}

	if(mDynaBuffer==0) // Create a fresh block
		{
		mMaxDynaBlockSize=GetTotalBytesToHold(size);
		mDynaBlockSize=size;
		if(mMaxDynaBlockSize==0) { return true; }

		mDynaBuffer=new(std::nothrow) char[mMaxDynaBlockSize];
		if(mDynaBuffer==0)
			{
			mMaxDynaBlockSize=0;
			mDynaBlockSize=0;
			return false;
			}
		
		MStdMemSet(mDynaBuffer,GBlockInitChar,mMaxDynaBlockSize);
		return true;
		}

	// If size <mMaxDynaBlockSize, still safe
	if(size<=mMaxDynaBlockSize) { mDynaBlockSize=size;  return true; }

	// size is bigger, we need to allocate more memory
	int newmaxdynasize=GetTotalBytesToHold(size);
	char *newbuffer=new(std::nothrow) char[newmaxdynasize];
	if(newbuffer==0)
		{
		return false;
		}
	
	MStdMemSet(newbuffer,GBlockInitChar,newmaxdynasize);  // Init memory block
	MStdMemCpy(newbuffer,mDynaBuffer,mDynaBlockSize); // Copy Old Data to fresh block
	delete[] mDynaBuffer;

	mDynaBuffer=newbuffer;
	mDynaBlockSize=size;
	mMaxDynaBlockSize=newmaxdynasize;
	return true;
	}


///////////////////////////////////////////
int MDynaBlock::GetSize(void)
	{
	return mDynaBlockSize;
	}


///////////////////////////////////////////
bool MDynaBlock::ChangeSize(int deltasize)
	{
	if(mDynaBlockSize+deltasize <0) { deltasize=-mDynaBlockSize; }

	if(SetSize(mDynaBlockSize+deltasize)==false)
		{
		return false;
		}

	return true;
	}


///////////////////////////////////////////
void *MDynaBlock::GetBuffer(void)
	{
	return mDynaBuffer;
	}

