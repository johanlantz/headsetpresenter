#include "StdAfx.h"
#include "..\Header Files\mixercontrol.h"
#include "mmsystem.h"
#include "SS_Log.h"

bool MixerControl::RUN_THREAD = true;
int MixerControl::pollInterval = 1000;
CString MixerControl::nameOfAudioInDevice = "";

MixerControl::MixerControl(void)
{
}

MixerControl::~MixerControl(void)
{
}


DWORD WINAPI ThreadFunc2(LPVOID lpParam)
{
	MixerControl* mc = (MixerControl*)lpParam;
	mc->RunForceThread();
	return 0;
}

void MixerControl::StartForceThread()
{
	DWORD dwThrdParam = 1;
	hThread = CreateThread(NULL,0,ThreadFunc2,this,0,&dwThreadId);

	if(hThread == NULL)
	{
		MessageBox(NULL,"Create thread failed in Cfg","",MB_OK);
	}
}

void MixerControl::RunForceThread()
{
	LogEnterFunction("MC:RunForceThread");
	RUN_THREAD = true;
	int mixerId = GetIdOfDeviceContaining(nameOfAudioInDevice);
	if(mixerId < 0)
	{
		RUN_THREAD = false;
		MessageBox(NULL,"No mixer found, thread not started","",MB_OK);
	}
	else
	{
		SetVolume(mixerId);
		SelectMic(mixerId);
	}
	while(RUN_THREAD)
	{
		UnMuteLineIn(mixerId);
		UnMuteMic(mixerId);
		Sleep(pollInterval);
	}
}

void MixerControl::StopForceThread()
{
	LogEnterFunction("MC:StopThread");
	RUN_THREAD=false;
}

void MixerControl::SetPollInterval(int newPollInterval)
{
	LogEnterFunction("MC:SetPollInterval");
	pollInterval = newPollInterval;
}

void MixerControl::SetNameOfAudioInDevice(CString iNameOfAudioInDevice)
{
	nameOfAudioInDevice = iNameOfAudioInDevice;
}

int MixerControl::GetIdOfDeviceContaining(CString deviceName)
{
	LogEnterFunction("MC:GetIdOfDeviceContaining");
	Log("MC:Will try to find mixer called %s",deviceName);
	int noOfDevices = mixerGetNumDevs();
	int retVal = NO_DEV_FOUND;
	if(noOfDevices == 0)
	{
		retVal = NO_DEVICES_FOUND;
	}
	else  //We have atleat one mixer
	{
		int deviceCounter = 0;
		HMIXER hmx;
		MIXERCAPS mxCaps;
		do
		{
			//1 Open the mixer to be able to get capabilities
			if(mixerOpen(&hmx, deviceCounter, 0, 0, 0) != MMSYSERR_NOERROR)
			{
				MessageBox(NULL,"Failed to open mixer","",MB_OK);
				return MIXER_OPEN_ERROR;
			}
			//2. Mixer open ok, now query for caps
			ZeroMemory(&mxCaps, sizeof(MIXERCAPS));
			if(mixerGetDevCaps(reinterpret_cast<UINT>(hmx), &mxCaps, sizeof(MIXERCAPS)) != MMSYSERR_NOERROR)
			{
				return MIXER_GET_CAPS_ERROR;
			}
			Log("MC:Found mixer:%s",mxCaps.szPname);
			//MessageBox(NULL,mxCaps.szPname,"",MB_OK);
			//if(CString(mxCaps.szPname).Find(partOfDeviceName) != -1)
			if(CString(mxCaps.szPname)== deviceName)
			{
				Log("MC:Mixer is a match");
				retVal = deviceCounter;
			}
			deviceCounter++;
			mixerClose(hmx);
		}while(deviceCounter < noOfDevices);	
	}
	if(retVal == NO_DEV_FOUND)
	{
		Log("MC:No mixer found for %s",deviceName);
		MessageBox(NULL,"HeadsetPresenter failed to find a mixer matching your audioIn device.\nThis is simply a case not implemented because it did not seemed likely to ever happen.\n\nContact us at info@headsetpresenter.com so that we can investigate why this happened and implement a solution asap.","no mixer",MB_OK|MB_TOPMOST|MB_ICONINFORMATION);
	}
	Log("MC:Returning id=%d",retVal);
	return retVal;
}

