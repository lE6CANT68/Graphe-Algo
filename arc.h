#ifndef ARC_H
#define ARC_H

#include<iostream>
#include"sommet.h"

class arc
{
    private :
        sommet* d_source;
        sommet* d_destination;

    public:
        arc(sommet source, sommet destination);
        virtual ~arc();
        sommet* renvoyerSommetSource() const;
        sommet* renvoyerSommetDestination() const;

};

class arcAvecPoids : public arc{
    private :
        int d_poids;
    public :
        arcAvecPoids(sommet source, sommet destination, int poids);
        int renvoyerPoidsArc() const;

};

#endif // ARC_H
