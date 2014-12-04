#ifndef __COLOR_MANIP_H__
#define __COLOR_MANIP_H__

#include <iostream>

void setOutputColor(int col);
void clearScreen();

struct cmanip {
    cmanip(int ii);
    int color;
};

template<typename C, typename Tr>
std::basic_ostream<C,Tr>& operator<<(std::basic_ostream<C,Tr>& os, const cmanip& m) {
    setOutputColor(m.color);
    return os;
}

inline cmanip col(int attr) {
    return cmanip(attr);
}


inline std::ios_base& cls(std::ios_base& __base) {
    clearScreen();
    return __base;
}


#endif // __COLOR_MANIP_H__