void MixerControl::SetVolume(int mixerId)
{
	LogEnterFunction("MC:SetVolume");
	// Open the mixer device
   HMIXER hmx;
   mixerOpen(&hmx, mixerId, 0, 0, 0);

   // Get the line info for the wave in destination line
   MIXERLINE mxl;
    mxl.cbStruct = sizeof(mxl);
    mxl.dwComponentType = MIXERLINE_COMPONENTTYPE_DST_WAVEIN;
    mixerGetLineInfo((HMIXEROBJ)hmx, &mxl,
MIXER_GETLINEINFOF_COMPONENTTYPE);

   // Now find the microphone source line connected to this wave in
   // destination
   DWORD cConnections = mxl.cConnections;
   for(DWORD j=0; j<cConnections; j++){
      mxl.dwSource = j;
      mixerGetLineInfo((HMIXEROBJ)hmx, &mxl, MIXER_GETLINEINFOF_SOURCE);
      if (MIXERLINE_COMPONENTTYPE_SRC_MICROPHONE == mxl.dwComponentType)
         break;
   }
   // Find a volume control, if any, of the microphone line
   LPMIXERCONTROL pmxctrl = (LPMIXERCONTROL)malloc(sizeof MIXERCONTROL);
   MIXERLINECONTROLS mxlctrl = {sizeof mxlctrl, mxl.dwLineID,
      MIXERCONTROL_CONTROLTYPE_VOLUME, 1, sizeof MIXERCONTROL, pmxctrl};
   if(!mixerGetLineControls((HMIXEROBJ) hmx, &mxlctrl,
MIXER_GETLINECONTROLSF_ONEBYTYPE)){
      // Found!
      DWORD cChannels = mxl.cChannels;
      if (MIXERCONTROL_CONTROLF_UNIFORM & pmxctrl->fdwControl)
         cChannels = 1;

      LPMIXERCONTROLDETAILS_UNSIGNED pUnsigned =
(LPMIXERCONTROLDETAILS_UNSIGNED)
         malloc(cChannels * sizeof MIXERCONTROLDETAILS_UNSIGNED);
      MIXERCONTROLDETAILS mxcd = {sizeof(mxcd), pmxctrl->dwControlID,
cChannels, (HWND)0,
         sizeof MIXERCONTROLDETAILS_UNSIGNED, (LPVOID) pUnsigned};
      mixerGetControlDetails((HMIXEROBJ)hmx, &mxcd,
MIXER_SETCONTROLDETAILSF_VALUE);
      // Set the volume to the middle  (for both channels as needed)
      pUnsigned[0].dwValue = pUnsigned[cChannels - 1].dwValue =
         (pmxctrl->Bounds.dwMinimum+pmxctrl->Bounds.dwMaximum)/2;
      mixerSetControlDetails((HMIXEROBJ)hmx, &mxcd,
MIXER_SETCONTROLDETAILSF_VALUE);

    free(pmxctrl);
    free(pUnsigned);
   }
  else
    free(pmxctrl);
   mixerClose(hmx);
}

void MixerControl::UnMuteMic(int mixerId)
{
	//LogEnterFunction("MC:UnMuteMic");
	//MessageBox("UnMuteMic");
   // Open the mixer device
   HMIXER hmx;
   mixerOpen(&hmx, mixerId, 0, 0, 0);

   // Get the line info for the wave in destination line
   MIXERLINE mxl;
  mxl.cbStruct = sizeof(mxl);
  mxl.dwComponentType = MIXERLINE_COMPONENTTYPE_DST_WAVEIN;
  mixerGetLineInfo((HMIXEROBJ)hmx, &mxl, MIXER_GETLINEINFOF_COMPONENTTYPE);

   // Now find the microphone source line connected to this wave in
   // destination
   DWORD cConnections = mxl.cConnections;
   for(DWORD j=0; j<cConnections; j++){
      mxl.dwSource = j;
      mixerGetLineInfo((HMIXEROBJ)hmx, &mxl, MIXER_GETLINEINFOF_SOURCE);
      if (MIXERLINE_COMPONENTTYPE_SRC_MICROPHONE == mxl.dwComponentType)
         break;
   }
   // Find a mute control, if any, of the microphone line
   LPMIXERCONTROL pmxctrl = (LPMIXERCONTROL)malloc(sizeof MIXERCONTROL);
   MIXERLINECONTROLS mxlctrl = {sizeof mxlctrl, mxl.dwLineID,
      MIXERCONTROL_CONTROLTYPE_MUTE, 1, sizeof MIXERCONTROL, pmxctrl};
   if(!mixerGetLineControls((HMIXEROBJ) hmx, &mxlctrl,
MIXER_GETLINECONTROLSF_ONEBYTYPE)){
      // Found, so proceed
      DWORD cChannels = mxl.cChannels;
      if (MIXERCONTROL_CONTROLF_UNIFORM & pmxctrl->fdwControl)
         cChannels = 1;

      LPMIXERCONTROLDETAILS_BOOLEAN pbool =
         (LPMIXERCONTROLDETAILS_BOOLEAN) malloc(cChannels * sizeof
MIXERCONTROLDETAILS_BOOLEAN);
      MIXERCONTROLDETAILS mxcd = {sizeof(mxcd), pmxctrl->dwControlID,
cChannels, (HWND)0,
         sizeof MIXERCONTROLDETAILS_BOOLEAN, (LPVOID) pbool};
      mixerGetControlDetails((HMIXEROBJ)hmx, &mxcd,
MIXER_SETCONTROLDETAILSF_VALUE);
      // Unmute the microphone line (for both channels)
      pbool[0].fValue = pbool[cChannels - 1].fValue = 0;
      mixerSetControlDetails((HMIXEROBJ)hmx, &mxcd,
MIXER_SETCONTROLDETAILSF_VALUE);

    free(pmxctrl);
    free(pbool);
   }
  else
    free(pmxctrl);

   mixerClose(hmx);
}

