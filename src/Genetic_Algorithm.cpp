#include <iostream>
#include <algorithm>
#include <chrono>
#include <ctime>
#include <math.h> /* pow */

#include "Genetic_Algorithm.h"

using namespace std;

GeneticAlgorithm::GeneticAlgorithm(vector<vector<int>> towns, int population_size, int stop_time, MutationOperation operation, double mutation_coefficient, double crossover_coefficient)
{
    matrix = towns;
    number_of_towns = matrix[0].size();
    this->population_size = population_size;
    this->stop_time = stop_time;
    this->operation = operation;
    this->mutation_coefficient = mutation_coefficient;
    this->crossover_coefficient = crossover_coefficient;
}

void GeneticAlgorithm::startGA()
{
    // rozpoczęcie pomiaru czasu
    chrono::system_clock::time_point start_time = chrono::system_clock::now();
    srand(time(NULL));
    // utworzenie populacji
    vector<PopulationElement> population(population_size);
    for (int i = 0; i < population_size; i++)
    {
        population[i].route = randomRoute();
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
    vector<PopulationElement> next_population(population);
    while (true)
    {
        // jeżeli przekroczono dozwolony czas to przerwij
        int64_t time_diff = chrono::duration_cast<chrono::seconds>(chrono::system_clock::now() - start_time).count();
        if (time_diff >= stop_time)
            break;

        double max_fitness = 0.0;
        for (int i = 0; i < population_size; i++)
        {
            population[i].cost = pathDistance(population[i].route);
            population[i].fitness = pow((double)(1.0 / population[i].cost), 10);
            // cout << population[i].fitness << ", " << population[i].cost << endl;
            if (population[i].fitness > max_fitness)
                max_fitness = population[i].fitness;
            if (population[i].cost < best.cost) // zapisanie najlepszej ścieżki z populacji
            {
                best.route = population[i].route;
                best.cost = population[i].cost;
                // cout << best.cost << endl
                // best.fitness = population[i].fitness;
            }
        }
        for (int i = 0; i < population_size; i++)
        {
            PopulationElement parent_a = selectElement(population, max_fitness);
            PopulationElement parent_b = selectElement(population, max_fitness);
            if ((double)rand() / RAND_MAX <= crossover_coefficient)
                orderCrossover(parent_a.route, parent_b.route, next_population[i].route);
            else
                next_population[i] = parent_a;

            if ((double)rand() / RAND_MAX <= mutation_coefficient)
                mutation(operation, next_population[i].route);
        }
        // swap(population, next_population);
        population.swap(next_population);
    }
}

PopulationElement GeneticAlgorithm::selectElement(vector<PopulationElement> &population, double &max_fitness)
{
    double r = (double)rand() / RAND_MAX;
    int i = rand() % population_size;
    while (population[i].fitness / max_fitness < r)
    {
        i = rand() % population_size;
    }
    return population[i];
}

void GeneticAlgorithm::orderCrossover(vector<int> &parent_a, vector<int> &parent_b, vector<int> &child)
{
    int first_rand_index = randomIndex();
    int second_rand_index = randomIndex();
    while (first_rand_index == second_rand_index) // wylosowane indeksy nie nogą być identyczne
        second_rand_index = randomIndex();

    if (first_rand_index > second_rand_index)
        swap(first_rand_index, second_rand_index);

    // przepisanie segmentu od rodzica
    vector<bool> is_in_vector(number_of_towns, false);
    for (int i = first_rand_index; i <= second_rand_index; i++)
    {
        child[i] = parent_a[i];
        is_in_vector[parent_a[i]] = true;
    }

    // przepisanie pozostałych wartości
    int iter_child = second_rand_index % (child.size() - 2);
    int iter_parent_b = second_rand_index % (parent_b.size() - 2);
    // int iter_parent_b = 0;

    // while (iter_parent_b < parent_b.size() - 1)
    while (iter_child != first_rand_index - 1)
    {
        // int temp = parent_b[iter_parent_b + 1];
        // jeśli wartość już jest to szukaj kolejnej
        while (is_in_vector[parent_b[iter_parent_b + 1]])
        {
            // iter_parent_b++;
            iter_parent_b = (iter_parent_b + 1) % (parent_b.size() - 2);
            // temp = parent_b[iter_parent_b];
        }
        // jeśli danej wartości jeszcze nie ma to przypisz i kontynuuj
        child[iter_child + 1] = parent_b[iter_parent_b + 1];
        iter_child = (iter_child + 1) % (child.size() - 2);
        iter_parent_b = (iter_parent_b + 1) % (parent_b.size() - 2);
        // iter_parent_b++;
    }
}

// przeprowadza mutację wybraną metodą oraz aktualizuje koszt ścieżki
void GeneticAlgorithm::mutation(MutationOperation &o, vector<int> &route)
{
    int first_rand_index = randomIndex();
    int second_rand_index = randomIndex();
    while (first_rand_index == second_rand_index) // wylosowane indeksy nie nogą być identyczne
        second_rand_index = randomIndex();

    switch (o)
    {
    case SwapOperation:
        swap(route[first_rand_index], route[second_rand_index]); // zamiana wartości pod danymi indeksami
        break;
    case InsertOperation: // przeniesienie wartości wskazywanej przez pierwszy indeks w miejsce wskazywane przez drugi indeks
        if (first_rand_index < second_rand_index)
            rotate(route.begin() + first_rand_index, route.begin() + first_rand_index + 1, route.begin() + second_rand_index + 1);
        else
            rotate(route.begin() + second_rand_index, route.begin() + first_rand_index, route.begin() + first_rand_index + 1);
        break;
    }
}

int GeneticAlgorithm::getRouteCost()
{
    return best.cost;
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
    for (auto i = 0; i < best.route.size() - 1; i++)
        cout << best.route[i] << " -> ";
    cout << best.route[best.route.size() - 1] << endl;
    cout << "Cost = " << best.cost << endl;
}
