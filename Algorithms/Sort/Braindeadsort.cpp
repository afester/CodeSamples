#include "Braindeadsort.h"

using namespace std;


BraindeadSort::BraindeadSort() : SortAlgorithm("BraindeadSort") {
}


/**
 * Brain-dead sorting algorithm:
 *  - Search the next value in the unsorted array
 *  - push this value into the sorted array
 *  - start from scratch
 *
 *  - Time complexity: O(n^2)
 *  - Space complexity: O(2n)
 */
void BraindeadSort::doSort() {
    vector<string> sortedArray;

    string prev;
    for (vector<string>::size_type i = 0;  i < unsortedArray.size();  i++) {

	// Find the smallest element which is still larger than the previous element
	string current;
	for (vector<string>::size_type idx = 0;  idx < unsortedArray.size();  idx++) {
	    if (unsortedArray[idx] > prev && (current == "" || unsortedArray[idx] < current)) {
		current = unsortedArray[idx];
	    }
        }
        sortedArray.push_back(current);

        prev = current;
    }
    
    unsortedArray = sortedArray;
}
