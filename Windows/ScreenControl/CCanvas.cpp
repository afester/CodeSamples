#include "pch.h"
#include "framework.h"
#include "afxdialogex.h"

#include "LoggingConsole.h"
#include "SystemMetrics.h"
#include "CCanvas.h"
#include <vector>


BEGIN_MESSAGE_MAP(CCanvas, CWnd)
	ON_WM_PAINT()
	ON_WM_LBUTTONDOWN()
END_MESSAGE_MAP()

CCanvas::CCanvas() : CWnd() { // CWnd* pParent) {
}

//TODO: Move to SystemMetrics.cpp
CSize getDesktopSize() {
	return CSize(GetSystemMetrics(SM_CXVIRTUALSCREEN),
		GetSystemMetrics(SM_CYVIRTUALSCREEN));
}

void CCanvas::setScreenData(const std::vector<SCREEN_DATA>& scr) {
	// scr = screenData;

	CRect clientRect;
	this->GetClientRect(&clientRect);

	CString str;
	str.Format(L"Win Size: %d,%d,%d,%d", clientRect.left, clientRect.top, clientRect.right, clientRect.bottom);
	Logger->log(str);

	CSize dSize = getDesktopSize();
	str.Format(L"Desktop size: %d x %d", dSize.cx, dSize.cy);
	Logger->log(str);

	const float factor = max((float) dSize.cx / clientRect.right, (float) dSize.cy / clientRect.bottom);
	str.Format(L"factor: %f", factor);
	Logger->log(str);

	// desktop area
	dRect = CRect(0, 0, dSize.cx / factor, dSize.cy / factor);

	// calculate offset of leftmost and topmost window
	LONG offsetX = 0;
	LONG offsetY = 0;
	for (std::vector<SCREEN_DATA>::const_iterator entry = scr.begin(); entry != scr.end(); entry++) {
		offsetX = min((*entry).position.x, offsetX);
		offsetY = min((*entry).position.y, offsetY);
	}

	// monitors
	for (std::vector<SCREEN_DATA>::const_iterator entry = scr.begin(); entry != scr.end(); entry++) {
		LONG x1 = -offsetX + (*entry).position.x;
		LONG y1 = -offsetY + (*entry).position.y;
		LONG x2 = -offsetX + (*entry).position.x + (*entry).resolution.x - 1;
		LONG y2 = -offsetY + (*entry).position.y + (*entry).resolution.y - 1;

		x1 /= factor;
		y1 /= factor;
		x2 /= factor;
		y2 /= factor;

		str.Format(L"   %d/%d - %d/%d", x1, y1, x2, y2);
		Logger->log(str);

		x1 += 3;
		y1 += 3;
		x2 -= 6;
		y2 -= 6;

		MONITOR_RECT mRect;
		mRect.rect = CRect(x1, y1, x2, y2);
		mRect.name = (*entry).deviceName;
		mRects.push_back(mRect);
	}
}

afx_msg void CCanvas::OnPaint() {
	Logger->log(L"YES!");
	CPaintDC dc(this);

	// available drawing rect (for debugging purposes)
	//CRect clientRect;
	//this->GetClientRect(&clientRect);
	// CBrush brush(RGB(0, 255, 0));
	//dc.SelectObject(&brush);
	//dc.Rectangle(clientRect.left, clientRect.top, clientRect.right-1, clientRect.bottom-1);

	// desktop area
	CBrush brush3(RGB(255, 255, 255));
	dc.SelectObject(&brush3);
	dc.Rectangle(&dRect);

	//// monitors
	CBrush brush2(RGB(200, 200, 200));
	dc.SelectObject(&brush2);
	for (std::vector<MONITOR_RECT>::const_iterator entry = mRects.begin(); entry != mRects.end(); entry++) {
		dc.Rectangle((*entry).rect);
	}
}


afx_msg void CCanvas::OnLButtonDown(UINT a, CPoint pos) {
	for (std::vector<MONITOR_RECT>::const_iterator entry = mRects.begin(); entry != mRects.end(); entry++) {
		if ((*entry).rect.PtInRect(pos)) {
			Logger->log(L"CLICK!" + (*entry).name);
		}
	}
}
