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
#ifndef MIntArray_h
#define MIntArray_h

//******************************************************
//**  MIntArray class
//******************************************************
class MIntArray
	{
	////////////////////////////////////////////////
	int *mArray;						// Pointer to dynamic memory
	int mCapacity;						// Maximum Capacity
	int mLength;						// Current Length

	////////////////////////////////////////////////
	void ClearObject(void);
	
	////////////////////////////////////////////////
	public:
	MIntArray(void);
	MIntArray(int length,int maxsize);
	~MIntArray(void);
	bool Create(int length,int maxsize);// maximum size of array
	bool Destroy(void);
	bool Set(int index,int value);		// Set value at index
	int Get(int index);					// Get value at index
	bool SetLength(int length);			// Set array length
	int GetCount(void);					// Get Count of Elements
	bool Swap(int index1,int index2);	// Swap values
	bool Set(int value=0);				// Set all items in array to value
	int GetMaxCapacity(void);			// return the maximum capacity
	bool Insert(int index,int value);	// Insert value at index
	bool Insert(int value);				// Insert value at end
	bool Remove(int index);				// Remove item at index
	bool Clear(void);					// Same as SetLength(0);
	bool Print(void);					// Print out array items
	};

#endif // MIntArray_h

