#include "Samples.h"


class A {
public:
  enum class AA
  {
    enumVal1,
    enumVal2
  };

  enum AB
  {
    enumVal1,
    enumVal3,
  };
};


EnumSample::EnumSample() : Sample("Enum sample") {
}

void EnumSample::run() {
	A::AB x = A::enumVal1;
}

