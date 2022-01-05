#include <iostream>
#include <algorithm>

#include "TSP_Algorithm.h"

using namespace std;

int TspAlgorithm::getRouteCost()
{
    return route_cost;
}

// zwraca długość trasy, trasa powinna zaczynać i kończyć się 0
int TspAlgorithm::pathDistance(vector<int> route_to_calculate)
{
    int path_distance = 0;
    // zliczenie sumy odległości w ścieżce aż do danego wierzchołka
    for (int i = 0; i < number_of_towns; i++)
        path_distance += matrix[route_to_calculate[i]][route_to_calculate[i + 1]];
    return path_distance;
}

// zwraca losowy index poza pierwszym i ostatnim
int TspAlgorithm::randomIndex()
{
    return rand() % (number_of_towns - 1) + 1;
}

// zwraca losową ścieżit_without_changeę zaczynającą i kończącą się na 0
vector<int> TspAlgorithm::randomRoute()
{
    vector<int> temp(number_of_towns + 1, 0);
    for (int i = 1; i < number_of_towns; i++)
        temp[i] = i; // trasa zaczynająca i kończąca się 0, pomiędzy nimi wszystkie indeksy

    random_shuffle(temp.begin() + 1, temp.end() - 1); // losowa trasa zaczynająca się od 0 i kończącą na 0

    return temp;
}

// wypisuje trasę oraz koszt
void TspAlgorithm::printRoute()
{
    for (auto i = 0; i < route.size() - 1; i++)
        cout << route[i] << " -> ";
    cout << route[route.size() - 1] << endl;
    cout << "Cost = " << route_cost << endl;
}
