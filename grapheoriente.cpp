#include "grapheoriente.h"
#include <queue>
#include <algorithm>

GrapheOriente::GrapheOriente(vector<sommet> tabSommets, vector<arcDUnGraphe*> tabArcs)
    : graphe(tabSommets, tabArcs) {}

void GrapheOriente::AlgorithmeDuRang(int*& rangs) {
    std::vector<sommet> sommets = renvoyerListeSommetsDuGraphe();
    std::vector<arcDUnGraphe*> arcs = renvoyerListeArcsDuGraphe();
    int n = sommets.size();

    // Allouer le tableau de rangs (+1 pour stocker la taille à l’indice 0)
    rangs = new int[n + 1];

    // Initialiser tous les rangs à -1
    for (int i = 0; i < n + 1; ++i)
        rangs[i] = -1;

    // Calculer le degré d’entrée
    std::vector<int> degreEntree(n, 0);
    for (arcDUnGraphe* a : arcs) {
        int destId = a->renvoyerSommetDestination()->renvoyerIdentifiant();
        degreEntree[destId]++;
    }

    // Pile (file) pour le tri topologique
    std::queue<int> pile;

    for (int i = 0; i < n; ++i) {
        if (degreEntree[i] == 0) {
            pile.push(i);
            rangs[i + 1] = 0;  // Rang 0 pour les sources
        }
    }

    // Parcours des sommets pour calculer les rangs
    while (!pile.empty()) {
        int sommetCourant = pile.front();
        pile.pop();

        for (arcDUnGraphe* a : arcs) {
            if (a->renvoyerSommetSource()->renvoyerIdentifiant() == sommetCourant) {
                int succId = a->renvoyerSommetDestination()->renvoyerIdentifiant();
                degreEntree[succId]--;

                // Mettre à jour le rang
                rangs[succId + 1] = std::max(rangs[succId + 1], rangs[sommetCourant + 1] + 1);

                if (degreEntree[succId] == 0)
                    pile.push(succId);
            }
        }
    }

    // Ajouter la taille du tableau à l’indice 0
    rangs[0] = n;
}

void GrapheOriente::dfsTarjan(int s, int** adj, int* tailles, int* num, int* ro, int* cfc,
               bool* enPileTarjan, int* pileTarjan, int& sommetPile, int& p, int& k)
{
    p++;
    num[s] = ro[s] = p;
    pileTarjan[sommetPile++] = s;
    enPileTarjan[s] = true;

    for (int i = 0; i < tailles[s]; ++i) {
        int succ = adj[s][i];
        if (num[succ] == 0) {
            dfsTarjan(succ, adj, tailles, num, ro, cfc, enPileTarjan, pileTarjan, sommetPile, p, k);
            ro[s] = std::min(ro[s], ro[succ]);
        } else if (enPileTarjan[succ]) {
            ro[s] = std::min(ro[s], num[succ]);
        }
    }

    if (ro[s] == num[s]) {
        k++;
        int v;
        do {
            v = pileTarjan[--sommetPile];
            enPileTarjan[v] = false;
            cfc[v] = k;
        } while (v != s);
    }
}



void GrapheOriente::trouverComposantesFortementConnexes() {
    int n = renvoyerListeSommetsDuGraphe().size();

    int** adj;
    int* tailles;
    creerListeAdjacence(adj, tailles);

    int* num = new int[n]{};
    int* ro = new int[n]{};
    int* cfc = new int[n]{};
    bool* enPileTarjan = new bool[n]{};
    int* pileTarjan = new int[n];
    int sommetPile = 0;

    int p = 0;
    int k = 0;

    for (int s = 0; s < n; ++s) {
        if (num[s] == 0) {
            dfsTarjan(s, adj, tailles, num, ro, cfc, enPileTarjan, pileTarjan, sommetPile, p, k);
        }
    }

    // Libération mémoire
    for (int i = 0; i < n; ++i)
        delete[] adj[i];
    delete[] adj;
    delete[] tailles;
    delete[] num;
    delete[] ro;
    delete[] cfc;
    delete[] enPileTarjan;
    delete[] pileTarjan;
}


