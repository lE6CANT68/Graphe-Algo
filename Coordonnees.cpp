#include "Coordonnees.h"

Coordonnees::Coordonnees(): d_x{0}, d_y{0}
{}

Coordonnees::Coordonnees(int x, int y): d_x{x}, d_y{y}
{}

int Coordonnees::getX() const { return d_x; }
int Coordonnees::getY() const { return d_y; }

void Coordonnees::setCoord(int x, int y) {
    d_x = x;
    d_y = y;
}
