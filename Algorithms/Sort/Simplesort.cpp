
#include "SortAlgorithm.h"
#include "Simplesort.h"


using namespace std;


SimpleSort::SimpleSort() : SortAlgorithm("SimpleSort") {
}

/**
 * SimpleSort sorting algorithm (in-place sorting):
 *  - Search the next value in the unsorted array
 *  - exchange values in the unsorted array
 *  - start from scratch
 *
 *  - Time complexity: O(n^2)
 *  - Space complexity: O(n)
 */
void SimpleSort::doSort() {
    for (vector<string>::size_type i = 0;  i < unsortedArray.size();  i++) {
	for (vector<string>::size_type idx = 0;  idx < unsortedArray.size();  idx++) {
	    if (unsortedArray[idx] > unsortedArray[i]) {
		string temp = unsortedArray[idx];
		unsortedArray[idx] = unsortedArray[i];
		unsortedArray[i] = temp;
	    }
        }
    }
}
