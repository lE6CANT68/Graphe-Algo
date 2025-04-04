#include "graphe.h"
#include <algorithm>

using std::cout;
using std::endl;

// Constructeur de la classe graphe
graphe::graphe(vector<sommet> tabSommets, vector<arc*> tabArcs)
{
    d_sommets = tabSommets;
    for (const auto& a : tabArcs)
    {
        d_arcs.push_back(a);
    }
}

graphe::~graphe()
{
    for (auto a : d_arcs)
        delete a;
}


// M�thodes de la classe graphe
vector<sommet> graphe::renvoyerListeSommetsDuGraphe() const
{
    return d_sommets;
}

vector<arc*> graphe::renvoyerListeArcsDuGraphe() const
{
    return d_arcs;
}

void graphe::ajouterUnSommetAuGraphe(const sommet &s)
{
    d_sommets.push_back(s);
}

void graphe::ajouterUnArcAuGraphe(arc *a)
{
    d_arcs.push_back(a);
}

void graphe::supprimerUnSommetDuGraphe(const sommet &s)
{
    // Supprimer tous les arcs li�s � ce sommet
    d_arcs.erase(remove_if(d_arcs.begin(), d_arcs.end(), [&s](arc* a) {
        if (a->renvoyerSommetSource()->renvoyerIdentifiant() == s.renvoyerIdentifiant() ||
            a->renvoyerSommetDestination()->renvoyerIdentifiant() == s.renvoyerIdentifiant()) {
            delete a;
            return true;
        }
        return false;
    }), d_arcs.end());

    // Supprimer le sommet
    d_sommets.erase(remove_if(d_sommets.begin(), d_sommets.end(), [&s](const sommet& sm) { return sm.renvoyerIdentifiant() == s.renvoyerIdentifiant(); }), d_sommets.end());
}

void graphe::supprimerUnArcDuGraphe(arc* a)
{
    d_arcs.erase(remove_if(d_arcs.begin(), d_arcs.end(), [&a](arc* ap) { return ap == a; }), d_arcs.end());
    delete a;
}

void graphe::afficherLeGraphe()
{
    cout << "Sommets du graphe:" << endl;
    for (const auto& s : d_sommets)
    {
        cout << "Sommet ID: " << s.renvoyerIdentifiant() << ", Etiquette: " << s.renvoyerEtiquette() << endl;
    }
    cout << "\nArcs du graphe:" << endl;
    for (const auto& a : d_arcs)
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

void graphe::creeFsAPartirDuGraphe(const graphe& g, int*& fs)
{
    int nb_sommets = g.d_sommets.size();
    int nb_arcs = g.d_arcs.size();
    int taille_fs = nb_arcs + nb_sommets + 1; // Taille initiale

    fs = new int[taille_fs]; // Allocation mémoire pour le tableau fs

    // Initialiser le tableau avec des zéros
    for (int i = 0; i < taille_fs; ++i)
    {
        fs[i] = 0;
    }

    int index = 1; // Commencer à remplir à partir de l'index 1

    // Remplir fs avec les successeurs pour chaque sommet
    for (const auto& sommet : g.d_sommets)
    {
        bool hasSuccessor = false; // Indicateur pour savoir si le sommet a des successeurs

        // Chercher les successeurs de ce sommet
        for (const auto& arc : g.d_arcs)
        {
            if (arc->renvoyerSommetSource()->renvoyerIdentifiant() == sommet.renvoyerIdentifiant())
            {
                fs[index++] = arc->renvoyerSommetDestination()->renvoyerIdentifiant(); // Ajouter le successeur
                hasSuccessor = true; // Marquer que ce sommet a des successeurs
            }
        }

        // Ajouter un 0 apr�s les successeurs
        fs[index++] = 0; // Ajouter un 0 pour s�parer les successeurs
    }

    fs[0] = index - 1; // Stocke le dernier index utilisé
}

void graphe::creeAPSAPartirDeFs(int *fs, int *&aps) {
    // Compter le nombre de 0 dans FS (� partir de l'indice 1 jusqu'� fs[0])
    int count = 0;
    for (int i = 1; i <= fs[0]; i++) {
         if (fs[i] == 0)
             count++;
    }
    // Allocation de APS (avec APS[0] contenant le nombre de sommets)
    aps = new int[count + 1];
    aps[0] = count;

    int vertex = 1;
    aps[vertex] = 1; // Le premier sommet commence � l'indice 1 dans FS
    for (int i = 1; i <= fs[0]; i++) {
         if (fs[i] == 0 && vertex < count) {
             vertex++;
             aps[vertex] = i + 1;
         }
    }
}

void graphe::creeMatriceAdajenceAPartirDuGraphe(const graphe& g, int **&matAdajacence)
{
    int n = g.d_sommets.size();

    // Allocation de la matrice d'adjacence
    matAdajacence = new int*[n];
    for (int i = 0; i < n; ++i)
    {
        matAdajacence[i] = new int[n]{}; // Initialisation � 0
    }

    // Remplissage de la matrice d'adjacence
    for (const auto& arc : g.d_arcs)
    {
        int src = arc->renvoyerSommetSource()->renvoyerIdentifiant() - 1;
        int dest = arc->renvoyerSommetDestination()->renvoyerIdentifiant() - 1;

        arcAvecPoids* ap = dynamic_cast<arcAvecPoids*>(arc);
        if (ap)
            matAdajacence[src][dest] = ap->renvoyerPoidsArc(); // Stocker le poids
        else
            matAdajacence[src][dest] = 1; // Arc sans poids
    }
}

