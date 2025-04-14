#ifndef ARC_H
#define ARC_H

#include "sommet.h"

class arcDUnGraphe {
    private:
        sommet* d_source;
        sommet* d_destination;

    public:
        arcDUnGraphe();
        arcDUnGraphe(sommet source, sommet destination);
        virtual ~arcDUnGraphe();

        sommet* renvoyerSommetSource() const;
        sommet* renvoyerSommetDestination() const;
};

class arcAvecPoids : public arcDUnGraphe {
    private:
        int d_poids;

    public:
        arcAvecPoids(sommet source, sommet destination, int poids);
        int renvoyerPoidsArc() const;
};

#endif // ARC_H
