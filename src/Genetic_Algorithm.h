#ifndef GENETIC_ALGORITHM_H_
#define GENETIC_ALGORITHM_H_

#include <vector>

struct PopulationElement
{
    std::vector<int> route;
    int cost;
};

class GeneticAlgorithm
{
public:
    enum MutationOperation
    {
        SwapOperation = 1,
        InsertOperation = 2
    };

private:
    std::vector<std::vector<int>> matrix;
    int number_of_towns;         // ilość miast
    int population_size;         // rozmiar populacji
    std::vector<int> best_route; // najlepsza ścieżka
    int best_route_cost;         // jej koszt
    int stop_time;               // czas w sekundach po jakim algorytm się zatrzyma
    MutationOperation operation;
    double mutation_coefficient;
    double crossover_coefficient;

    void mutation(MutationOperation o, PopulationElement &elem);
    std::vector<int> randomRoute();                        // zwraca losową ścieżkę zaczynającą i kończącą się na 0
    int pathDistance(std::vector<int> route_to_calculate); // oblicza koszt ścieżki
    int randomIndex();                                     // zwraca losowy indeks miasta

public:
    GeneticAlgorithm(std::vector<std::vector<int>> towns, int population_size, int stop_time, MutationOperation operation, double mutation_coefficient, double crossover_coefficient);
    void startGA();
    void printRoute();  // wyświela wynik
    int getRouteCost(); // zwraca najlepszą ścieżkę
};

#endif /* GENETIC_ALGORITHM_H_ */