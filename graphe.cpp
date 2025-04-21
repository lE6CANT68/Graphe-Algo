#include "graphe.h"
#include <algorithm>

using std::cout;
using std::endl;

graphe::graphe(): d_sommets{}, d_arcs{}
{}

// Constructeur de la classe graphe
graphe::graphe(vector<sommet> tabSommets, vector<arcDUnGraphe*> tabArcs)
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


// Methodes de la classe graphe
vector<sommet> graphe::renvoyerListeSommetsDuGraphe() const
{
    return d_sommets;
}

vector<sommet>& graphe::renvoyerListeSommetsDuGraphe()
{
    return d_sommets;
}

vector<arcDUnGraphe*> graphe::renvoyerListeArcsDuGraphe() const
{
    return d_arcs;
}

void graphe::ajouterUnSommetAuGraphe(const sommet &s)
{
    d_sommets.push_back(s);
}

void graphe::ajouterUnArcAuGraphe(arcDUnGraphe *a)
{
    d_arcs.push_back(a);
}

void graphe::supprimerUnSommetDuGraphe(const sommet &s)
{
    // Supprimer tous les arcs li�s � ce sommet
    d_arcs.erase(remove_if(d_arcs.begin(), d_arcs.end(), [&s](arcDUnGraphe* a) {
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

void graphe::supprimerUnArcDuGraphe(arcDUnGraphe* a)
{
    d_arcs.erase(remove_if(d_arcs.begin(), d_arcs.end(), [&a](arcDUnGraphe* ap) { return ap == a; }), d_arcs.end());
    delete a;
}

void graphe::viderGraphe() {
    d_sommets.clear();
    for (auto a : d_arcs) delete a;
    d_arcs.clear();
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
        // Chercher les successeurs de ce sommet
        for (const auto& arc : g.d_arcs)
        {
            if (arc->renvoyerSommetSource()->renvoyerIdentifiant() == sommet.renvoyerIdentifiant())
            {
                fs[index++] = arc->renvoyerSommetDestination()->renvoyerIdentifiant(); // Ajouter le successeur
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

void graphe::creeMatriceAdajenceAPartirDuGraphe(const graphe& g, int**& matAdajacence)
{
    int n = g.d_sommets.size();
    int m = g.d_arcs.size();

    // Allouer une matrice de (n+1) x (n+1) pour inclure la ligne/colonne 0
    matAdajacence = new int*[n + 1];
    for (int i = 0; i <= n; ++i) {
        matAdajacence[i] = new int[n + 1];
        for (int j = 0; j <= n; ++j)
            matAdajacence[i][j] = 0;
    }

    // Mettre le nombre de sommets et d’arcs dans [0][0] et [0][1]
    matAdajacence[0][0] = n;
    matAdajacence[0][1] = m;

    // Remplir la matrice à partir des arcs
    for (const auto& arc : g.d_arcs) {
        int src = arc->renvoyerSommetSource()->renvoyerIdentifiant();
        int dest = arc->renvoyerSommetDestination()->renvoyerIdentifiant();

        const arcAvecPoids* ap = dynamic_cast<arcAvecPoids*>(arc);
        if (ap)
            matAdajacence[src][dest] = ap->renvoyerPoidsArc();
        else
            matAdajacence[src][dest] = 1;
    }
}

void graphe::creerListeAdjacence(int**& adj, int*& tailles) const {
    int n = d_sommets.size();

    // 1. Compter le nombre de successeurs pour chaque sommet
    tailles = new int[n](); // tailles[i] = nombre de successeurs de i
    for (const auto& arc : d_arcs) {
        int u = arc->renvoyerSommetSource()->renvoyerIdentifiant() - 1;
        tailles[u]++;
    }

    // 2. Allouer le tableau de listes d'adjacence
    adj = new int*[n];
    for (int i = 0; i < n; ++i) {
        adj[i] = new int[tailles[i]];
    }

    // 3. Remplir les adjacences
    // On a besoin d'un compteur temporaire pour suivre la position de remplissage
    int* compteur = new int[n](); // initialisé à 0
    for (const auto& arc : d_arcs) {
        int u = arc->renvoyerSommetSource()->renvoyerIdentifiant() - 1;
        int v = arc->renvoyerSommetDestination()->renvoyerIdentifiant() - 1;
        adj[u][compteur[u]++] = v;
    }

    delete[] compteur;
}

bool graphe::estOriente() const
{
    for (const auto& arc : d_arcs) {
        int idSource = arc->renvoyerSommetSource()->renvoyerIdentifiant();
        int idDest = arc->renvoyerSommetDestination()->renvoyerIdentifiant();

        bool inverseExiste = false;
        for (const auto& autre : d_arcs) {
            if (autre->renvoyerSommetSource()->renvoyerIdentifiant() == idDest &&
                autre->renvoyerSommetDestination()->renvoyerIdentifiant() == idSource) {
                inverseExiste = true;
                break;
            }
        }

        if (!inverseExiste) return true;  // Si l'inverse n'existe pas, c'est orienté
    }
    return false;  // Tous les arcs ont leur inverse → non orienté
}

bool graphe::arcExisteDeja(int idSrc, int idDst) {
    for (const auto& a : d_arcs) {
        int srcId = a->renvoyerSommetSource()->renvoyerIdentifiant();
        int dstId = a->renvoyerSommetDestination()->renvoyerIdentifiant();

        // On interdit uniquement si exactement le même arc existe déjà
        if (srcId == idSrc && dstId == idDst)
            return true;
    }
    return false;
}

