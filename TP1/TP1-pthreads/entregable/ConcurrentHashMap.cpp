#include <utility>

#include <utility>

#ifndef CHM_CPP
#define CHM_CPP

#include <algorithm>
#include <atomic>
#include <cctype>
#include <utility>
#include <fstream>
#include <list>
#include <vector>
#include <string>
#include <pthread.h>
#include <fstream>
#include <sstream>
#include <semaphore.h>
#include "ConcurrentHashMap.hpp"

using namespace std;

ConcurrentHashMap::ConcurrentHashMap() {
    for (int i = 0; i < 26; i++){
        tabla[i] = new Lista<pair<string, unsigned int> >();
        sem_init(&semaforo[i], 0, 1);
    }
    pthread_mutex_init(&lock_add, NULL);
    sem_init(&lock_max, 0, 1);
    max = make_pair("", 0);
    escritores = 0;
}

ConcurrentHashMap::~ConcurrentHashMap() {
    for (int i = 0; i < 26; i++){
        delete tabla[i];
        sem_destroy(&semaforo[i]);
    }
}

ConcurrentHashMap::ConcurrentHashMap(ConcurrentHashMap&& otro){
    for (int i = 0; i < 26; i++){
        tabla[i] = otro.tabla[i];
        otro.tabla[i] = NULL;
        sem_init(&semaforo[i], 0, 1);
    }
    max = otro.max;
}

void ConcurrentHashMap::addAndInc(string key) {
    pthread_mutex_lock(&lock_add);
    escritores++;

    if(escritores == 1)
        sem_wait(&lock_max);

    pthread_mutex_unlock(&lock_add);
    int index = hash_key(key);
    // Obtengo acceso exclusivo de la lista a modificar
    sem_wait(&semaforo[index]);

    if (value(key) == 0) {
        pair<string, unsigned int> p(key, 1);
        tabla[index]->push_front(p);
    } else {
        for (auto it = tabla[index]->CrearIt(); it.HaySiguiente(); it.Avanzar()) {
            auto &t = it.Siguiente();
            t.second++;
        }
    }

    sem_post(&semaforo[index]);
    pthread_mutex_lock(&lock_add);
    escritores--;
    if(escritores == 0)
        sem_post(&lock_max);

    pthread_mutex_unlock(&lock_add);
}

list <string> ConcurrentHashMap::keys() {
    list <string> keys;

    for (int i = 0; i < 26; i++) {
        for (auto it = tabla[i]->CrearIt(); it.HaySiguiente(); it.Avanzar()) {
            auto &t = it.Siguiente();
            keys.push_front(t.first);
        }
    }

    return keys;
}

unsigned int ConcurrentHashMap::value(string key) {
    int index = hash_key(key);
    unsigned int value = 0;
    Lista<pair<string, unsigned int>> *tablaValue = tabla[index];

    for (auto it = tablaValue->CrearIt(); it.HaySiguiente(); it.Avanzar()) {
        auto t = it.Siguiente();
        if (t.first == key) {
            value = t.second;
        }
    }

    return value;
}

struct argsMaximum{
    void *map;
    unsigned int t_id;
    unsigned int n;
};

// Función ejecutada por cada thread de maximum
void *ConcurrentHashMap::searchMaximum( unsigned int id , unsigned int n ){
    for (unsigned int i = id; i < 26; i += n) {

        pair<string, unsigned int> max_fila("", 0);

        for (auto it = tabla[i]->CrearIt(); it.HaySiguiente(); it.Avanzar()){
            auto t = it.Siguiente();
            if (t.second > max_fila.second)
                max_fila = t;
        }
        while(true){
            while(lock.load());
            if (!lock.exchange(true))
                break;
        }

        if (max_fila.second > max.second || (max_fila.second == max.second && max_fila.first < max.first))
            max = max_fila;

        lock.store(false);
    }
    return NULL;
}

void *ConcurrentHashMap::maximumWrapper(void* context){
    struct argsMaximum *args = (struct argsMaximum*) context;
    ConcurrentHashMap* clase = (ConcurrentHashMap*) args->map;
    return clase->searchMaximum(args->t_id, args->n);
}

pair<string, unsigned int> ConcurrentHashMap::maximum(unsigned int n) {
    sem_wait(&lock_max);

    pthread_t thread[n];
    unsigned int tid;
    argsMaximum tids[n];
    lock.store(false);

    for (tid = 0; tid < n; ++tid) {
        tids[tid].map = this;
        tids[tid].t_id = tid;
        tids[tid].n = n;
        pthread_create(&thread[tid], NULL, maximumWrapper, &tids[tid]);
    }

    for (tid = 0; tid < n; ++tid)
        pthread_join(thread[tid], NULL);

    pair<string, unsigned int> maximum = max;
    sem_post(&lock_max);

    return maximum;
}

int ConcurrentHashMap::hash_key(string key) {
    return key.at(0) - 97;
}

