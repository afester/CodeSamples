#include "SortAlgorithm.h"
#include "Quicksort.h"

using namespace std;

QuickSort::QuickSort() : SortAlgorithm("Quicksort") {
}


int QuickSort::divide(int left, int right) {
    int i = left; 
    int j = right - 1;
    string pivot = unsortedArray[right];

    do {
        while(unsortedArray[i] <= pivot && i < right) {
	    i++;
	}

	while(unsortedArray[j] >= pivot && j > left) {
            j--;
        }

	if (i < j) {
	    string temp = unsortedArray[j];
	    unsortedArray[j] = unsortedArray[i];
	    unsortedArray[i] = temp;
        }
    } while(i < j);

    if (unsortedArray[i] > pivot) {
	string temp = unsortedArray[right];
	unsortedArray[right] = unsortedArray[i];
	unsortedArray[i] = temp;
    }

    return i;
}


void QuickSort::quicksort(int left, int right) {
    if (left < right) {
        int divider = divide(left, right);
	quicksort(left, divider - 1);
	quicksort(divider + 1, right);
    }
}


/**
 * QuickSort sorting algorithm (in-place sorting)
 */
void QuickSort::doSort() {
    quicksort(0, unsortedArray.size() - 1);
}
