#include "DessinateurGraphe.h"
#include "graphics.h"
#include <cmath>
#include <string>

void DessinateurGraphe::dessinerSommet(const sommet& s, int rayon, const std::string& couleur) {
    int x = s.getCoordonnees().getX();
    int y = s.getCoordonnees().getY();

    // Choix de la couleur
    if (couleur == "RED") setfillstyle(SOLID_FILL, RED);
    else if (couleur == "GREEN") setfillstyle(SOLID_FILL, GREEN);
    else if (couleur == "BLUE") setfillstyle(SOLID_FILL, BLUE);
    else setfillstyle(SOLID_FILL, BLACK);

    settextstyle(DEFAULT_FONT, HORIZ_DIR, 1);

    // Dessin du cercle
    fillellipse(x, y, rayon, rayon);

    // Affichage du numéro (ID) au centre du cercle
    std::string idText = std::to_string(s.renvoyerIdentifiant());
    outtextxy(x - 5, y - 5, idText.c_str());

    // Affichage de l'étiquette à côté
    outtextxy(x + rayon + 5, y - 10, s.renvoyerEtiquette().c_str());
}


void DessinateurGraphe::dessinerArc(const arcDUnGraphe& a, bool oriente) {
    // Coordonnées
    int x1 = a.renvoyerSommetSource()->getCoordonnees().getX();
    int y1 = a.renvoyerSommetSource()->getCoordonnees().getY();
    int x2 = a.renvoyerSommetDestination()->getCoordonnees().getX();
    int y2 = a.renvoyerSommetDestination()->getCoordonnees().getY();

    // Ligne
    line(x1, y1, x2, y2);

    int dx = x2 - x1;
    int dy = y2 - y1;
    float norm = sqrt(dx * dx + dy * dy);
    float ux = dx / norm;
    float uy = dy / norm;

    int baseX = x1 + int(norm * 0.25f * ux);
    int baseY = y1 + int(norm * 0.25f * uy);

    int arrowSize = 10;
    int wingOffset = 6;

    int tipX = baseX;
    int tipY = baseY;
    int leftX = baseX - int(arrowSize * ux - wingOffset * uy);
    int leftY = baseY - int(arrowSize * uy + wingOffset * ux);
    int rightX = baseX - int(arrowSize * ux + wingOffset * uy);
    int rightY = baseY - int(arrowSize * uy - wingOffset * ux);

    if(oriente == true){
        // Flèche (remplie) placée à 25% du chemin
        int arrow[6] = { tipX, tipY, leftX, leftY, rightX, rightY };
        setfillstyle(SOLID_FILL, WHITE);
        fillpoly(3, arrow);
    }

    // Si l'arc est pondéré : affichage du poids au milieu
    const arcAvecPoids* ap = dynamic_cast<const arcAvecPoids*>(&a);
    if (ap != nullptr) {
        // Décaler légèrement le texte à droite du triangle
        int labelX = tipX + int(15 * uy);  //  déplacement perpendiculaire
        int labelY = tipY - int(15 * ux);  //  déplacement perpendiculaire
        outtextxy(labelX, labelY, std::to_string(ap->renvoyerPoidsArc()).c_str());
    }
}

void DessinateurGraphe::dessinerGraphe(const graphe& g, bool oriente, int rayon, const string& couleur) {

    const auto& arcs = g.renvoyerListeArcsDuGraphe();
    const auto& sommets = g.renvoyerListeSommetsDuGraphe();

    for (const auto& a : arcs) {
        dessinerArc(*a, oriente);
    }

    for (const auto& s : sommets) {
        dessinerSommet(s, rayon, couleur);
    }
}
