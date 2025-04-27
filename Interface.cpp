#include "Interface.h"
#include <iostream>
#include <limits> // Pour la fonction pauseConsole()
#include <algorithm> // Pour la fonction reverse
#include <functional> // Pour la methode function

#include "grapheoriente.h"
#include "graphenonoriente.h"
#include "graphics.h"

using std::cout;
using std::cin;
using std::endl;

HWND d_hwnd = nullptr;

Interface::Interface(){}

//////////////////////////////////////////////////////////PRIVEE//////////////////////////////////////////////////////////

void Interface::ouvrirFenetre() {
    if (!fenetreOuverte) {
        opengraphsize(largeurFenetreGraphe, hauteurFenetreGraphe);
        setcolor(WHITE);
        fenetreOuverte = true;

        // Sauvegarde le handle pour pouvoir le masquer/afficher plus tard
        d_hwnd = GetForegroundWindow();
    }
    else {
        cleardevice();
        if (d_hwnd) {
            ShowWindow(d_hwnd, SW_SHOW);
        }
    }
}

void Interface::fermerFenetre() {
    if (fenetreOuverte && d_hwnd) {
        getch();
        ShowWindow(d_hwnd, SW_HIDE);
    }
}

void Interface::effacerContenuConsole(){
    #ifdef _WIN32
        std::system("cls");
    #else
        std::system("clear");
    #endif
}

void Interface::pauseConsole() {
    cout << "\n[Appuyez sur ENTREE pour continuer...]";
    cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Vider le buffer
    cin.get(); // Attente sur entrée
}

void Interface::choixOrientation(){
    char reponse;
    do{
        cout<<"Est-ce que votre graphe est oriente (O/N) ?\n";
        cin>>reponse;
    }while(reponse != 'o' && reponse != 'O' && reponse != 'n' && reponse != 'N');

    if(reponse == 'o' || reponse == 'O')
        d_orientationGraphe = true;
    else
        d_orientationGraphe = false;
}

void Interface::choixPoids(){
    char reponse;
    do{
        cout<<"Est-ce que votre graphe est pondere (O/N) ?\n";
        cin>>reponse;
    }while(reponse != 'o' && reponse != 'O' && reponse != 'n' && reponse != 'N');

    if(reponse == 'o' || reponse == 'O')
        d_affecterUnPoids = true;
    else
        d_affecterUnPoids = false;
}

void Interface::choixNommage() {
    char reponse;
    do{
        cout<<"Est-ce que les sommets de votre graphe sont nommees (O/N) ?\n";
        cin>>reponse;
    }while(reponse != 'o' && reponse != 'O' && reponse != 'n' && reponse != 'N');

    if(reponse == 'o' || reponse == 'O')
        d_nommerSommets = true;
    else
        d_nommerSommets = false;
}

void Interface::choisirUneCouleur(std::string& couleur) {
    cout << "Couleur ? (RED / GREEN / BLUE / BLACK) : ";
    cin >> couleur;
}

void Interface::insererCoordonneesAuHasard(int& x, int& y) {
    x = 100 + rand() % 400;
    y = 100 + rand() % 300;
}

bool Interface::verficationGrapheCharge() {
    return !d_graphe.renvoyerListeSommetsDuGraphe().empty();
}

bool Interface::verificationOrientationGraphe() {
    return d_graphe.estOriente();
}

bool Interface::verificationOrientationCorrecte() {
    return d_orientationGraphe == verificationOrientationGraphe();
}

bool Interface::verificationNbArcs() {
    return d_graphe.renvoyerListeArcsDuGraphe().size() == 0;
}

bool Interface::verificationGrapheEstPondere() const {
    for (const auto& arc : d_graphe.renvoyerListeArcsDuGraphe()) {
        if (dynamic_cast<arcAvecPoids*>(arc) != nullptr)
            return true;
    }
    return false;
}

bool Interface::verificationGrapheEstNomme() const {
    for (const auto& s : d_graphe.renvoyerListeSommetsDuGraphe()) {
        if (!s.renvoyerEtiquette().empty())
            return true;
    }
    return false;
}


bool Interface::verificationPoidsNegatifs() {
    for (auto arc : d_graphe.renvoyerListeArcsDuGraphe()) {
        arcAvecPoids* ap = dynamic_cast<arcAvecPoids*>(arc);
        if (ap && ap->renvoyerPoidsArc() < 0) {
            return true;
        }
    }
    return false;
}

bool Interface::verificationCyclePresentDansGrapheOriente() const {
    int n = d_graphe.renvoyerListeSommetsDuGraphe().size();
    int** adj = nullptr;
    int* tailles = nullptr;
    d_graphe.creerListeAdjacence(adj, tailles);

    std::vector<bool> visite(n, false);
    std::vector<bool> dansPile(n, false);

    // Fonction DFS récursive
    std::function<bool(int)> dfs = [&](int u) {
        visite[u] = true;
        dansPile[u] = true;

        for (int i = 0; i < tailles[u]; ++i) {
            int v = adj[u][i];
            if (!visite[v] && dfs(v)) return true;
            if (dansPile[v]) return true;
        }

        dansPile[u] = false;
        return false;
    };

    for (int i = 0; i < n; ++i) {
        if (!visite[i]) {
            if (dfs(i)) {
                // Libération mémoire
                for (int i = 0; i < n; ++i) delete[] adj[i];
                delete[] adj;
                delete[] tailles;
                return true;
            }
        }
    }

    // Libération mémoire
    for (int i = 0; i < n; ++i) delete[] adj[i];
    delete[] adj;
    delete[] tailles;

    return false;
}

