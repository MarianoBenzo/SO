#include <iostream>
#include <cstdlib>
#include <list>
#include "test.hpp"
#include "ConcurrentHashMap.cpp"

using namespace std;

int Test::run(int argc, char **argv) {
    pair<string, unsigned int> p;
    list <string> l = {"texto.txt", "texto-2.txt", "texto-3.txt"};

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