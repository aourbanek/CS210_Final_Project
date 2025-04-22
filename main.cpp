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