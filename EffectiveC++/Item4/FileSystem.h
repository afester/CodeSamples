/*
 * FileSystem.h
 *
 *  Created on: 11.01.2013
 *      Author: afester
 */

#ifndef FILESYSTEM_H_
#define FILESYSTEM_H_

#include <cstddef>

class FileSystem { // from your library’s header file
public:
	FileSystem();

	// ...
	std::size_t numDisks() const; // one of many member functions
	// ...

private:
	int count;
};

// declare object for clients to use
// (“tfs” = “the file system” ); definition
// is in some .cpp file in your library

// extern FileSystem tfs;
FileSystem& tfs();

#endif /* FILESYSTEM_H_ */
