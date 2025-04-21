#ifndef GRAPHEORIENTE_H
#define GRAPHEORIENTE_H

#include <vector>
#include "graphe.h"
#include <climits>  // Pour INT_MAX

class GrapheOriente : public graphe {
private:
    constexpr static int MAXPOIDS = INT_MAX;
    constexpr static int INF = INT_MAX;

public:
    GrapheOriente(vector<sommet> tabSommets, vector<arcDUnGraphe*> tabArcs);
    void AlgorithmeDuRang(int*& rangs);

    void dfsTarjan(int s, int** adj, int* tailles, int* num, int* ro, int* cfc,
               bool* enPileTarjan, int* pileTarjan, int& sommetPile, int& p, int& k);
    void trouverComposantesFortementConnexes();
    void Dijkstra(int* fs, int* aps, int** p, int s, int*& d, int*& pr);
    void Dantzig(int** &L, const int** C, int n);
    void Ordonnancement(int* fs, int* aps, int* durees, int*& tot, int*& tard, int*& marge, int& dureeTotale);
};


#endif // GRAPHEORIENTE_H
