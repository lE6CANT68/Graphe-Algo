#include "GestionnaireFichierGraphe.h"
#include <fstream>
#include <sstream>
#include <cstdlib>  // Pour rand(), srand()
#include <ctime>    // Pour time()

GestionnaireFichierGraphe::GestionnaireFichierGraphe(){}

void GestionnaireFichierGraphe::chargerDepuisFichier(const std::string& nomFichier, graphe& g) {
    std::ifstream fichier(nomFichier);
    if (!fichier) {
        std::cerr << "Erreur ouverture fichier " << nomFichier << "\n";
        return;
    }

    std::string ligne;
    enum Etat { LECTURE_SOMMETS, LECTURE_ARCS_PONDERES, LECTURE_ARCS_NON_PONDERES };
    Etat etatLecture = LECTURE_SOMMETS;

    srand(time(nullptr)); // Pour positions aléatoires

    while (std::getline(fichier, ligne)) {
        if (ligne.empty()) continue;
        if (ligne[0] == '#') {
            if (ligne.find("ArcsAvecPoids") != std::string::npos) {
                etatLecture = LECTURE_ARCS_PONDERES;
            } else if (ligne.find("ArcNonPonderer") != std::string::npos) {
                etatLecture = LECTURE_ARCS_NON_PONDERES;
            } else if (ligne.find("Sommets") != std::string::npos) {
                etatLecture = LECTURE_SOMMETS;
            }
            continue;
        }

        std::istringstream iss(ligne);

        if (etatLecture == LECTURE_SOMMETS) {
            int id;
            std::string nom;
            if (iss >> id >> nom) {
                int x = 30 + rand() % (600 - 60);
                int y = 30 + rand() % (600 - 60);
                sommet s(id, nom);
                s.setCoordonnees(Coordonnees(x, y));
                g.ajouterUnSommetAuGraphe(s);
            }

        } else if (etatLecture == LECTURE_ARCS_PONDERES) {
            int idSrc, idDest, poids;
            if (iss >> idSrc >> idDest >> poids) {
                const auto& sommets = g.renvoyerListeSommetsDuGraphe();
                sommet* src = nullptr;
                sommet* dest = nullptr;
                for (const auto& s : sommets) {
                    if (s.renvoyerIdentifiant() == idSrc) src = new sommet(s);
                    if (s.renvoyerIdentifiant() == idDest) dest = new sommet(s);
                }
                if (src && dest) {
                    arcDUnGraphe* a = new arcAvecPoids(*src, *dest, poids);
                    g.ajouterUnArcAuGraphe(a);
                }
            }

        } else if (etatLecture == LECTURE_ARCS_NON_PONDERES) {
            int idSrc, idDest;
            if (iss >> idSrc >> idDest) {
                const auto& sommets = g.renvoyerListeSommetsDuGraphe();
                sommet* src = nullptr;
                sommet* dest = nullptr;
                for (const auto& s : sommets) {
                    if (s.renvoyerIdentifiant() == idSrc) src = new sommet(s);
                    if (s.renvoyerIdentifiant() == idDest) dest = new sommet(s);
                }
                if (src && dest) {
                    arcDUnGraphe* a = new arcDUnGraphe(*src, *dest);
                    g.ajouterUnArcAuGraphe(a);
                }
            }
        }
    }

    fichier.close();
}

void GestionnaireFichierGraphe::sauvegarderDansFichier(const std::string& nomFichier, const graphe& g) {
    std::ofstream fichier(nomFichier);
    if (!fichier) {
        std::cerr << "Erreur sauvegarde fichier " << nomFichier << "\n";
        return;
    }

    // Sommets
    fichier << "# Sommets\n";
    for (const auto& s : g.renvoyerListeSommetsDuGraphe()) {
        fichier << s.renvoyerIdentifiant() << " " << s.renvoyerEtiquette() << "\n";
    }

    // Vérification du type d'arcs
    const auto& arcs = g.renvoyerListeArcsDuGraphe();
    bool arcsPonderes = false;

    if (!arcs.empty()) {
        arcsPonderes = dynamic_cast<arcAvecPoids*>(arcs[0]) != nullptr;
    }

    if (arcsPonderes) {
        fichier << "\n# ArcsAvecPoids\n";
        for (const auto& a : arcs) {
            arcAvecPoids* ap = dynamic_cast<arcAvecPoids*>(a);
            if (ap) {
                fichier << ap->renvoyerSommetSource()->renvoyerIdentifiant() << " "
                        << ap->renvoyerSommetDestination()->renvoyerIdentifiant() << " "
                        << ap->renvoyerPoidsArc() << "\n";
            }
        }
    } else {
        fichier << "\n# ArcNonPonderer\n";
        for (const auto& a : arcs) {
            fichier << a->renvoyerSommetSource()->renvoyerIdentifiant() << " "
                    << a->renvoyerSommetDestination()->renvoyerIdentifiant() << "\n";
        }
    }

    fichier.close();
}
