#include <iostream>
#include "Samples.h"


class VSample {
public:
	int x;

	virtual ~VSample() {
		std::cerr << "~Sample" << std::endl;
	}

	virtual void setX() {
		std::cerr << "setX: this=" << std::hex << this << ", x=" << x << std::endl;
	}
};

void vtableSample()  {
	VSample* s = new VSample();
	s->x = 0xabcdef12;

	std::cerr << "s=" << std::hex << s << std::endl;

	std::cerr << "sizeof(*s): " << sizeof(*s) << std::endl;
	unsigned char* data = (unsigned char*) s;
	for (size_t  i = 0;  i < sizeof(*s);  i++) {
		std::cerr << std::hex << (unsigned int) data[i] << " ";
	}
	std::cerr << std::endl;

	// unsigned int *vptr = (unsigned int*) (*((unsigned int*) data));
	unsigned int *vptr = *((unsigned int**) data);
	std::cerr << std::hex << vptr << std::endl;
	data = (unsigned char*) vptr;
	for (size_t  i = 0;  i < 16;  i++) {
		std::cerr << std::hex << (unsigned int) data[i] << " ";
	}
	std::cerr << std::endl;

	// call s->setX() in the usual way
	s->setX();

	// call s->setX() through the vtable - also pass the object as "this"
	( (void (*)(void*)) vptr[2] )(s);	// setX
}


VTableSample::VTableSample() : Sample("VTable sample") {
}

void VTableSample::run() {
    vtableSample();
}
