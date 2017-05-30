#ifndef PUSHDLL_H
#define PUSHDLL_H
//Pushkeys DLL
//Alan Warriner
//alan.warriner@bigfoot.com
//http://www.alan-warriner.co.uk

//control key states
typedef int PKKeyState;
enum PKKeyStates {PKksShift=1,PKksCtrl=2,PKksAlt=4};

//keyevent callback function type
typedef void CALLBACK PKKeyEvent (char*,PKKeyState *ss);

//accuracy of window match
enum WindowMatch {wmPKExactMatch,wmPKStartMatch,wmPKPartialMatch};

//extended pushkeys info
struct PushInfo
        {
        char* Keys;
        char* WindowTitle;
        WindowMatch WindowMatch;
        BOOL ReturnFocus;
        int KeyDelay;
        BOOL UseANSI;
        BOOL TrackTarget;
        PKKeyEvent *KeyEvent;
        };

extern "C" {

HWND __stdcall PushKeys (char* Keys);
HWND __stdcall  PushKeysEx (PushInfo* Info);
BOOL __stdcall  IsPKPushing ();
int __stdcall  GimmeTheKey(char*);

}

#endif