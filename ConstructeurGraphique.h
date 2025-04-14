#ifndef CONSTRUCTEURGRAPHIQUE_H
#define CONSTRUCTEURGRAPHIQUE_H

#include "graphe.h"
#include "DessinateurGraphe.h"

class ConstructeurGraphique {
    private:
        graphe& d_graphe;
        DessinateurGraphe& d_dessinateur;
        int d_rayon;
        std::string d_couleur;

        void ajouterSommet(int id, bool avecNom);
        void ajouterArc(bool avecPoids, bool oriente);

    public:
        ConstructeurGraphique(graphe& g, DessinateurGraphe& d, int rayon, const std::string& couleur);
        void lancerSaisie();
};

#endif // CONSTRUCTEURGRAPHIQUE_H
