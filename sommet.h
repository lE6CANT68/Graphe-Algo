#ifndef SOMMET_H
#define SOMMET_H

#include <iostream>
#include "Coordonnees.h"

using std::string;

class sommet
{
    private:
        int d_identifiant;
        string d_etiquette;
        Coordonnees d_coord;

    public:
        sommet(int id, string etiquette);
        sommet(int id);
        sommet();

        int renvoyerIdentifiant() const;
        string renvoyerEtiquette() const;

        void setInformationsSommets(int id, string etiquette);

        Coordonnees getCoordonnees() const;
        void setCoordonnees(const Coordonnees& c);
};

#endif // SOMMET_H
