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
    lock = false;
    max = make_pair("", 0);
    cantWords = 0;
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
    cantWords.store(otro.cantWords.load());
}

ConcurrentHashMap::ConcurrentHashMap(ConcurrentHashMap& otro){
    for (int i = 0; i < 26; i++){
        tabla[i] = otro.tabla[i];
        otro.tabla[i] = NULL;
        sem_init(&semaforo[i], 0, 1);
    }
    max = otro.max;
    cantWords.store(otro.cantWords.load());
}

ConcurrentHashMap& ConcurrentHashMap::operator=(ConcurrentHashMap& otro){
    list<string> l = otro.keys();
    for (auto it = l.begin(); it != l.end(); it++)
    {   
        int cant_repeticiones = otro.value(*it);
        for (int i = 0; i < cant_repeticiones; i++)
        {
            addAndInc(*it);
        }
    }

    return *this;
}

void ConcurrentHashMap::addAndInc(string key) {
    int k = hash_key(key);
    // Obtengo acceso exclusivo de la lista a modificar
    sem_wait(&semaforo[k]);

    bool no_esta = true;
    for (auto it = tabla[k]->CrearIt(); it.HaySiguiente(); it.Avanzar()){
        auto& t = it.Siguiente();
        if (t.first == key){
            t.second++;
            no_esta = false;
        }
    }
    if (no_esta){
        pair<string, unsigned int> palabra(key, 1);
        tabla[k]->push_front(palabra);
    }
    
    cantWords++; // Operación atómica
    sem_post(&semaforo[k]);
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
};

// Función ejecutada por cada thread de maximum
void *ConcurrentHashMap::searchMaximum(){
    while(true){
        int i = i_max.fetch_add(1); // Operación atómica, devuelve el valor que tenia antes

        if (i >= 26) // No quedan más filas por procesar
            return NULL;

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
}

void *ConcurrentHashMap::maximumWrapper(void* context){
    struct argsMaximum *args = (struct argsMaximum*) context;
    ConcurrentHashMap* clase = (ConcurrentHashMap*) args->map;
    return clase->searchMaximum();
}

pair<string, unsigned int> ConcurrentHashMap::maximum(unsigned int n) {
    for (int i = 0; i < 26; i++)
        sem_wait(&semaforo[i]);

    pthread_t thread[n];
    unsigned int tid;
    argsMaximum tids[n];
    lock.store(false);
    i_max = 0;

    for (tid = 0; tid < n; ++tid) {
        tids[tid].map = this;
        pthread_create(&thread[tid], NULL, maximumWrapper, &tids[tid]);
    }

    for (tid = 0; tid < n; ++tid)
        pthread_join(thread[tid], NULL);

    pair<string, unsigned int> maximum = max;

    for (int i = 0; i < 26; i++)
        sem_post(&semaforo[i]);

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
    pthread_mutex_t* mutex = acw->mutex;
    list<string>* files = acw->files;

    string file;
    while(true){
        pthread_mutex_lock(mutex);
        if (!files->empty()){
            file = files->front();
            files->pop_front();
        }else{
            file.clear();
        }
        pthread_mutex_unlock(mutex);
        if (file.empty()) 
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
    pthread_mutex_init(&lock, NULL);

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
