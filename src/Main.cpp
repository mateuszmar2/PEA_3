#include "Towns.h"
#include "Genetic_Algorithm.h"
#include "TSP_Algorithm.h"

#include <iostream>
#include <chrono>

using namespace std;

void menuGA()
{
    Towns towns;
    char filename[50];
    int stop_time = 60;
    int data_read_count = 0;
    int action;
    int value;
    do
    {
        cout << endl
             << "Which action you want to perform? Type appropriate number " << endl;
        cout << "1 - Print data " << endl;
        cout << "2 - Load data from file " << endl;
        cout << "3 - Modify stop time, current = " << stop_time << endl;
        cout << "4 - Modify stop time, current = " << stop_time << endl;
        cout << "5 - Modify stop time, current = " << stop_time << endl;
        cout << "6 - Modify stop time, current = " << stop_time << endl;
        cout << "7 - Modify stop time, current = " << stop_time << endl;
        cout << "8 - Run Genetic Algorithm " << endl;
        cout << "9 - Exit the program " << endl;
        cout << "GA> ";
        cin >> action;
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << endl;
        switch (action)
        {
        case 1: // wyświetlenie danych
            if (towns.getTowns().empty())
            {
                cout << "Load data first" << endl;
                break;
            }
            towns.printData();
            break;
        case 2: // wczytanie danych
            cout << "Enter file name: ";
            cin >> filename;
            cout << endl;
            towns.loadDataFromFile(filename);
            if (!towns.getTowns().empty() && data_read_count == 0)
            {
                data_read_count++;
                int number_of_towns = towns.getTowns()[0].size();
            }
            break;
        case 3: // kryterium stopu
            cout << "Enter new stop time: ";
            cin >> value;
            if (value <= 0 || cin.fail())
            {
                cout << "Invalid stop time" << endl;
                break;
            }
            stop_time = value;
            break;
        case 4: // wielkość populacji początkowej
            break;
        case 5: // współczynnik mutacji
            break;
        case 6: // współczynnik krzyżowania
            break;
        case 7: // współczynnik mutacji
            break;
        case 8: // algorytm
        {
            if (towns.getTowns().empty())
            {
                cout << "Load data first" << endl;
                break;
            }
            std::chrono::steady_clock::time_point start =
                std::chrono::steady_clock::now();
            // GeneticAlgoritm ga(towns.getTowns(), operation, max_it_without_change, stop_time, tabu_lifetime, diversification);
            // ga.startGA();
            std::chrono::steady_clock::time_point end =
                std::chrono::steady_clock::now();
            // ga.printRoute();
            std::cout << "Execution time: "
                      << std::chrono::duration_cast<std::chrono::microseconds>(
                             end - start)
                             .count()
                      << "us\n";
            if (towns.getOptimalResult() != 0)
                // cout << "Relative error: " << float(abs(ga.getRouteCost() - towns.getOptimalResult())) / towns.getOptimalResult() * 100 << "%" << endl;
                break;
        }
        case 9: // wyjście
            break;
        default:
            cout << "Type appropriate number" << endl;
            break;
        }
    } while (action != 9);
}

int main()
{
    menuGA();
    return 0;
}
