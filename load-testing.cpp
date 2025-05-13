#include <iostream>
#include <fstream>
#include <vector>
#include <random>
#include <algorithm>
#include "main.cpp"
#include <chrono>
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

    // Generate random indices for test
    mt19937 rng(static_cast<unsigned int>(0));
    uniform_int_distribution<int> dist(0, 299); // Upper bound < for loop stop condition should guarantee some cache hits

    cout << "Starting test..." << endl;

    for (int i = 1; i <= 500; ++i)
    {
        int index = dist(rng);
        string searchCountry = cities[index][0];
        string searchCity = cities[index][1];

        // Commented out for easier timing data extraction
        //cout << "Test " << i << ": Searching for " << searchCity << ", " << searchCountry << endl;

        auto startSearch = chrono::high_resolution_clock::now();

        bool foundInCache = hashTable.find(searchCountry, searchCity);

        if (!foundInCache)
        {
            string foundPopulation = trie.search(searchCity, searchCountry);
            if (foundPopulation != "NULL")
            {
                if (hashTable.itemCount == 10)
                {
                    switch (deletionMethod)
                    {
                    case 1:
                        hashTable.deleteLFU();
                        break;
                    case 2:
                        hashTable.deleteOldest();
                        break;
                    case 3:
                        hashTable.deleteRandom();
                        break;
                    }
                }
                hashTable.insert(City(searchCountry, searchCity, foundPopulation, hashTable.itemCount + 1, 0));
            }
            else {
                cout << "City not found." << endl;
            }
        }

        auto endSearch = chrono::high_resolution_clock::now();
        auto timeSearch = chrono::duration<double, micro>(endSearch - startSearch).count();
        cout << timeSearch << "us";
        if (foundInCache)
        {
            cout << ",hit";
        }
        else
        {
            cout << ",miss";
        }
        cout << endl;
    }

    cout << endl << "Test completed." << endl;
    return 0;
}