void  GrapheOriente::Dijkstra(int* fs, int* aps, int** p, int s, int*& d, int*& pr) {
    int n = aps[0];   // Nombre de sommets
    int m = fs[0];    // Nombre d’arcs

    d = new int[n + 1];
    pr = new int[n + 1];
    int* inS = new int[n + 1]; // inS[i] = 1 si sommet i est encore à traiter

    // Initialisation des tableaux
    for (int i = 1; i <= n; i++) {
        d[i] = p[s][i];     // Distance initiale depuis s
        inS[i] = 1;         // Tous les sommets sont à traiter
        pr[i] = -1;         // Prédécesseurs non définis
    }

    d[s] = 0;
    pr[s] = 0;
    inS[s] = 0; // Sommet source retiré de l’ensemble S
    int ind = n - 1; // Nombre de sommets à traiter

    while (ind > 0) {
        int m = MAXPOIDS;
        int j = -1;

        // Recherche du sommet j de S ayant la distance minimale
        for (int i = 1; i <= n; i++) {
            if (inS[i] == 1 && d[i] < m) {
                m = d[i];
                j = i;
            }
        }

        if (m == MAXPOIDS) break; // Plus de sommets atteignables

        inS[j] = 0; // Marquer comme traité
        ind--;

        int k = aps[j]; // Parcours des successeurs de j
        while (fs[k] != 0) {
            int succ = fs[k];
            if (inS[succ] == 1) {
                int v = d[j] + p[j][succ]; // Distance via j
                if (v < d[succ]) {
                    d[succ] = v;
                    pr[succ] = j;
                }
            }
            k++;
        }
    }

    delete[] inS;
}
// Fonction qui implémente l'algorithme de Dantzig
// L : matrice des plus courts chemins (initialement copie de C)
// C : matrice des coûts directs (dite matrice initiale)
// n : nombre de sommets (en supposant indices de 1 à n)

void GrapheOriente::Dantzig(int** &L, const int** C, int n) {
    // On considère que pour k=1 la matrice L est déjà initialisée avec C.
    // Ensuite, on ajoute progressivement les sommets intermédiaires k+1, pour k allant de 1 à n-1.
    for (int k = 1; k < n; k++) {
        // 1. Mise à jour de la colonne pour le sommet k+1
        for (int i = 1; i <= n; i++) {
            int nouvelleDistance = INF;
            // Pour chaque j de 1 à k, on cherche le minimum de L(i,j) + C(j,k+1)
            for (int j = 1; j <= k; j++) {
                if (L[i][j] != INF && C[j][k+1] != INF &&
                    L[i][j] + C[j][k+1] < nouvelleDistance) {
                    nouvelleDistance = L[i][j] + C[j][k+1];
                }
            }
            L[i][k+1] = nouvelleDistance;
        }

        // 2. Mise à jour de la ligne pour le sommet k+1
        for (int i = 1; i <= n; i++) {
            int nouvelleDistance = INF;
            // Pour chaque j de 1 à k, on calcule le minimum de C(k+1,j) + L(j,i)
            for (int j = 1; j <= k; j++) {
                if (C[k+1][j] != INF && L[j][i] != INF &&
                    C[k+1][j] + L[j][i] < nouvelleDistance) {
                    nouvelleDistance = C[k+1][j] + L[j][i];
                }
            }
            L[k+1][i] = nouvelleDistance;
        }

        // 3. Mise à jour de la matrice centrale avec le nouvel indice k+1
        for (int i = 1; i <= n; i++) {
            for (int j = 1; j <= n; j++) {
                if (L[i][k+1] != INF && L[k+1][j] != INF &&
                    L[i][k+1] + L[k+1][j] < L[i][j]) {
                    L[i][j] = L[i][k+1] + L[k+1][j];
                }
            }
        }
    }
}

void GrapheOriente::Ordonnancement(int* fs, int* aps, int* durees, int*& tot, int*& tard, int*& marge, int& dureeTotale)
{
    int n = aps[0];

    tot = new int[n];
    tard = new int[n];
    marge = new int[n];

    for (int i = 0; i < n; ++i) tot[i] = 0;

    // Phase avant
    for (int i = 0; i < n; ++i) {
        for (int k = aps[i]; fs[k] != 0; ++k) {
            int j = fs[k];
            if (tot[j] < tot[i] + durees[i])
                tot[j] = tot[i] + durees[i];
        }
    }

    dureeTotale = *std::max_element(tot, tot + n);

    for (int i = 0; i < n; ++i) tard[i] = dureeTotale;

    // Phase arrière
    for (int i = n - 1; i >= 0; --i) {
        for (int k = aps[i]; fs[k] != 0; ++k) {
            int j = fs[k];
            if (tard[i] > tard[j] - durees[i])
                tard[i] = tard[j] - durees[i];
        }
    }

    for (int i = 0; i < n; ++i)
        marge[i] = tard[i] - tot[i];
}

