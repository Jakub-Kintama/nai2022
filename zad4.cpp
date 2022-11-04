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

auto get_crossover_one_point = [](auto pop, double p_crossover = 0.9) {
    auto crossover_one_point = [p_crossover](decltype(pop) population) {
        decltype(pop) ret_pop;
        std::uniform_real_distribution<double> r_pcross(0.0, 1.0);
        for (int i = 0; i < (population.size() - 1); i += 2)
        {
            auto a = population.at(i);
            auto b = population.at(i + 1);

            if (r_pcross(mt_generator) < p_crossover)
            {
                std::uniform_int_distribution<int> dist(0, a.chromosome.size() - 1);
                auto crossover_p = dist(mt_generator);
                for (int g = crossover_p; g < a.chromosome.size(); g++)
                {
                    std::swap(a.chromosome[g], b.chromosome[g]);
                }
            }
            ret_pop.push_back(a);
            ret_pop.push_back(b);
        }
        return ret_pop;
    };
    return crossover_one_point;
};

auto get_selection_roulette = [](auto pop) {
    auto selection_roulette = [](auto population) {
        using namespace std;
        decltype(population) selected_specimens;
        double sum_fitness = accumulate(population.begin(), population.end(), 0.0, [](auto a, auto b) { return a + b.fit; });

        uniform_real_distribution<double> dist(0.0, sum_fitness);
        for (int c = 0; c < population.size(); c++)
        {
            double r = dist(mt_generator);
            double s = 0.0;
            for (unsigned int i = 0; i < population.size(); i++)
            {
                s += population[i].fit;
                if (r < s)
                {
                    selected_specimens.push_back(population.at(i));
                    break;
                }
            }
        }
        return selected_specimens;
    };
    return selection_roulette;
};

auto get_mutation_probabilitic = [](auto pop, double p_mutation = 0.1) {
    auto mutation_probabilitic = [p_mutation](decltype(pop) population) {
        decltype(pop) ret_pop;
        std::uniform_real_distribution<double> r_mut(0.0, 1.0);
        for (int i = 0; i < population.size(); i++)
        {
            auto a = population.at(i);
            for (int l = 0; l < a.chromosome.size(); l++)
            {
                if (r_mut(mt_generator) < p_mutation)
                {
                    a.chromosome[l] = 1 - a.chromosome[l];
                }
            }
            ret_pop.push_back(a);
        }
        return ret_pop;
    };
    return mutation_probabilitic;
};

double ackley_f(double x, double y) {
    return -20 * exp(-0.2 * sqrt(0.5 * x * x + y * y)) - exp(0.5 * (cos(2 * M_PI * x) + cos(2 * M_PI * y))) + exp(1) + 20;
};

struct genotype{
    vector<bool> chromosome;
};

pair<double, double> decode(genotype gen) {
    double x = 0;
    double y = 0;
    for (int i = 0; i < gen.chromosome.size() / 2; i++)
    {
        x = x * 2 + gen.chromosome[i];
    }
    for (int i = gen.chromosome.size() / 2; i < gen.chromosome.size(); i++)
    {
        y = y * 2 + gen.chromosome[i];
    }

    x = x / pow(2.0, (gen.chromosome.size() / 2 - 4)) - 8;
    y = y / pow(2.0, (gen.chromosome.size() / 2 - 4)) - 8;
    return {x, y};
}

double fitness(genotype gen)
{
    auto [x, y] = decode(gen);
    return 1.0 / (1.0 + std::abs(ackley_f(x, y)));
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