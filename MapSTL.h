#ifndef MAP_STL_H
#define MAP_STL_H

#include <map>

class MapSTL {
private:
    std::map<int, int> donnees;

public:
    void inserer(int valeur) { donnees.emplace(valeur, valeur); }
    bool rechercher(int valeur) const { return donnees.find(valeur) != donnees.end(); }
    void supprimer(int valeur) { donnees.erase(valeur); }
};

#endif