// Algorithme de Bellman-Ford
bool Interface::verificationCyclePoidsNegatifPresent() const {
    const auto& sommets = d_graphe.renvoyerListeSommetsDuGraphe();
    const auto& arcs = d_graphe.renvoyerListeArcsDuGraphe();
    int n = sommets.size();

    std::vector<int> distance(n, INT_MAX);
    distance[0] = 0; // Choix arbitraire du sommet de départ (index 0)

    for (int i = 1; i < n; ++i) {
        for (const auto& arc : arcs) {
            int u = arc->renvoyerSommetSource()->renvoyerIdentifiant() - 1;
            int v = arc->renvoyerSommetDestination()->renvoyerIdentifiant() - 1;

            int poids = 1; // Valeur par défaut
            if (auto* pondere = dynamic_cast<arcAvecPoids*>(arc))
                poids = pondere->renvoyerPoidsArc();

            if (distance[u] != INT_MAX && distance[u] + poids < distance[v]) {
                distance[v] = distance[u] + poids;
            }
        }
    }

    // Une passe supplémentaire : si amélioration possible -> cycle négatif
    for (const auto& arc : arcs) {
        int u = arc->renvoyerSommetSource()->renvoyerIdentifiant() - 1;
        int v = arc->renvoyerSommetDestination()->renvoyerIdentifiant() - 1;

        int poids = 1;
        if (auto* pondere = dynamic_cast<arcAvecPoids*>(arc))
            poids = pondere->renvoyerPoidsArc();

        if (distance[u] != INT_MAX && distance[u] + poids < distance[v]) {
            return true;  // Cycle de poids négatif détecté
        }
    }

    return false;
}

void Interface::messageErreurSansPoids(std::string nomAlgo) {
    effacerContenuConsole();
    cout << "\n[ERREUR] Ce graphe ne contient pas de **poids**.\n";
    cout << "L algorithme "+nomAlgo+" necessite un graphe pondere.\n";
    pauseConsole();
}

void Interface::messageErreurCyclePresent(std::string nomAlgo) {
    effacerContenuConsole();
    cout << "\n[ERREUR] Ce graphe contient un **cycle**.\n";
    cout << "L algorithme "+nomAlgo+" necessite un graphe sans cycle.\n";
    pauseConsole();
}

void Interface::messageErreurCyclePoidsNegatifsPresent() {
    effacerContenuConsole();
    cout << "\n[ERREUR] Ce graphe contient un **cycle de poids negatif**.\n";
    cout << "L algorithme de Dantzig ne peut pas être applique sur un tel graphe.\n";
    pauseConsole();
}

void Interface::messageErreurPoidsNegatifsPresent() {
    effacerContenuConsole();
    cout << "\n[ERREUR] Ce graphe contient **des arcs avec des poids negatifs**.\n";
    cout << "L algorithme de Dijkstra ne fonctionne qu’avec des poids positifs ou nuls.\n";
    pauseConsole();
}

void Interface::reinitialisation() {
    // Message utilisateur
    effacerContenuConsole();
    cout << "=== Reinitialisation de l'interface ===" << endl;

    if(!verificationOrientationCorrecte())
    {
        cout << "Cause : incoherence de l'orientation : "<<endl;
        cout << "Orientation donnee par vous : "<<(d_orientationGraphe ? "Oriente" : "Non oriente")<<"\n";
        cout << "Orientation reel du graphe : "<<(d_graphe.estOriente() ? "Oriente" : "Non oriente")<<"\n";
    }
    else{
        cout << "Cause : aucun arcs dans le graphe pour execution algo ! "<<endl;
    }

    // Réinitialisation du graphe
    d_graphe.viderGraphe();

    // Réinitialisation des attributs d'affichage
    d_couleur = "BLACK";
    d_orientationGraphe = false;
    d_nommerSommets = false;

    cout << "Le graphe a ete vide. Tous les parametres ont ete reinitialises." << endl;
    cout << "Retour au menu principal de saisie..." << endl;

    pauseConsole();

    // Relancer le menu de saisie
    menuChoixSaisieGraphe();
}

void Interface::afficherAideUtilisationInterface() {
    ouvrirFenetre();
    d_aide.afficherAideInterface();
    fermerFenetre();
}

void Interface::afficherLeGraphe() const
{
    cout << "Sommets du graphe:" << endl;
    for (const auto& s : d_graphe.renvoyerListeSommetsDuGraphe())
    {
        cout << "Sommet ID: " << s.renvoyerIdentifiant() << ", Etiquette: " << s.renvoyerEtiquette() << endl;
    }
    cout << "\nArcs du graphe:" << endl;
    for (const auto& a : d_graphe.renvoyerListeArcsDuGraphe())
    {
        arcAvecPoids* ap = dynamic_cast<arcAvecPoids*>(a);
        if (ap)
        {
            cout << "Arc avec poids de " << ap->renvoyerSommetSource()->renvoyerIdentifiant() << " a " << ap->renvoyerSommetDestination()->renvoyerIdentifiant()
                 << " avec poids: " << ap->renvoyerPoidsArc() << endl;
        }
        else
        {
            cout << "Arc sans poids de " << a->renvoyerSommetSource()->renvoyerIdentifiant() << " a " << a->renvoyerSommetDestination()->renvoyerIdentifiant() << endl;
        }
    }
}

