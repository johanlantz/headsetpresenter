
// Franson C++ Simple Framework
// (c) 2006 Franson Technology AB, Sweden, All rights reserved
// http://franson.com

#pragma once

#include "xObject.h"

namespace xCore
{
	class xStream;
	class xIStreamEvents;

	class XLIB xStreamResult : public xObject
	{
		friend xStream;
	public:

		virtual ~xStreamResult();

		// Parameters from BeginWrite() or BeginRead()
		//

		// Buffer of data
		virtual unsigned char* GetBuffer();
		// Offset into buffer where data is stored
		virtual int GetOffset();
		// Length
		virtual int GetCount();
		// State object
		virtual void GetState(xObject** retVal); 
		virtual void SetState(xObject* objState); 

		// Returns a handle to an event that will be set when async operation is finished.
		virtual HANDLE GetEventHandle();

		// Return event interface, is called if set after async operation is completed
		virtual xIStreamEvents* GetEvents();

	protected:
		xStreamResult(unsigned char* buffer, int iOffset, int iCount, xIStreamEvents* callback, xObject* objState);

	private:
		HANDLE m_evCompleted;
		xObject* m_objState;
		unsigned char* m_buffer;
		int m_iOffset;
		int m_iCount;
		xIStreamEvents* m_events;
		xStream* m_streamOwner;
	};
}
