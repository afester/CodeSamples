

#pragma once

class LoggingConsole : public CEdit {
	HBRUSH bgBrush;
	COLORREF bkColor;
	COLORREF fgColor;

	CString getTimeStamp();

protected:
	HBRUSH CtlColor(CDC* pDC, UINT);
	DECLARE_MESSAGE_MAP()

		// Construction
public:
	LoggingConsole(CWnd* pParent = nullptr);

	void setBgColor(const COLORREF& col);

	void setFgColor(const COLORREF& col);

	void log(const CString& msg);

	void logLastError();
};

// A global logging console poínter so that the logging console can be used by any component
extern LoggingConsole* Logger;
