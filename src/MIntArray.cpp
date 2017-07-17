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


//v1.7 copyright Comine.com 20170703M1820
#include "MStdLib.h"
#include "MIntArray.h"


//******************************************************
//**  MIntArray class
//******************************************************
void MIntArray::ClearObject(void)
	{
	mArray=NULL;
	mCapacity=0;
	mLength=0;	
	}


////////////////////////////////////////////////
MIntArray::MIntArray(void)
	{	ClearObject();  }


////////////////////////////////////////////////
MIntArray::MIntArray(int length,int maxsize)
	{
	ClearObject();
	if(Create(length,maxsize)==false)
		{
		return;
		}
	}


////////////////////////////////////////////////
MIntArray::~MIntArray(void)
	{  Destroy();  }


////////////////////////////////////////////////
bool MIntArray::Create(int length,int maxcapacity)
	{
	Destroy();

	if(length<0 || maxcapacity<=0)
		{
		Destroy();
		return false; 
		}

	if(maxcapacity<length){ maxcapacity=length;  }

	mArray=new(std::nothrow) int[maxcapacity];
	if(mArray==NULL)
		{
		Destroy();
		return false;
		}

	mCapacity=maxcapacity;
	if(length>=0)
		{ mLength=length; }
	else
		{ mLength=0;  }
	
	return true;
	}


////////////////////////////////////////////////
bool MIntArray::Destroy(void)
	{
	if(mArray!=NULL)
		{
		delete[] mArray;
		mArray=NULL;
		}

	ClearObject();
	return true;
	}


///////////////////////////////////////////////
bool MIntArray::Set(int index,int value)
	{
	MStdAssert(index>=0 && index<mLength);
	mArray[index]=value;
	return true;
	}


///////////////////////////////////////////////
int MIntArray::Get(int index)
	{
	MStdAssert(index>=0 && index<mLength);
	return mArray[index];
	}


///////////////////////////////////////////////
bool MIntArray::SetLength(int length)		// Set array length
	{
	if(length<0 || length>=mCapacity)
		{
		return false;
		}

	mLength=length;
	return true;
	}


///////////////////////////////////////////////
int MIntArray::GetCount(void)				// Get Count of Elements
	{
	return mLength;
	}


////////////////////////////////////////////////
bool MIntArray::Swap(int index1,int index2)	// Swap values
	{
	MStdAssert(index1>=0 && index1<mLength);
	MStdAssert(index2>=0 && index2<mLength);

	// Swap routine
	int *p1=mArray+index1; int *p2=mArray+index2;
	int tmp=*p1; *p1=*p2; *p2=tmp;

	return true;
	}


//////////////////////////////////////////////////
bool MIntArray::Set(int value)
	{
	for(int *p=mArray+mLength-1;p>=mArray;--p)
		{ *p=value; }

	return true;
	}


///////////////////////////////////////////////////
int MIntArray::GetMaxCapacity(void)		// return the maximum capacity
	{
	return mCapacity;
	}


////////////////////////////////////////////////////
bool MIntArray::Insert(int index,int value)	// Insert value at index
	{
	if(mLength>=mCapacity)
		{
		return false;
		}

	if(index<0 || index>mLength)
		{
		return false;
		}

	for(int i=mLength;i>index;--i)
		{  mArray[i]=mArray[i-1];  }

	mArray[index]=value;
	mLength+=1;  return true;
	}


////////////////////////////////////////////////////
bool MIntArray::Insert(int value)				// Insert value at end
	{
	if(mLength>=mCapacity)
		{
		return false;
		}

	mArray[mLength]=value;  mLength+=1;
	return true;
	}


////////////////////////////////////////////////////
bool MIntArray::Remove(int index)				// Remove item at index
	{
	if(index<0 || index>=mLength)
		{
		return false;
		}

	for(int i=index+1;i<mLength;++i)
		{  mArray[i-1]=mArray[i];  }

	mLength-=1;  return true;
	}


/////////////////////////////////////////////////
bool MIntArray::Clear(void)
	{
	mLength=0;
	return true;
	}


/////////////////////////////////////////////////
bool MIntArray::Print(void)
	{
	int i;
	for(i=0;i<mLength;++i)
		{
		MStdPrintf("%d ",mArray[i] );
		}

	MStdPrintf("\n");
	return true;
	}


