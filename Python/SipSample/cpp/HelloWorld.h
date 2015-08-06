/*
 * HelloWorld.h
 *
 *  Created on: 11.03.2015
 *      Author: afester
 */

#ifndef HELLOWORLD_H_
#define HELLOWORLD_H_

class HelloWorld {
public:
	HelloWorld();
	virtual ~HelloWorld();

	const char* getGreeting() const;
};

#endif /* HELLOWORLD_H_ */
