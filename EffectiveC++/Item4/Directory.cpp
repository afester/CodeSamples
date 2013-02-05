/*
 * Directory.cpp
 *
 *  Created on: 11.01.2013
 *      Author: afester
 */

#include "Directory.h"
#include "FileSystem.h"

#include <stdio.h>

Directory::Directory(const std::string& root) {
	printf("Directory::Directory()\n");

	//std::size_t disks = tfs.numDisks(); // use the tfs object
	//									// POSSIBLY NOT INITIALIZED YET!!!

	std::size_t disks = tfs().numDisks(); // use the tfs object

	printf("NumDisks: %d\n", disks);
}
