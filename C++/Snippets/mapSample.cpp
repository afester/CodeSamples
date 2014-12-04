#include <iostream>
#include <map>
#include <string>
#include <vector>
#include <inttypes.h>

#include "Samples.h"

void simpleSample() {
    std::cerr << std::endl
              << "std::map<K, V> sample:" << std::endl
              << "==========================" << std::endl;

    std::map<std::string, std::vector<int> > theMap;

    theMap["10"].push_back(1);
    theMap["10"].push_back(2);
    theMap["10"].push_back(3);

    theMap["30"].push_back(33);
    theMap["30"].push_back(36);
    theMap["30"].push_back(39);

    theMap["20"].push_back(25);
    theMap["20"].push_back(21);

    for (std::map<std::string, std::vector<int> >::const_iterator it = theMap.begin();
         it != theMap.end();
         it++) {
        std::map<std::string, std::vector<int> >::value_type p = *it;
        std::string key = p.first;
        std::vector<int> value = p.second;

        std::cerr << "   " << key << std::endl;
        for(std::vector<int>::const_iterator vit = value.begin();
            vit != value.end();
            vit++) {
            std::cerr << std::dec << "       " << *vit << std::endl;
        }

    }
}


class BigNumber {
public:
    uint32_t first;
    uint32_t second;
};


bool operator< (const BigNumber& lhs, const BigNumber& rhs){
    return lhs.second < rhs.second && lhs.first < rhs.first;
}

void userDefinedSample() {
    std::cerr << std::endl
              << "std::map<BigNumber, vector<int> > sample:" << std::endl
              << "=========================================" << std::endl;

    std::map<BigNumber, std::vector<int> > theMap;

    BigNumber b1 = {0x12345678, 0x98765432};
    BigNumber b2 = {0x23456789, 0x87654321};
    BigNumber b3 = {0x34567891, 0x76543210};
    BigNumber b4 = {0x45678912, 0x65432109};

    theMap[b1].push_back(b1.first);
    theMap[b1].push_back(b1.second);

    theMap[b4].push_back(b4.first);
    theMap[b4].push_back(b4.second);

    theMap[b2].push_back(b2.first);
    theMap[b2].push_back(b2.second);

    theMap[b3].push_back(b3.first);
    theMap[b3].push_back(b3.second);


    for (std::map<BigNumber, std::vector<int> >::const_iterator it = theMap.begin();
         it != theMap.end();
         it++) {
        std::map<BigNumber, std::vector<int> >::value_type p = *it;
        BigNumber key = p.first;
        std::vector<int> value = p.second;

        std::cerr << std::hex << "   " << key.first << "/" << key.second << std::endl;
        for(std::vector<int>::const_iterator vit = value.begin();
            vit != value.end();
            vit++) {
            std::cerr << std::hex << "       " << *vit << std::endl;
        }

    }

}


MapSample::MapSample() : Sample("Map sample") {
}

void MapSample::run() {
    simpleSample();
    userDefinedSample();
}
