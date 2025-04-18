#include "grapheoriente.h"
#include <queue>
#include <algorithm>

GrapheOriente::GrapheOriente(vector<sommet> tabSommets, vector<arcDUnGraphe*> tabArcs)
    : graphe(tabSommets, tabArcs) {}

std::vector<int> GrapheOriente::AlgorithmeDuRang(std::vector<int> d_rangs) {
    std::vector<sommet> sommets = renvoyerListeSommetsDuGraphe();
    std::vector<arcDUnGraphe*> arcs = renvoyerListeArcsDuGraphe();

    // Initialiser les rangs à -1 (non calculé)
    d_rangs.resize(sommets.size(), -1);


    // Calculer le degré d'entrée de chaque sommet
    std::vector<int> degreEntree(sommets.size(), 0);
    for (arcDUnGraphe* a : arcs) {
        int destId = a->renvoyerSommetDestination()->renvoyerIdentifiant();
        degreEntree[destId]++;
    }

    // pile pour le tri topologique
    std::queue<int> pile;

    // Ajouter les sommets de degré d'entrée 0
    for (size_t i = 0; i < sommets.size(); ++i) {
        if (degreEntree[i] == 0) {
            pile.push(i);
            d_rangs[i] = 0; // Les sommets sans prédécesseurs ont un rang de 0
        }
    }

    // Calculer les rangs
    while (!pile.empty()) {
        int sommetCourant = pile.front();
        pile.pop();

        // Parcourir tous les arcs pour trouver les successeurs
        for (arcDUnGraphe* a : arcs) {
            if (a->renvoyerSommetSource()->renvoyerIdentifiant() == sommetCourant) {
                int succId = a->renvoyerSommetDestination()->renvoyerIdentifiant();
                degreEntree[succId]--;

                // Mettre à jour le rang du successeur
                d_rangs[succId] = std::max(d_rangs[succId], d_rangs[sommetCourant] + 1);

                // Si le sommet n'a plus de prédécesseurs, l'ajouter à la pile
                if (degreEntree[succId] == 0) {
                    pile.push(succId);
                }
            }
        }
    }

    d_rangs.insert(d_rangs.begin(), sommets.size());
    return d_rangs;
}

/*void GrapheOriente::afficherRangs() const {
    std::cout << "Rangs des sommets :" << std::endl;
    std::cout<<"[";
    for (size_t i = 0; i < d_rangs.size(); ++i){
        std::cout<<d_rangs[i]<<" | ";
    }
    std::cout<<"]";

    std::vector<sommet> sommets = renvoyerListeSommetsDuGraphe();
    std::cout << "Rangs des sommets :" << std::endl;
    for (size_t i = 0; i < sommets.size(); ++i) {
        std::cout << "Sommet " << sommets[i].renvoyerIdentifiant()
                  << " (" << sommets[i].renvoyerEtiquette()
                  << ") : rang " << d_rangs[i] << std::endl;
    }
}*/



void dfsTarjan(int s, int** adj, int* tailles, int* num, int* ro, int* cfc,
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

    for (int i = 0; i < n; ++i) {
        std::cout << "Sommet " << i + 1 << " appartient à la composante C" << cfc[i] << std::endl;
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

void GrapheOriente::Dantzig(int** L, const int** C, int n) {
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

void GrapheOriente::Ordonnancement(int* fs, int* aps, int* durees) {
    int n = aps[0];
    std::vector<int> tot(n, 0);  // Dates au plus tôt
    std::vector<int> tard(n);    // Dates au plus tard
    std::vector<int> marge(n);   // Marges

    // Phase avant : calcul des dates au plus tôt
    for (int i = 0; i < n; ++i) {
        for (int k = aps[i]; fs[k] != 0; ++k) {
            int j = fs[k];
            if (tot[j] < tot[i] + durees[i])
                tot[j] = tot[i] + durees[i];
        }
    }

    int dureeTotale = *std::max_element(tot.begin(), tot.end());

    // Initialiser toutes les dates au plus tard à la durée totale du projet
    for (int i = 0; i < n; ++i)
        tard[i] = dureeTotale;

    // Phase arrière : calcul des dates au plus tard
    for (int i = n - 1; i >= 0; --i) {
        for (int k = aps[i]; fs[k] != 0; ++k) {
            int j = fs[k];
            if (tard[i] > tard[j] - durees[i])
                tard[i] = tard[j] - durees[i];
        }
    }

    std::cout << "\nDuree totale du projet : " << dureeTotale << " unites de temps.\n";
}


