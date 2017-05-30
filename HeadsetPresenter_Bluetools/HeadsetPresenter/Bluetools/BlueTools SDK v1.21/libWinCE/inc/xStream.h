
// Franson C++ Simple Framework
// (c) 2006 Franson Technology AB, Sweden, All rights reserved
// http://franson.com

#pragma once

#include "xObject.h"

namespace xCore
{
	class xStream;
	class xStreamResult;

	class xIStreamEvents
	{
	public:
		virtual void OnReadCompleted(xStream* source, xStreamResult* results) = 0;
		virtual void OnWriteCompleted(xStream* source, xStreamResult* results) = 0; 
	};

	// .NET like Stream

	class XLIB xStream : public xObject
	{
	public:
		~xStream();

		// Number of bytes in buffer
		virtual int GetLength();

		// Sync reading
		virtual int Read(unsigned char* buffer, int offset, int count);
		virtual int ReadByte();

		// Sync writing
		virtual void Write(unsigned char* buffer, int offset, int count);
		virtual void WriteByte(unsigned char value);

		// Async reading
		// buffer: Buffer to store read data into
		// offset: Offset into buffer were data is to be stored
		// count:  Length of buffer
		// events: Pointer to object that implements xIStreamEvents::OnReadCompleted()
		// retVal: Pointer to pointer where xStreamResult can be returned. xStreamResult must be passed to EndRead()
		virtual void BeginRead(unsigned char* buffer, int offset, int count, xIStreamEvents* events, xObject* objState, xStreamResult** retVal);

		// Will lock until BeginRead() has finished.
		// Can typically be called in xIStreamEvents::OnReadCompleted() callback
		virtual int EndRead(xStreamResult* streamResults);

		// Async writing
		virtual void BeginWrite(unsigned char* buffer, int offset, int count, xIStreamEvents* events, xObject* objState, xStreamResult** retVal);
		virtual void EndWrite(xStreamResult* streamResults);

		// Close stream
		virtual void Close();

	protected:
		xStream(void* internal);

	private:
		void m_thread_async_read();
		void m_thread_async_write();

		HANDLE m_evReadAsyncTask;
		HANDLE m_evWriteAsyncTask;

		HANDLE m_evReadAsyncExit;
		HANDLE m_evWriteAsyncExit;

		HANDLE m_threadReadAsync;
		HANDLE m_threadWriteAsync;

		xStreamResult* m_resultsReadAsync;
		xStreamResult* m_resultsWriteAsync;

		static DWORD WINAPI m_static_stub_read(LPVOID lpParameter);
		static DWORD WINAPI m_static_stub_write(LPVOID lpParameter);

		bool m_bClosed;
	};
}
