#ifndef INTERFACE_H
#define INTERFACE_H

#include "graphe.h"
#include "DessinateurGraphe.h"
#include "GestionnaireFichierGraphe.h"
#include "ConstructeurGraphique.h"
#include "AideAlgorithmique.h"
#include <string>
#include <windows.h>

/**
    Comment l'interface s'articule ?
    0. L'utilisateur pourra consulter l'aide pour comprendre brievement comment l'interface s'articule
       Elle est disponible quelle que soit ses actions

    1. L'utilisateur choisit comment le graphe sera saisie :
        a. Saisie sur la console
        b. Saisie sur l'interface (il pourra le visualiser aussi)
        c. Saisie d'apres un fichier (il devra renseigner le nom du fichier)
        d. Quitter l'interface

    --> Il devra aussi renseigner si le graphe est oriente ou non (un test sera effectue aussi pour verifier sa reponse)

    2. L'utilisateur choisit quelle action il veut faire :
        a. Afficher le graphe
        b. Passer le graphe en l'une des structures proposees : FS et APS ou Matrice d'adjacence
        c. Executer un algorithme
        d. Sauvegarder le graphe
        e. Retourner sur le menu preccedent
        f. Quitter l'interface

    3. Si l'utilisateur a choisit d'afficher le graphe, il aura le choix entre :
        a. Un affichage sur console
        b. Un affichage sur une interface (les coord seront mis au hasard dans l'interface)
        c. Retourner sur le menu preccedent

    --> Il pourra mettre les sommets en couleurs pour mieux les visualiser

    4. Si l'utilisateur choisie de passer le graphe en l'une des structures proposees :
        a. FS et APS avec visualisation sur la console
        b. Matrice d'adjacence avec visualisation sur la console
        c. Retourner sur le menu preccedent

    5. Si l'utilisateur a choisit d'executer un algorithme :
        a. Si le graphe est oriente :
            1) Algorithme Du Rang
            2) Algorithme de Tarjan
            3) Dijkstra
            4) Dantzig
            5) Ordonnancement
            6) Retourner sur le menu preccedent

        b. Si le graphe est non oriente :
            1) Points d'articulation
            2) Isthmies
            3) arbre recouvrant minimal
            4) Retourner sur le menu preccedent

    --> Pour chaque algorithme, il pourra consulter l'aide qui permet d'expliquer brièvement chacun des
        algorithmes étudiés en précisant la problématique qu'il traite, le principe de l'algorithme et les
        conditions de son utilisation.

    --> Pour chaque algorithme, un visuel des resultats sera directement mis pour que l'utilisateur puisse
        valider chaque algo.

    6. Si l'utilisateur a choisit de sauvegarder le graphe :
        --> il devra renseigner le nom du fichier
        --> le fichier sera enregistre dans le projet
        --> il pourra revenir sur le menu preccedent

    --> A la fin de chaque choix, l'utilisateur reviendra sur le menu de choix d'action

*/

class Interface
{
    private:
        graphe d_graphe;
        DessinateurGraphe d_dessinateur;
        GestionnaireFichierGraphe d_gestionnaire;
        AideAlgorithmique d_aide;

        constexpr static int RAYON_CERCLE = 10;
        std::string d_couleur = "BLACK";

        bool d_orientationGraphe = false;
        bool d_nommerSommets = false;
        bool d_affecterUnPoids = false;
        bool fenetreOuverte = false;

        // Dimensions fenetre
        int largeurFenetreGraphe = 600;
        int hauteurFenetreGraphe = 600;

        // Ouvrir et fermer la fenetre graphique
        void ouvrirFenetre();
        void fermerFenetre();

        // Choix
        void choixOrientation();
        void choixPoids();
        void choixNommage();
        void choisirUneCouleur(std::string& choixCouleur);

        // Inserer des coord au hazard dans la fenetre
        void insererCoordonneesAuHasard(int& x, int &y);

        // Aide
        void afficherAideUtilisationInterface();

        // Verifications
        bool verficationGrapheCharge();
        bool verificationOrientationGraphe();
        bool verificationOrientationCorrecte();
        bool verificationNbArcs();
        bool verificationGrapheEstPondere() const;
        bool verificationGrapheEstNomme() const;
        bool verificationPoidsNegatifs();
        bool verificationCyclePresentDansGrapheOriente() const;
        bool verificationCyclePoidsNegatifPresent() const;

        // Messages d'erreurs
        void messageErreurSansPoids(std::string nomAlgo);
        void messageErreurCyclePresent(std::string nomAlgo);
        void messageErreurCyclePoidsNegatifsPresent();
        void messageErreurPoidsNegatifsPresent();

        // Menus
        void menuChoixSaisieGraphe();
        void menuChoixApresSaisie();
        void menuChoixAffichageGraphe();
        void menuChoixConversionStructure();
        void menuChoixAideAlgoGrapheOriente();
        void menuChoixAlgoGrapheOriente();
        void menuChoixAideAlgoGrapheNonOriente();
        void menuChoixAlgoGrapheNonOriente();
        void menuChoixSauvegarde();

        // Retour
        void revenirSurMenuSaisieGraphe();
        void revenirSurMenuChoixApresSaisie();
        void revenirSurMenuChoixAlgoGrapheOriente();
        void revenirSurMenuChoixAlgoGrapheNonOriente();

        // Reinitialisation de l'interface
        void reinitialisation();

        // Effacer le contenu de la fenetre
        void effacerContenuConsole();

        // Quitter l'interface
        void quitter();

        // Pause
        void pauseConsole();

        // Saisie
        void creerGrapheClavier();
        void creerGrapheGraphiquement();
        void chargerGrapheDepuisFichier();

        // Affichage
        void afficherLeGraphe() const;
        void afficherSurConsole() const;
        void afficherGraphiquement();

        // Sauvegarde
        void sauvegarderGrapheDansFichier() const;

        // Conversions de structures
        void afficherFS_APS();
        void afficherMatriceAdjacence();

        // Algorithmes orientés
        void lancerTarjan();
        void lancerDijkstra();
        void lancerDantzig();
        void traiterOrdonnancement();
        void afficherRangs();

        // Algorithmes non orientés
        void lancerKruskal();
        void afficherPointsArticulation();
        void afficherIsthmes();


    public:
        Interface();
        void lancer();
};

#endif // INTERFACE_H
