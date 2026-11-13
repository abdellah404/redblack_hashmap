#ifndef HASH_MAP_H
#define HASH_MAP_H

#include <vector>
#include <list>
#include <functional>

// Table de hachage personnalisee avec chainage separe et redimensionnement.
class HashMap {
private:
    std::vector<std::list<int>> table;
    int nombre = 0;
    long long collisions = 0; // Insertions dans un bucket deja occupe

    int indice(int valeur) const {
        return static_cast<int>(std::hash<int>{}(valeur) % table.size());
    }

    void agrandir() {
        std::vector<std::list<int>> nouvelle(table.size() * 2 + 1);
        for (const auto& liste : table) {
            for (int valeur : liste) {
                int i = static_cast<int>(std::hash<int>{}(valeur) % nouvelle.size());
                nouvelle[i].push_back(valeur);
            }
        }
        table.swap(nouvelle);
    }

public:
    long long getCollisions() const { return collisions; }
    int getCapacite() const { return static_cast<int>(table.size()); }

    explicit HashMap(int capacite = 16) : table(capacite > 0 ? capacite : 16) {}

    void inserer(int valeur) {
        if (rechercher(valeur)) return;
        if (nombre >= static_cast<int>(table.size())) agrandir();
        int i = indice(valeur);
        if (!table[i].empty()) collisions++;
        table[i].push_back(valeur);
        nombre++;
    }

    bool rechercher(int valeur) const {
        for (int element : table[indice(valeur)])
            if (element == valeur) return true;
        return false;
    }

    void supprimer(int valeur) {
        std::list<int>& liste = table[indice(valeur)];
        for (auto it = liste.begin(); it != liste.end(); ++it) {
            if (*it == valeur) {
                liste.erase(it);
                nombre--;
                return;
            }
        }
    }
};

#endif
