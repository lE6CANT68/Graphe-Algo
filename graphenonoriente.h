#ifndef GRAPHENONORIENTE_H
#define GRAPHENONORIENTE_H

#include <vector>
#include <utility>
#include "graphe.h"

class GrapheNonOriente : public graphe {
public:

    GrapheNonOriente(const std::vector<sommet>& listeSommets, const std::vector<arcDUnGraphe*>& listeArcs);

    /**
     * @brief Détecte et renvoie les points d'articulation du graphe.
     * @return std::vector<int> Liste des identifiants des points d'articulation.
     */
    int* trouverPointsDArticulation();

    /**
     * @brief Détecte et renvoie les isthmes (ponts) du graphe.
     * @return std::vector<std::pair<int, int>> Liste des paires d'identifiants représentant les isthmes.
     */
    std::vector<std::pair<int, int>> trouverIsthmies();

    /**
     * @brief Calcule et renvoie un arbre recouvrant minimal (MST) du graphe via l'algorithme de Kruskal.
     * @return graphe* Pointeur vers le graphe représentant l'arbre recouvrant minimal.
     */
    graphe* arbreRecouvrantMinimal();
};

#endif // GRAPHENONORIENTE_H