ostream &ConcurrentHashMap::operator<<(ostream &os) {
    os << "Imprimiendo ConcurrentHashMap" << endl;
    for (int i = 0; i < 26; i++) {
        os << "tabla[" << i << "]= ";
        for (auto it = tabla[i]->CrearIt(); it.HaySiguiente(); it.Avanzar()) {
            auto elto_atomicList = it.Siguiente();
            os << "<" << elto_atomicList.first << ", " << elto_atomicList.second << ">";
        }
        os << endl;
    }

    return os;
}


struct argsCountWords {
    string path;
    list <string> *files;
    pthread_mutex_t* mutex;
    ConcurrentHashMap *map;
};

void countWordsInFileToConcurrentHashMap(string filePath, ConcurrentHashMap *map) {
    string line;
    ifstream file;
    file.open(filePath);
    if (file.is_open()) {
        string word;
        while (file >> word) {
            map->addAndInc(word);
        }
    } else {
        perror("Error al abrir el archivo: ");
    }
    file.close();
}

static ConcurrentHashMap countWordsInFile(string filePath) {
    ConcurrentHashMap map;
    countWordsInFileToConcurrentHashMap(filePath, &map);
    return map;
}

void *countWordsFromArguments(void *args) {
    argsCountWords *acw = (argsCountWords *) args;
    ConcurrentHashMap *map = acw->map;

    countWordsInFileToConcurrentHashMap(acw->path, map);
    return 0;
}

static ConcurrentHashMap countWordsOneThreadPerFile(list <string> filePaths) {
    ConcurrentHashMap map;
    int numberOfThreads = filePaths.size();
    pthread_t thread[numberOfThreads];
    argsCountWords args[numberOfThreads];

    for (int i = 0; i < numberOfThreads; i++) {
        args[i].path = filePaths.front();
        filePaths.pop_front();
        args[i].map = &map;
    }

    for (int i = 0; i < numberOfThreads; i++)
        pthread_create(&thread[i], NULL, countWordsFromArguments, (void *) &args[i]);

    for (int i = 0; i < numberOfThreads; i++)
        pthread_join(thread[i], NULL);

    return map;
}

void* countWordsArbitraryFromArguments(void * args){
    argsCountWords* acw = (argsCountWords*)args;
    ConcurrentHashMap* map = acw->map;
    list<string>* files = acw->files;
    pthread_mutex_t* mutex = acw->mutex;

    string file;
    while(true){
        pthread_mutex_lock(mutex);

        if (!files->empty()){
            file = files->front();
            files->pop_front();
        }else{
            files->clear();
        }

        pthread_mutex_unlock(mutex);
        if(file.empty())
            return NULL;

        countWordsInFileToConcurrentHashMap(file, map);
    }
}

static ConcurrentHashMap countWordsArbitraryThreads(unsigned int n, list <string> filePaths) {
    ConcurrentHashMap map;
    pthread_t thread[n];
    argsCountWords args[n];

    pthread_mutex_t lock;
    pthread_mutex_init(&lock, NULL);

    for(int i = 0; i < n; i++){
        args[i].files = &filePaths;
        args[i].map = &map;
        args[i].mutex = &lock;
    }

    for(int i = 0; i < n; i++)
        pthread_create(&thread[i], NULL, countWordsArbitraryFromArguments, (void*)&args[i]);

    for(int i = 0; i < n; i++)
        pthread_join(thread[i], NULL);

    return map;
}

static pair<string, unsigned int>
maximumOne(unsigned int readingThreads, unsigned int maxingThreads, list <string> filePaths) {

    ConcurrentHashMap maps[readingThreads];
    pthread_t thread[readingThreads];
    argsCountWords args[readingThreads];
    pthread_mutex_t lock;

    for(int i = 0; i < readingThreads; i++){
        args[i].files = &filePaths;
        args[i].map = &maps[i];
        args[i].mutex = &lock;
    }

     for(int i = 0; i < readingThreads; i++)
        pthread_create(&thread[i], NULL, countWordsArbitraryFromArguments, (void*)&args[i]);

     for(int i = 0; i < readingThreads; i++)
         pthread_join(thread[i], NULL);

    ConcurrentHashMap map;
    for (int j = 0; j < readingThreads; j++){
        for (int i = 0; i < 26; i++){
            for (auto it = maps[j].tabla[i]->CrearIt(); it.HaySiguiente(); it.Avanzar()){
                auto t = it.Siguiente();
                for (int k = 0; k < t.second; k++)
                    map.addAndInc(t.first);
            }
        }
    }

    return map.maximum(maxingThreads);
}

static pair<string, unsigned int>
maximumTwo(unsigned int readingThreads, unsigned int maxingThreads, list <string> filePaths) {

    ConcurrentHashMap map = countWordsArbitraryThreads(readingThreads, filePaths);

    return map.maximum(maxingThreads);
}


#endif
