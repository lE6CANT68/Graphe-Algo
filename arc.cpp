#include "arc.h"

// Constructeur de la classe arc
arc::arc(sommet source, sommet destination)
{
    d_source = new sommet(source);
    d_destination = new sommet(destination);
}

// Destructeur virtuel de la classe arc
arc::~arc()
{
    delete d_source;
    delete d_destination;
}

// M�thode pour r�cup�rer le sommet source
sommet* arc::renvoyerSommetSource() const
{
    return d_source;
}

// M�thode pour r�cup�rer le sommet destination
sommet* arc::renvoyerSommetDestination() const
{
    return d_destination;
}


// Constructeur de la classe arcAvecPoids
arcAvecPoids::arcAvecPoids(sommet source, sommet destination, int poids) : arc(source, destination), d_poids(poids) {}

// M�thode pour r�cup�rer le poids de l'arc
int arcAvecPoids::renvoyerPoidsArc() const
{
    return d_poids;
}
