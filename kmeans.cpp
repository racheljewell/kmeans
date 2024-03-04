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
bool areEquivalent(list<list<pair<int,int>>> oldClusters, list<list<pair<int,int>>> newClusters);
list<list<pair<int,int>>> Kmeans(list<pair<int, int>> points, int k);

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
    
    auto finalClusters = (Kmeans(points, k));
    PrintClusters(finalClusters);
}

/*
    Readfile takes a filename and parses it into a list of points. 
    It takes a string "filename" as it's parameter and expects the 
    file to be a text file with the integer coordinates of a point 
    on each line. 
    It returns the points as a list of pairs.
*/
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

/*
    Distance takes two integer pairs and calculates the Euclidean distance
    between them. It returns the distance as an integer.
*/
int Distance(pair<int,int> point1, pair<int,int> point2) {
    return sqrt(pow((point2.first-point1.first), 2) + pow((point2.second-point1.second), 2));
}

/*
    RandomPartition takes a list of integer points and an integer value for k
    and randomly partitions the points into k cluseters. It returns a list of 
    integer pair lists. Each integer pair list is a cluster. If any of the 
    clusters are empty, it repartitions the points.
*/
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

/*
    ComputeCentroids takes a list of integer pair lists and an integer value for k.
    It computes the centroids for each of the k clusters, which it returns as
    a list of k integer pairs.
*/
list<pair<int, int>> ComputeCentroids(list<list<pair<int, int>>> partitionedPoints, int k) {
    
    list<pair<int, int>> centroids(k);
    int i = 0;
    for (auto cluster : partitionedPoints) {
        int sumX = 0;
        int sumY = 0;
        for (auto point : cluster) {
            sumX += point.first;
            sumY += point.second;
        }

        if (cluster.size() == 0) {
            continue;
        }
        
        int centerPointX = sumX / cluster.size();
        int centerPointY = sumY / cluster.size();

        pair<int, int> centroid = make_pair(centerPointX, centerPointY);
        auto it = centroids.begin();
        advance(it, i);
        *it = centroid;
        i++;
    }
    return centroids;
}

/*
    Recluster reclusters points into the cluster with which they have the closest centroid. It takes
    a list of integer pairs as points, a list of integer pairs as centroids, and an integer value k.
    It returns the newly clustered points as a list of k integer pair lists, each being an individual
    cluster.
*/
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

/*
    PrintCluseters takes a list of integer pair lists and 
    displays the points in each cluster.
*/
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

/*
    areEquivalent determines if the points in two clusters are equivalent.
*/
bool areEquivalent(list<list<pair<int,int>>> oldClusters, list<list<pair<int,int>>> newClusters) {
    if (oldClusters.size() != newClusters.size()) {
        return false;
    }

    auto it1 = oldClusters.begin();
    auto it2 = newClusters.begin();

    for (; it1 != oldClusters.end() && it2 != newClusters.end(); it1++, it2++) {
        if (*it1 != *it2) {
            return false; 
        }
    }

    return true;
}

/*
    Kmeans takes a list of integer pairs as points and and integer value k.
    It then clusters the points into k clusters. It returns the clusters 
    as a list of k integer pair lists.
*/
list<list<pair<int,int>>> Kmeans(list<pair<int, int>> points, int k) {
    list<list<pair<int,int>>> oldClusters = RandomPartition(points, k);
    list<list<pair<int,int>>> newClusters(k);
    list<pair<int,int>> centroids = ComputeCentroids(oldClusters, k);
    
    while (!areEquivalent(oldClusters, newClusters)) {
        oldClusters = newClusters;
        newClusters = Recluster(points, centroids, k);
        centroids = ComputeCentroids(newClusters, k);
    }
    return newClusters;
}