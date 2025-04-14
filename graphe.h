#ifndef GRAPHE_H
#define GRAPHE_H

#include <iostream>
#include <vector>

#include "sommet.h"
#include "arc.h"

using std::vector;

class graphe
{
    private:
        vector<sommet> d_sommets;
        vector<arcDUnGraphe*> d_arcs;
    public:
        graphe();
        graphe(vector<sommet> tabSommets, vector<arcDUnGraphe*> tabArcs);
        ~graphe();

        vector<sommet> renvoyerListeSommetsDuGraphe() const;
        vector<arcDUnGraphe*> renvoyerListeArcsDuGraphe() const;

        void ajouterUnSommetAuGraphe(const sommet &s);
        void ajouterUnArcAuGraphe(arcDUnGraphe* a);

        void supprimerUnSommetDuGraphe(const sommet &s);
        void supprimerUnArcDuGraphe(arcDUnGraphe* a);

        void afficherLeGraphe();

        void creeFsAPartirDuGraphe(const graphe& g, int*&fs);
        void creeAPSAPartirDeFs(int*fs, int *&aps);
        void creeMatriceAdajenceAPartirDuGraphe(const graphe& g, int **&matAdajacence);
        vector<vector<int>> creerListeAdjacence() const;

};



#endif // GRAPHE_H
