
// Franson C++ Simple Framework
// (c) 2006 Franson Technology AB, Sweden, All rights reserved
// http://franson.com

#pragma once

#ifdef XLIB_EXPORT
#define XLIB __declspec(dllexport)
#else
#define XLIB __declspec(dllexport)
#endif

namespace xCore
{
	// All xObjects in the C++ Framework must inherit from Core::xObject

	class xString;
	class xException;

	class XLIB xObject
	{
	public:
		xObject();
		virtual ~xObject();

		// Returns true if argument is logical equal to "this".
		virtual bool Equals(xObject*);

		// Returns a xString representation of object 
		virtual void ToString(xString** retVal);

		// Memory management.
		// Reference counting. Same paradigm as for ActiveX/COM

		// Call AddReference() when you assign a new pointer to this xObject
		void AddReference();

		// Call ReleaseReference() when a pointer no linger references this xObject
		// When all references are released delete this; will be called
		long ReleaseReference();

		// Set to true if C++ exception should be used
		static void SetUseExceptions(bool bState);
		static bool GetUseExceptions();

		// Locking an object for other threads
		void LockObject();
		void UnlockObject();

		// Error handling without exceptions
		//

		// Returns true if last operation on this object generated an error
		virtual bool IsError();

		// Returns the xException object
		virtual xException* GetException();

		// Returns number of object instances of xObject currently allocated.
		// Can be used for simple memory leak detection
		static int GetInstanceCount();

		// START: Internal use only
		void* GetInternal();
		virtual void DispatchEvent(void*) {}
		// END: Internal use only

	protected:
		xObject(void* internal);
		void* m_internal;

		virtual void m_check_error();

		// Memory leak check
		static int m_iInstanceCount;

	private:
		long m_iRefCount;
		static bool m_bUseExceptions;

		bool m_bIsError;
		xException* m_lastException;
		xString* m_strErrorMessage;
		CRITICAL_SECTION* m_pCS;

		void m_init();
	};

	// Some macros to make reference counting simpler

	// Use X_RELEASE() when a pointer is no longer referenceing an xObject
	// Calls obj->ReleaseReference() if pointer obj is not NULL
	// And sets obj to NULL
	#define X_RELEASE(obj) if(obj){(obj)->ReleaseReference(); obj = NULL;}

	// Calls AddReference() and returns obj if obj is not NULL
	// Or returns NULL if obj is NULL
	#define X_RETURN(obj) if(obj){(obj)->AddReference();} return obj;

	// Set trg to the same xObject as src is pointing to
	#define X_REPLACE(trg, src) if(src){(src)->AddReference();} X_RELEASE(trg); trg = src;
}


