#pragma once

#define NO_DEVICES_FOUND -1
#define NO_MATCHING_DEVICE_FOÖUND -2
#define MIXER_OPEN_ERROR -3
#define MIXER_GET_CAPS_ERROR -4
#define NO_DEV_FOUND -5

class MixerControl
{
public:
	MixerControl(void);
	~MixerControl(void);
	void StartForceThread();
	void RunForceThread();
	void StopForceThread();
	void SetPollInterval(int newPollInterval);
	void SetNameOfAudioInDevice(CString iNameOfAudioInDevice);
	int GetIdOfDeviceContaining(CString deviceName);
	void SetVolume(int mixerId);
	void UnMuteMic(int mixerId);
	void UnMuteLineIn(int mixerId);
	void SelectMic(int mixerId);

	HANDLE hThread;
	DWORD dwThreadId;
	static bool RUN_THREAD;
	static int pollInterval;
	static CString nameOfAudioInDevice;
};
