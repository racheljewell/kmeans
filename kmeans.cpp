#include <iostream>
#include <fstream>
#include <string>
#include <list>
#include <utility>
#include <sstream>
#include <cmath>
#include <random>
#include <ctime>
#include <iterator>

using namespace std;

list<pair<int, int>> ReadFile(string filename);
int Distance(pair<int, int> point);
list<list<pair<int,int>>> RandomPartition(list<pair<int, int>> points, int k);
list<pair<int, int>> ComputeCentroids(list<list<pair<int, int>>> partitionedPoints, int k);
list<list<pair<int,int>>> Recluster(list<pair<int, int>> points, list<pair<int, int>> centroids, int k);
void PrintClusters(list<list<pair<int, int>>> partitions);

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
    

    list<list<pair<int, int>>> partitions = RandomPartition(points, k);

    PrintClusters(partitions);

    auto centroids = ComputeCentroids(partitions, k);

    auto newClusters = Recluster(points, centroids, k);

    PrintClusters(newClusters);
    
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

int Distance(pair<int,int> point1, pair<int,int> point2) {
    return sqrt(pow((point2.first-point1.first), 2) + pow((point2.second-point1.second), 2));
}

list<list<pair<int,int>>> RandomPartition(list<pair<int, int>> points, int k) {

    list<list<pair<int,int>>> partitions(k);

    for (pair<int, int> p : points) {
        random_device rd;
        mt19937 gen(rd());
        uniform_int_distribution<> dis(0, k-1);
        int clusterNumber = dis(gen);

        auto it = partitions.begin();
        advance(it, clusterNumber);
        it->push_back(p);
    }
    
    for (auto list : partitions) {
        if (list.empty()) {
            return RandomPartition(points, k);
        }
    }
    
    return partitions;
}

list<pair<int, int>> ComputeCentroids(list<list<pair<int, int>>> partitionedPoints, int k) {
    
    list<pair<int, int>> centroids;

    for (auto cluster : partitionedPoints) {
        int sumX = 0;
        int sumY = 0;
        for (auto point : cluster) {
            sumX += point.first;
            sumY += point.second;
        }

        int centerPointX = sumX / cluster.size();
        int centerPointY = sumY / cluster.size();

        centroids.push_back(make_pair(centerPointX, centerPointY));
    }

    return centroids;
}

list<list<pair<int,int>>> Recluster(list<pair<int, int>> points, list<pair<int, int>> centroids, int k) {
    list<list<pair<int, int>>> clusters(k);

    for (auto point: points) {
        int clusterNumber = 0;
        int minDistance = INT_MAX;
        int closestCluster;
        for (auto centroid : centroids) {
            int distance = Distance(point, centroid);
            if (distance < minDistance) {
                minDistance = distance;
                closestCluster = clusterNumber;
            }
            clusterNumber++;
        }
        auto it = clusters.begin();
        advance(it, closestCluster);
        it->push_back(point);
    } 
    return clusters;  
}

void PrintClusters(list<list<pair<int, int>>> partitions) {
    int clusterNumber = 1;
    for (auto partition : partitions) {
        cout << "Cluster " << clusterNumber << ": \n";
        for (auto point : partition) {
            cout << "(" << point.first << ", " << point.second << ")\n";
        }
        clusterNumber++;
    }
}