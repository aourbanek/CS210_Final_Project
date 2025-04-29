#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <limits>
using namespace std;

struct City
{
    string countryCode;
    string cityName;
    string population;
    int id; // Used for FIFO in cache
    int used; // Used for LFU deletion in cache
    City(string val1, string val2, string val3, int val4, int val5) : countryCode(val1), cityName(val2), population(val3), id(val4), used(val5) {}
};

class CityHashTable
{
private:
    // Hash Table initialization
    vector<vector<City>> table;
    int size;

    int hashFunction(string key, int tableSize) {
        int hash = 0;
        for (char ch : key) {
            hash += ch;
        }
        return hash % tableSize;
    }

public:
    int itemCount = 0;
    // Original Hash Table implementation can return its size (# of buckets),
    // but not the actual number of items across all buckets

    CityHashTable(int size = 10) : size(size)
    {
        table.resize(size);
    }

    void insert(City city)
    {
        int key = hashFunction(city.countryCode + city.cityName, size);

        table[key].push_back(city);

        itemCount++;

        cout << "City added to search cache." << endl;
        cout << endl;

        return;
    }

    void deleteOldest()
    {
        // First reduces every City's id/"index" to maintain the <= 10 limit
        for (vector<City>& bucket : table)
        {
            for (City& city : bucket)
            {
                city.id--;
            }
        }
        // Then oldest City (originally id 1, now 0 from above loop) is actually deleted
        for (vector<City>& bucket : table)
        {
            for (auto it = bucket.begin(); it != bucket.end(); ++it)
            {
                if (it->id == 0)
                {
                    bucket.erase(it);
                    cout << "Oldest item in cache deleted." << endl;
                    break;
                }
            }
        }
        itemCount--; // Must be decremented manually for same reason itemCount was initialized
        return;
    }

    void deleteLFU()
    {
        int min_access = std::numeric_limits<int>::max();

        City leastUsed = City(NULL, NULL, NULL, NULL, NULL);

        for (vector<City>& bucket : table)
        {
            for (auto it = bucket.begin(); it != bucket.end(); ++it)
            {
                if (it->used < min_access)
                {
                    min_access = it->used;
                    leastUsed.countryCode = it->countryCode;
                    leastUsed.cityName = it->cityName;
                    leastUsed.population = it->population;
                    leastUsed.id = it->id;
                    leastUsed.used = it->used;
                }
            }
        }

        for (vector<City>& bucket : table)
        {
            for (auto it = bucket.begin(); it != bucket.end(); ++it)
            {
                if (it->countryCode == leastUsed.countryCode && it->cityName == leastUsed.cityName)
                {
                    bucket.erase(it);
                    cout << "Least used item in cache deleted." << endl;
                    break;
                }
            }
        }
    }

    // Returns a bool so whether or not the database needs to
    // be searched can be determined directly from this search
    bool find(const string& code, const string& name)
    {
        cout << "Searching cache..." << endl;
        int key = hashFunction(code + name, size);
        for (City& city : table[key])
        {
            if (city.countryCode == code && city.cityName == name)
            {
                cout << "City found in cache!" << endl;
                cout << "Country code  : " << city.countryCode << endl;
                cout << "City Name     : " << city.cityName << endl;
                cout << "Population    : " << city.population << endl;
                city.used++;
                cout << "Item use count: " << city.used << endl;
                cout << endl;

                return true;
            }
        }

        cout << "City " << name << " not found in cache." << endl;
        cout << endl;
        return false;
    }
};

// Exact copy of CSVReader from Midterm Project
class CSVReader {
public:
    static vector<vector<string>> readCSV(const string& filename) {
        ifstream file(filename);
        vector<vector<string>> data;
        string line, word;

        if (!file.is_open()) {
            cerr << "Error: Could not open file " << filename << endl;
            cout << endl;
            return data;
        }

        while (getline(file, line)) {
            stringstream ss(line);
            vector<string> row;
            while (getline(ss, word, ',')) {
                row.push_back(word);
            }
            data.push_back(row);
        }
        file.close();
        return data;
    }
};

int main()
{
    bool running = true;
    char keepRunning = 'y'; // Effectively an input alias for bool running
    string searchCountry = "";
    string searchCity = "";
    CSVReader fileReading;
    int deletionMethod;

    CityHashTable hashTable(10); // 10 buckets

    cout << "CS 210 CITY SEARCH (MILESTONE 1)" << endl;
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

    while (running)
    {
        cout << "Enter a country code (lowercase):" << endl;
        std::cin >> searchCountry;
        cout << "Enter a city name (lowercase):" << endl;
        std::getline(std::cin >> std::ws, searchCity); // Same whitespace fix as midterm project
        cout << endl;

        // Cache search first
        bool foundInCache = false;
        foundInCache = hashTable.find(searchCountry, searchCity); // foundInCache search truthiness determined by search itself

        // Then search database if cache search unsuccessful
        if (!foundInCache)
        {
            cout << "Searching database..." << endl;
            bool foundInFile = false;
            for (vector<string> item : fileReading.readCSV("world_cities.csv"))
            {
                if (item[0] == searchCountry && item[1] == searchCity)
                {
                    foundInFile = true;
                    cout << "City found in database!" << endl;
                    cout << "Country code: " << item[0] << endl;
                    cout << "City Name   : " << item[1] << endl;
                    cout << "Population  : " << item[2] << endl;
                    cout << endl;

                    if (hashTable.itemCount == 10)
                    {
                        switch (deletionMethod)
                        {
                        case 1:
                            cout << "Cache full! Deleting least used..." << endl;
                            hashTable.deleteLFU();
                            break;
                        case 2:
                            cout << "Cache full! Deleting oldest..." << endl;
                            hashTable.deleteOldest();
                            break;
                        case 3:
                            cout << "Cache full! Deleting random..." << endl;
                            hashTable.deleteRandom();
                            break;
                        }
                    }

                    hashTable.insert(City(item[0], item[1], item[2], hashTable.itemCount + 1, 0));

                    break;
                }
            }
            // Then be sad if database search unsuccessful
            if (!foundInFile)
            {
                cout << "City not found." << endl; // :(
            }
        }

        // User prompt for continued operation
        cout << "Would you like to do more? (y/n)" << endl;
        cin >> keepRunning;
        if (keepRunning == 'n')
        {
            cout << "Ok. Goodbye!";
            running = false;
        }
    }

    return 0;
}