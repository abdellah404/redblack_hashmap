#ifndef UNORDERED_MAP_STL_H
#define UNORDERED_MAP_STL_H

#include <unordered_map>

class UnorderedMapSTL {
private:
    std::unordered_map<int, int> donnees;

public:
    void inserer(int valeur) { donnees.emplace(valeur, valeur); }
    bool rechercher(int valeur) const { return donnees.find(valeur) != donnees.end(); }
    void supprimer(int valeur) { donnees.erase(valeur); }
};

#endif
