#include <iostream>
#include <fstream>
#include <string>
#include <list>
#include <utility>
#include <sstream>

using namespace std;

list<pair<int, int>> ReadFile(string filename);
int Distance(pair<int, int> point);

int main(int argc, char* argv[]) {
    int k;
    try {
        k = stoi(argv[1]);
    } catch (...) {
        cout << "Invalid value for k.\n";
        return 1;
    }
    
    string file = argv[2];

    list<pair<int, int>> points = ReadFile(file);

    for (pair<int, int> p : points) {
        cout << "("<< p.first << ", " << p.second << ")\n";
    }
}

list<pair<int, int>> ReadFile(string filename) {

    list<pair<int, int>> dataPoints;

    ifstream fin;
    fin.open(filename);

    if (!fin) {

        cout << "Invalid file. File not found.\n";
        exit(EXIT_FAILURE);

    }

    int point1, point2;
    string line;

    while (getline(fin, line)) {

        istringstream iss(line);

        if (!(iss >> point1 >> point2)) {

            std::cerr << "Error reading line: " << line << std::endl;
            continue;
        }

        dataPoints.push_back(make_pair(point1, point2));
    }

    fin.close();

    return dataPoints;
    
}