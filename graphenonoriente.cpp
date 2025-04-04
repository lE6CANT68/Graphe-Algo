#include "graphenonoriente.h"
#include "arc.h"
#include "sommet.h"
#include <vector>
#include <utility>
#include <algorithm>
#include <queue>

using std::vector;
using std::pair;
using std::make_pair;
using std::sort;

/*------------------------------------------------------------------------
 * Fonction auxiliaire : construit la liste d'adjacence pour un graphe non orienté.
 * Pour chaque arc, on ajoute les deux directions.
 *------------------------------------------------------------------------*/
static vector<vector<int>> construireListeAdjacence(const graphe& g) {
    int n = g.renvoyerListeSommetsDuGraphe().size();
    vector<vector<int>> adj(n);
    vector<arc*> listeArcs = g.renvoyerListeArcsDuGraphe();
    for (arc* a : listeArcs) {
        int u = a->renvoyerSommetSource()->renvoyerIdentifiant();
        int v = a->renvoyerSommetDestination()->renvoyerIdentifiant();
        // Ajout de l'arête dans les deux sens
        adj[u].push_back(v);
        adj[v].push_back(u);
    }
    return adj;
}

/*------------------------------------------------------------------------
 * Fonction DFS utilisée pour détecter les points d'articulation et les ponts.
 *------------------------------------------------------------------------*/
static void explorerAP(int u, int parent, int& temps,
                       const vector<vector<int>>& adj,
                       vector<bool>& visite,
                       vector<int>& decouverte,
                       vector<int>& bas,
                       vector<bool>& pointArticulation,
                       vector<pair<int,int>>& ponts) {
    visite[u] = true;
    decouverte[u] = bas[u] = ++temps;
    int enfants = 0;
    for (int v : adj[u]) {
        if (!visite[v]) {
            enfants++;
            explorerAP(v, u, temps, adj, visite, decouverte, bas, pointArticulation, ponts);
            bas[u] = std::min(bas[u], bas[v]);
            // Si u n'est pas racine et que bas[v] >= decouverte[u], u est un point d'articulation.
            if (parent != -1 && bas[v] >= decouverte[u])
                pointArticulation[u] = true;
            // Si bas[v] > decouverte[u], l'arête (u,v) est un pont.
            if (bas[v] > decouverte[u])
                ponts.push_back(make_pair(u, v));
        } else if (v != parent) {
            bas[u] = std::min(bas[u], decouverte[v]);
        }
    }
    // Pour une racine, s'il a plus d'un enfant, c'est un point d'articulation.
    if (parent == -1 && enfants > 1)
        pointArticulation[u] = true;
}

/*------------------------------------------------------------------------
 * Constructeur de GrapheNonOriente.
 *------------------------------------------------------------------------*/
GrapheNonOriente::GrapheNonOriente(const vector<sommet>& listeSommets, const vector<arc*>& listeArcs)
    : graphe(listeSommets, listeArcs)
{
}

/*------------------------------------------------------------------------
 * Méthode pour trouver les points d'articulation du graphe.
 *------------------------------------------------------------------------*/
vector<int> GrapheNonOriente::trouverPointsDArticulation() {
    vector<sommet> listeSommets = renvoyerListeSommetsDuGraphe();
    int n = listeSommets.size();
    vector<vector<int>> adj = construireListeAdjacence(*this);
    vector<bool> visite(n, false);
    vector<int> decouverte(n, 0);
    vector<int> bas(n, 0);
    vector<bool> pointArticulation(n, false);
    vector<pair<int,int>> ponts; // Calculé ici mais non utilisé dans cette méthode
    int temps = 0;
    for (int i = 0; i < n; i++) {
        if (!visite[i])
            explorerAP(i, -1, temps, adj, visite, decouverte, bas, pointArticulation, ponts);
    }
    vector<int> pointsArticul;
    for (int i = 0; i < n; i++) {
        if (pointArticulation[i])
            pointsArticul.push_back(i);
    }
    return pointsArticul;
}

/*------------------------------------------------------------------------
 * Méthode pour trouver les ponts (isthmes) du graphe.
 *------------------------------------------------------------------------*/
vector<pair<int, int>> GrapheNonOriente::trouverIsthmies() {
    vector<sommet> listeSommets = renvoyerListeSommetsDuGraphe();
    int n = listeSommets.size();
    vector<vector<int>> adj = construireListeAdjacence(*this);
    vector<bool> visite(n, false);
    vector<int> decouverte(n, 0);
    vector<int> bas(n, 0);
    vector<bool> pointArticulation(n, false);
    vector<pair<int,int>> ponts;
    int temps = 0;
    for (int i = 0; i < n; i++) {
        if (!visite[i])
            explorerAP(i, -1, temps, adj, visite, decouverte, bas, pointArticulation, ponts);
    }
    return ponts;
}

