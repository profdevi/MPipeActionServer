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
#ifndef MDynaBlock_h
#define MDynaBlock_h

//*****************************************************
//  MDynaBlock is a moving block of memory which grows 
//  in large block increments.  When memory block is grown,
//  all the old data is moved to the fresh memory location.
//******************************************************

/////////////////////////////////////////////////////////
class MDynaBlock  
	{
	char *mDynaBuffer;  int mDynaBlockSize;
	int mMaxDynaBlockSize;  int mBlockSize;

	///////////////////////////////////////////////
	void ClearObject(void);
	int GetTotalBytesToHold(int databytes);

	///////////////////////////////////////////////
	public:
	MDynaBlock(void);
	MDynaBlock(int dynablocksize,int memblocksize=512);
	~MDynaBlock(void);
	bool Create(int dynablocksize,int memblocksize=512);
	bool Destroy(void);
	bool SetSize(int size);  // If Block Is smaller, access memory is not released!
	int GetSize(void);
	bool ChangeSize(int deltasize);
	void *GetBuffer(void);
	};

#endif //MDynaBlock_h