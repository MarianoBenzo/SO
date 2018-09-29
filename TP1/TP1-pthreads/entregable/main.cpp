#include <iostream>
#include <cstdlib>
#include <list>
#include "ConcurrentHashMap.cpp"

using namespace std;

int main(){
	ConcurrentHashMap map;
	map = countWordsInFile("texto.txt");

	list<string> keys = map.keys();

	// for (auto it = keys.begin(); it != keys.end(); it++)
	// {
	// 	map.value(*it);
	// }


	return 0;
}