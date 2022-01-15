#include <iostream>
#include <algorithm>
#include <chrono>
#include <ctime>
#include <math.h>  /* pow */

#include "Genetic_Algorithm.h"

using namespace std;

GeneticAlgorithm::GeneticAlgorithm(
    vector<vector<int>> towns,
    int population_size,
    int stop_time,
    MutationOperation mutation_operation,
    CrossoverOperation crossover_operation,
    double mutation_coefficient,
    double crossover_coefficient)
{
    matrix = towns;
    number_of_towns = matrix[0].size();
    this->population_size = population_size;
    this->stop_time = stop_time;
    this->mutation_operation = mutation_operation;
    this->crossover_operation = crossover_operation;
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
        population[i].route = randomRoute();

    vector<PopulationElement> next_population(population);

    while (true)
    {
        // jeżeli przekroczono dozwolony czas to przerwij
        int64_t time_diff = chrono::duration_cast<chrono::seconds>(chrono::system_clock::now() - start_time).count();
        if (time_diff >= stop_time)
            break;

        double max_fitness = 0.0;
        // pętla obliczająca fitess oraz koszt i znajdujące najlepsze te wartości
        for (int i = 0; i < population_size; i++)
        {
            population[i].cost = pathDistance(population[i].route);
            population[i].fitness = pow((double)(1.0 / population[i].cost), 10);

            if (population[i].fitness > max_fitness)
                max_fitness = population[i].fitness;

            // zapisanie najlepszej ścieżki z populacji
            if (population[i].cost < best.cost)
            {
                best.route = population[i].route;
                best.cost = population[i].cost;
            }
        }
        for (int i = 0; i < population_size; i++)
        {
            PopulationElement parent_a = selectElement(population, max_fitness);
            PopulationElement parent_b = selectElement(population, max_fitness);

            if ((double)rand() / RAND_MAX <= crossover_coefficient)
            {
                crossover(parent_a.route, parent_b.route, next_population[i].route, crossover_operation);
            }
            else
                next_population[i] = parent_a;

            if ((double)rand() / RAND_MAX <= mutation_coefficient)
                mutation(mutation_operation, next_population[i].route);
        }
        population.swap(next_population);
    }
}

// wybór ruletkowy poprzez akceptację stochastyczną, zamiast przeszukiwania
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

void GeneticAlgorithm::crossover(
    vector<int> &parent_a,
    vector<int> &parent_b,
    vector<int> &offspring,
    CrossoverOperation &o)
{
    int first_rand_index = randomIndex();
    int second_rand_index = randomIndex();
    // wylosowane indeksy nie nogą być identyczne
    while (first_rand_index == second_rand_index)
        second_rand_index = randomIndex();

    // pierwszy indeks musi być mniejszy
    if (first_rand_index > second_rand_index)
        swap(first_rand_index, second_rand_index);

    // przepisanie segmentu od rodzica
    vector<bool> is_in_offspring(number_of_towns, false);
    for (int i = first_rand_index; i <= second_rand_index; i++)
    {
        offspring[i] = parent_a[i];
        is_in_offspring[parent_a[i]] = true;
    }
    // zależnie od wybranej metody krzyżowania
    switch (o)
    {
    // order crossover
    case 1:
    {
        int iter_offspring = second_rand_index % (offspring.size() - 2);
        int iter_parent_b = second_rand_index % (parent_b.size() - 2);
        // w tym przypadku is_in_offspring przechowuje tylko dane z przypisania pierwszego bloku danych
        // zmienne przechowujące indeks używany w danym vectorze
        // dopóki pozostałe wartości nie zostaną przypisane
        while (iter_offspring != first_rand_index - 1)
        {
            // jeśli wartość już jest to szukaj na kolejnej pozycji
            while (is_in_offspring[parent_b[iter_parent_b + 1]])
                iter_parent_b = (iter_parent_b + 1) % (parent_b.size() - 2);

            // jeśli danej wartości jeszcze nie ma to przypisz i kontynuuj
            offspring[iter_offspring + 1] = parent_b[iter_parent_b + 1];
            iter_offspring = (iter_offspring + 1) % (offspring.size() - 2);
            iter_parent_b = (iter_parent_b + 1) % (parent_b.size() - 2);
        }
        break;
    }
    // partially matched crossover
    case 2:
        for (int i = 1; i < offspring.size() - 1; i++)
        {
            // wartości pod tymi indeksami są już przepisane
            if (i >= first_rand_index && i <= second_rand_index)
                continue;

            // jeżeli dana wartość nie została jeszcze wpisana
            if (!is_in_offspring[parent_b[i]])
            {
                offspring[i] = parent_b[i];
                is_in_offspring[parent_b[i]] = true;
            }
            // jeśli została już przypisana to to miejsce zostaje oznaczone jako do przypisania
            else
                offspring[i] = -1;
        }
        for (int i = 1; i < offspring.size() - 1; i++)
        {
            if (offspring[i] != -1)
                continue;

            int temp = parent_b[i];
            // dopóki nie zostanie zostanie znaleziona wartość, której jeszcze nie ma
            while (is_in_offspring[temp])
            {
                // znajdz wartość temp w parent_a
                vector<int>::iterator it = find(parent_a.begin(), parent_a.end(), temp);
                // wpisz do temp wartość z parent_b pod indeksem wskazywanym przez temp w parent_a
                temp = parent_b[it - parent_a.begin()];
            }
            offspring[i] = temp;
            is_in_offspring[temp] = true;
        }
        break;
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
int GeneticAlgorithm::pathDistance(vector<int> &route_to_calculate)
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