/*------------------------------------------------------------------------
 * Structures et fonctions auxiliaires pour l'algorithme de Kruskal.
 *------------------------------------------------------------------------*/

// Structure représentant un sous-ensemble pour l'union-find.
struct SousEnsemble {
    int parent;
    int rang;
};

// Fonction pour trouver le parent d'un élément avec compression de chemin.
static int trouverParent(vector<SousEnsemble>& sousEnsembles, int i) {
    if (sousEnsembles[i].parent != i)
        sousEnsembles[i].parent = trouverParent(sousEnsembles, sousEnsembles[i].parent);
    return sousEnsembles[i].parent;
}

// Fonction pour unir deux sous-ensembles.
static void unirSousEnsembles(vector<SousEnsemble>& sousEnsembles, int x, int y) {
    int parentX = trouverParent(sousEnsembles, x);
    int parentY = trouverParent(sousEnsembles, y);
    if (sousEnsembles[parentX].rang < sousEnsembles[parentY].rang) {
        sousEnsembles[parentX].parent = parentY;
    } else if (sousEnsembles[parentX].rang > sousEnsembles[parentY].rang) {
        sousEnsembles[parentY].parent = parentX;
    } else {
        sousEnsembles[parentY].parent = parentX;
        sousEnsembles[parentX].rang++;
    }
}

// Structure représentant une arête.
struct Arete {
    int u;
    int v;
    int poids;
};

// Comparateur pour trier les arêtes par ordre croissant de poids.
static bool comparerAretes(const Arete& a, const Arete& b) {
    return a.poids < b.poids;
}

/*------------------------------------------------------------------------
 * Méthode pour calculer l'arbre recouvrant minimal (MST) via l'algorithme de Kruskal.
 *------------------------------------------------------------------------*/
graphe* GrapheNonOriente::arbreRecouvrantMinimal() {
    vector<sommet> listeSommets = renvoyerListeSommetsDuGraphe();
    int n = listeSommets.size();
    vector<arc*> listeArcs = renvoyerListeArcsDuGraphe();

    // Construction de la liste des arêtes avec leur poids.
    vector<Arete> listeAretes;
    for (arc* a : listeArcs) {
        int u = a->renvoyerSommetSource()->renvoyerIdentifiant();
        int v = a->renvoyerSommetDestination()->renvoyerIdentifiant();
        int poids = 1; // Poids par défaut
        // Si l'arc est pondéré, récupérer le poids.
        arcAvecPoids* arcPondere = dynamic_cast<arcAvecPoids*>(a);
        if (arcPondere)
            poids = arcPondere->renvoyerPoidsArc();
        // Pour éviter de compter deux fois une même arête dans un graphe non orienté.
        if (u <= v)
            listeAretes.push_back({u, v, poids});
    }
    sort(listeAretes.begin(), listeAretes.end(), comparerAretes);

    // Initialisation des sous-ensembles pour l'union-find.
    vector<SousEnsemble> sousEnsembles(n);
    for (int i = 0; i < n; i++) {
        sousEnsembles[i].parent = i;
        sousEnsembles[i].rang = 0;
    }

    vector<arc*> arcsMST;
    int nbArcsSelectionnes = 0;
    int indexArete = 0;
    while (nbArcsSelectionnes < n - 1 && indexArete < listeAretes.size()) {
        Arete areteCourante = listeAretes[indexArete++];
        int parentU = trouverParent(sousEnsembles, areteCourante.u);
        int parentV = trouverParent(sousEnsembles, areteCourante.v);
        if (parentU != parentV) {
            // Recherche de l'arc d'origine correspondant à cette arête.
            arc* arcTrouve = nullptr;
            for (arc* orig : listeArcs) {
                int uOrig = orig->renvoyerSommetSource()->renvoyerIdentifiant();
                int vOrig = orig->renvoyerSommetDestination()->renvoyerIdentifiant();
                if ((uOrig == areteCourante.u && vOrig == areteCourante.v) ||
                    (uOrig == areteCourante.v && vOrig == areteCourante.u)) {
                    arcTrouve = orig;
                    break;
                }
            }
            if (arcTrouve != nullptr)
                arcsMST.push_back(arcTrouve);
            unirSousEnsembles(sousEnsembles, parentU, parentV);
            nbArcsSelectionnes++;
        }
    }
    // Création d'un nouveau graphe représentant l'arbre recouvrant minimal.
    graphe* arbreMST = new graphe(listeSommets, arcsMST);
    return arbreMST;
}
