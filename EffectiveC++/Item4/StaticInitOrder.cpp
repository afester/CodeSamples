/*
 * StaticInitOrder.cpp
 *
 *  Created on: 11.01.2013
 *      Author: afester
 */

#include "Directory.h"

// Directory tempDir("/tmp");
Directory& tempDir() {
	static Directory tempDir("/tmp");
	return tempDir;
}

int main() {
	/*Directory& t = */ tempDir();
}
