#include <iostream>
#include "ConcurrentHashMap.hpp"

using namespace std;

int main(void) {
	int i;
	
	ConcurrentHashMap h(count_words("corpus"));
	for (i = 0; i < 26; i++) {
		for (auto it = h.tabla[i]->CrearIt(); it.HaySiguiente(); it.Avanzar()) {
			auto t = it.Siguiente();
			cout << t.first << " " << t.second << endl;
		}
	}

	return 0;
}