void MixerControl::UnMuteLineIn(int mixerId)
{
	//LogEnterFunction("MC:UnMuteLineIn");
		//MessageBox("UnMuteLineIn");
   // Open the mixer device
   HMIXER hmx;
   mixerOpen(&hmx, mixerId, 0, 0, 0);

   // Get the line info for the wave in destination line
   MIXERLINE mxl;
  mxl.cbStruct = sizeof(mxl);
  mxl.dwComponentType = MIXERLINE_COMPONENTTYPE_DST_WAVEIN;
  mixerGetLineInfo((HMIXEROBJ)hmx, &mxl, MIXER_GETLINEINFOF_COMPONENTTYPE);

   // Now find the microphone source line connected to this wave in
   // destination
   DWORD cConnections = mxl.cConnections;
   for(DWORD j=0; j<cConnections; j++){
      mxl.dwSource = j;
      mixerGetLineInfo((HMIXEROBJ)hmx, &mxl, MIXER_GETLINEINFOF_SOURCE);
      if (MIXERLINE_COMPONENTTYPE_SRC_LINE == mxl.dwComponentType)
         break;
   }
   // Find a mute control, if any, of the microphone line
   LPMIXERCONTROL pmxctrl = (LPMIXERCONTROL)malloc(sizeof MIXERCONTROL);
   MIXERLINECONTROLS mxlctrl = {sizeof mxlctrl, mxl.dwLineID,
      MIXERCONTROL_CONTROLTYPE_MUTE, 1, sizeof MIXERCONTROL, pmxctrl};
   if(!mixerGetLineControls((HMIXEROBJ) hmx, &mxlctrl,
MIXER_GETLINECONTROLSF_ONEBYTYPE)){
      // Found, so proceed
      DWORD cChannels = mxl.cChannels;
      if (MIXERCONTROL_CONTROLF_UNIFORM & pmxctrl->fdwControl)
         cChannels = 1;

      LPMIXERCONTROLDETAILS_BOOLEAN pbool =
         (LPMIXERCONTROLDETAILS_BOOLEAN) malloc(cChannels * sizeof
MIXERCONTROLDETAILS_BOOLEAN);
      MIXERCONTROLDETAILS mxcd = {sizeof(mxcd), pmxctrl->dwControlID,
cChannels, (HWND)0,
         sizeof MIXERCONTROLDETAILS_BOOLEAN, (LPVOID) pbool};
      mixerGetControlDetails((HMIXEROBJ)hmx, &mxcd,
MIXER_SETCONTROLDETAILSF_VALUE);
      // Unmute the microphone line (for both channels)
      pbool[0].fValue = pbool[cChannels - 1].fValue = 0;
      mixerSetControlDetails((HMIXEROBJ)hmx, &mxcd,
MIXER_SETCONTROLDETAILSF_VALUE);

    free(pmxctrl);
    free(pbool);
   }
  else
    free(pmxctrl);

   mixerClose(hmx);
}