void Interface::quitter() {
    cout << "Merci d'avoir utilise l'interface.\n";
    closegraph();
    exit(0);
}

void Interface::creerGrapheClavier() {
    int nbSommets;
    cout << "\n--- Creation du graphe via saisie clavier ---\n";
    cout << "Nombre de sommets : ";
    cin >> nbSommets;

    choixNommage();
    choixPoids();

    // Saisie des sommets
    for (int i = 1; i <= nbSommets; ++i) {
        sommet s;
        std::string etiquette = "";
        if (d_nommerSommets) {
            cout << "Nom du sommet " << i << " : ";
            cin >> etiquette;
        }

        s.setInformationsSommets(i, etiquette);

        int x, y;
        insererCoordonneesAuHasard(x, y);
        s.setCoordonnees(Coordonnees(x, y));
        d_graphe.ajouterUnSommetAuGraphe(s);
    }

    // Saisie des arcs
    char continuer = 'o';
    while (continuer == 'o' || continuer == 'O') {
        int src, dst, poids = 1;
        cout << "ID du sommet source : ";
        cin >> src;
        cout << "ID du sommet destination : ";
        cin >> dst;

        if (src == dst) {
            cout << "Erreur : Un arc ne peut pas relier un sommet à lui-même.\n";
            continue;
        }

        if (d_affecterUnPoids && d_orientationGraphe == true) {
            cout << "Poids : ";
            cin >> poids;
        }

        // Vérifier si l’arc existe déjà
        if (d_graphe.arcExisteDeja(src, dst)) {
            cout << "Erreur: Cet arc existe deja, ajout ignore.\n";
        } else {
            // Trouver les pointeurs vers les sommets
            sommet* s1 = nullptr;
            sommet* s2 = nullptr;
            for (auto& s : d_graphe.renvoyerListeSommetsDuGraphe()) {
                if (s.renvoyerIdentifiant() == src) s1 = &s;
                if (s.renvoyerIdentifiant() == dst) s2 = &s;
            }

            if (s1 && s2) {
                arcDUnGraphe* arc = nullptr;
                if (d_orientationGraphe && d_affecterUnPoids)
                    arc = new arcAvecPoids(*s1, *s2, poids);
                else
                    arc = new arcDUnGraphe(*s1, *s2);

                d_graphe.ajouterUnArcAuGraphe(arc);
            } else {
                cout << "Erreur : Sommets introuvables.\n";
            }
        }

        cout << "Ajouter un autre arc ? (O/N) : ";
        cin >> continuer;
    }

    cout << ">>> Graphe construit avec succes !\n";
}



void Interface::creerGrapheGraphiquement() {
    choisirUneCouleur(d_couleur);
    ConstructeurGraphique c(d_graphe, d_dessinateur, RAYON_CERCLE, d_couleur);
    ouvrirFenetre();
    c.lancerSaisie(d_orientationGraphe);
    fermerFenetre();
}

void Interface::chargerGrapheDepuisFichier() {
    string nom;
    cout << "Nom du fichier a charger : ";
    cin >> nom;
    d_gestionnaire.chargerDepuisFichier(nom, d_graphe);

    d_orientationGraphe = verificationOrientationGraphe();
    d_nommerSommets = verificationGrapheEstNomme();
    d_affecterUnPoids = verificationGrapheEstPondere();
}

void Interface::menuChoixSaisieGraphe() {
    effacerContenuConsole();
    int choix;
    do {
        cout << "\n--- MENU DE SAISIE DU GRAPHE ---\n";
        cout << "1. Saisie via le clavier\n";
        cout << "2. Saisie graphique\n";
        cout << "3. Charger depuis un fichier\n";
        cout << "4. Quitter\n";
        cout << "Votre choix : ";
        cin >> choix;

        switch (choix) {
            case 1:
                choixOrientation();
                creerGrapheClavier();
                pauseConsole();
                menuChoixApresSaisie();
                break;
            case 2:
                creerGrapheGraphiquement();
                pauseConsole();
                menuChoixApresSaisie();
                break;
            case 3:
                choixOrientation();
                chargerGrapheDepuisFichier();
                pauseConsole();
                menuChoixApresSaisie();
                break;
            case 4:
                quitter();
                break;
            default:
                cout << "Choix invalide. Recommencez.\n";
        }
    } while (choix != 4);
}

void Interface::revenirSurMenuSaisieGraphe(){
    effacerContenuConsole();

    // Réinitialisation du graphe (nouvelle instance vide)
    d_graphe.viderGraphe();

    // Réinitialisation des attributs d'affichage
    d_couleur = "BLACK";
    d_orientationGraphe = false;
    d_nommerSommets = false;

    cout << "Le graphe a ete vide. Tous les parametres ont ete reinitialises." << endl;
    cout << "Retour au menu principal de saisie..." << endl;

    pauseConsole();
    menuChoixSaisieGraphe();
}

