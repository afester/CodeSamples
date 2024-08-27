
#include "pch.h"
#include "framework.h"
#include "afxdialogex.h"

#include "LoggingConsole.h"
#include <string>

BEGIN_MESSAGE_MAP(LoggingConsole, CEdit)
	ON_WM_CTLCOLOR_REFLECT()
END_MESSAGE_MAP()

LoggingConsole* Logger;

HBRUSH LoggingConsole::CtlColor(CDC* pDC, UINT) {
	pDC->SetBkColor(bkColor);
	pDC->SetTextColor(fgColor);

	if (!bgBrush) {
		bgBrush = CreateSolidBrush(bkColor);
	}
	return bgBrush;
}

LoggingConsole::LoggingConsole(CWnd* pParent) : bgBrush(0) {
	bkColor = RGB(0, 0, 0);
	fgColor = RGB(255, 255, 255);
	::Logger = this;
}


void LoggingConsole::setBgColor(const COLORREF& col) {
	bkColor = col;
}

void LoggingConsole::setFgColor(const COLORREF& col) {
	fgColor = col;
}

CString LoggingConsole::getTimeStamp() {
	SYSTEMTIME lt;
	GetLocalTime(&lt);

	CString result;
	result.Format(L"%02d:%02d:%02d.%03d\n", lt.wHour, lt.wMinute, lt.wSecond, lt.wMilliseconds);
	return result;
}

void LoggingConsole::log(const CString& msg) {
	SetLimitText(0);
	// get the initial text length
	int nLength = GetWindowTextLength();
	// put the selection at the end of text
	SetSel(nLength, nLength);
	// replace the selection
	CString strLine;
	// add CR/LF to text
	strLine.Format(_T("[%s] %s\r\n"), getTimeStamp(), msg);

	ReplaceSel(strLine);
}


void LoggingConsole::logLastError() {
	DWORD dwError = GetLastError();
	LPTSTR lpBuffer = NULL;

	CString msg;
	if (FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS, NULL,
		dwError, 0, (LPTSTR)&lpBuffer, 0, NULL)) {
		msg = CString(lpBuffer);
		msg.Trim();
	}
	else {
		msg = CString(L"Unknown Error");
	}
	LocalFree(lpBuffer);

	CString logMsg;
	logMsg.Format(L"%s (%d)", msg, dwError);
	log(logMsg);
}
