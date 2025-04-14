#include "sommet.h"

sommet::sommet(): d_identifiant{0}, d_etiquette{""}, d_coord{}
{}

sommet::sommet(int id, string etiquette) : d_identifiant{id}, d_etiquette{etiquette}, d_coord{}
{}

sommet::sommet(int id) : d_identifiant{id}, d_etiquette{""}, d_coord{}
{}

int sommet::renvoyerIdentifiant() const
{
    return d_identifiant;
}

string sommet::renvoyerEtiquette() const
{
    return d_etiquette;
}

Coordonnees sommet::getCoordonnees() const { return d_coord; }

void sommet::setCoordonnees(const Coordonnees& c) { d_coord = c; }
