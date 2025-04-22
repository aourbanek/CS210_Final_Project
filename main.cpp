#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
using namespace std;

struct City
{
    string countryCode;
    string countryName;
    double population;
    int cacheNum;
    City(string val1, string val2, double val3, int val4) : countryCode(val1), countryName(val2), population(val3), cacheNum(val4) {}
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
    CityHashTable(int size = 10) : size(size)
    {
        table.resize(size);
    }

    void insert(const City& city)
    {
        int key = hashFunction(city.countryCode + city.countryName, size);

        table[key].push_back(city);
        return;
    }

    //void deleteByName(const string& name)
    //{
    //    int key = hashFunction(name, size);
    //    auto& bucket = table[key];

    //    for (auto it = bucket.begin(); it != bucket.end(); ++it)
    //    {
    //        if (it->name == name)
    //        {
    //            bucket.erase(it);
    //            return;
    //        }
    //    }

    //    cout << "City " << name << " not found in hash table." << endl;
    //    return;
    //}

    void find(const string& code, const string& name)
    {
        int key = hashFunction(code + name, size);
        for (City& city : table[key])
        {
            if (city.countryCode == code && city.countryName == name)
            {

                cout << "City found!" << endl;
                cout << "Country code: " << city.countryCode << endl;
                cout << "City Name   : " << city.countryName << endl;
                cout << "Population  : " << city.population << endl;

                return;
            }
        }

        cout << "City " << name << " not found in hash table." << endl;
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

void interface(char choice)
{
    char doMore = ' ';
    string searchCountry = "";
    string searchCity = "";
    CSVReader fileReading;

    if (choice == 'y')
    {
        cout << "CS 210 CITY SEARCH (MILESTONE 1)" << endl;
        cout << "Enter a country code (lowercase):" << endl;
        cin >> searchCountry;
        cout << "Enter a city name (lowercase):" << endl;
        std::getline(std::cin >> std::ws, searchCity); // Same whitespace fix as midterm project

        bool found = false;
        for (vector<string> item : fileReading.readCSV("world_cities.csv"))
        {
            if (item[0] == searchCountry && item[1] == searchCity)
            {
                found = true;
                cout << "City found!" << endl;
                cout << "Country code: " << item[0] << endl;
                cout << "City Name   : " << item[1] << endl;
                cout << "Population  : " << item[2] << endl;
                break;
            }
        }
        if (!found)
        {
            cout << "City not found." << endl;
        }

        cout << "Would you like to do more? (y/n)" << endl;
        cin >> doMore;
        interface(doMore);
    }
    if (choice == 'n')
    {
        cout << "Ok. Goodbye!";
        return;
    }
}

int main()
{
    interface('y');
}