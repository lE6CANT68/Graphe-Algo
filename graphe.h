#ifndef GRAPHE_H
#define GRAPHE_H

#include <iostream>
#include <vector>

#include "sommet.h"
#include "arc.h"

using std::vector;

class graphe
{
    protected :
        vector<sommet> d_sommets;
        vector<arc*> d_arcs;
    public:
        graphe(vector<sommet> tabSommets, vector<arc*> tabArcs);
        ~graphe();

        vector<sommet> renvoyerListeSommetsDuGraphe() const;
        vector<arc*> renvoyerListeArcsDuGraphe() const;

        void ajouterUnSommetAuGraphe(const sommet &s);
        void ajouterUnArcAuGraphe(arc* a);

        void supprimerUnSommetDuGraphe(const sommet &s);
        void supprimerUnArcDuGraphe(arc* a);

        void afficherLeGraphe();

        void creeFsAPartirDuGraphe(const graphe& g, int*&fs);
        void creeAPSAPartirDeFs(int*fs, int *&aps);
        void creeMatriceAdajenceAPartirDuGraphe(const graphe& g, int **&matAdajacence);

        vector<vector<int>> graphe::creerListeAdjacence()
};



#endif // GRAPHE_H
