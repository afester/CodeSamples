/**
 * ConstCast.cpp
 */

#include <cstddef>
#include <stdio.h>

class CTextBlock {
public:
	CTextBlock(const char* source) : pText(source) {

	}

	// Implementation of the [] operator
	const char& operator[](std::size_t position) const	{
		fprintf(stderr, "[] const\n");

		// ... // do bounds checking
		// ... // log access data
		// ... // verify data integrity
		return pText[position];
	}

	// Non-const implementation of the [] operator delegates to the const [] operator
	char& operator[](std::size_t position) {
		fprintf(stderr, "[]\n");

		// ... // do bounds checking
		// ... // log access data
		// ... // verify data integrity
		// return pText[position];
		// return (*this)[position];	// recursively calls this operator!
		// static_cast<const CTextBlock&>(*this)[position]; // call operator[] const
		// return static_cast<const CTextBlock&>(*this)[position]; // call operator[] const, but can not
			  													   // return a const char& as a char&

		// call the const char& operator[] const and cast away the char& constness
		return const_cast<char&>(static_cast<const CTextBlock&>(*this)[position]);
	}

#if 0
	// What is the disadvantage of this solution? it requires one cast only!
	// "Even more  worth knowing is that  trying  to  do  things the  other  way
	//  around  —  avoiding  duplication  by  having  the  const  version  call  the
	//  non-const version — is not something you want to do"
	const char& operator[](std::size_t position) const	{
		fprintf(stderr, "[] const\n");

		// ... // do bounds checking
		// ... // log access data
		// ... // verify data integrity
		// return (*this)[position];	// recursively calls this operator
		return const_cast<CTextBlock&>(*this)[position];
	}

	char& operator[](std::size_t position) {
		fprintf(stderr, "[]\n");
		// ... // do bounds checking
		// ... // log access data
		// ... // verify data integrity
		return pText[position];
	}
#endif

private:
	const char *pText;
};


int main(){
	CTextBlock a("Hello");
	const CTextBlock& b = a;

	char& c1 = a[1];
	const char& c2 = b[1];

	return 0;
}