void Interface::menuChoixApresSaisie() {
    effacerContenuConsole();
    int choix;
    do {
        cout << "\n--- MENU ACTIONS APRES SAISIE ---\n";
        cout << "1. Afficher le graphe\n";
        cout << "2. Convertir en FS/APS ou matrice\n";
        cout << "3. Executer un algorithme\n";
        cout << "4. Sauvegarder le graphe\n";
        cout << "5. Revenir a la saisie\n";
        cout << "6. Quitter\n";
        cout << "Votre choix : ";
        cin >> choix;

        switch (choix) {
            case 1:
                pauseConsole();
                menuChoixAffichageGraphe();
                break;

            case 2:
                pauseConsole();
                menuChoixConversionStructure();
                break;

            case 3: {
                if(verificationOrientationCorrecte() && !verificationNbArcs())
                {
                    if (verificationOrientationGraphe()){
                        pauseConsole();
                        menuChoixAlgoGrapheOriente();
                        break;
                    }

                    else{
                        pauseConsole();
                        menuChoixAlgoGrapheNonOriente();
                        break;
                    }
                }
                else{
                    reinitialisation();
                    return;
                }
            }
            case 4:
                pauseConsole();
                menuChoixSauvegarde(); break;

            case 5: revenirSurMenuSaisieGraphe(); return;
            case 6: quitter(); break;
            default: cout << "Choix invalide.\n";
        }
    } while (choix != 6);
}

void Interface::sauvegarderGrapheDansFichier() const {
    string nom;
    cout << "Nom du fichier pour sauvegarder : ";
    cin >> nom;
    d_gestionnaire.sauvegarderDansFichier(nom, d_graphe);
}

void Interface::revenirSurMenuChoixApresSaisie() {
    pauseConsole();
    menuChoixApresSaisie();
}

void Interface::menuChoixSauvegarde() {
    char choix;
    do{
        cout<<"Voulez-vous sauvegarder votre fichier ? (O/N) \n";
        cin>>choix;
    }while(choix != 'o' && choix != '0' && choix != 'n' && choix != 'N');

    if(choix == 'o' || choix == 'O')
        sauvegarderGrapheDansFichier();
    else
        revenirSurMenuChoixApresSaisie();

}

void Interface::afficherSurConsole() const {
    afficherLeGraphe();
}

void Interface::afficherGraphiquement() {
    string couleur;
    choisirUneCouleur(couleur);
    ouvrirFenetre();
    d_dessinateur.dessinerGraphe(d_graphe, d_orientationGraphe ,RAYON_CERCLE, couleur);
    fermerFenetre();
}

void Interface::menuChoixAffichageGraphe() {
    effacerContenuConsole();
    int choix;
    do {
        cout << "\n--- MENU CHOIX AFFICHAGE ---\n";
        cout << "1. Afficher le graphe sur la console\n";
        cout << "2. Afficher le graphe dans une interface\n";
        cout << "3. Revenir au menu de choix d'actions\n";
        cout << "Votre choix : ";
        cin >> choix;

        switch (choix) {
            case 1: afficherSurConsole(); break;
            case 2: afficherGraphiquement(); break;
            case 3: revenirSurMenuChoixApresSaisie(); return;
            default: cout << "Choix invalide.\n";
        }
    } while (choix != 3);

}

void Interface::afficherFS_APS(){
    int* fs;
    int* aps;

    d_graphe.creeFsAPartirDuGraphe(d_graphe, fs);
    d_graphe.creeAPSAPartirDeFs(fs, aps);

    cout << "\n===== AFFICHAGE STRUCTURES FS ET APS =====\n";

    int tailleFS = fs[0];
    int tailleAPS = aps[0];

    // --- FS ---
    cout << "\nFS :\n| ";
    for (int i = 0; i < tailleFS; ++i)
        cout << fs[i] << " | ";
    cout << "\n  ";
    for (int i = 0; i < tailleFS; ++i)
        cout << i << "   ";
    cout << "\n";

    // --- APS ---
    cout << "\nAPS :\n| ";
    for (int i = 0; i < tailleAPS; ++i)
        cout << aps[i] << " | ";
    cout << "\n  ";
    for (int i = 0; i < tailleAPS; ++i)
        cout << i << "   ";
    cout << "\n";

    // Libération mémoire si nécessaire (selon implémentation de creeFs/creeAPS)
    delete[] fs;
    delete[] aps;
}

void Interface::afficherMatriceAdjacence() {
    int **mad;

    d_graphe.creeMatriceAdajenceAPartirDuGraphe(d_graphe,mad);
    int n = mad[0][0]; // nb sommets
    int m = mad[0][1]; // nb arcs

    cout << "\n=== MATRICE D'ADJACENCE ===\n\n";

    // En-tête colonnes
    cout << "     ";
    for (int j = 0; j <= n; ++j)
        cout << j << "       ";
    cout << "\n    ";
    for (int j = 0; j <= n; ++j)
        cout << "--------";
    cout << "\n";

    // Affichage des lignes
    for (int i = 0; i <= n; ++i) {
        cout << i << " | ";
        for (int j = 0; j <= n; ++j) {
            if (i == 0 && j >= 2)
                cout << "Null    ";
            else if (j == 0 && i >= 1)
                cout << "Null    ";
            else if (mad[i][j] == 0 && i > 0 && j > 0)
                cout << "Null    ";
            else
                cout << mad[i][j] << "       ";
        }
        cout << "\n";
    }

    // Libération mémoire
    for (int i = 0; i <= n; ++i)
        delete[] mad[i];
    delete[] mad;
}

