#include <iostream>
#include <map>
#include <vector>
#include <algorithm>

using namespace std;

int main() {
    map<int, string> mapa = {
        {3, "Tres"},
        {1, "Uno"},
        {4, "Cuatro"},
        {2, "Dos"}
    };

    // Copiar a vector para ordenar por valor
    vector<pair<int, string>> vec(mapa.begin(), mapa.end());

    // Ordenar por valor (second) ascendente
    sort(vec.begin(), vec.end(), 
        [](const auto& a, const auto& b) {
            return a.second < b.second;
        });

    cout << "Ordenado por valor ascendente:" << endl;
    for (const auto& par : vec) {
        cout << par.first << ": " << par.second << endl;
    }

    // Ordenar por valor descendente
    sort(vec.begin(), vec.end(), 
        [](const auto& a, const auto& b) {
            return a.second > b.second;
        });

    cout << "\nOrdenado por valor descendente:" << endl;
    for (const auto& par : vec) {
        cout << par.first << ": " << par.second << endl;
    }
    

    return 0;
}