void MixerControl::SelectMic(int mixerId)
{
	LogEnterFunction("MC:SelectMic");
	// Open the mixer device
   HMIXER hmx;
   mixerOpen(&hmx, mixerId, 0, 0, 0);

   // Get the line info for the wave in destination line
   MIXERLINE mxl;
    mxl.cbStruct      = sizeof(mxl);
    mxl.dwComponentType = MIXERLINE_COMPONENTTYPE_DST_WAVEIN;
    mixerGetLineInfo((HMIXEROBJ)hmx, &mxl,
MIXER_GETLINEINFOF_COMPONENTTYPE);

   // Find a LIST control, if any, for the wave in line
   LPMIXERCONTROL pmxctrl = (LPMIXERCONTROL)malloc(mxl.cControls * sizeof
MIXERCONTROL);
   MIXERLINECONTROLS mxlctrl = {sizeof mxlctrl, mxl.dwLineID, 0,
      mxl.cControls, sizeof MIXERCONTROL, pmxctrl};
   mixerGetLineControls((HMIXEROBJ) hmx, &mxlctrl,
MIXER_GETLINECONTROLSF_ALL);

   // Now walk through each control to find a type of LIST control. This
   // can be either Mux, Single-select, Mixer or Multiple-select.
   DWORD i;
   for(i=0; i < mxl.cControls; i++)
      if (MIXERCONTROL_CT_CLASS_LIST == (pmxctrl[i].dwControlType
&MIXERCONTROL_CT_CLASS_MASK))
         break;
   if (i < mxl.cControls) { // Found a LIST control
      // Check if the LIST control is a Mux or Single-select type
      BOOL bOneItemOnly = FALSE;
      switch (pmxctrl[i].dwControlType) {
         case MIXERCONTROL_CONTROLTYPE_MUX:
         case MIXERCONTROL_CONTROLTYPE_SINGLESELECT:
            bOneItemOnly = TRUE;
      }

      DWORD cChannels = mxl.cChannels, cMultipleItems = 0;
      if (MIXERCONTROL_CONTROLF_UNIFORM & pmxctrl[i].fdwControl)
         cChannels = 1;
      if (MIXERCONTROL_CONTROLF_MULTIPLE & pmxctrl[i].fdwControl)
         cMultipleItems = pmxctrl[i].cMultipleItems;

      // Get the text description of each item
      LPMIXERCONTROLDETAILS_LISTTEXT plisttext =
(LPMIXERCONTROLDETAILS_LISTTEXT)
         malloc(cChannels * cMultipleItems * sizeof
MIXERCONTROLDETAILS_LISTTEXT);
      MIXERCONTROLDETAILS mxcd = {sizeof(mxcd), pmxctrl[i].dwControlID,
cChannels,
         (HWND)cMultipleItems, sizeof MIXERCONTROLDETAILS_LISTTEXT,
(LPVOID) plisttext};
      mixerGetControlDetails((HMIXEROBJ)hmx, &mxcd,
MIXER_GETCONTROLDETAILSF_LISTTEXT);

      // Now get the value for each item
      LPMIXERCONTROLDETAILS_BOOLEAN plistbool =
(LPMIXERCONTROLDETAILS_BOOLEAN)
         malloc(cChannels * cMultipleItems * sizeof
MIXERCONTROLDETAILS_BOOLEAN);
      mxcd.cbDetails = sizeof MIXERCONTROLDETAILS_BOOLEAN;
      mxcd.paDetails = plistbool;
      mixerGetControlDetails((HMIXEROBJ)hmx, &mxcd,
MIXER_GETCONTROLDETAILSF_VALUE);

      // Select the "Microphone" item
      for (DWORD j=0; j<cMultipleItems; j = j + cChannels)
         if (0 == strcmp(plisttext[j].szName, "Microphone"))
            // Select it for both left and right channels
            plistbool[j].fValue = plistbool[j+ cChannels - 1].fValue = 1;
         else if (bOneItemOnly)
            // Mux or Single-select allows only one item to be selected
            // so clear other items as necessary
            plistbool[j].fValue = plistbool[j+ cChannels - 1].fValue = 0;
      // Now actually set the new values in
      mixerSetControlDetails((HMIXEROBJ)hmx, &mxcd,
MIXER_GETCONTROLDETAILSF_VALUE);

    free(pmxctrl);
    free(plisttext);
    free(plistbool);
   }
  else
    free(pmxctrl);
   mixerClose(hmx);
}