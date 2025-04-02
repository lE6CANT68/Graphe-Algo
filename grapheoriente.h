#ifndef GRAPHEORIENTE_H
#define GRAPHEORIENTE_H

#include <vector>
#include "graphe.h"

class GrapheOriente : public graphe {
private:
    std::vector<int> d_rangs;

public:
    GrapheOriente(vector<sommet> tabSommets, vector<arc*> tabArcs);
    std::vector<int> AlgorithmeDuRang();
    void afficherRangs() const;
};

#endif // GRAPHEORIENTE_H 