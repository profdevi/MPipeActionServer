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


//v1.6 copyright Comine.com 20150902R1013
#ifndef TList_h
#define TList_h

////////////////////////////////////////////
#include "MStdLib.h"

////////////////////////////////////////////
template <class DataInfo>
class TList
	{
	struct TListNode
		{
		DataInfo Data;
		TListNode *Next;
		};

	//////////////////////////////////////////
	TListNode *mListHead;					// Head of List
	TListNode *mListTail;					// End of List
	TListNode *mReaderNode;					// Reader Reference
	int mCount;								// Count of nodes

	//////////////////////////////////////////
	void ClearObject(void)
		{
		mListHead=NULL;
		mListTail=NULL;
		mReaderNode=NULL;
		mCount=0;
		}

	//////////////////////////////////////////
	public:
	TList(bool init=false)
		{
		ClearObject();
		if(init==true && Create()==false)
			{
			return;
			}
		}

	//////////////////////////////////////////
	~TList(void)
		{  Destroy();  }

	//////////////////////////////////////////
	bool Create(void)
		{
		Destroy();

		return true;
		}

	//////////////////////////////////////////
	bool Destroy(void)
		{
		TListNode *p;
		TListNode *tmp;
		for(p=mListHead;p!=NULL; )
			{
			tmp=p->Next;
			delete p;
			p=tmp;
			}

		ClearObject();
		return true;
		}


	//////////////////////////////////////////
	bool Add(DataInfo *&ref)
		{
		TListNode *newnode=new(std::nothrow) TListNode;
		if(newnode==NULL)
			{
			ref=NULL;
			return false;
			}

		if(mListHead==NULL)
			{
			mCount=mCount+1;
			newnode->Next=NULL;
			mListHead=newnode;
			mListTail=newnode;
			ref=&(newnode->Data);
			return true;
			}

		// Add to end of list
		newnode->Next=NULL;
		mCount=mCount+1;
		mListTail->Next=newnode;
		mListTail=newnode;
		ref = &(newnode->Data);
		return true;
		}

	//////////////////////////////////////////
	DataInfo *Add(DataInfo &ref)
		{
		DataInfo *ptr;
		ptr=NULL;
		if(Add(ptr)==false)
			{
			return NULL;
			}

		// Copy Construction must be set
		*ptr = ref;
		return ptr;
		}


	/////////////////////////////////////////
	bool Remove(DataInfo *ref)
		{
		MStdAssert(ref!=NULL);
		if(mListHead==NULL)
			{
			return false;
			}

		// Check if node is in first node
		if(&(mListHead->Data)==ref)
			{
			TListNode *tmp=mListHead;
			mListHead=mListHead->Next;
			delete tmp;

			mCount=mCount-1;

			if(mListHead==NULL)
				{  mListTail=NULL;  }

			return true;
			}

		// Unable to alloc node
		TListNode *p;
		for(p=mListHead;p->Next!=NULL;p=p->Next)
			{
			if(&(p->Next->Data)!=ref)
				{  continue;  }

			TListNode *tmp=p->Next;
			p->Next=p->Next->Next;
			delete tmp;
			mCount=mCount-1;

			if(tmp==mListTail)
				{  mListTail=p; }

			return true;
			}
	
		return false;
		}

	/////////////////////////////////////////
	bool Remove(DataInfo &ref)
		{
		if(mListHead==NULL)
			{
			return false;
			}

		// Check if node is in first node
		// Operator == must be overloaded
		if(mListHead->Data==ref)
			{
			TListNode *tmp=mListHead;
			mListHead=mListHead->Next;
			delete tmp;
			mCount=mCount-1;

			if(mListHead==NULL)
				{  mListTail=NULL;  }

			return true;
			}

		// Unable to alloc node
		TListNode *p;
		for(p=mListHead;p->Next!=NULL;p=p->Next)
			{
			if(!(p->Next->Data==ref))
				{  continue;  }

			TListNode *tmp=p->Next;
			p->Next=p->Next->Next;
			delete tmp;
			mCount=mCount-1;

			if(tmp==mListTail)
				{  mListTail=p; }

			return true;
			}

		return false;
		}

	///////////////////////////////////////////
	bool ReadReset(void)
		{
		mReaderNode=mListHead;
		return true;
		}

	///////////////////////////////////////////
	bool Read(DataInfo *&ref)
		{
		if(mReaderNode==NULL)
			{  
			ref=NULL;
			return false;
			}

		ref=&(mReaderNode->Data);
		mReaderNode=mReaderNode->Next;
		return true;
		}

	///////////////////////////////////////////
	int GetCount(void)
		{
		return mCount;
		}

	////////////////////////////////////////////
	bool IsMember(DataInfo &data)
		{
		TListNode *p;
		for(p=mListHead;p!=NULL;p=p->Next)
			{
			// Overloaded == must be set
			if(p->Data == data)
				{  return true;  }
			}
		
		return false;
		}
	};


#endif // TList_h

