#pragma once
#include "GenericController.h"
#include "BluetoolsController.h"

#ifdef MS_STACK
#include "MicrosoftController.h"
#endif

#include "BSController.h"

#include "SuperController.h"
#include "SettingsControl.h"

#define NO_MANUAL_STACK_SELECTED -1

class ControllerFactory
{
public:
	ControllerFactory(SettingsControl* iSettingsControl);
	~ControllerFactory(void);
	int GetSupportedModes();
	SuperController* GetController(int controllerMode);
private:
	void IsManualStackSettingsNeeded();
	bool CheckMicrosoftSupport();
	bool CheckBlueSoleilSupport();
	bool CheckToshibaSupport();
	//bool CheckWidcommVersion();
	bool CheckRegistryForBlueSoleil();
	int GetOSId();
	bool CheckRegistryForWidcomm();
	GenericController* myGenericController;
	BluetoolsController* myBluetoolsController;
	int SanityCheckSupportedModes(int supportedModes);
#ifdef MS_STACK
	MicrosoftController* myMicrosoftController;
#endif
	BSController* myBSController;
	int supportedModes;
	int lastUsedControllerMode;
	int manuallySelectedStack;
	SettingsControl* mySettingsControl;
	//xManager* m_manager;
	//void m_show_error(xObject* obj);

};
