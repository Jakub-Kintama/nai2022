#include <iostream>
#include <vector>
#include <random>
#include <list>
using namespace std;

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

auto simulated_annealing = [](auto f, auto min, auto max, auto iterations){
    uniform_real_distribution<double> dist(min, max);
    uniform_real_distribution<double> uk(0, 1);
    auto current_x = dist(rng);
    auto current_y = dist(rng);
    auto best_point = f(current_x, current_y);
    list<double> my_list = {best_point};
    double tk;
    for(int i=0; i<iterations; i++) {
        tk = f(current_x - i, current_y - i);
        if (best_point < tk){
            best_point = tk;
            my_list.insert(best_point);
        }else{
            if(uk < exp( - abs(best_point - tk) * log(i))){
                best_point = tk;
                my_list.insert(best_point);
            }
        }
    }
    return best_point;
};

int main() {
    auto camel_f = [](double x, double y) {return 2 * x * x - 1.05 * pow(x, 4) + pow(x, 6) / 6 + x * y + y * y;};
    auto himmelblau_f = [](double x, double y){return (x + 2 * y - 7) * (x + 2 * y - 7) + (2 * x + y - 5) * (2 * x + y - 5);};
    auto matyas_f = [](double x, double y){return 0.26 * (x * x + y * y) - 0.48 * x * y;};
    double best_camel, best_himmelblau, best_matyas = 0;
    int exp = 2048;
    clock_t start, end;

    start = clock();
    for(int i=0; i<exp; i++){
        best_camel += brute_force(camel_f, -5, 5, 1000);
        best_himmelblau += brute_force(himmelblau_f, -5, 5, 1000);
        best_matyas += brute_force(matyas_f, -10, 10, 1000);
    }
    end = clock();
    cout << "best camel " << best_camel/exp << "best himmelblau " << best_himmelblau/exp << "best matyas " << best_matyas/exp << " with time = " << double(end-start)/exp << endl;
    best_camel = 0;
    best_himmelblau = 0;
    best_matyas = 0;

    start = clock();
    for(int i=0; i<exp; i++) {
        best_camel += brute_force(camel_f, -5, 5, 1000);
        best_himmelblau += brute_force(himmelblau_f, -5, 5, 1000);
        best_matyas += brute_force(matyas_f, -10, 10, 1000);
    }
    end = clock();
    cout << "best camel " << best_camel/exp << "best himmelblau " << best_himmelblau/exp << "best matyas " << best_matyas/exp << " with time = " << double(end-start)/exp << endl;
    best_camel = 0;
    best_himmelblau = 0;
    best_matyas = 0;

    start = clock();
    for(int i=0; i<exp; i++) {
        best_camel += brute_force(camel_f, -5, 5, 1000);
        best_himmelblau += brute_force(himmelblau_f, -5, 5, 1000);
        best_matyas += brute_force(matyas_f, -10, 10, 1000);
    }
    end = clock();
    cout << "best camel " << best_camel/exp << "best himmelblau " << best_himmelblau/exp << "best matyas " << best_matyas/exp << " with time = " << double(end-start)/exp << endl;

    return 0;
}

/*
domain_t tabu_method(
        const std::function<double(domain_t)> &f, domain_t start_point,
        std::function<std::vector<domain_t>(domain_t)> get_close_points,
        int max_iterations) {
    using namespace std;
    domain_t best_point = start_point;
    list<domain_t> tabu_list = {start_point};
    for (int iteration = 0; iteration < max_iterations; iteration++) {
        cout << iteration << " " << tabu_list.back() << " " << f(tabu_list.back())
             << endl;
        for (auto tabu_i = tabu_list.rbegin(); tabu_i != tabu_list.rend();
             tabu_i++) {
            auto close_points_all = get_close_points(*tabu_i);
            vector<domain_t> close_points;
            copy_if(close_points_all.begin(), close_points_all.end(),
                    back_inserter(close_points), [&](auto p) {
                        return !count(tabu_list.begin(), tabu_list.end(), p);
                    });
            if (close_points.size() != 0) {
                tabu_list.push_back(
                        *min_element(close_points.begin(), close_points.end(),
                                     [f](auto a, auto b) { return f(a) < f(b); }));
                break;
            }
        }
        if (f(best_point) > f(tabu_list.back())) best_point = tabu_list.back();
    }
    return best_point;
}
*/