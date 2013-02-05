/*
 * FileSystem.cpp
 *
 *  Created on: 11.01.2013
 *      Author: afester
 */

#include "FileSystem.h"

#include <stdio.h>

// FileSystem tfs;
FileSystem& tfs() {	// this replaces the tfs object; it could be
 					// static in the FileSystem class
	static FileSystem fs; // define and initialize a local static object
	return fs; 			  // return a reference to it
}


FileSystem::FileSystem() : count(51) {
	printf("FileSystem::FileSystem()\n");
}

std::size_t FileSystem::numDisks() const {
	return count;
}
