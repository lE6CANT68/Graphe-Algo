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
    GrapheOriente(vector<sommet> tabSommets, vector<arc*> tabArcs);
    std::vector<int> AlgorithmeDuRang(std::vector<int> d_rangs);
    void afficherRangs() const;


    void dfsTarjan(int s,const vector<vector<int>>& adj,vector<int>& num,vector<int>& ro,vector<int>& cfc
                    ,vector<bool>& enPileTarjan,vector<int>& pileTarjan,int& p,int& k)
    void trouverComposantesFortementConnexes();
    void Dijkstra(int* fs, int* aps, int** p, int s, int*& d, int*& pr);
    void Dantzig(vector<vector<int>> &L, const vector<vector<int>> &C, int n);
};

#endif // GRAPHEORIENTE_H
