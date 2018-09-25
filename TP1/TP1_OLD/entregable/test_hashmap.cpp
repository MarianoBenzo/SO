#include <iostream>
#include <pthread.h>
#include "ConcurrentHashMap.hpp"

#define  CANT_THREADS  10
#define  CANT_ELEM     10000

using namespace std;

ConcurrentHashMap h;

void *listar(void *p_minumero)
{
    int minumero = *((int *) p_minumero);
    if (minumero < (CANT_THREADS / 2)){
        for (int i = 0; i < CANT_ELEM; i++){
            h.addAndInc("perro");
            h.addAndInc("asteroide");
            h.addAndInc("gato");
            h.addAndInc("perro");
            h.addAndInc("flor");
            h.addAndInc("parasito");
            h.addAndInc("astilla");
            h.addAndInc("florencia");
            h.addAndInc("flaco");
            h.addAndInc("gastronomico");
            h.addAndInc("gato");
        }
    }else{
        pair<string, unsigned int> maximo = h.maximum(4);
        cout << maximo.first << " " << maximo.second << endl;
    }
    return NULL;
}

int main(){
    cout << "Corriendo test_hashmap... " << endl;
    pthread_t thread[CANT_THREADS];
    int tids[CANT_THREADS], tid;

    for (tid = 0; tid < CANT_THREADS; ++tid) {
         tids[tid] = tid;
         pthread_create(&thread[tid], NULL, listar, &tids[tid]);
    }

    for (tid = 0; tid < CANT_THREADS; ++tid)
         pthread_join(thread[tid], NULL);

	for (int i = 0; i < 26; i++) {
		for (auto it = h.tabla[i]->CrearIt(); it.HaySiguiente(); it.Avanzar()) {
			auto t = it.Siguiente();
			cout << t.first << " " << t.second << endl;
		}
	}

    pair<string, unsigned int> maximo = h.maximum(4);
    if (maximo.first == "gato" && maximo.second == CANT_THREADS * CANT_ELEM)
        cout << "OK!" << endl;
    else
        cout << "ERROR!" << endl;
    
	return 0;
}