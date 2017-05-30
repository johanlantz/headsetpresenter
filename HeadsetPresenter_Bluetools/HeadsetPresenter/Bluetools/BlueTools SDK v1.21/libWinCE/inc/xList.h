
// Franson C++ Simple Framework
// (c) 2006 Franson Technology AB, Sweden, All rights reserved
// http://franson.com

#pragma once

#include "xObject.h"

namespace xCore
{
	typedef struct x_node
	{
		xObject* obj;
		x_node* next;
	} x_node;

	class xList : public xObject
	{
	public:
		xList();
		~xList();

		// Add object to end of list
		virtual void Add(xObject*);

		// Return object at specified index
		virtual void Get(int iIndex, xObject** retVal);

		// Removes objects, and returns which index removed object had
		// Returns -1 if object was not part of list
		virtual int Remove(xObject*);

		// Remove object at specified index
		virtual void Remove(int iIndex);

		// Returns number of items in list
		virtual int GetLength();

		// Removes all items in list
		virtual void Clear();

	private:
		xList(void* internal);

		x_node* m_nodeFirst;

		int m_iLength;
		void m_init();
	};
}
