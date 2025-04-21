#include "ConstructeurGraphique.h"
#include "graphics.h"
#include "arc.h"

#include <iostream>
#include <conio.h>
#include <windows.h>

ConstructeurGraphique::ConstructeurGraphique(graphe& g, DessinateurGraphe& d, int rayon, const std::string& couleur)
    : d_graphe(g), d_dessinateur(d), d_rayon(rayon), d_couleur(couleur) {}


void ConstructeurGraphique::viderEvenementsSouris() {
    while (buttonhit()) {
        int x, y;
        getmouse(x, y); // Vide les événements en attente
    }
}

void ConstructeurGraphique::attendreClic() {
    // Ignorer tout clic en attente dans le buffer
    while (buttonhit()) {
        int x, y;
        getmouse(x, y);
    }

    // Maintenant, attendre le prochain vrai clic utilisateur
    while (!buttonhit()) {
        delay(10); // Pour ne pas surcharger le CPU
    }
}



void ConstructeurGraphique::ajouterSommet(int id, bool avecNom) {
    std::string nom;

    if (avecNom) {
        std::cout << "Entrez le nom du sommet " << id << " : ";
        std::cin >> nom;
    } else {
        nom = "";
    }

    std::cout << "Cliquez dans la fenetre pour placer le sommet...\n";
    attendreClic();
    int x = 0, y = 0;
    while (!buttonhit()) {}

    getmouse(x, y);

    sommet s(id, nom);
    s.setCoordonnees(Coordonnees(x, y));
    d_graphe.ajouterUnSommetAuGraphe(s);
    d_dessinateur.dessinerSommet(s, d_rayon, d_couleur);
}

void ConstructeurGraphique::ajouterArc(bool avecPoids, bool oriente) {
    viderEvenementsSouris();
    auto& sommets = d_graphe.renvoyerListeSommetsDuGraphe();

    sommet* sourcePtr = nullptr;
    sommet* destinationPtr = nullptr;
    bool trouver = false;
    int poids;

    std::cout << "Cliquez sur le sommet **source**...\n";
    while (!trouver) {
        int x = 0, y = 0;
        while (!buttonhit()) {}
        getmouse(x, y);

        for (auto& s : sommets) {
            int dx = x - s.getCoordonnees().getX();
            int dy = y - s.getCoordonnees().getY();
            if (dx * dx + dy * dy <= d_rayon * d_rayon) {
                sourcePtr = &s;
                trouver = true;
                break;
            }
        }
    }

    trouver = false;
    std::cout << "Cliquez sur le sommet **destination**...\n";
    while (!trouver) {
        int x = 0, y = 0;
        while (!buttonhit()) {}
        getmouse(x, y);

        for (auto& s : sommets) {
            int dx = x - s.getCoordonnees().getX();
            int dy = y - s.getCoordonnees().getY();
            if (dx * dx + dy * dy <= d_rayon * d_rayon && &s != sourcePtr) {
                destinationPtr = &s;
                trouver = true;
                break;
            }
        }
    }

    if (!sourcePtr || !destinationPtr) {
        std::cerr << "Erreur: source ou destination non valide.\n";
        return;
    }

    int idSrc = sourcePtr->renvoyerIdentifiant();
    int idDst = destinationPtr->renvoyerIdentifiant();

    if (d_graphe.arcExisteDeja(idSrc, idDst)) {
        std::cout << "Erreur: Cet arc existe deja, ajout ignore.\n";
        return;
    }

    arcDUnGraphe* arcAjoute = nullptr;

    if (avecPoids) {
        std::cout << "Donnez le poids de l'arc : ";
        std::cin >> poids;
        arcAjoute = new arcAvecPoids(*sourcePtr, *destinationPtr, poids);
    } else {
        arcAjoute = new arcDUnGraphe(*sourcePtr, *destinationPtr);
    }

    d_graphe.ajouterUnArcAuGraphe(arcAjoute);
    d_dessinateur.dessinerArc(*arcAjoute, oriente);
}


void ConstructeurGraphique::lancerSaisie(bool& orientation) {

    std::cout << "=== Saisie interactive ===\n";
    std::cout << "Cliquez pour creer un sommet.\n";

    int id = 1;
    char reponse, avecNom;
    bool quitter = false;

    // Demander si l'utilisateur souhaite nommer les sommets
    do {
        std::cout << "Voulez-vous donner un nom aux sommets ? (O/N) ";
        std::cin >> avecNom;
    } while (avecNom != 'o' && avecNom != 'O' && avecNom != 'n' && avecNom != 'N');

    bool nommer = (avecNom == 'o' || avecNom == 'O');

    // Phase 1 : création des sommets
    while (!quitter) {
        ajouterSommet(id, nommer);
        if (id>= 2)
        {
            do {
                std::cout << "Voulez-vous ajouter un autre sommet (O/N) ? ";
                std::cin >> reponse;
            } while (reponse != 'o' && reponse != 'O' && reponse != 'n' && reponse != 'N');

            if (reponse == 'n' || reponse == 'N')
                quitter = true;
        }
        id++;

    }

    // Phase 2 : création des arcs
    char creerArc, graphePondere, grapheOriente;
    do {
        std::cout << "\nVoulez-vous ajouter des arcs au graphe ? (O/N) ";
        std::cin >> creerArc;
    } while (creerArc != 'o' && creerArc != 'O' && creerArc != 'n' && creerArc != 'N');

    if (creerArc == 'o' || creerArc == 'O') {
        bool avecPoids = false;
        bool oriente = false;

        // Type d’arcs
        do {
            std::cout << "Le graphe est-il pondere ? (O/N) ";
            std::cin >> graphePondere;
        } while (graphePondere != 'o' && graphePondere != 'O' && graphePondere != 'n' && graphePondere != 'N');

        avecPoids = (graphePondere == 'o' || graphePondere == 'O');

        do {
            std::cout << "Le graphe est-il oriente ? (O/N) ";
            std::cin >> grapheOriente;
        } while (grapheOriente != 'o' && grapheOriente != 'O' && grapheOriente != 'n' && grapheOriente != 'N');

        oriente = (grapheOriente == 'o' || grapheOriente == 'O');

        orientation = oriente;

        // Boucle de création d’arcs
        char continuerArc;
        do {
            ajouterArc(avecPoids, oriente);

            do {
                std::cout << "Ajouter un autre arc ? (O/N) ";
                std::cin >> continuerArc;
            } while (continuerArc != 'o' && continuerArc != 'O' && continuerArc != 'n' && continuerArc != 'N');

        } while (continuerArc == 'o' || continuerArc == 'O');
    }

    std::cout << "\nAppuyez sur ENTREE pour quitter l'application...\n";

}




