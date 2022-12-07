#define _USE_MATH_DEFINES
#include <iostream>
#include <vector>
#include <functional>
#include <random>
#include <cmath>
#include <algorithm>
#include <string>

using namespace std;

random_device rd;
mt19937 mt_generator(rd());

using chromosome_t = vector<bool>;
using population_t = vector<chromosome_t>;
using fitness_f = function<double(const chromosome_t &)>;
using term_condition_f = function<bool(const population_t &, const vector<double> &)>;
using selection_f = function<int(const vector<double> &)>;
using crossover_f = function<vector<chromosome_t>(const vector<chromosome_t> &)>;
using mutation_f = function<chromosome_t(const chromosome_t, double)>;

population_t genetic_algorithm(population_t initial_population,
                               fitness_f fitness,
                               term_condition_f term_condition,
                               selection_f selection, double p_crossover,
                               crossover_f crossover, double p_mutation,
                               mutation_f mutation) {
    uniform_real_distribution<double> uniform(0.0, 1.0);
    auto population = initial_population;
    vector<double> population_fit(population.size());
    transform(population.begin(), population.end(), population_fit.begin(),fitness);
    while (!term_condition(population, population_fit)) {
        vector<int> parents_indexes(population.size());
        population_t new_population(population.size());
        // calculate fitness
        transform(population_fit.begin(), population_fit.end(),
                  parents_indexes.begin(),
                  [&](auto e) { return selection(population_fit); });
        // perform crossover
        for (int i = 0; i < parents_indexes.size() - 1; i += 2) {
            vector<chromosome_t> child = {population[parents_indexes[i]], population[parents_indexes[i + 1]]};
            if (uniform(mt_generator) < p_crossover) {
                child = crossover(child);
            }
            new_population[i] = child[0];
            new_population[i + 1] = child[1];
        }
        // perform mutation
        for (auto &chromosome : new_population) {
            chromosome = mutation(chromosome, p_mutation);
        }
        population = new_population;
        transform(population.begin(), population.end(), population_fit.begin(), fitness);
    }
    return population;
};

vector<chromosome_t> crossover_two_point(vector<chromosome_t> parents) {
    uniform_int_distribution<int> locus(0,parents.at(0).size()-1);
    int a = locus(mt_generator);
    int b = locus(mt_generator);
    if (a > b) swap(a,b);
    auto children = parents;
    for (int i = a; i < b; i++) {
        swap(children[0][i],children[1][i]);
    }
    return children;
}

int selection_tournament_2(vector<double> fitnesses) {
    uniform_int_distribution<int> uniform(0, fitnesses.size()-1);
    int a = uniform(mt_generator);
    int b = uniform(mt_generator);
    return (fitnesses[a]>fitnesses[b])?a:b;
}

chromosome_t mutation_one_point(const chromosome_t parent, double p_mutation) {
    uniform_real_distribution<double> uni(0.0,1.0);
    if (uni(mt_generator) <= p_mutation) {
        uniform_int_distribution<int> locus(0,parent.size()-1);
        chromosome_t child = parent;
        int l = locus(mt_generator);
        child[l] = child[l] == 0;
        return child;
    }else
        return parent;
}

double ackley_f(double x, double y) {
    return -20 * exp(-0.2 * sqrt(0.5 * x * x + y * y)) - exp(0.5 * (cos(2 * M_PI * x) + cos(2 * M_PI * y))) + exp(1) + 20;
};

pair<double, double> decode(chromosome_t chromosome) {
    double x = 0;
    double y = 0;
    for (int i = 0; i < chromosome.size() / 2; i++)
    {
        x += chromosome[i];
        x /= 10;
    }
    for (int i = chromosome.size() / 2; i < chromosome.size(); i++)
    {
        y += chromosome[i];
        y /= 10;
    }
    x -= 0.5; //cap max value to 5.0 => 110 / 2 = 55, 55 / 10 = 5.5, 5.5 - 0.5 = 5.0
    y -= 0.5;
    return {x, y};
}

auto get_fitness_f = [](chromosome_t chromosome){
    pair<double, double> pair = decode(chromosome);
    return 1.0 / (1.0 + abs(ackley_f(pair.first, pair.second)));
};

chromosome_t generate_chromosome(int chromosome_size){
    uniform_real_distribution<double> dist(0, 10);
    chromosome_t chromosome;
    int temp;
    for(int i=0; i<chromosome_size; i++){
        if(dist(mt_generator) < 5){temp = 0;}else{temp = 1;}
        chromosome.push_back(temp);
    }
    return chromosome;
}

population_t generate_population(int population_size, int chromosome_size){
    population_t population;
    for(int i=0; i<population_size; i++){
        population.push_back(generate_chromosome(chromosome_size));
    }
    return population;
}

void print_population(population_t population, fitness_f fitness){
    cout << endl;
    for (chromosome_t chromosome: population) {
        cout << "[";
        for (int p: chromosome) {
            cout << p;
        }
        cout << "]";
        if(fitness){
            cout << fitness(chromosome);
        }
        cout << endl;
    }
}

double standard_deviation_f(const vector<double>& fitnesses) {
    double avg = 0.0, deviation = 0.0;
    for(double fitness : fitnesses){
        avg += fitness;
    }
    avg = avg / fitnesses.size();
    for(double fitness : fitnesses){
        deviation += pow((fitness - avg), 2);
    }
    return sqrt(deviation/fitnesses.size());
}

void print_population_stats(population_t population, fitness_f fitness){
    double min = 100, max = 0, avg = 0, temp;
    pair <double, double> minp, maxp;
    for (chromosome_t chromosome: population){
        temp = fitness(chromosome);
        avg += temp;
        if (temp > max){
            max = temp;
            maxp = decode(chromosome);
        }
        if(temp < min){
            min = temp;
            minp = decode(chromosome);
        }
    }
    avg = avg / population.size();
    cout << "min " << min << " for x=" << minp.first << " y=" << minp.second << ", max: " << max << " for x=" << maxp.first << " y=" << maxp.second << ", avg: " << avg << endl;
}

int main(int argc, char *argv[]) {
    int population_size=stoi(argv[1]), iterations=stoi(argv[2]), print_switch=stoi(argv[5]);
    double p_crossover=stod(argv[3]), p_mutation=stod(argv[4]), standard_deviation=stod(argv[6]);

    /*
    cout << "Population size (even):";
    cin >> population_size;
    cout << "Iterations:";
    cin >> iterations;
    cout << "Crossover probability:";
    cin >> p_crossover;
    cout << "Mutation probability:";
    cin >> p_mutation;
    cout << "Print (1/0):";
    cin >> print_switch;
    cout << "Standard deviation (0 for none):";
    cin >> standard_deviation;
    */

    population_t population = generate_population(population_size, 110); // 100 + (22835 % 10) * 2) = 110
    print_population(population, get_fitness_f);
    population_t result = genetic_algorithm(population,
                                                 get_fitness_f,
                                                 [iterations, standard_deviation](auto a, auto b) {
                                                     static int i = 0;
                                                     if(standard_deviation_f(b) < standard_deviation){
                                                         cout << "Standard deviation limit reached after: " << i << " iterations";
                                                         return true;
                                                     }
                                                     i++;
                                                     return i > iterations;
                                                 },
                                                 selection_tournament_2,
                                                 p_crossover, crossover_two_point,
                                                 p_mutation, mutation_one_point);
    print_population(result, get_fitness_f);
    if(print_switch){
        print_population_stats(population, get_fitness_f);
        print_population_stats(result, get_fitness_f);
    }

    return 0;
}