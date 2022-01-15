#include "Towns.h"
#include "Genetic_Algorithm.h"

#include <iostream>
#include <chrono>

using namespace std;

void menuGA()
{
    Towns towns;
    int number_of_towns;
    GeneticAlgorithm::MutationOperation mutation_operation = GeneticAlgorithm::SwapOperation;
    GeneticAlgorithm::CrossoverOperation crossover_operation = GeneticAlgorithm::OrderCrossover;
    char filename[50];
    int stop_time = 1;          // 60
    int population_size = 1000; // 1000
    double mutation_coefficient = 0.01;
    double crossover_coefficient = 0.8;
    double temp;
    int action;
    int value;
    do
    {
        cout << endl
             << "Which action you want to perform? Type appropriate number " << endl;
        cout << "1 - Run Genetic Algorithm " << endl;
        cout << "2 - Print data " << endl;
        cout << "3 - Load data from file " << endl;
        cout << "4 - Modify stop time, current = " << stop_time << endl;
        cout << "5 - Modify population size, current = " << population_size << endl;
        cout << "6 - Modify mutation coefficient, current = " << mutation_coefficient << endl;
        cout << "7 - Modify crossover coefficient, current = " << crossover_coefficient << endl;
        cout << "8 - Modify mutation method, current = ";
        switch (mutation_operation)
        {
        case 1:
            cout << "Swap Operation" << endl;
            break;
        case 2:
            cout << "Insert Operation" << endl;
            break;
        }
        cout << "9 - Modify crossover method, current = ";
        switch (crossover_operation)
        {
        case 1:
            cout << "Order crossover" << endl;
            break;
        case 2:
            cout << "Partially matched crossover" << endl;
            break;
        }
        cout << "0 - Exit the program " << endl;
        cout << "GA> ";
        cin >> action;
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << endl;
        switch (action)
        {
        case 1: // algorytm
        {
            if (towns.getTowns().empty())
            {
                cout << "Load data first" << endl;
                break;
            }
            std::chrono::steady_clock::time_point start =
                std::chrono::steady_clock::now();
            GeneticAlgorithm ga(
                towns.getTowns(),
                population_size,
                stop_time,
                mutation_operation,
                crossover_operation,
                mutation_coefficient,
                crossover_coefficient);
            ga.startGA();
            std::chrono::steady_clock::time_point end =
                std::chrono::steady_clock::now();
            ga.printRoute();
            std::cout << "Execution time: "
                      << std::chrono::duration_cast<std::chrono::microseconds>(
                             end - start)
                             .count()
                      << "us\n";
            if (towns.getOptimalResult() != 0)
                cout << "Relative error: "
                     << float(abs(ga.getRouteCost() - towns.getOptimalResult())) / towns.getOptimalResult() * 100
                     << "%" << endl;
            break;
        }
        case 2: // wyświetlenie danych
            if (towns.getTowns().empty())
            {
                cout << "Load data first" << endl;
                break;
            }
            towns.printData();
            break;
        case 3: // wczytanie danych
            cout << "Enter file name: ";
            cin >> filename;
            cout << endl;
            towns.loadDataFromFile(filename);
            number_of_towns = towns.getTowns()[0].size();
            break;
        case 4: // kryterium stopu
            cout << "Enter new stop time: ";
            cin >> value;
            if (value <= 0 || cin.fail())
            {
                cout << "Invalid stop time" << endl;
                break;
            }
            stop_time = value;
            break;
        case 5: // wielkość populacji początkowej
            cout << "Enter new population size: ";
            cin >> value;
            if (value <= 0 || cin.fail())
            {
                cout << "Invalid population size" << endl;
                break;
            }
            population_size = value;
            break;
        case 6: // współczynnik mutacji
            cout << "Enter new mutation coefficient: ";
            cin >> temp;
            if (temp <= 0 || cin.fail() || temp >= 1)
            {
                cout << "Invalid mutation coefficient" << endl;
                break;
            }
            mutation_coefficient = temp;
            break;
        case 7: // współczynnik krzyżowania
            cout << "Enter new crossover coefficient: ";
            cin >> temp;
            if (temp <= 0 || cin.fail() || temp >= 1)
            {
                cout << "Invalid crossover coefficient" << endl;
                break;
            }
            crossover_coefficient = temp;
            break;
        case 8: // metoda mutacji
            cout << "1 - Swap Operation" << endl;
            cout << "2 - Insert Operation" << endl;
            cout << "Enter new mutation operation: ";
            cin >> value;
            if (value <= 0 || cin.fail() || value >= 3)
            {
                cout << "Invalid mutation operation" << endl;
                break;
            }
            switch (value)
            {
            case 1:
                mutation_operation = GeneticAlgorithm::SwapOperation;
                break;
            case 2:
                mutation_operation = GeneticAlgorithm::InsertOperation;
                break;
            }
            break;
        case 9: // metoda krzyżowania
            cout << "1 - Order crossover" << endl;
            cout << "2 - Partially matched crossover" << endl;
            cout << "Enter new crossover operation: ";
            cin >> value;
            if (value <= 0 || cin.fail() || value >= 3)
            {
                cout << "Invalid crossover operation" << endl;
                break;
            }
            switch (value)
            {
            case 1:
                crossover_operation = GeneticAlgorithm::OrderCrossover;
                break;
            case 2:
                crossover_operation = GeneticAlgorithm::PartiallyMatchedCrossover;
                break;
            }
            break;
        case 0: // wyjście
            break;
        default:
            cout << "Type appropriate number" << endl;
            break;
        }
    } while (action != 0);
}

int main()
{
    menuGA();
    return 0;
}
