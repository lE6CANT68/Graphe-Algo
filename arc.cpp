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

// Méthode pour récupérer le sommet source
sommet* arc::renvoyerSommetSource() const
{
    return d_source;
}

// Méthode pour récupérer le sommet destination
sommet* arc::renvoyerSommetDestination() const
{
    return d_destination;
}


// Constructeur de la classe arcAvecPoids
arcAvecPoids::arcAvecPoids(sommet source, sommet destination, int poids) : arc(source, destination), d_poids(poids) {}

// Méthode pour récupérer le poids de l'arc
int arcAvecPoids::renvoyerPoidsArc() const
{
    return d_poids;
}
