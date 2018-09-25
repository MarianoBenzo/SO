#include <iostream>
#include "test.hpp"
#include "ConcurrentHashMap.cpp"

using namespace std;

int Test::run(int argc, char **argv) {
    ConcurrentHashMap h;
    h = countWordsInFile("corpus");
    for (int i = 0; i < 26; i++) {
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
}