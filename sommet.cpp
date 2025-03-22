#include "sommet.h"

sommet::sommet(int id, string etiquette) : d_identifiant{id}, d_etiquette{etiquette}
{}

sommet::sommet(int id) : d_identifiant{id}, d_etiquette{""}
{}

int sommet::renvoyerIdentifiant() const
{
    return d_identifiant;
}

string sommet::renvoyerEtiquette() const
{
    return d_etiquette;
}
