#include "AideAlgorithmique.h"
#include "graphics.h"
#include <windows.h>
#include <cstring>
#include <iostream>

AideAlgorithmique::AideAlgorithmique() {}

void AideAlgorithmique::afficherTexteDansFenetre(const std::vector<std::string>& lignes, const std::string& titre) {

    // V�rifiez si l'initialisation a r�ussi
    if (graphresult() != grOk) {
        std::cerr << "Erreur d'initialisation de la fen�tre graphique." << std::endl;
        return;
    }

    settextstyle(SANSSERIF_FONT, HORIZ_DIR, 1);

    int x = 50;
    int y = 50;
    int lineHeight = 25;
    int largeur = getmaxx();
    int hauteur = getmaxy();

    for (const auto& ligne : lignes) {
        outtextxy(x, y, ligne.c_str());
        y += lineHeight;

        if (y > hauteur - 60) {
            outtextxy(x, hauteur - 40, "[Appuyez sur une touche pour continuer...]");
            getch();
            cleardevice();
            y = 50;
        }
    }

    outtextxy(x, y + 20, "[Appuyez sur une touche pour fermer la fenetre...]");
}

void AideAlgorithmique::afficherAideInterface() {
    afficherTexteDansFenetre(getAideUtilisationInterface(), "AIDE");
}

void AideAlgorithmique::afficherDijkstra() {
    afficherTexteDansFenetre(getTexteDijkstra(), "Dijkstra");
}

void AideAlgorithmique::afficherDantzig() {
    afficherTexteDansFenetre(getTexteDantzig(), "Dantzig");
}

void AideAlgorithmique::afficherTarjan() {
    afficherTexteDansFenetre(getTexteTarjan(), "Tarjan");
}

void AideAlgorithmique::afficherOrdonnancement() {
    afficherTexteDansFenetre(getTexteOrdonnancement(), "Ordonnancement");
}

void AideAlgorithmique::afficherRang() {
    afficherTexteDansFenetre(getTexteRang(), "Rangs");
}

void AideAlgorithmique::afficherPointsArticulation() {
    afficherTexteDansFenetre(getTextePointsArticulation(), "Points d'articulation");
}

void AideAlgorithmique::afficherIsthmes() {
    afficherTexteDansFenetre(getTexteIsthmes(),"Isthmes");
}

void AideAlgorithmique::afficherGrapheReduit() {
    afficherTexteDansFenetre(getTexteGrapheReduit(),"Graphe reduit");
}

std::vector<std::string> AideAlgorithmique::getAideUtilisationInterface() {
    return {
        "=== Informations utilisation Interface ===",
        "",
        "0. L'utilisateur peut consulter l'aide pour comprendre bri�vement l'interface.",
        "",
        "1. Choix de saisie du graphe :",
        "   a. Saisie via la console",
        "   b. Saisie via interface graphique",
        "   c. Chargement depuis un fichier",
        "   d. Quitter",
        "   -> L'utilisateur pr�cise si le graphe est orient� (avec v�rification).",
        "",
        "2. Actions possibles sur le graphe :",
        "   a. Afficher le graphe",
        "   b. Convertir le graphe (FS/APS ou matrice)",
        "   c. Ex�cuter un algorithme",
        "   d. Sauvegarder le graphe",
        "   e. Revenir au menu pr�c�dent",
        "   f. Quitter l'interface",
        "",
        "3. Choix d'affichage :",
        "   a. Console",
        "   b. Interface graphique (positions al�atoires)",
        "   c. Revenir",
        "   -> Possibilit� de changer la couleur des sommets.",
        "",
        "4. Conversion de structures :",
        "   a. FS / APS",
        "   b. Matrice d'adjacence",
        "   c. Revenir",
        "",
        "5. Algorithmes :",
        "   a. Si le graphe est oriente :",
        "      - Rang",
        "      - Tarjan",
        "      - Dijkstra",
        "      - Dantzig",
        "      - Ordonnancement",
        "      - Revenir",
        "",
        "   b. Si non oriente :",
        "      - Points d'articulation",
        "      - Isthmes",
        "      - Arbre recouvrant minimal avec l'algorithme de Kruskal avec pssibilit� de le sauvegarder pour le r�utiliser",
        "      - Revenir",
        "",
        "   -> Une aide est fournie pour chaque algorithme :",
        "      - Probl�matique",
        "      - Principe",
        "      - Conditions d'utilisation",
        "      -> R�sultats affich�s visuellement",
        "",
        "6. Sauvegarde :",
        "   - Nom du fichier demand�",
        "   - Sauvegarde locale dans le projet",
        "   - Retour possible",
        "",
        "A la fin de chaque action, retour au menu des actions."
    };
}

