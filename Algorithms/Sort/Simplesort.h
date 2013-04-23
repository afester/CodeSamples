#ifndef __SIMPLESORT_H__
#define __SIMPLESORT_H__

#include "SortAlgorithm.h"

class SimpleSort : public SortAlgorithm {
public:
    SimpleSort();

protected:
    virtual void doSort();
};

#endif