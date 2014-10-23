#include <iostream>
#include <iomanip>
#include <map>

#include "Samples.h"
#include "colorManip.h"

using namespace std;

string readInput(const string& prompt) {
    cout << col(12) << prompt;

    string line;
    getline(cin, line);
    return line;
}


int main() {
#if 0
    // simple rectangle
    cout << col(2) <<                 "+---------------------------------------------------+" << endl
         << col(2) << '|' << col(3) << " Lorem ipsum dolor sit amet, consetetur sadipscing " << col(2) << '|' << endl
         << col(2) << '|' << col(3) << " elitr, sed diam nonumy eirmod " << col(5) << "tempor" << col(3) << " invidunt ut  " << col(2) << '|' << endl
         << col(2) << '|' << col(3) << " labore et dolore magna aliquyam erat, sed diam    " << col(2) << '|' << endl
         << col(2) << '|' << col(3) << " voluptua. At vero eos et accusam et justo duo     " << col(2) << '|' << endl
         << col(2) << '|' << col(3) << " dolores et ea rebum. Stet clita kasd gubergren,   " << col(2) << '|' << endl
         << col(2) <<                 "+---------------------------------------------------+" << endl;
    return 0;

    // you can loop k higher to see more color choices
    for(int k = 1; k < 256; k++) {
        cout << col(k) << k << " I want to be nice today!" << endl;
    }
    return 0;
#endif

    map<string, SamplePtr> samples = {
        {"1", SamplePtr(new BitsSample())},
        {"2", SamplePtr(new ForEachSample())},
        {"3", SamplePtr(new InitializerSample())},
        {"4", SamplePtr(new OutputSample())},
        {"5", SamplePtr(new ShapeOperatorSample())},
        {"6", SamplePtr(new VTableSample())},
        {"7", SamplePtr(new EnumSample())},
        {"8", SamplePtr(new SizeofSample())},
        {"9", SamplePtr(new MapSample())}
    };

    cout << col(33) << cls;
    while(1) {
        cout << "Please choose a sample:" << endl;
        for (map<string, SamplePtr>::const_iterator it = samples.begin();
             it != samples.end();
             it++) {

            map<string, SamplePtr>::value_type p = *it;
            string id = p.first;
            SamplePtr s = p.second;

            cout << setw(2) << id << ". " << s->getName() << endl;
        }
        cout << "-----------------------" << endl
             << " 0. All of the above" << endl
             << " q. Quit" << endl
             << "=======================" << endl;
        string choice = readInput("Which sample to run (enter number and press <RETURN>)? ");
        cout << cls;

        if (choice == "q") {
            return 0;
        } else if (choice == "0") {
            for (map<string, SamplePtr>::const_iterator it = samples.begin();
                 it != samples.end();
                 it++) {

                map<string, SamplePtr>::value_type p = *it;
                string id = p.first;
                SamplePtr s = p.second;

                cout << endl
                     << s->getName() << endl
                     << "==================================================================================" << endl;
                s->run();
                cout << "==================================================================================" << endl
                     << endl;
            }
        } else {
            map<string, SamplePtr>::const_iterator it = samples.find(choice);
            if (it != samples.end()) {
                SamplePtr s = (*it).second;

                cout << endl
                     << s->getName() << endl
                     << "==================================================================================" << endl;
                s->run();
                cout << "==================================================================================" << endl
                     << endl;
            } else {
                cout << "\"" << choice << "\" not found!" << endl;
            }
        }
    }

    return 0;
}
