#include "Arc.h"

// Constructeurs de la classe arc
arcDUnGraphe::arcDUnGraphe(): d_source{nullptr}, d_destination{nullptr}
{}

arcDUnGraphe::arcDUnGraphe(sommet source, sommet destination)
{
    d_source = new sommet(source);
    d_destination = new sommet(destination);
}

// Destructeur virtuel de la classe arc
arcDUnGraphe::~arcDUnGraphe()
{
    delete d_source;
    delete d_destination;
}

// Méthode pour récupérer le sommet source
sommet* arcDUnGraphe::renvoyerSommetSource() const
{
    return d_source;
}

// Méthode pour récupérer le sommet destination
sommet* arcDUnGraphe::renvoyerSommetDestination() const
{
    return d_destination;
}

// Constructeur de la classe arcAvecPoids
arcAvecPoids::arcAvecPoids(sommet source, sommet destination, int poids)
    : arcDUnGraphe(source, destination), d_poids(poids) {}

// Méthode pour récupérer le poids de l'arc
int arcAvecPoids::renvoyerPoidsArc() const
{
    return d_poids;
}
