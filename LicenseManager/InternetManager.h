#pragma once


// InternetManager

class InternetManager : public CWnd
{
	DECLARE_DYNAMIC(InternetManager)

public:
	InternetManager();
	virtual ~InternetManager();
	CString SendRequestToServer(CString queryString);
private:
	CString httpAddress;
protected:
	DECLARE_MESSAGE_MAP()
};


