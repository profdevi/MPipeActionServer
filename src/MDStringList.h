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
#ifndef MDStringList_h
#define MDStringList_h

////////////////////////////////////////////////////
#include "MString.h"
#include "TList.h"

//******************************************************
//**  MDStringList class
//******************************************************
class MDStringList
	{
	////////////////////////////////////////////////
	struct GNode
		{
		MString First;
		MString Second;
		};

	// List of Items
	TList<GNode> mList;
	GNode *mReadNode;

	////////////////////////////////////////////////
	void ClearObject(void);
	
	////////////////////////////////////////////////
	public:
	MDStringList(void);
	~MDStringList(void);
	bool Create(void);
	bool Destroy(void);
	bool Add(const char *first,const char *second);
	bool ReadReset(void);
	bool Read(void);
	const char *GetFirst(void);				// Get First String
	const char *GetSecond(void);			// Get Second String
	int GetCount(void);						// Return Count of items
	};

#endif // MDStringList_h

