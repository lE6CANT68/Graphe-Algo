#include <iostream>
#include "grapheoriente.h"
#include "sommet.h"
#include "arc.h"

using namespace std;

int main() {
    // Création des sommets
    vector<sommet> sommets;
    sommets.push_back(sommet(0, "A"));
    sommets.push_back(sommet(1, "B"));
    sommets.push_back(sommet(2, "C"));
    sommets.push_back(sommet(3, "D"));
    sommets.push_back(sommet(4, "E"));

    // Création des arcs
    vector<arc*> arcs;
    arcs.push_back(new arc(sommets[0], sommets[1])); // A -> B
    arcs.push_back(new arc(sommets[0], sommets[2])); // A -> C
    arcs.push_back(new arc(sommets[1], sommets[3])); // B -> D
    arcs.push_back(new arc(sommets[2], sommets[3])); // C -> D
    arcs.push_back(new arc(sommets[3], sommets[4])); // D -> E

    // Création du graphe orienté
    GrapheOriente grapheOriente(sommets, arcs);

    // Affichage du graphe
    cout << "Structure du graphe :" << endl;
    grapheOriente.afficherLeGraphe();
    cout << endl;

    // Calcul et affichage des rangs
    vector<int> rangs = grapheOriente.AlgorithmeDuRang();
    grapheOriente.afficherRangs();

    // Nettoyage de la mémoire
    for (arc* a : arcs) {
        delete a;
    }

    return 0;
} 