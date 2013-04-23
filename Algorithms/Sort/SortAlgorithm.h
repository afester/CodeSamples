#ifndef __SORT_ALGORITHM_H__
#define __SORT_ALGORITHM_H__

#include <iostream>
#include <fstream>
#include <vector>


class SortAlgorithm {
private:
    std::string algorithmName;

protected:
    std::vector<std::string> unsortedArray;

    std::vector<std::string> loadList(const char* fileName);

    virtual void doSort() = 0;

public:
    SortAlgorithm(const std::string& name = "Unknown");

    virtual ~SortAlgorithm();

    void generateData();

    void run();
};

#endif
