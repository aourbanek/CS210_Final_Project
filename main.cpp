#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
using namespace std;

struct City
{
    string countryCode;
    string cityName;
    string population;
    int id;
    City(string val1, string val2, string val3, int val4) : countryCode(val1), cityName(val2), population(val3), id(val4) {}

    void displayPrivate() const
    {
        cout << countryCode << endl;
        cout << cityName << endl;
        cout << population << endl;
        cout << id << endl;
    }
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

    CityHashTable(int size = 10) : size(size)
    {
        table.resize(size);
    }

    void insert(City city)
    {
        int key = hashFunction(city.countryCode + city.cityName, size);

        table[key].push_back(city);

        itemCount++;

        cout << "Item successfully added to hash table" << endl;
        cout << endl;

        return;
    }

    void deleteOldest()
    {
        for (vector<City> bucket : table)
        {
            for (City city : bucket)
            {
                city.id--;
            }
        }
        for (vector<City> bucket : table)
        {
            for (auto it = bucket.begin(); it != bucket.end(); ++it)
            {
                if (it->id == 0)
                {
                    bucket.erase(it);
                }
            }
        }
        itemCount--;
        return;
    }

    bool find(const string& code, const string& name)
    {
        int key = hashFunction(code + name, size);
        for (City city : table[key])
        {
            if (city.countryCode == code && city.cityName == name)
            {
                cout << "City found in cache!" << endl;
                cout << "Country code: " << city.countryCode << endl;
                cout << "City Name   : " << city.cityName << endl;
                cout << "Population  : " << city.population << endl;

                return true;
            }
        }

        cout << "City " << name << " not found in hash table." << endl;
        cout << endl;
        return false;
    }

    void display() const
    {
        cout << "Displaying all cities:" << endl;
        for (int i = 0; i < size; ++i)
        {
            if (!table[i].empty())
            {
                cout << "Bucket " << i << ": " << endl;
                for (const City& city : table[i])
                {
                    city.displayPrivate();
                }
            }
        }

        return;
    }
};

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
    char keepRunning = 'y';
    char doMore = ' ';
    string searchCountry = "";
    string searchCity = "";
    CSVReader fileReading;

    CityHashTable hashTable(10); // 10 buckets

    while (running)
    {
        cout << "CS 210 CITY SEARCH (MILESTONE 1)" << endl;
        cout << hashTable.itemCount << endl;
        cout << "Enter a country code (lowercase):" << endl;
        cin >> searchCountry;
        cout << "Enter a city name (lowercase):" << endl;
        std::getline(std::cin >> std::ws, searchCity); // Same whitespace fix as midterm project
        cout << endl;

        bool foundInCache = false;
        foundInCache = hashTable.find(searchCountry, searchCity);

        if (!foundInCache)
        {
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
                        cout << "Cache full! Deleting oldest...";
                        hashTable.deleteOldest();
                    }

                    hashTable.insert(City(item[0], item[1], item[2], hashTable.itemCount + 1));
                    break;
                }
            }
            if (!foundInFile)
            {
                cout << "City not found." << endl;
            }
        }

        hashTable.display();

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