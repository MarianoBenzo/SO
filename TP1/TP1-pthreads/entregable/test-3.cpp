#include <iostream>
#include <cstdlib>
#include <list>
#include "test.hpp"
#include "ConcurrentHashMap.cpp"

using namespace std;

int Test::run(int argc, char **argv) {
    pair<string, unsigned int> p;
    list <string> l = {"corpus-0", "corpus-1", "corpus-2", "corpus-3", "corpus-4"};

    if (argc != 3) {
        cerr << "uso: " << argv[0] << " #tarchivos #tmaximum" << endl;
        return 1;
    }

    p = maximumOne(atoi(argv[1]), atoi(argv[2]), l);
    cout << p.first << " " << p.second << endl;

    return 0;
}

int main(int argc, char **argv) {
    Test test;
    int exitCode = test.run(argc, argv);
    return exitCode;
}