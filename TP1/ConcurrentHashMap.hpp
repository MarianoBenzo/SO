#ifndef CHM_HPP
#define CHM_HPP

#include <atomic>
#include <ctype.h>
#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <list>
#include <string>
#include <pthread.h>
#include <map>
#include <semaphore.h>
#include "ListaAtomica.hpp"
#include "test.hpp"

using namespace std;

class ConcurrentHashMap {
public:
    ConcurrentHashMap();
    ~ConcurrentHashMap();
    ConcurrentHashMap(ConcurrentHashMap&& otro);
    ConcurrentHashMap(ConcurrentHashMap& otro);
    ConcurrentHashMap& operator=(ConcurrentHashMap& otro);

    Lista<pair<string, unsigned int>>* tabla[26];

    void addAndInc(string key);

    list<string> keys();

    unsigned int value(string key);

    atomic_int cantWords;

    pair<string, unsigned int> maximum(unsigned int n);

    void * searchMaximum();

    static void *maximumWrapper(void* context);

    ostream& operator<<(ostream& os);

    friend Test;

private:
    int hash_key(string key);

    sem_t semaforo[26];
    atomic_bool lock;
    atomic_int i_max;

    pair<string, unsigned int> max;
};


static ConcurrentHashMap countWordsInFile(string filePath);

static ConcurrentHashMap countWordsOneThreadPerFile(list <string> filePaths);

static ConcurrentHashMap countWordsArbitraryThreads(unsigned int n, list <string> filePaths);

static pair<string, unsigned int> maximumOne(unsigned int readingThreads, unsigned int maxingThreads, list <string> filePaths);

static pair<string, unsigned int> maximumTwo(unsigned int readingThreads, unsigned int maxingThreads, list <string> filePaths);

#endif
