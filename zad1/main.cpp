#include <iostream>
#include <map>
#include <string>
#include <functional>
#include <cmath>
#include <vector>
using namespace std;
using calculate = function<double(vector<string>)>;

int main(int argc, char **argv){
    map<string, calculate> calculator;
    calculator["sin"] = [] (vector<string> x) {return sin(stod(x[2]));};
    calculator["add"] = [] (vector<string> x) {return stod(x[2]) + stod(x[3]);};
    calculator["mod"] = [] (vector<string> x) {return stoi(x[2]) % stoi(x[3]);};
    try
    {
        vector<string> arguments(argv, argv + argc);
        auto fun = calculator.at(arguments.at(1));
        cout << fun(arguments) << endl;
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
    }
    return 0;
}

/*
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