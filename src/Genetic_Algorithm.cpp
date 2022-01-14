#include <iostream>
#include <algorithm>
#include <chrono>
#include <ctime>

#include "Genetic_Algorithm.h"

using namespace std;

GeneticAlgorithm::GeneticAlgorithm(vector<vector<int>> towns, int population_size, int stop_time, MutationOperation operation, double mutation_coefficient, double crossover_coefficient)
{
    matrix = towns;
    number_of_towns = matrix[0].size();
    this->population_size = population_size;
    this->stop_time = stop_time;
    this->operation = operation;
}

void GeneticAlgorithm::startGA()
{
    // rozpoczęcie pomiaru czasu
    chrono::system_clock::time_point start_time = chrono::system_clock::now();

    // utworzenie populacji
    vector<PopulationElement> population(population_size);
    for (int i = 0; i < population_size; i++)
    {
        population[i].route = randomRoute();
        population[i].cost = pathDistance(population[i].route);
        if (population[i].cost < best_route_cost || i == 0) // zapisanie najlepszej ścieżki z populacji
        {
            best_route = population[i].route;
            best_route_cost = population[i].cost;
        }

        // for (auto j = 0; j < number_of_towns; j++)
        //     cout << population[i].first[j] << " -> ";
        // cout << population[i].first[number_of_towns] << endl;
        // cout << "Pop_elem: " << i << ", Cost = " << population[i].second << endl;
    }
    // vector<int> current_best_neighbour;
    // vector<int> current_best = randomRoute();
    // int current_best_cost = pathDistance(current_best);
    // route = current_best;
    // route_cost = current_best_cost;
    while (true)
    {
        // jeżeli przekroczono dozwolony czas to przerwij
        int64_t time_diff = chrono::duration_cast<chrono::seconds>(chrono::system_clock::now() - start_time).count();
        if (time_diff >= stop_time)
            break;
    }
}




// przeprowadza mutację wybraną metodą oraz aktualizuje koszt ścieżki
void GeneticAlgorithm::mutation(MutationOperation o, PopulationElement &elem)
{
    int first_rand_index = randomIndex();
    int second_rand_index = randomIndex();
    while (first_rand_index == second_rand_index) // wylosowane indeksy nie nogą być identyczne
        second_rand_index = randomIndex();

    switch (o)
    {
    case SwapOperation:
        swap(elem.route[first_rand_index], elem.route[second_rand_index]); // zamiana wartości pod danymi indeksami
        break;
    case InsertOperation: // przeniesienie wartości wskazywanej przez pierwszy indeks w miejsce wskazywane przez drugi indeks
        if (first_rand_index < second_rand_index)
            rotate(elem.route.begin() + first_rand_index, elem.route.begin() + first_rand_index + 1, elem.route.begin() + second_rand_index + 1);
        else
            rotate(elem.route.begin() + second_rand_index, elem.route.begin() + first_rand_index, elem.route.begin() + first_rand_index + 1);
        break;
    }
    elem.cost = pathDistance(elem.route);
}

int GeneticAlgorithm::getRouteCost()
{
    return best_route_cost;
}

// zwraca długość trasy, trasa powinna zaczynać i kończyć się 0
int GeneticAlgorithm::pathDistance(vector<int> route_to_calculate)
{
    int path_distance = 0;
    // zliczenie sumy odległości w ścieżce aż do danego wierzchołka
    for (int i = 0; i < number_of_towns; i++)
        path_distance += matrix[route_to_calculate[i]][route_to_calculate[i + 1]];
    return path_distance;
}

// zwraca losowy index poza pierwszym i ostatnim
int GeneticAlgorithm::randomIndex()
{
    return rand() % (number_of_towns - 1) + 1;
}

// zwraca losową ścieżę zaczynającą i kończącą się na 0
vector<int> GeneticAlgorithm::randomRoute()
{
    vector<int> temp(number_of_towns + 1, 0);
    for (int i = 1; i < number_of_towns; i++)
        temp[i] = i; // trasa zaczynająca i kończąca się 0, pomiędzy nimi wszystkie indeksy

    random_shuffle(temp.begin() + 1, temp.end() - 1); // losowa trasa zaczynająca się od 0 i kończącą na 0

    return temp;
}

// wypisuje trasę oraz koszt
void GeneticAlgorithm::printRoute()
{
    for (auto i = 0; i < best_route.size() - 1; i++)
        cout << best_route[i] << " -> ";
    cout << best_route[best_route.size() - 1] << endl;
    cout << "Cost = " << best_route_cost << endl;
}