void Interface::menuChoixConversionStructure() {
    effacerContenuConsole();
    int choix;
    do {
        cout << "\n--- MENU CHOIX CONVERSION ---\n";
        cout << "1. Passer du graphe a la structure fs et aps \n";
        cout << "2. Passer du graphe a la structure de matrice d'adjacence \n";
        cout << "3. Revenir au menu de choix d'actions\n";
        cout << "Votre choix : ";
        cin >> choix;

        switch (choix) {
            case 1: afficherFS_APS(); break;
            case 2: afficherMatriceAdjacence(); break;
            case 3: revenirSurMenuChoixApresSaisie(); return;
            default: cout << "Choix invalide.\n";
        }
    } while (choix != 3);
}

void Interface::afficherRangs() {
    effacerContenuConsole();
    int* rang;

    // Conversion du graphe vers GrapheOriente
    GrapheOriente go(d_graphe.renvoyerListeSommetsDuGraphe(), d_graphe.renvoyerListeArcsDuGraphe());
    go.AlgorithmeDuRang(rang);

    int n = rang[0]; // nombre de sommets

    cout << "\n=== AFFICHAGE DES RANGS DES SOMMETS ===\n";
    cout << "Sommet\t|\tRang\n";
    cout << "---------------------------\n";

    for (int i = 1; i <= n; ++i) {
        cout << "  " << i << "\t|\t" << rang[i] << "\n";
    }

    delete[] rang; // Libération mémoire

    pauseConsole();
}

void Interface::lancerTarjan() {
    effacerContenuConsole();
    std::vector<sommet> sommets = d_graphe.renvoyerListeSommetsDuGraphe();
    std::vector<arcDUnGraphe*> arcs = d_graphe.renvoyerListeArcsDuGraphe();
    int n = sommets.size();

    // Conversion vers graphe orienté
    GrapheOriente go(sommets, arcs);

    // Création liste d’adjacence
    int** adj;
    int* tailles;
    go.creerListeAdjacence(adj, tailles);

    // Allocation mémoire pour Tarjan
    int* num = new int[n]{};
    int* ro = new int[n]{};
    int* cfc = new int[n]{};
    bool* enPileTarjan = new bool[n]{};
    int* pileTarjan = new int[n];
    int sommetPile = 0;
    int p = 0, k = 0;

    // Lancer Tarjan pour chaque sommet non visité
    for (int s = 0; s < n; ++s) {
        if (num[s] == 0) {
            go.dfsTarjan(s, adj, tailles, num, ro, cfc, enPileTarjan, pileTarjan, sommetPile, p, k);
        }
    }

    // Affichage résultat
    std::cout << "\n=== COMPOSANTES FORTEMENT CONNEXES (CFC) ===\n";
    for (int i = 0; i < n; ++i) {
        std::string nom = sommets[i].renvoyerEtiquette();
        std::cout << "Sommet " << (nom.empty() ? std::to_string(i + 1) : nom)
                  << " appartient à la composante C" << cfc[i] << "\n";
    }

    // Libération mémoire
    for (int i = 0; i < n; ++i) delete[] adj[i];
    delete[] adj;
    delete[] tailles;
    delete[] num;
    delete[] ro;
    delete[] cfc;
    delete[] enPileTarjan;
    delete[] pileTarjan;

    pauseConsole();
}

void Interface::lancerDijkstra() {
    effacerContenuConsole();
    int* fs;
    int* aps;

    // Construction FS & APS à partir du graphe
    d_graphe.creeFsAPartirDuGraphe(d_graphe, fs);
    d_graphe.creeAPSAPartirDeFs(fs, aps);

    int n = aps[0];
    int** matricePoids = new int*[n + 1];
    for (int i = 0; i <= n; ++i)
        matricePoids[i] = new int[n + 1];

    // Initialisation matrice poids (MAXPOIDS = non accessible)
    const int MAXPOIDS = 99999;
    for (int i = 1; i <= n; ++i)
        for (int j = 1; j <= n; ++j)
            matricePoids[i][j] = MAXPOIDS;

    // Remplir la matrice des poids à partir des arcs
    for (const auto& arc : d_graphe.renvoyerListeArcsDuGraphe()) {
        int i = arc->renvoyerSommetSource()->renvoyerIdentifiant();
        int j = arc->renvoyerSommetDestination()->renvoyerIdentifiant();

        if (auto* ap = dynamic_cast<arcAvecPoids*>(arc))
            matricePoids[i][j] = ap->renvoyerPoidsArc();
        else
            matricePoids[i][j] = 1; // Non pondéré, poids = 1
    }

    int s;
    do {
        cout << "\nEntrez l'identifiant du sommet de depart (entre 1 et " << n << ") : ";
        cin >> s;
    } while (s < 1 || s > n);

    int* d;  // Tableau des distances minimales
    int* pr; // Tableau des prédécesseurs

    // Algorithme de Dijkstra
    GrapheOriente go(d_graphe.renvoyerListeSommetsDuGraphe(), d_graphe.renvoyerListeArcsDuGraphe());
    go.Dijkstra(fs, aps, matricePoids, s, d, pr);

    // Résultats
    cout << "\n=== Resultat de Dijkstra (depuis le sommet " << s << ") ===\n";
    cout << "Sommet\tDistance\tChemin\n";

    for (int i = 1; i <= n; ++i) {
        cout << i << "\t";
        if (d[i] == MAXPOIDS) {
            cout << "INF\t\t-";
        } else {
            cout << d[i] << "\t\t";
            std::vector<int> chemin;
            for (int v = i; v != 0; v = pr[v])
                chemin.push_back(v);
            reverse(chemin.begin(), chemin.end());

            for (size_t j = 0; j < chemin.size(); ++j) {
                cout << chemin[j];
                if (j != chemin.size() - 1) cout << " -> ";
            }
        }
        cout << "\n";
    }

    // Libération mémoire
    for (int i = 0; i <= n; ++i)
        delete[] matricePoids[i];
    delete[] matricePoids;
    delete[] fs;
    delete[] aps;
    delete[] d;
    delete[] pr;

    pauseConsole();
}

