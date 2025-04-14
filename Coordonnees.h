#ifndef COORDONNEES_H
#define COORDONNEES_H


class Coordonnees
{
    private:
        int d_x, d_y;

    public:
        Coordonnees();
        Coordonnees(int x, int y);
        int getX() const;
        int getY() const;
        void setCoord(int x, int y);
};

#endif // COORDONNEES_H
