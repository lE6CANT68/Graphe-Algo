#include "AideAlgorithmique.h"
#include "graphics.h"
#include <windows.h>
#include <cstring>
#include <iostream>

AideAlgorithmique::AideAlgorithmique() {}

void AideAlgorithmique::afficherTexteDansFenetre(const std::vector<std::string>& lignes, const std::string& titre) {

    // Vérifiez si l'initialisation a réussi
    if (graphresult() != grOk) {
        std::cerr << "Erreur d'initialisation de la fenêtre graphique." << std::endl;
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
        "0. L'utilisateur peut consulter l'aide pour comprendre brièvement l'interface.",
        "",
        "1. Choix de saisie du graphe :",
        "   a. Saisie via la console",
        "   b. Saisie via interface graphique",
        "   c. Chargement depuis un fichier",
        "   d. Quitter",
        "   -> L'utilisateur précise si le graphe est orienté (avec vérification).",
        "",
        "2. Actions possibles sur le graphe :",
        "   a. Afficher le graphe",
        "   b. Convertir le graphe (FS/APS ou matrice)",
        "   c. Exécuter un algorithme",
        "   d. Sauvegarder le graphe",
        "   e. Revenir au menu précédent",
        "   f. Quitter l'interface",
        "",
        "3. Choix d'affichage :",
        "   a. Console",
        "   b. Interface graphique (positions aléatoires)",
        "   c. Revenir",
        "   -> Possibilité de changer la couleur des sommets.",
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
        "      - Arbre recouvrant minimal avec l'algorithme de Kruskal avec pssibilité de le sauvegarder pour le réutiliser",
        "      - Revenir",
        "",
        "   -> Une aide est fournie pour chaque algorithme :",
        "      - Problématique",
        "      - Principe",
        "      - Conditions d'utilisation",
        "      -> Résultats affichés visuellement",
        "",
        "6. Sauvegarde :",
        "   - Nom du fichier demandé",
        "   - Sauvegarde locale dans le projet",
        "   - Retour possible",
        "",
        "A la fin de chaque action, retour au menu des actions."
    };
}

std::vector<std::string> AideAlgorithmique::getTexteDijkstra() {
    return {
        "\n=== Algorithme de Dijkstra ===",
        "Problème : Trouver le plus court chemin depuis une source vers tous les sommets.",
        "Principe : Mise à jour itérative des distances avec une file de priorité.",
        "Conditions : Graphe pondéré sans poids négatifs."
    };
}

std::vector<std::string> AideAlgorithmique::getTexteDantzig() {
    return {
        "\n=== Algorithme de Dantzig ===",
        "Problème : Trouver tous les plus courts chemins entre chaque paire de sommets.",
        "Principe : Mise à jour de la matrice des distances avec une approche de programmation dynamique.",
        "Conditions : Graphe pondéré, sans cycle de poids négatif."
    };
}

std::vector<std::string> AideAlgorithmique::getTexteTarjan() {
    return {
        "\n=== Algorithme de Tarjan ===",
        "Problème : Trouver les composantes fortement connexes dans un graphe orienté.",
        "Principe : Parcours en profondeur (DFS) avec indexation et pile pour isoler les composantes.",
        "Conditions : Graphe orienté."
    };
}

std::vector<std::string> AideAlgorithmique::getTexteOrdonnancement() {
    return {
        "\n=== Problème d'ordonnancement ===",
        "Problème : Organiser des tâches sous contraintes d’antériorité.",
        "Principe : Graphe orienté acyclique (DAG), calcul de rangs, dates au plus tôt/tard.",
        "Conditions : Graphe orienté, sans cycle."
    };
}

std::vector<std::string> AideAlgorithmique::getTexteRang() {
    return {
        "\n=== Calcul des rangs ===",
        "Problème : Identifier l'ordre d'exécution des sommets dans un DAG.",
        "Principe : Utilise un tri topologique pour affecter un rang croissant.",
        "Conditions : Graphe orienté sans cycle."
    };
}

std::vector<std::string> AideAlgorithmique::getTextePointsArticulation() {
    return {
        "\n=== Points d’articulation ===",
        "Problème : Identifier les sommets et arcs critiques dont la suppression déconnecte le graphe.",
        "Principe : DFS avec suivi des temps de découverte et de retour.",
        "Conditions : Graphe non orienté."
    };
}

std::vector<std::string> AideAlgorithmique::getTexteIsthmes() {
    return {
        "\n=== Isthmes ===",
        "Problème : Identifier les sommets et arcs critiques dont la suppression déconnecte le graphe.",
        "Principe : DFS avec suivi des temps de découverte et de retour.",
        "Conditions : Graphe non orienté."
    };
}

std::vector<std::string> AideAlgorithmique::getTexteGrapheReduit() {
    return {
        "\n=== Algorithme de Kruskal ===",
        "Problème : Trouver l’arbre couvrant de poids minimal d’un graphe non orienté.",
        "Principe : Tri des arcs par poids croissant et union des composantes.",
        "Conditions : Graphe non orienté, pondéré et connexe."
    };
}
