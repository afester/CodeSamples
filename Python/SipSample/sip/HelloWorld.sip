%Module helloworld

class HelloWorld {

%TypeHeaderCode
#include "HelloWorld.h"
%End

public:
    HelloWorld();

    const char* getGreeting() const;
};
