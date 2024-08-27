#pragma once

#include <vector>


typedef struct {
	CString deviceName;
	POINT resolution;
	POINTL position;
} SCREEN_DATA;

std::vector<SCREEN_DATA> getScreenData();
