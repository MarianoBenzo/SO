#include <pthread.h>
#include <stdio.h>
#include <iostream>
#include "ListaAtomica.hpp"

#define  CANT_THREADS  10
#define  CANT_ELEM     100000

using namespace std;

Lista<int> list;

void *listar(void *p_minumero)
{
    int minumero = *((int *) p_minumero);
    for (int i = minumero * CANT_ELEM; i < (minumero + 1) * CANT_ELEM; i++){
    	list.push_front(i);
    }
    return NULL;
}

int main(int argc, char **argv) 
{
    cout << "Corriendo test-lista... ";
    pthread_t thread[CANT_THREADS];
    int tids[CANT_THREADS], tid;

    for (tid = 0; tid < CANT_THREADS; ++tid) {
         tids[tid] = tid;
         pthread_create(&thread[tid], NULL, listar, &tids[tid]);
    }

    for (tid = 0; tid < CANT_THREADS; ++tid)
         pthread_join(thread[tid], NULL);

    // En un principio los listaba, pero como eran pocos elementos (10 por thread)
    // no había concurrencia. Es decir, cada thread terminaba antes de ser desalojado por otro
    int cant = 0;
    for (auto it = list.CrearIt(); it.HaySiguiente(); it.Avanzar()) {
    	cant++;
    }

    if (cant == CANT_THREADS * CANT_ELEM)
        cout << "OK!" << endl;
    else
        cout << "FALTAN ELEMENTOS!" << endl;

    return 0;
}
