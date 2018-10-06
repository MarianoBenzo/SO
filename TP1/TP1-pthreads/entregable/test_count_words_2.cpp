#include <iostream>
#include <pthread.h>
#include "ConcurrentHashMap.cpp"
#include <list>
#include <cassert>
#include <string>

using namespace std;

int main(){
    cout << "Corriendo test-correctitud... ";
    list<string> archivos;
    for (int i = 0; i < 100; i++){
        string path = "casos_tests_corpus/corpus_";
        path += to_string(i);
        archivos.push_back(path);
    }
    ConcurrentHashMap h(countWordsOneThreadPerFile(archivos));
    ConcurrentHashMap h_2(countWordsArbitraryThreads(3, archivos));

    pair<string, unsigned int> max = h.maximum(4);
    pair<string, unsigned int> max_2 = h_2.maximum(4);

    assert(max == max_2);
    assert(h.cantWords == h_2.cantWords);
    
    ifstream archivo("casos_tests_corpus/corpus_sol");
    if (archivo.is_open()){

        int cantTotal;
        archivo>>cantTotal;

        int maximo;
        archivo>>maximo;

        assert(max.second == maximo);
        assert(h.cantWords == cantTotal);
        cout << "OK!" << endl;
    }else {
        perror("Error al abrir el archivo: ");
    }   
    archivo.close();

    return 0;
}