void Interface::lancerDantzig() {
    effacerContenuConsole();
    std::vector<sommet> sommets = d_graphe.renvoyerListeSommetsDuGraphe();
    std::vector<arcDUnGraphe*> arcs = d_graphe.renvoyerListeArcsDuGraphe();
    int n = sommets.size();

    int INF = 9999;

    // Allocation et initialisation des matrices C et L
    int** C = new int*[n + 1];
    int** L = new int*[n + 1];
    for (int i = 1; i <= n; ++i) {
        C[i] = new int[n + 1];
        L[i] = new int[n + 1];
        for (int j = 1; j <= n; ++j) {
            if (i == j)
                C[i][j] = 0;
            else
                C[i][j] = INF;
        }
    }

    // Remplir la matrice C avec les coûts des arcs
    for (arcDUnGraphe* a : arcs) {
        int u = a->renvoyerSommetSource()->renvoyerIdentifiant();
        int v = a->renvoyerSommetDestination()->renvoyerIdentifiant();

        if (arcAvecPoids* ap = dynamic_cast<arcAvecPoids*>(a)) {
            C[u][v] = ap->renvoyerPoidsArc();
        } else {
            C[u][v] = 1;
        }
    }

    // Copier C dans L
    for (int i = 1; i <= n; ++i)
        for (int j = 1; j <= n; ++j)
            L[i] = new int[n + 1], L[i][j] = C[i][j];

    // Exécuter l'algorithme de Dantzig
    GrapheOriente go(sommets, arcs);
    go.Dantzig(L, (const int**)C, n);

    // Affichage
    std::cout << "\n=== MATRICE DES PLUS COURTS CHEMINS (DANTZIG) ===\n";
    std::cout << "     ";
    for (int j = 1; j <= n; ++j) std::cout << j << "\t";
    std::cout << "\n";

    for (int i = 1; i <= n; ++i) {
        std::cout << i << " | ";
        for (int j = 1; j <= n; ++j) {
            if (L[i][j] == INF)
                std::cout << "INF\t";
            else
                std::cout << L[i][j] << "\t";
        }
        std::cout << "\n";
    }

    // Libération mémoire
    for (int i = 1; i <= n; ++i) {
        delete[] C[i];
        delete[] L[i];
    }
    delete[] C;
    delete[] L;

    pauseConsole();
}


void Interface::traiterOrdonnancement() {
    effacerContenuConsole();
    int* fs = nullptr;
    int* aps = nullptr;

    d_graphe.creeFsAPartirDuGraphe(d_graphe, fs);
    d_graphe.creeAPSAPartirDeFs(fs, aps);

    int n = aps[0];
    int* durees = new int[n];

    cout << "\n=== ORDONNANCEMENT ===\n";
    for (int i = 0; i < n; ++i) {
        cout << "Saisir duree de la tache " << i << " : ";
        cin >> durees[i];
    }

    int* tot = nullptr;
    int* tard = nullptr;
    int* marge = nullptr;
    int dureeTotale = 0;

    GrapheOriente go(d_graphe.renvoyerListeSommetsDuGraphe(), d_graphe.renvoyerListeArcsDuGraphe());
    go.Ordonnancement(fs, aps, durees, tot, tard, marge, dureeTotale);

    cout << "\nTache | Duree | Tot | Tard | Marge | Critique\n";
    cout << "---------------------------------------------\n";
    for (int i = 0; i < n; ++i) {
        cout << "  " << i << "   |   " << durees[i] << "   | "
                  << tot[i] << "   |  " << tard[i] << "   |   " << marge[i]
                  << "   |   " << (marge[i] == 0 ? "Oui" : "Non") << "\n";
    }

    cout << "\nDuree minimale du projet : " << dureeTotale << "\n";

    delete[] fs;
    delete[] aps;
    delete[] durees;
    delete[] tot;
    delete[] tard;
    delete[] marge;

    pauseConsole();
}

void Interface::revenirSurMenuChoixAlgoGrapheOriente() {
    pauseConsole();
    menuChoixAlgoGrapheOriente();
}

