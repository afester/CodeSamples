/**
 * LogicalConst.cpp
 */

#include <cstring>

class CTextBlock {
public:
	CTextBlock(const char* source) : pText(source), lengthIsValid(false) {
	}

	// const method can modify textLength and lengthIsValid since they are
	// declared "mutable"
	std::size_t length() const {
		if (!lengthIsValid) {
			textLength = std::strlen(pText);
			lengthIsValid = true;
		}
		return textLength;
	}

private:
	const char *pText;

	mutable std::size_t textLength; // last calculated length of textblock
	mutable bool lengthIsValid; 	// whether length is currently valid
};


int main() {
	CTextBlock a("Hello");
	a.length();

	return 0;
}
