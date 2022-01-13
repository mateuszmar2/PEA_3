#ifndef GENETIC_ALGORITHM_H_
#define GENETIC_ALGORITHM_H_

#include <vector>

class GeneticAlgorithm
{
private:
    std::vector<std::vector<int>> matrix;
    int number_of_towns;    // ilość miast
    int population_size;    // rozmiar populacji
    std::vector<int> route; // najlepsza ścieżka
    int route_cost;         // jej koszt
    int stop_time;          // czas w sekundach po jakim algorytm się zatrzyma

    void crossover();
    void mutation();
    int getRouteCost();                                    // zwraca najlepszą ścieżkę
    std::vector<int> randomRoute();                        // zwraca losową ścieżkę zaczynającą i kończącą się na 0
    int pathDistance(std::vector<int> route_to_calculate); // oblicza koszt ścieżki
    int randomIndex();                                     // zwraca losowy indeks miasta

public:
    GeneticAlgorithm(std::vector<std::vector<int>> towns, int population_size, int stop_time);
    void startGA();
    void printRoute(); // wyświela wynik
};

#endif /* GENETIC_ALGORITHM_H_ */