void Interface::menuChoixAideAlgoGrapheOriente() {
    effacerContenuConsole();
    int choix;
    do {
        cout << "\n--- MENU CHOIX AIDE SUR ALGORITHMES POUR UN GRAPHE ORIENTE ---\n";
        cout << "1. Rang \n";
        cout << "2. Tarjan \n";
        cout << "3. Dijkstra \n";
        cout << "4. Dantzig \n";
        cout << "5. Ordonnancement \n";
        cout << "6. Revenir au menu de choix d'execution d'algo pour un graphe oriente\n";
        cout << "Votre choix : ";
        cin >> choix;

        switch (choix) {
            case 1: ouvrirFenetre(); d_aide.afficherRang(); fermerFenetre(); break;
            case 2: ouvrirFenetre(); d_aide.afficherTarjan(); fermerFenetre(); break;
            case 3: ouvrirFenetre(); d_aide.afficherDijkstra(); fermerFenetre(); break;
            case 4: ouvrirFenetre(); d_aide.afficherDantzig(); fermerFenetre(); break;
            case 5: ouvrirFenetre(); d_aide.afficherOrdonnancement(); fermerFenetre(); break;
            case 6: revenirSurMenuChoixAlgoGrapheOriente(); return;
            default: cout << "Choix invalide.\n";
        }
    } while (choix != 6);
}

void Interface::menuChoixAlgoGrapheOriente() {
    effacerContenuConsole();
    int choix;
    do {
        cout << "\n--- MENU CHOIX ALGORITHME POUR UN GRAPHE ORIENTE ---\n";
        cout << "0. Consulter aide sur ces algorithmes \n";
        cout << "1. Rang \n";
        cout << "2. Tarjan \n";
        cout << "3. Dijkstra \n";
        cout << "4. Dantzig \n";
        cout << "5. Ordonnancement \n";
        cout << "6. Revenir au menu de choix d'actions\n";
        cout << "Votre choix : ";
        cin >> choix;

        switch (choix) {
            case 0:
                menuChoixAideAlgoGrapheOriente();
                break;

            case 1:
                if (verificationCyclePresentDansGrapheOriente())
                    messageErreurCyclePresent("du Rang");
                else
                    afficherRangs();
                break;

            case 2:
                lancerTarjan(); // Aucune contrainte
                break;

            case 3:
                if (!d_affecterUnPoids)
                    messageErreurSansPoids("Dijkstra");
                else if (verificationPoidsNegatifs())
                    messageErreurPoidsNegatifsPresent();
                else
                    lancerDijkstra();
                break;

            case 4:
                if (!d_affecterUnPoids)
                    messageErreurSansPoids("Dantzig");
                else if (verificationCyclePoidsNegatifPresent())
                    messageErreurCyclePoidsNegatifsPresent();
                else
                    lancerDantzig();
                break;

            case 5:
                if (verificationCyclePresentDansGrapheOriente())
                    messageErreurCyclePresent("d Ordonnancement");
                else
                    traiterOrdonnancement();
                break;

            case 6:
                revenirSurMenuChoixApresSaisie();
                return;

            default:
                cout << "Choix invalide.\n";
        }

    } while (choix != 6);
}


void Interface::revenirSurMenuChoixAlgoGrapheNonOriente() {
    pauseConsole();
    menuChoixAlgoGrapheNonOriente();
}

void Interface::menuChoixAideAlgoGrapheNonOriente() {
    effacerContenuConsole();
    int choix;
    do {
        cout << "\n--- MENU CHOIX AIDE SUR ALGORITHMES POUR UN GRAPHE NON ORIENTE ---\n";
        cout << "1. Points d'articulations \n";
        cout << "2. Isthmes \n";
        cout << "3. Arbre recouvrant minimal avec l'algorithme de Kruskal \n";
        cout << "4. Revenir au menu de choix d'execution d'algo pour un graphe non oriente\n";
        cout << "Votre choix : ";
        cin >> choix;

        switch (choix) {
            case 1: ouvrirFenetre(); d_aide.afficherPointsArticulation(); fermerFenetre(); break;
            case 2: ouvrirFenetre(); d_aide.afficherIsthmes(); fermerFenetre(); break;
            case 3: ouvrirFenetre(); d_aide.afficherGrapheReduit(); fermerFenetre(); break;
            case 4: revenirSurMenuChoixAlgoGrapheNonOriente(); return;
            default: cout << "Choix invalide.\n";
        }
    } while (choix != 6);
}

void Interface::afficherPointsArticulation() {
    effacerContenuConsole();

    GrapheNonOriente gno(d_graphe.renvoyerListeSommetsDuGraphe(), d_graphe.renvoyerListeArcsDuGraphe());

    int* articulation = gno.trouverPointsDArticulation();
    if (!articulation) {
        cout << "[ERREUR] Impossible de calculer les points d articulation.\n";
        return;
    }

    cout << "\n=== Points d articulation ===\n";

    bool aucun = true;
    for (int i = 0; articulation[i] != -1; ++i) {
        int id = articulation[i] + 1; // +1 car indexé à 0
        auto& sommets = d_graphe.renvoyerListeSommetsDuGraphe();
        auto it = std::find_if(sommets.begin(), sommets.end(),
                               [id](const sommet& s) { return s.renvoyerIdentifiant() == id; });

        if (it != sommets.end()) {
            cout << "Sommet ID: " << id;
            if (!it->renvoyerEtiquette().empty())
                cout << " ( " << it->renvoyerEtiquette() << " )";
            cout << " est un point d articulation.\n";
        } else {
            cout << "Sommet ID: " << id << " est un point d articulation.\n";
        }

        aucun = false;
    }

    if (aucun)
        cout << "Aucun point d'articulation dans ce graphe.\n";

    delete[] articulation;
}


