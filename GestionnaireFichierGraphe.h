#ifndef GESTIONNAIREFICHIERGRAPHE_H
#define GESTIONNAIREFICHIERGRAPHE_H

#include <string>
#include "graphe.h"
#include "DessinateurGraphe.h"

/**
    Format attendu
        # Sommets
        1 A
        2 B
        3 C

        # ArcsAvecPoids
        1 2 8
        2 3 5

        # ArcNonPonderer
        1 2
        2 3

*/

class GestionnaireFichierGraphe {
public:
    GestionnaireFichierGraphe();
    static void chargerDepuisFichier(const std::string& nomFichier, graphe& g);
    static void sauvegarderDansFichier(const std::string& nomFichier, const graphe& g);
};

#endif // GESTIONNAIREFICHIERGRAPHE_H
