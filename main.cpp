#include <iostream>
#include <vector>
#include <algorithm>
#include <random>
#include <chrono>
#include <iomanip>
#include <string>
#include <type_traits>
#include "ArbreRedBlack.h"
#include "HashMap.h"
#include "MapSTL.h"
#include "UnorderedMapSTL.h"

using namespace std;
using namespace chrono;

struct Resultat {
    string nom;
    double insertion;
    double recherche;
    double suppression;
    int trouves;
    int restants;
    long long collisions;
};

template <typename Structure>
Resultat tester(string nom, const vector<int>& valeurs) {
    Structure structure;

    auto debut = steady_clock::now();
    for (int v : valeurs)
        structure.inserer(v);
    auto fin = steady_clock::now();
    long long collisions = 0;
    if constexpr (is_same_v<Structure, HashMap>)
        collisions = structure.getCollisions();
    double insertion = duration<double, milli>(fin - debut).count();

    int trouves = 0;
    debut = steady_clock::now();
    for (int v : valeurs)
        if (structure.rechercher(v)) trouves++;
    fin = steady_clock::now();
    double recherche = duration<double, milli>(fin - debut).count();

    debut = steady_clock::now();
    for (int v : valeurs)
        structure.supprimer(v);
    fin = steady_clock::now();
    double suppression = duration<double, milli>(fin - debut).count();

    int restants = 0;
    for (int v : valeurs)
        if (structure.rechercher(v)) restants++;

    return {nom, insertion, recherche, suppression, trouves, restants, collisions};
}

bool comparer(string scenario, const vector<int>& valeurs) {
    vector<Resultat> resultats;
    resultats.push_back(tester<ArbreRedBlack>("Red-Black", valeurs));
    resultats.push_back(tester<HashMap>("HashMap POO", valeurs));
    resultats.push_back(tester<MapSTL>("map STL", valeurs));
    resultats.push_back(tester<UnorderedMapSTL>("unordered_map STL", valeurs));

    cout << "\n=== " << scenario << " ===\n";
    cout << fixed << setprecision(2);
    cout << left << setw(20) << "Structure"
         << right << setw(16) << "Insertion ms"
         << setw(16) << "Recherche ms"
         << setw(16) << "Suppression ms"
         << setw(12) << "Trouves"
         << setw(12) << "Restants"
         << setw(16) << "Collisions" << '\n';
    cout << string(108, '-') << '\n';

    bool ok = true;
    for (const Resultat& r : resultats) {
        cout << left << setw(20) << r.nom
             << right << setw(16) << r.insertion
             << setw(16) << r.recherche
             << setw(16) << r.suppression
             << setw(12) << r.trouves
             << setw(12) << r.restants
             << setw(16) << (r.nom == "HashMap POO" ? to_string(r.collisions) : "-")
             << '\n';
        if (r.trouves != static_cast<int>(valeurs.size()) || r.restants != 0)
            ok = false;
    }
    cout << "Controles : " << (ok ? "OK" : "ERREUR") << '\n';
    return ok;
}

int main() {
    const int tailles[] = {1000, 10000, 100000, 1000000, 10000000};
    bool toutOk = true;

    for (int N : tailles) {
        vector<int> valeurs;
        valeurs.reserve(N);
        for (int i = 0; i < N; i++)
            valeurs.push_back(i);

        cout << "\n========================================================\n";
        cout << "N = " << N << " valeurs distinctes\n";
        cout << "========================================================\n";

        bool ok = comparer("CROISSANT", valeurs);

        reverse(valeurs.begin(), valeurs.end());
        if (!comparer("DECROISSANT", valeurs)) ok = false;

        mt19937 generateur(42);
        shuffle(valeurs.begin(), valeurs.end(), generateur);
        if (!comparer("ALEATOIRE", valeurs)) ok = false;

        if (!ok) toutOk = false;
    }

    cout << "\nBILAN FINAL : " << (toutOk ? "OK" : "ERREUR") << '\n';
    return toutOk ? 0 : 1;
}
