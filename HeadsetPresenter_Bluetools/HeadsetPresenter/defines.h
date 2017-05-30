//////////////////////////////////////////////////////////////////////
//Headset Presenter by w2solutions (Johan Rosenquist)
//(C) All rights reserved 2002-2007
//////////////////////////////////////////////////////////////////////
enum FontTypes
{
	HEADLINE,
	NORMAL_TEXT,
	SMALL_TEXT,
	SMALL_HEADLINE,
	BOLD_STANDARD_FONT,
	STANDARD_FONT
};

enum VolSupportInNormalMode
{
	NO_VOL_SUPPORT		 = 0x00,
	PARTIAL_VOL_SUPPORT  = 0x01, //15 is not repeated
	FULL_VOL_SUPPORT	 = 0x02
};

enum AudioFeedbackMode
{
	AUDIO_FEEDBACK_LAST_WORD_ONLY = 0x01,
	AUDIO_FEEDBACK_WHOLE_PHRASE = 0x02
};

enum CommandTypes
{
	ERROR_COMMAND,
	AT_COMMAND,
	AVRCP_COMMAND,
	SAPI_COMMAND
};

#define NO_IMAGE_TO_DRAW -1

#define DONT_USE_KEEPALIVE_TIMER 0
enum StackNames  //could be problem with atoi in SettingsControl if this becomes two digits
{
	NO_STACK,
	WIDCOMM,
	MICROSOFT,
	BLUESOLEIL,
	TOSHIBA,
	AVM,
	GENERIC
};


enum Modes
{
	ERROR_MODE = 0x00,
	NORMAL_MODE_WIDCOMM = 0x01,
	NORMAL_MODE_MICROSOFT = 0x02,  //If we use Bluetools for the MS stack then this is not needed
	NORMAL_MODE_BLUESOLEIL = 0x04,  //TBI
	NORMAL_MODE_TOSHIBA = 0x08,
	ADVANCED_MODE = 0x10  //Perhaps there will be a speech only mode? double check settingscontrol if this enum is larger then 1 byte
};  //NEVER FORGET THE ; It will destroy HPDialog

//perhaps use this define in ControllerFactory, not used yet
#define ANY_NORMAL_MODE_SUPPORTED NORMAL_MODE_WIDCOMM | NORMAL_MODE_MICROSOFT | NORMAL_MODE_BLUESOLEIL

enum RestartDeviceHandling
{
	RESTART_DEVICE_ALWAYS = 0x01,	//old mode, restart every time
	RESTART_DEVICE_NEVER = 0x02		//Computer must be restarted for log file to be created
};

enum OS
{
	WINDOWS_95 = 0x01,
	WINDOWS_98 = 0x02,
	WINDOWS_MILLENIUM = 0x04,
	WINDOWS_NT = 0x08,
	WINDOWS_2000 = 0x10,
	WINDOWS_SERVER_2003 = 0x20,
	WINDOWS_XP = 0x40,
	WINDOWS_XP_SP2 = 0x80,
	WINDOWS_VISTA = 0x100
};

#define SUPPORTED_STACKS = 0 | WIDCOMM | MICROSOFT

//Generic solution is only available for 2000 and XP, investigate Vista 
#define OS_THAT_REQUIRE_WIDCOMM         0 | WINDOWS_95 | WINDOWS_98 | WINDOWS_MILLENIUM | WINDOWS_NT

enum SpeechSettings
{
	SPEECH_CONTROL_ACTIVE = 0x01,
	VOICE_FEEDBACK_ACTIVE = 0x02,
	REQUIRE_HIGH_CONFIDENCE = 0x04,
	TEST_MODE_ACTIVE = 0x08,
	LOG_DISABLED = 0x10
};

enum Help_Ids
{
	HELP_ID_DEMO_REQ	= 1,
	HELP_ID_CONNECT_ERROR,				
	HELP_ID_DEVICE_SELECTION_CANCELED,	
	HELP_ID_NO_LICENSE_FOR_HS,			
	HELP_ID_WIDCOMM						
};//NEVER FORGET THE ; It will destroy HPDialog

