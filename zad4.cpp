#define _USE_MATH_DEFINES
#include <iostream>
#include <vector>
#include <functional>
#include <random>
#include <cmath>
using namespace std;

random_device rd;
mt19937 mt_generator(rd());

auto genetic_algorithm = [](
        auto initial_population, auto fitness, auto term_condition,
        auto selection, double p_crossover,
        auto crossover, double p_mutation, auto mutation) {
    using namespace std;
    uniform_real_distribution<double> uniform(0.0, 1.0);
    auto population = initial_population;
    vector<double> population_fit = fitness(population);
    while (!term_condition(population, population_fit)) {
        auto parents_indexes = selection(population_fit);
        decltype(population) new_population;
        for (int i = 0; i < parents_indexes.size(); i += 2) {
            decltype(initial_population) offspring = {population[i], population[i + 1]};
            if (uniform(mt_generator) < p_crossover) {
                offspring = crossover(offspring);
            }
            for (auto chromosome: offspring) new_population.push_back(chromosome);
        }
        for (auto &chromosome: new_population) {
            chromosome = mutation(chromosome, p_mutation);
        }
        population = new_population;
        population_fit = fitness(population);
    }
    return population;
};
using chromosome_t = std::vector<int>;
using population_t = std::vector<chromosome_t>;

std::vector<double> fintess_function(population_t pop) {
    return {};
}

std::vector<int> selection_empty(std::vector<double> fitnesses) {
    return {};
}

std::vector<chromosome_t> crossover_empty(std::vector<chromosome_t> parents) {
    return parents;
}

chromosome_t mutation_empty(chromosome_t parents, double p_mutation) {
    return parents;
}

double ackley_f(double x, double y) {
    return -20 * exp(-0.2 * sqrt(0.5 * x * x + y * y)) - exp(0.5 * (cos(2 * M_PI * x) + cos(2 * M_PI * y))) + exp(1) + 20;
};

pair<double, double> decode(vector<bool> genotype) {
    double x = 0.0;
    double y = 0.0;
    for (int i = 0; i < genotype.chromosome.size() / 2; i++)
    {
        x = x * 2 + genotype.chromosome[i];
    }
    for (int i = genotype.chromosome.size() / 2; i < genotype.chromosome.size(); i++)
    {
        y = y * 2 + genotype.chromosome[i];
    }

    x = x / pow(2.0, (genotype.chromosome.size() / 2 - 4)) - 8;
    y = y / pow(2.0, (genotype.chromosome.size() / 2 - 4)) - 8;
    return {x, y};
}

int main() {
    using namespace std;
    population_t population = {{1, 0, 1, 0, 1, 0, 1},
                               {1, 0, 1, 0, 1, 0, 1}};
    auto result = genetic_algorithm(population,
                                    fintess_function,
                                    [](auto a, auto b) { return true; },
                                    selection_empty, 1.0,
                                    crossover_empty,
                                    0.01, mutation_empty);
    for (chromosome_t chromosome: result) {
        cout << "[";
        for (int p: chromosome) {
            cout << p;
        }
        cout << "] ";
    }
    cout << endl;
    return 0;
}