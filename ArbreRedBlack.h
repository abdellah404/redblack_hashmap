#ifndef ARBRE_RED_BLACK_H
#define ARBRE_RED_BLACK_H

class ArbreRedBlack {
private:
    struct Noeud {
        int valeur;
        bool rouge;
        Noeud* gauche;
        Noeud* droite;
        Noeud* parent;
        Noeud(int v) : valeur(v), rouge(true), gauche(nullptr), droite(nullptr), parent(nullptr) {}
    };

    Noeud* racine = nullptr;

    bool estRouge(Noeud* n) const { return n != nullptr && n->rouge; }
    bool estNoir(Noeud* n) const { return n == nullptr || !n->rouge; }

    void rotationGauche(Noeud* x) {
        Noeud* y = x->droite;
        x->droite = y->gauche;
        if (y->gauche != nullptr) y->gauche->parent = x;
        y->parent = x->parent;
        if (x->parent == nullptr) racine = y;
        else if (x == x->parent->gauche) x->parent->gauche = y;
        else x->parent->droite = y;
        y->gauche = x;
        x->parent = y;
    }

    void rotationDroite(Noeud* x) {
        Noeud* y = x->gauche;
        x->gauche = y->droite;
        if (y->droite != nullptr) y->droite->parent = x;
        y->parent = x->parent;
        if (x->parent == nullptr) racine = y;
        else if (x == x->parent->gauche) x->parent->gauche = y;
        else x->parent->droite = y;
        y->droite = x;
        x->parent = y;
    }

    void corrigerInsertion(Noeud* n) {
        while (n != racine && estRouge(n->parent)) {
            Noeud* parent = n->parent;
            Noeud* grandParent = parent->parent;
            if (parent == grandParent->gauche) {
                Noeud* oncle = grandParent->droite;
                if (estRouge(oncle)) {
                    parent->rouge = false;
                    oncle->rouge = false;
                    grandParent->rouge = true;
                    n = grandParent;
                } else {
                    if (n == parent->droite) {
                        n = parent;
                        rotationGauche(n);
                        parent = n->parent;
                        grandParent = parent->parent;
                    }
                    parent->rouge = false;
                    grandParent->rouge = true;
                    rotationDroite(grandParent);
                }
            } else {
                Noeud* oncle = grandParent->gauche;
                if (estRouge(oncle)) {
                    parent->rouge = false;
                    oncle->rouge = false;
                    grandParent->rouge = true;
                    n = grandParent;
                } else {
                    if (n == parent->gauche) {
                        n = parent;
                        rotationDroite(n);
                        parent = n->parent;
                        grandParent = parent->parent;
                    }
                    parent->rouge = false;
                    grandParent->rouge = true;
                    rotationGauche(grandParent);
                }
            }
        }
        racine->rouge = false;
    }

    Noeud* trouver(int valeur) const {
        Noeud* courant = racine;
        while (courant != nullptr) {
            if (valeur == courant->valeur) return courant;
            courant = (valeur < courant->valeur) ? courant->gauche : courant->droite;
        }
        return nullptr;
    }

    void remplacer(Noeud* ancien, Noeud* nouveau) {
        if (ancien->parent == nullptr) racine = nouveau;
        else if (ancien == ancien->parent->gauche) ancien->parent->gauche = nouveau;
        else ancien->parent->droite = nouveau;
        if (nouveau != nullptr) nouveau->parent = ancien->parent;
    }

    // x peut etre nullptr ; parent garde alors le parent de x.
    void corrigerSuppression(Noeud* x, Noeud* parent) {
        while (x != racine && estNoir(x)) {
            if (parent == nullptr) break;
            if (x == parent->gauche) {
                Noeud* frere = parent->droite;
                if (estRouge(frere)) {
                    frere->rouge = false;
                    parent->rouge = true;
                    rotationGauche(parent);
                    frere = parent->droite;
                }
                if (frere == nullptr || (estNoir(frere->gauche) && estNoir(frere->droite))) {
                    if (frere != nullptr) frere->rouge = true;
                    x = parent;
                    parent = x->parent;
                } else {
                    if (estNoir(frere->droite)) {
                        if (frere->gauche != nullptr) frere->gauche->rouge = false;
                        frere->rouge = true;
                        rotationDroite(frere);
                        frere = parent->droite;
                    }
                    frere->rouge = parent->rouge;
                    parent->rouge = false;
                    if (frere->droite != nullptr) frere->droite->rouge = false;
                    rotationGauche(parent);
                    x = racine;
                }
            } else {
                Noeud* frere = parent->gauche;
                if (estRouge(frere)) {
                    frere->rouge = false;
                    parent->rouge = true;
                    rotationDroite(parent);
                    frere = parent->gauche;
                }
                if (frere == nullptr || (estNoir(frere->gauche) && estNoir(frere->droite))) {
                    if (frere != nullptr) frere->rouge = true;
                    x = parent;
                    parent = x->parent;
                } else {
                    if (estNoir(frere->gauche)) {
                        if (frere->droite != nullptr) frere->droite->rouge = false;
                        frere->rouge = true;
                        rotationGauche(frere);
                        frere = parent->gauche;
                    }
                    frere->rouge = parent->rouge;
                    parent->rouge = false;
                    if (frere->gauche != nullptr) frere->gauche->rouge = false;
                    rotationDroite(parent);
                    x = racine;
                }
            }
        }
        if (x != nullptr) x->rouge = false;
    }

    void liberer(Noeud* n) {
        if (n == nullptr) return;
        liberer(n->gauche);
        liberer(n->droite);
        delete n;
    }

public:
    ArbreRedBlack() = default;
    ArbreRedBlack(const ArbreRedBlack&) = delete;
    ArbreRedBlack& operator=(const ArbreRedBlack&) = delete;
    ~ArbreRedBlack() { liberer(racine); }

    void inserer(int valeur) {
        Noeud* courant = racine;
        Noeud* parent = nullptr;
        while (courant != nullptr) {
            parent = courant;
            if (valeur == courant->valeur) return;
            courant = (valeur < courant->valeur) ? courant->gauche : courant->droite;
        }
        Noeud* n = new Noeud(valeur);
        n->parent = parent;
        if (parent == nullptr) racine = n;
        else if (valeur < parent->valeur) parent->gauche = n;
        else parent->droite = n;
        corrigerInsertion(n);
    }

    bool rechercher(int valeur) const { return trouver(valeur) != nullptr; }

    void supprimer(int valeur) {
        Noeud* z = trouver(valeur);
        if (z == nullptr) return;
        Noeud* y = z;
        bool yEtaitRouge = y->rouge;
        Noeud* x = nullptr;
        Noeud* parentX = nullptr;

        if (z->gauche == nullptr) {
            x = z->droite;
            parentX = z->parent;
            remplacer(z, z->droite);
        } else if (z->droite == nullptr) {
            x = z->gauche;
            parentX = z->parent;
            remplacer(z, z->gauche);
        } else {
            y = z->droite;
            while (y->gauche != nullptr) y = y->gauche;
            yEtaitRouge = y->rouge;
            x = y->droite;
            if (y->parent == z) {
                parentX = y;
                if (x != nullptr) x->parent = y;
            } else {
                parentX = y->parent;
                remplacer(y, y->droite);
                y->droite = z->droite;
                y->droite->parent = y;
            }
            remplacer(z, y);
            y->gauche = z->gauche;
            y->gauche->parent = y;
            y->rouge = z->rouge;
        }
        delete z;
        if (!yEtaitRouge) corrigerSuppression(x, parentX);
    }
};

#endif
