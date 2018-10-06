#include <iostream>
#include <cstdlib>
#include <list>
#include "test.hpp"
#include "ConcurrentHashMap.cpp"

using namespace std;

int Test::run(int argc, char **argv) {
    ConcurrentHashMap h;
    list <string> l = {"corpus-0", "corpus-1", "corpus-2"};
    int i;

    if (argc != 2) {
        cerr << "uso: " << argv[0] << " #threads" << endl;
        return 1;
    }

    h = countWordsArbitraryThreads(atoi(argv[1]), l);
    for (i = 0; i < 26; i++) {
        for (auto it = h.tabla[i]->CrearIt(); it.HaySiguiente(); it.Avanzar()) {
            auto t = it.Siguiente();
            cout << t.first << " " << t.second << endl;
        }
    }

    return 0;
}

int main(int argc, char **argv) {

    Test test;
    int exitCode = test.run(argc, argv);
    return exitCode;
    return 0;
}

