#ifndef GRAPHEORIENTE_H
#define GRAPHEORIENTE_H

#include <vector>
#include "graphe.h"

class GrapheOriente : public graphe {
private:


public:
    GrapheOriente(vector<sommet> tabSommets, vector<arc*> tabArcs);
    std::vector<int> AlgorithmeDuRang(std::vector<int> d_rangs);
    void afficherRangs() const;


    void dfsTarjan(int s,const vector<vector<int>>& adj,vector<int>& num,vector<int>& ro,vector<int>& cfc
                    ,vector<bool>& enPileTarjan,vector<int>& pileTarjan,int& p,int& k)
    void trouverComposantesFortementConnexes();
};

#endif // GRAPHEORIENTE_H