void Interface::afficherIsthmes() {
    effacerContenuConsole();
    GrapheNonOriente gno(d_graphe.renvoyerListeSommetsDuGraphe(), d_graphe.renvoyerListeArcsDuGraphe());

    vector<std::pair<int, int>> isthmes = gno.trouverIsthmies();

    cout << "\n=== Isthmes (ponts critiques) ===\n";

    if (isthmes.empty()) {
        cout << "Aucun isthme detecte dans ce graphe.\n";
        return;
    }

    for (const auto& arc : isthmes) {
        int u = arc.first + 1;  // +1 pour ajuster si indexés à partir de 0
        int v = arc.second + 1;

        cout << "Arc entre le sommet " << u << " et le sommet " << v << " est un isthme.\n";
    }
}


void Interface::lancerKruskal() {
    effacerContenuConsole();

    // Créer un graphe non orienté temporaire
    GrapheNonOriente gno(d_graphe.renvoyerListeSommetsDuGraphe(), d_graphe.renvoyerListeArcsDuGraphe());

    // Calculer l'arbre recouvrant minimal
    graphe* mst = gno.arbreRecouvrantMinimal();
    if (!mst) {
        cout << "[ERREUR] Impossible de generer l'arbre recouvrant minimal.\n";
        return;
    }

    cout << "\n=== Arbre recouvrant minimal généré ===\n";

    // Choix affichage
    char choixAffichage;
    do {
        cout << "Afficher l'arbre en console (C) ou graphiquement (G) ? ";
        cin >> choixAffichage;
        choixAffichage = tolower(choixAffichage);
    } while (choixAffichage != 'c' && choixAffichage != 'g');

    if (choixAffichage == 'c') {
        // Affichage console
        cout << "Sommets :\n";
        for (const auto& s : mst->renvoyerListeSommetsDuGraphe())
            cout << "- " << s.renvoyerIdentifiant() << " " << s.renvoyerEtiquette() << "\n";

        cout << "\nArcs :\n";
        for (const auto& a : mst->renvoyerListeArcsDuGraphe()) {
            arcAvecPoids* ap = dynamic_cast<arcAvecPoids*>(a);
            if (ap)
                cout << ap->renvoyerSommetSource()->renvoyerIdentifiant() << " -> "
                          << ap->renvoyerSommetDestination()->renvoyerIdentifiant()
                          << " [Poids = " << ap->renvoyerPoidsArc() << "]\n";
            else
                cout << a->renvoyerSommetSource()->renvoyerIdentifiant() << " -> "
                          << a->renvoyerSommetDestination()->renvoyerIdentifiant() << "\n";
        }
    } else {
        // Affichage graphique
        DessinateurGraphe dessinateurTemp;
        dessinateurTemp.dessinerGraphe(*mst, false, RAYON_CERCLE, d_couleur);
    }

    // Sauvegarde du graphe
    char sauvegarde;
    cout << "Souhaitez-vous sauvegarder l'arbre recouvrant minimal ? (O/N) ";
    std::cin >> sauvegarde;

    if (sauvegarde == 'o' || sauvegarde == 'O') {
        std::string nomFichier;
        cout << "Entrez le nom du fichier (ex: arbre_kruskal.txt) : ";
        cin >> nomFichier;
        d_gestionnaire.sauvegarderDansFichier(nomFichier, *mst);
        cout << "Graphe sauvegarde dans " << nomFichier << "\n";
    }

    delete mst;
}


void Interface::menuChoixAlgoGrapheNonOriente() {
    effacerContenuConsole();
    int choix;
    do {
        cout << "\n--- MENU CHOIX ALGORITHME POUR UN GRAPHE NON ORIENTE ---\n";
        cout << "0. Consulter aide sur ces algorithmes \n";
        cout << "1. Points d'articulations \n";
        cout << "2. Isthmes \n";
        cout << "3. Arbre recouvrant minimal avec l'algorithme de Kruskal \n";
        cout << "4. Revenir au menu de choix d'actions\n";
        cout << "Votre choix : ";
        cin >> choix;

        switch (choix) {
            case 0: menuChoixAideAlgoGrapheNonOriente(); break;
            case 1: afficherPointsArticulation(); break;
            case 2: afficherIsthmes(); break;
            case 3:
                if(!d_affecterUnPoids)
                    messageErreurSansPoids("Kruskal");
                else
                    lancerKruskal();
                break;
            case 4: revenirSurMenuChoixApresSaisie(); return;
            default: cout << "Choix invalide.\n";
        }
    } while (choix != 6);
}


//////////////////////////////////////////////////////PUBLIC//////////////////////////////////////////////////////////////
void Interface::lancer()
{
    afficherAideUtilisationInterface();
    menuChoixSaisieGraphe();
}
