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


void enumSample() {
	A::AB x = A::enumVal1;
}
