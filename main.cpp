#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
using namespace std;

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

    if (choice == 'y')
    {
        cout << "CS 210 CITY SEARCH (MILESTONE 1)" << endl;
        cout << "Enter a country code (lowercase):" << endl;
        cin >> searchCountry;
        cout << "Enter a city name (lowercase):" << endl;
        cin >> searchCity;

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


    CSVReader fileReading;
    string filename = "world_cities.csv";
    vector<vector<string>> data = fileReading.readCSV("world_cities.csv");
}