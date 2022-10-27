/*
#include <iostream>
#include <random>
#include <string>
#include <list>
#include <cmath>
using namespace std;
#define M_PI 3.14159265358979323846

random_device rd;
mt19937 rng(rd());

auto brute_force = [](auto f, auto min, auto max, auto iterations) {
    uniform_real_distribution<double> dist(min, max);
    auto current_x = dist(rng);
    auto current_y = dist(rng);
    auto best_point = f(current_x, current_y);
    for(int i=0; i<iterations; i++) {
        if (f(current_x, current_y) < best_point) {
            best_point = f(current_x, current_y);
        }
        current_x = dist(rng);
        current_y = dist(rng);
    }
    return best_point;
};

auto true_brute_force = [](auto f, auto min, auto max, auto precision) {
    auto current_x = min;
    auto current_y = min;
    auto best_point = f(current_x, current_y);
    while (current_x < max){
        while (current_y < max){
            if(f(current_x, current_y) < best_point){best_point = f(current_x, current_y);}
            current_y += precision;
        }
        current_x += precision;
    }
    return best_point;
};

auto simulated_annealing = [](auto f, auto min, auto max, auto iterations){
    uniform_real_distribution<double> dist(min, max);
    uniform_real_distribution<double> uk(0, 1);
    auto current_x = dist(rng);
    auto current_y = dist(rng);
    auto best_point = f(current_x, current_y);
    list<double> my_list = {best_point};
    double tk;
    for(int i=0; i < iterations; i++) {
        tk = f(dist(rng), dist(rng));
        if (tk < best_point){
            best_point = tk;
            my_list.push_back(best_point);
        }else{
            if(uk(rng) < exp( - abs(best_point - tk) * log(i))){
                best_point = tk;
                my_list.push_back(best_point);
            }
        }
    }
    for (auto point : my_list) {
        if(point < best_point){best_point = point;}
    }
    return best_point;
};

auto hill_climb = [](auto f, auto min, auto max, auto iterations) {
    uniform_real_distribution<double> dist(min, max);
    auto current_x = dist(rng);
    auto current_y = dist(rng);
    auto best_point = f(current_x, current_y);
    double new_point;
    for(int i=0; i<iterations; i++) {
        current_x = dist(rng);
        current_y = dist(rng);
        new_point = f(current_x, current_y);
        for (int j=0; j < max; j++) {
            uniform_real_distribution<double> dist_2(-1/128,1/128);
            current_x += dist_2(rng);
            current_y += dist_2(rng);
            if(f(current_x, current_y) < new_point){
                new_point = f(current_x, current_y);
            } else{
                break;
            }
        }

        if(new_point < best_point){
            best_point = new_point;
        }
    }
    return best_point;
};

int main() {
    auto ackley_f = [](double x, double y) {return -20 * exp(-0.2 * sqrt(0.5 * x * x + y * y)) - exp(0.5 * (cos(2 * M_PI * x) + cos(2 * M_PI * y))) + exp(1) + 20;};
    auto himmelblau_f = [](double x, double y){return (x + 2 * y - 7) * (x + 2 * y - 7) + (2 * x + y - 5) * (2 * x + y - 5);};
    auto eggholder_f = [](double x, double y){return -(y + 47) * sin(sqrt(abs(x/2 + y + 47))) - x * sin(sqrt(abs(x - (y + 47))));};
    double best_ackley = 0;
    double best_himmelblau = 0;
    double best_eggholder = 0;
    int exp = 2048;
    clock_t start, end;

    start = clock();
    for(int i=0; i<exp; i++){
        best_ackley += brute_force(ackley_f, -5, 5, 1000);
        best_himmelblau += brute_force(himmelblau_f, -5, 5, 1000);
        best_eggholder += brute_force(eggholder_f, -512, 512, 1000);
    }
    end = clock();
    string text = "ackley " + to_string(best_ackley/exp) + ", himmelblau " + to_string(best_himmelblau/exp) + ", matyas " + to_string(best_eggholder/exp) + ", for brute force with time = " + to_string(double(end-start)/exp) + "\n";
    best_ackley = 0;
    best_himmelblau = 0;
    best_eggholder = 0;

    start = clock();
    for(int i=0; i<exp; i++) {
        best_ackley += true_brute_force(ackley_f, -5, 5, 1);
        best_himmelblau += true_brute_force(himmelblau_f, -5, 5, 1);
        best_eggholder += true_brute_force(eggholder_f, -512, 512, 1);
    }
    end = clock();
    text += "ackley " + to_string(best_ackley/exp) + ", himmelblau " + to_string(best_himmelblau/exp) + ", matyas " + to_string(best_eggholder/exp) + ", for real brute force with time = " + to_string(double(end-start)/exp) + "\n";
    best_ackley = 0;
    best_himmelblau = 0;
    best_eggholder = 0;

    start = clock();
    for(int i=0; i<exp; i++) {
        best_ackley += simulated_annealing(ackley_f, -5, 5, 1000);
        best_himmelblau += simulated_annealing(himmelblau_f, -5, 5, 1000);
        best_eggholder += simulated_annealing(eggholder_f, -512, 512, 1000);
    }
    end = clock();
    text += "ackley " + to_string(best_ackley/exp) + ", himmelblau " + to_string(best_himmelblau/exp) + ", matyas " + to_string(best_eggholder/exp) + ", for simulated annealing with time = " + to_string(double(end-start)/exp) + "\n";
    best_ackley = 0;
    best_himmelblau = 0;
    best_eggholder = 0;

    start = clock();
    for(int i=0; i<exp; i++) {
        best_ackley += hill_climb(ackley_f, -5, 5, 1000);
        best_himmelblau += hill_climb(himmelblau_f, -5, 5, 1000);
        best_eggholder += hill_climb(eggholder_f, -512, 512, 1000);
    }
    end = clock();
    text += "ackley " + to_string(best_ackley/exp) + ", himmelblau " + to_string(best_himmelblau/exp) + ", matyas " + to_string(best_eggholder/exp) + ", for hill climb with time = " + to_string(double(end-start)/exp) + "\n";
    cout << text;

    return 0;
}
 */