std::vector<std::string> AideAlgorithmique::getTexteDijkstra() {
    return {
        "\n=== Algorithme de Dijkstra ===",
        "Probl�me : Trouver le plus court chemin depuis une source vers tous les sommets.",
        "Principe : Mise � jour it�rative des distances avec une file de priorit�.",
        "Conditions : Graphe pond�r� sans poids n�gatifs."
    };
}

std::vector<std::string> AideAlgorithmique::getTexteDantzig() {
    return {
        "\n=== Algorithme de Dantzig ===",
        "Probl�me : Trouver tous les plus courts chemins entre chaque paire de sommets.",
        "Principe : Mise � jour de la matrice des distances avec une approche de programmation dynamique.",
        "Conditions : Graphe pond�r�, sans cycle de poids n�gatif."
    };
}

std::vector<std::string> AideAlgorithmique::getTexteTarjan() {
    return {
        "\n=== Algorithme de Tarjan ===",
        "Probl�me : Trouver les composantes fortement connexes dans un graphe orient�.",
        "Principe : Parcours en profondeur (DFS) avec indexation et pile pour isoler les composantes.",
        "Conditions : Graphe orient�."
    };
}

std::vector<std::string> AideAlgorithmique::getTexteOrdonnancement() {
    return {
        "\n=== Probl�me d'ordonnancement ===",
        "Probl�me : Organiser des t�ches sous contraintes d�ant�riorit�.",
        "Principe : Graphe orient� acyclique (DAG), calcul de rangs, dates au plus t�t/tard.",
        "Conditions : Graphe orient�, sans cycle."
    };
}

std::vector<std::string> AideAlgorithmique::getTexteRang() {
    return {
        "\n=== Calcul des rangs ===",
        "Probl�me : Identifier l'ordre d'ex�cution des sommets dans un DAG.",
        "Principe : Utilise un tri topologique pour affecter un rang croissant.",
        "Conditions : Graphe orient� sans cycle."
    };
}

std::vector<std::string> AideAlgorithmique::getTextePointsArticulation() {
    return {
        "\n=== Points d�articulation ===",
        "Probl�me : Identifier les sommets et arcs critiques dont la suppression d�connecte le graphe.",
        "Principe : DFS avec suivi des temps de d�couverte et de retour.",
        "Conditions : Graphe non orient�."
    };
}

std::vector<std::string> AideAlgorithmique::getTexteIsthmes() {
    return {
        "\n=== Isthmes ===",
        "Probl�me : Identifier les sommets et arcs critiques dont la suppression d�connecte le graphe.",
        "Principe : DFS avec suivi des temps de d�couverte et de retour.",
        "Conditions : Graphe non orient�."
    };
}

std::vector<std::string> AideAlgorithmique::getTexteGrapheReduit() {
    return {
        "\n=== Algorithme de Kruskal ===",
        "Probl�me : Trouver l�arbre couvrant de poids minimal d�un graphe non orient�.",
        "Principe : Tri des arcs par poids croissant et union des composantes.",
        "Conditions : Graphe non orient�, pond�r� et connexe."
    };
}
