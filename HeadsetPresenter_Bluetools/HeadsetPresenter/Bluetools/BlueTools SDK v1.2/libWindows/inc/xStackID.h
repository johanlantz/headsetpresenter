
// Franson C++ BlueTools
// (c) 2006 Franson Technology AB, Sweden, All rights reserved
// http://franson.com

#pragma once

namespace xBlueTools
{
	typedef enum 
	{
		STACK_UNDETERMINED = -1,	// Internal value, no search for stack has been made
		STACK_UNKNOWN = 0,			// Stack is unknown or not present
		STACK_WIDCOMM = 1,			// WidComm/Broadcom stack
		STACK_MICROSOFT = 2,		// Microsoft stack
		STACK_MOCKUP = 3			// Mockup stack, used for testing
	} xStackID;
}
