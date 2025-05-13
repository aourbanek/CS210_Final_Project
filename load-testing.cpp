#include <iostream>
#include <fstream>
#include <vector>
#include <random>
#include <algorithm>
#include "main.cpp"
using namespace std;

int main() {
    CityTrie trie;
    CityHashTable hashTable(10);
    CSVReader fileReading;
    vector<vector<string>> cities = fileReading.readCSV("world_cities.csv");
    int deletionMethod;

    for (vector<string> item : cities)
    {
        trie.insert(item[1], item[0], item[2]);
    }

    cout << "CS 210 CITY SEARCH (MILESTONE 4)" << endl;
    cout << "Choose a deletion scheme:" << endl;
    cout << "1. Least Frequently Used" << endl;
    cout << "2. First In, First Out" << endl;
    cout << "3. Random" << endl;
    std::cin >> deletionMethod;
    while (deletionMethod < 1 || deletionMethod > 3)
    {
        cout << "Invalid selection. Select again:" << endl;
        cout << "1. Least Frequently Used" << endl;
        cout << "2. First In, First Out" << endl;
        cout << "3. Random" << endl;
        std::cin >> deletionMethod;
    }
    cout << "Deletion scheme " << deletionMethod << " chosen." << endl;

    return 0;
}