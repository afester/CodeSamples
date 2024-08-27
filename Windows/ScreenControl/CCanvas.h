
#pragma once

#include "SystemMetrics.h"
#include <vector>

typedef struct {
	CRect rect;
	CString name;
}MONITOR_RECT;


class CCanvas: public CWnd {
	std::vector<MONITOR_RECT> mRects;	// the rectangles for the monitor visualizations
	CRect dRect;				// the rectangle for the desktop visualization

protected:
	DECLARE_MESSAGE_MAP()

	void OnPaint();

	void OnLButtonDown(UINT a, CPoint pos);

public:
	CCanvas();

	void setScreenData(const std::vector<SCREEN_DATA>& screenData);
};
