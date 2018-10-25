#include <iostream>
#include <pthread.h>
#include "ConcurrentHashMap.cpp"
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
    pair<string, unsigned int> maximum1 = maximumOne(cantThreads, 6, archivos);
    cout<<", ";
    pair<string, unsigned int> maximum2 = maximumTwo(cantThreads, 6, archivos);
    cout<<"\n";

    return 0;
}
