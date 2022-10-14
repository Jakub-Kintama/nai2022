#include <iostream>
#include <vector>
#include <random>
using namespace std;

random_device rd;
mt19937 rng(rd());

auto brute_force = [](auto f, auto min, auto max, auto iterations) {
    uniform_real_distribution<double> dist(min, max);
    auto current_x = dist(rng);
    auto current_y = dist(rng);
    auto best_point = f(current_x, current_y);
    try {
        for(int i=0; i<iterations; i++) {
            if (f(current_x, current_y) < best_point) {
                best_point = f(current_x, current_y);
            }
            current_x = dist(rng);
            current_y = dist(rng);
        }
    } catch (exception &e) {
    }
    return best_point;
};

int main() {
    auto sphere_f = [](double x, double y) {return x*x + y*y;};
    auto booth_f = [](double x, double y){return (x + 2 * y - 7) * (x + 2 * y - 7) + (2 * x + y - 5) * (2 * x + y - 5);};
    auto matyas_f = [](double x, double y){return 0.26 * (x * x + y * y) - 0.48 * x * y;};
    auto best_sphere = brute_force(sphere_f, -10, 10, 1000);
    auto best_booth = brute_force(booth_f, -10, 10, 1000);
    auto best_matyas = brute_force(matyas_f, -10, 10, 1000);
    cout << "best sphere = " << best_sphere << "\nbest booth = " << best_booth << "\nbest matyas = " << best_matyas << endl;
    return 0;
}


/*
 auto brute_force = [](auto f, auto domain) {
    auto current_p = domain();
    auto best_point = current_p;
    try {
        while (true) {
            if (f(current_p) < f(best_point)) {
                best_point = current_p;
            }
            current_p = domain();
        }
    } catch (std::exception &e) {
    }
    return best_point;
};
using domain_t = std::vector<double>;
std::random_device rd;
std::mt19937 mt_generator(rd());
domain_t hill_climbing(const std::function<double(domain_t)> &f, domain_t minimal_d, domain_t maximal_d, int max_iterations) {
    domain_t current_p(minimal_d.size());
    for (int i = 0; i < minimal_d.size(); i++) {
        std::uniform_real_distribution<double> dist(minimal_d[i], maximal_d[i]);
        current_p[i] = dist(mt_generator);
    }
    for (int iteration = 0; iteration < max_iterations; iteration++) {
        domain_t new_p(minimal_d.size());
        for (int i = 0; i < minimal_d.size(); i++) {
            std::uniform_real_distribution<double> dist(-1.0/128.0, 1.0/128.0);
            new_p[i] = current_p[i] + dist(mt_generator);
        }
        if (f(current_p) > f(new_p)) {
            current_p = new_p;
        }
    }
    return current_p;
}
int main() {
    auto sphere_f = [](double x) {return x*x;};
    double current_sphere_x = -10;
    auto sphere_generator = [&]() {
        current_sphere_x+= 1.0/128.0;
        if (current_sphere_x >= 10) throw std::invalid_argument("finished");
        return current_sphere_x;
    };
    auto best_point = brute_force(sphere_f, sphere_generator);
    std::cout << "best x = " << best_point << std::endl;
    auto sphere_f_v = [](domain_t x) {return x[0]*x[0];};
    auto best2 = hill_climbing(sphere_f_v, {-10},{10},10000);
    std::cout << "best x = " << best2[0] << std::endl;
    return 0;
}
 */