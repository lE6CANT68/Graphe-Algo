#include "grapheoriente.h"
#include <queue>
#include <algorithm>

GrapheOriente::GrapheOriente(vector<sommet> tabSommets, vector<arc*> tabArcs) 
    : graphe(tabSommets, tabArcs) {}

std::vector<int> GrapheOriente::AlgorithmeDuRang(std::vector<int> d_rangs) {
    std::vector<sommet> sommets = renvoyerListeSommetsDuGraphe();
    std::vector<arc*> arcs = renvoyerListeArcsDuGraphe();
    
    // Initialiser les rangs à -1 (non calculé)
    d_rangs.resize(sommets.size(), -1);

    
    // Calculer le degré d'entrée de chaque sommet
    std::vector<int> degreEntree(sommets.size(), 0);
    for (arc* a : arcs) {
        int destId = a->renvoyerSommetDestination()->renvoyerIdentifiant();
        degreEntree[destId]++;
    }
    
    // pile pour le tri topologique
    std::queue<int> pile;
    
    // Ajouter les sommets de degré d'entrée 0
    for (size_t i = 0; i < sommets.size(); ++i) {
        if (degreEntree[i] == 0) {
            pile.push(i);
            d_rangs[i] = 0; // Les sommets sans prédécesseurs ont un rang de 0
        }
    }
    
    // Calculer les rangs
    while (!pile.empty()) {
        int sommetCourant = pile.front();
        pile.pop();
        
        // Parcourir tous les arcs pour trouver les successeurs
        for (arc* a : arcs) {
            if (a->renvoyerSommetSource()->renvoyerIdentifiant() == sommetCourant) {
                int succId = a->renvoyerSommetDestination()->renvoyerIdentifiant();
                degreEntree[succId]--;
                
                // Mettre à jour le rang du successeur
                d_rangs[succId] = std::max(d_rangs[succId], d_rangs[sommetCourant] + 1);
                
                // Si le sommet n'a plus de prédécesseurs, l'ajouter à la pile
                if (degreEntree[succId] == 0) {
                    pile.push(succId);
                }
            }
        }
    }
    
    d_rangs.insert(d_rangs.begin(), sommets.size());
    return d_rangs;
}

/*void GrapheOriente::afficherRangs() const {
    std::cout << "Rangs des sommets :" << std::endl;
    std::cout<<"[";
    for (size_t i = 0; i < d_rangs.size(); ++i){
        std::cout<<d_rangs[i]<<" | ";
    }
    std::cout<<"]";

    std::vector<sommet> sommets = renvoyerListeSommetsDuGraphe();
    std::cout << "Rangs des sommets :" << std::endl;
    for (size_t i = 0; i < sommets.size(); ++i) {
        std::cout << "Sommet " << sommets[i].renvoyerIdentifiant() 
                  << " (" << sommets[i].renvoyerEtiquette() 
                  << ") : rang " << d_rangs[i] << std::endl;
    }
}*/ 