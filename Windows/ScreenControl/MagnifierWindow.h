
#pragma once

BOOL SetupMagnifier(HINSTANCE hinst, HWND hwnd); //  , LoggingConsole& console);

void StopMagnifier();

void SetMagFactor(float factor);

void SetMagWindowCenter(const POINT& center);
