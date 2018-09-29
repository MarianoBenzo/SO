#include <iostream>
#include <cstdlib>
#include <list>
#include "ConcurrentHashMap.cpp"

using namespace std;

int main(){

	ConcurrentHashMap map;
	map = countWordsInFile("texto.txt");

	list<string> keys = map.keys();

	map.operator<<(cout);

	return 0;
}