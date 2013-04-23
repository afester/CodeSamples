#include <iostream>
#include <fstream>
#include <vector>
#include <iterator>

#include "SortAlgorithm.h"

#include "Braindeadsort.h"
#include "Simplesort.h"
#include "Quicksort.h"


using namespace std;


/**
 * Dumps a vector of strings to an output stream.
 */
ostream& operator<<(ostream& stream, const vector<string>& data) {
    stream << "[";

    vector<string>::const_iterator iter = data.begin();
    if (iter != data.end()) {
        stream << '"' << *iter << '"';
	iter++;
    }

    while(iter != data.end()) {
        stream << ", \"" << *iter << '"';
	iter++;
    }

    stream << "]";
    return stream;
}


void saveList(const char* fileName, const vector<string>& data) {
    ofstream ofs(fileName);
    ofs << data.size();
    ofs << "\n";	// need some sort of separator!
    copy(data.begin(), data.end(),
	 ostream_iterator<string>(ofs, "\n"));
    ofs.close();
}


SortAlgorithm::SortAlgorithm(const string& name) : algorithmName(name) {
    unsortedArray = loadList("array.dat");
}


SortAlgorithm::~SortAlgorithm() {
}


vector<string> SortAlgorithm::loadList(const char* fileName) {
    vector<string> result;
    ifstream ifs(fileName);
    size_t size;
    ifs >> size;
    result.resize(size);
    for (size_t i=0; i<size; i++) {
       ifs >> result[i];
    }
    ifs.close();
    return result;
}


void SortAlgorithm::generateData() {
    vector<string> someList;
    someList.push_back("Hello");
    someList.push_back("World");
    someList.push_back("Moon");
    someList.push_back("Earth");
    someList.push_back("Venus");
    someList.push_back("Merkur");
    someList.push_back("Mars");
    someList.push_back("Saturn");
    someList.push_back("Neptun");
    someList.push_back("Uranus");
    someList.push_back("Pluto");
    someList.push_back("Asteroid");
    someList.push_back("Sun");

    saveList("array.dat", someList);
}


void SortAlgorithm::run() {
    cerr << algorithmName << ":" << std::endl;
    cerr << "  BEFORE: " << unsortedArray << std::endl;
    doSort();
    cerr << "  AFTER : " << unsortedArray << std::endl;
}


int main() {
    BraindeadSort sort1;
    sort1.run();

    SimpleSort sort2;
    sort2.run();

    QuickSort sort3;
    sort3.run();

    return 0;
}
