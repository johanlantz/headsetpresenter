#pragma once

#include "ppControl.h"
#include "UtilFunctions.h"

class PowerPointProxy
{
public:
	PowerPointProxy(void);
	~PowerPointProxy(void);
	void IncomingCommand(CString command);
	int GetVGS(CString ATCommand);
	bool Start();
	bool isStarted();
	void SetForcedVGSMode(bool VGSMode);
private:
	int lastVGS;
	bool started;
	bool forcedVGSEnabled;
	ppControl* myPPControl;
};
