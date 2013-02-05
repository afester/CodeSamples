/**
 * Const.cpp
 *
 *  Created on: 28.11.2012
 *      Author: afester
 */


class Const {
public:
	Const();
	virtual ~Const();
};


Const::Const() {

	char greeting[] = "Hello";

	/* Some people find it helpful to read pointer declarations right to left,
	 * e.g., to read "const char* const p;" as “p is a constant pointer to constant chars.”
	 */
	char* pd = greeting; // non-const pointer,
			  			 // non-const data
//-----------------
	const char* pdc = greeting; // non-const pointer,
							    // const data
	// identical to
	char const * pdc1 = greeting; // non-const pointer,
							     // const data
//-----------------
	char* const pcd = greeting; // const pointer,
							    // non-const data
//-----------------
//  data   Data type   ptr
//  const  (char ptr)  const
//	vvvvv  vvvvvvvv    vvvvv
	const  char *      const pcdc = greeting; // const pointer,
						                      // const data
	// identical to
	char const *      const pcdc1 = greeting; // const pointer,
					                          // const data
}


Const::~Const() {
	// TODO Auto-generated destructor stub
}


int main() {
	return 0;
}
