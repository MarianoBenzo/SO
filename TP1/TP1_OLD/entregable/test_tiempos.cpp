#include <iostream>
#include <pthread.h>
#include "ConcurrentHashMap.hpp"
#include <list>
#include <string>

using namespace std;

int main(int argc, const char** argv){

    if (argc < 2)
        cout<<"LA CAGASTE"<<endl;

    int cantThreads = atoi(argv[1]);
    int cantArchivos = atoi(argv[2]);

    list<string> archivos;
    for (int i = 0; i < cantArchivos; i++){
        string path = "casos_tests_corpus/corpus_";
        path += to_string(i);
        archivos.push_back(path);
    }

    cout<< cantThreads<<", "<<cantArchivos<<", ";
    pair<string, unsigned int> maximo = maximum(cantThreads, 6, archivos);
    cout<<", ";
    pair<string, unsigned int> maximo_ = maximum_c(cantThreads, 6, archivos);
    cout<<"\n";

    return 0;
}
