#ifndef __QUICKSORT_H__
#define __QUICKSORT_H__

#include "SortAlgorithm.h"

class QuickSort : public SortAlgorithm {
    void quicksort(int left, int right);
    int divide(int left, int right);

public:
    QuickSort();

protected:
    virtual void doSort();
};

#endif
