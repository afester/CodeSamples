#include "pch.h"
#include "framework.h"
#include "afxdialogex.h"

#include <vector>
#include "SystemMetrics.h"


//void getDisplayDevices(CArray<CString>& result) {
//	result.Add(CString("Hello"));
//	result.Add(CString("World"));
//}

/**
 * Returns a list of DISPLAY_DEVICE structures, one entry for each device.
 *
 * @param attachedOnly True if only the adapters which are currently attached to the desktop shall be returned, false otherwise.
 */
std::vector<DISPLAY_DEVICE> getDisplayDevices(boolean attachedOnly = false) {
	std::vector<DISPLAY_DEVICE> result;

	DISPLAY_DEVICE displayInfo;
	memset(&displayInfo, 0, sizeof(DISPLAY_DEVICE));
	displayInfo.cb = sizeof(DISPLAY_DEVICE);

	DWORD devNum = 0;
	BOOL done = false;
	do {
		done = EnumDisplayDevices(NULL, devNum, &displayInfo, 0);
		CString str;
		if (done != 0) {
			if (!attachedOnly || (displayInfo.StateFlags & DISPLAY_DEVICE_ATTACHED_TO_DESKTOP)) {
				result.push_back(displayInfo);
			}

			devNum++;
		}
	} while (done != 0);

	return result;
}


/**
 */
std::vector<SCREEN_DATA> getScreenData() {
	std::vector<SCREEN_DATA> result;

	std::vector<DISPLAY_DEVICE> devices = getDisplayDevices(true);
	for (std::vector<DISPLAY_DEVICE>::const_iterator entry = devices.begin(); entry != devices.end(); entry++) {
		// Query device parameters
		DEVMODE myDevMode;
		memset(&myDevMode, 0, sizeof(DEVMODE));
		myDevMode.dmSize = sizeof(DEVMODE);

		EnumDisplaySettings(
			(*entry).DeviceName,
			ENUM_CURRENT_SETTINGS,
			&myDevMode
		);

		SCREEN_DATA data;
		POINT res;
		res.x = myDevMode.dmPelsWidth;
		res.y = myDevMode.dmPelsHeight;
		data.deviceName = (*entry).DeviceName;
		data.resolution = res;
		data.position = myDevMode.dmPosition;

		result.push_back(data);
	}

	return result;
}
