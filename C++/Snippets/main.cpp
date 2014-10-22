#include <iostream>
#include <iomanip>
#include <map>

#include "Samples.h"

using namespace std;

string readInput(const string& prompt) {
    cout << prompt;

    string line;
    getline(cin, line);
    return line;
}

int main() {
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
