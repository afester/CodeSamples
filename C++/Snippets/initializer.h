/*
 * initializer.h
 *
 *  Created on: 10.06.2013
 *      Author: afester
 */

#ifndef INITIALIZER_H_
#define INITIALIZER_H_

class TestInitialize {
	int value;

public:
	TestInitialize();

	TestInitialize(int val);

	int getValue();
};

void initializerMain();

#endif /* INITIALIZER_H_ */
