#ifndef DESSINATEURGRAPHE_H
#define DESSINATEURGRAPHE_H

#include "sommet.h"
#include "arc.h"
#include "graphe.h"
#include <vector>

class DessinateurGraphe
{
    public:
        void dessinerSommet(const sommet& s, int rayon, const std::string& couleur);
        void dessinerArc(const arcDUnGraphe& a, bool oriente);
        void dessinerGraphe(const graphe& g, bool oriente, int rayon, const string& couleur);
};

#endif // DESSINATEURGRAPHE_H
