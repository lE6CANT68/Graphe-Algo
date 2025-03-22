#ifndef SOMMET_H
#define SOMMET_H

#include <iostream>

using std::string;

class sommet
{
    private:
        int d_identifiant;
        string d_etiquette;

    public:
        sommet(int id, string etiquette);
        sommet(int id);

        int renvoyerIdentifiant() const;
        string renvoyerEtiquette() const;
};

#endif // SOMMET_H
