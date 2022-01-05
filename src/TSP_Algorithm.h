#ifndef TSP_ALGORITHM_H_
#define TSP_ALGORITHM_H_

#include <vector>

class TspAlgorithm
{
public:
    enum NeighbourOperation
    {
        SwapOperation = 1,
        ReverseOperation = 2,
        InsertOperation = 3
    };

public:
    std::vector<std::vector<int>> matrix;
    int number_of_towns;
    std::vector<int> route; // najlepsza ścieżka
    int route_cost;         // jej koszt
    int stop_time;          // czas w sekundach po jakim algorytm się zatrzyma
    NeighbourOperation operation;

public:
    int getRouteCost();                                    // zwraca najlepszą ścieżkę
    std::vector<int> randomRoute();                        // zwraca losową ścieżkę zaczynającą i kończącą się na 0
    int pathDistance(std::vector<int> route_to_calculate); // oblicza koszt ścieżki
    int randomIndex();                                     // zwraca losowy indeks miasta
    void printRoute();                                     // wyświela wynik
};

#endif /* TSP_ALGORITHM_H_ */