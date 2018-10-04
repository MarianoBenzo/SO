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
#include "ConcurrentHashMap.hpp"

using namespace std;

ConcurrentHashMap::ConcurrentHashMap() {
    // Completar
    for (auto &i : tabla) {
        i = new Lista<pair<string, unsigned int>>();
    }
}

ConcurrentHashMap::~ConcurrentHashMap(){
    // for (int i = 0; i < 26; i++){
    //     delete tabla[i];
    //     sem_destroy(&semaforo[i]);
    // }
}

void ConcurrentHashMap::addAndInc(string key) {
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
}

list<string> ConcurrentHashMap::keys() {
    list<string> keys;

    for (int i = 0; i < 26; i++)
    {
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

pair<string, unsigned int> ConcurrentHashMap::maximum(unsigned int n) {
    // Completar
}

int ConcurrentHashMap::hash_key(string key) {
    return key.at(0) - 97;
}

ostream& ConcurrentHashMap::operator<<(ostream& os) {
    os<<"Imprimiendo ConcurrentHashMap"<<endl;
    for (int i = 0; i < 26; i++)
    {
        os << "tabla[" << i << "]= ";
        for (auto it = tabla[i]->CrearIt(); it.HaySiguiente(); it.Avanzar()) {
            auto elto_atomicList = it.Siguiente();
            os<< "<" << elto_atomicList.first << ", " << elto_atomicList.second << ">";
        }
        os << endl;
    }

    return os;
}

static ConcurrentHashMap countWordsInFile(string filePath) {
    // Completar
    auto map = ConcurrentHashMap();
    string line;
    ifstream file;
    file.open(filePath);
    if (file.is_open()){
        string word;
        while (file >> word)
        {
            map.addAndInc(word);
        }
    }else {
        perror("Error al abrir el archivo: ");
    }
    file.close();
    return map;
}

static ConcurrentHashMap countWordsOneThreadPerFile(list <string> filePaths) {
    // Completar
}

static ConcurrentHashMap countWordsArbitraryThreads(unsigned int n, list <string> filePaths) {
    // Completar
}

static pair<string, unsigned int>  maximumOne(unsigned int readingThreads, unsigned int maxingThreads, list <string> filePaths) {
    // Completar
}

static pair<string, unsigned int>  maximumTwo(unsigned int readingThreads, unsigned int maxingThreads, list <string> filePaths) {
    // Completar
}



#endif
