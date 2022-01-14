#ifndef GENETIC_ALGORITHM_H_
#define GENETIC_ALGORITHM_H_

#include <vector>
#include <climits>

struct PopulationElement
{
    std::vector<int> route;
    int cost = INT_MAX;
    double fitness = 0.0;
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
    int number_of_towns; // ilość miast
    int population_size; // rozmiar populacji
    int stop_time;       // czas w sekundach po jakim algorytm się zatrzyma
    PopulationElement best;
    MutationOperation operation;
    double mutation_coefficient;
    double crossover_coefficient;

    void orderCrossover(std::vector<int> &parent_a, std::vector<int> &parent_b, std::vector<int> &child);
    PopulationElement selectElement(std::vector<PopulationElement> &population, double &max_fitness);
    void mutation(MutationOperation &o, std::vector<int> &route);
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