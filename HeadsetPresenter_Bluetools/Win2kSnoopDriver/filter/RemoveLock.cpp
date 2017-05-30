#ifdef WIN98

// RemoveLock.cpp -- Temporary implementation for remove-lock functions
// Generated by Walt Oney's driver wizard

#include "stddcls.h"

///////////////////////////////////////////////////////////////////////////////
#pragma PAGEDCODE

VOID IoInitializeRemoveLock(PIO_REMOVE_LOCK lock, ULONG tag, ULONG minutes, ULONG maxcount)
{
	PAGED_CODE();
	KeInitializeEvent(&lock->evRemove, NotificationEvent, FALSE);
	lock->usage = 1;
	lock->removing = FALSE;
}

///////////////////////////////////////////////////////////////////////////////

#pragma LOCKEDCODE

NTSTATUS IoAcquireRemoveLock(PIO_REMOVE_LOCK lock, PVOID tag)
{
	LONG usage = InterlockedIncrement(&lock->usage);
	if (lock->removing)
	{						// removal in progress
		if (InterlockedDecrement(&lock->usage) == 0)
			KeSetEvent(&lock->evRemove, 0, FALSE);
		return STATUS_DELETE_PENDING;
	}						// removal in progress
	return STATUS_SUCCESS;
}

///////////////////////////////////////////////////////////////////////////////

VOID IoReleaseRemoveLock(PIO_REMOVE_LOCK lock, PVOID tag)
{
	if (InterlockedDecrement(&lock->usage) == 0)
		KeSetEvent(&lock->evRemove, 0, FALSE);
}

///////////////////////////////////////////////////////////////////////////////

#pragma PAGEDCODE

VOID IoReleaseRemoveLockAndWait(PIO_REMOVE_LOCK lock, PVOID tag)
{
	PAGED_CODE();
	lock->removing = TRUE;
	IoReleaseRemoveLock(lock, tag);
	IoReleaseRemoveLock(lock, NULL);
	KeWaitForSingleObject(&lock->evRemove, Executive, KernelMode, FALSE, NULL);
}
#endif