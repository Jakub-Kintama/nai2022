#include <iostream>
#include <map>
#include <string>
#include <functional>
#include <cmath>
using namespace std;
using calculate = function<double(vector<double>)>;
using calculations = map<string, calculate>;

void print(calculate calc, vector<double> vals){
    cout << "wynik: " << calc(vals) << endl;
}

int main(int argc, char **argv){
    calculations["sin"] = [](vector<double> x) {return sin(x[2]);};
    calculations["add"] = [](vector<double> x) {return x[2] + x[3];};
    calculations["mod"] = [](vector<double> x) {return (int)x[2] % (int)x[3];};
    try
    {
        vector<string> arguments(argv, argv + argc);
        auto option = arguments.at(1);
        print(option, arguments);
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
    }
    return 0;
}

/*
#include <any>
#include <functional>
#include <iostream>
#include <map>
#include <string>
#include <vector>
using mojamapa_t = std::map<std::string, std::string>;
using mojafunkcja_t = std::function<std::string(std::string)>;
void wypisz(mojamapa_t mapa, mojafunkcja_t fun) {
  using namespace std;
  for (auto kv : mapa) {
    auto [k, v] = kv;
    cout << "klucz: " << k << "; wartosc " << fun(v) << endl;
  }
}
int main(int argc, char **argv) {
  using namespace std;
  map<string, string> mapa = {{"imie", "Jan"}};
  map<string, mojafunkcja_t> formatery;
  formatery["r"] = [](string x) { return "== " + x + " =="; };
  formatery["p"] = [](string x) { return "__" + x + "__"; };
  try {
    vector<string> argumenty(argv, argv + argc);
    auto selected_f = argumenty.at(1);
    wypisz(mapa, formatery.at(selected_f));
  } catch (std::out_of_range aor) {
    cout << "podaj argument. Dostępne to: ";
    for (auto [k, v] : formatery) cout << " " << k;
    cout << endl;
  }
  return 0;
}
*/