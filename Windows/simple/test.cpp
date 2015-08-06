#include <windows.h>

int main (int argc, char ** argv) {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    DWORD written = 0;
    WriteConsole(hConsole, "Hello World", 11, &written, NULL);

    return 0;
}
