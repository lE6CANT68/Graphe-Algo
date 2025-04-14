#include <iostream>
#include "graphics.h"
#include "sommet.h"
#include "arc.h"
#include "graphe.h"
#include "DessinateurGraphe.h"
#include "ConstructeurGraphique.h"
#include "GestionnaireFichierGraphe.h"

using namespace std;

void test()
{
    int rayon;
    string couleur;
    bool oriente = true;
    cout << "Rayon ? "; cin >> rayon;
    cout << "Couleur (RED / GREEN / BLUE) ? "; cin >> couleur;

    sommet sd(1,"Paris");
    sommet s2(2,"Londres");
    sommet s3(3,"Madrid");
    sommet sf(4,"Rome");

    sd.setCoordonnees(Coordonnees(300, 300));
    s2.setCoordonnees(Coordonnees(300, 100));
    s3.setCoordonnees(Coordonnees(500, 150));
    sf.setCoordonnees(Coordonnees(500, 500));

    //arcDUnGraphe* a12 = new arcDUnGraphe(sd,s2);
    //arcDUnGraphe* a13 = new arcDUnGraphe(sd,s3);
    //arcDUnGraphe* a21 = new arcDUnGraphe(s2,sd);
    //arcDUnGraphe* a23 = new arcDUnGraphe(s2,s3);
    //arcDUnGraphe* a3f = new arcDUnGraphe(s3,sf);

    arcDUnGraphe* ap12 = new arcAvecPoids(sd,s2,2);
    arcDUnGraphe* ap13 = new arcAvecPoids(sd,s3,3);
    arcDUnGraphe* ap21 = new arcAvecPoids(s2,sd,5);
    arcDUnGraphe* ap23 = new arcAvecPoids(s2,s3,10);
    arcDUnGraphe* ap3f = new arcAvecPoids(s3,sf,0);

    graphe monGraphe;

    monGraphe.ajouterUnSommetAuGraphe(sd);
    monGraphe.ajouterUnSommetAuGraphe(s2);
    monGraphe.ajouterUnSommetAuGraphe(s3);
    monGraphe.ajouterUnSommetAuGraphe(sf);

    monGraphe.ajouterUnArcAuGraphe(ap12);
    monGraphe.ajouterUnArcAuGraphe(ap13);
    monGraphe.ajouterUnArcAuGraphe(ap21);
    monGraphe.ajouterUnArcAuGraphe(ap23);
    monGraphe.ajouterUnArcAuGraphe(ap3f);

    DessinateurGraphe d;
    d.dessinerGraphe(monGraphe, oriente, rayon, couleur);
    //monGraphe.afficherLeGraphe();
}

void tester()
{
    int larg = 600;
    int haut = 600;
    opengraphsize(larg,haut );

    test();
    getch();
    closegraph();
}

void test2()
{
    graphe g;

    DessinateurGraphe dessinateur;
    ConstructeurGraphique constructeur(g, dessinateur, 10, "BLUE");

    constructeur.lancerSaisie();
    //g.afficherLeGraphe();
}

void test3()
{
    graphe g;
    GestionnaireFichierGraphe gestionnaire;

    // Charger depuis fichier
   gestionnaire.chargerDepuisFichier("test1.txt", g);
   cout<<"Fichier bien charge !! \n";

    // Afficher
    g.afficherLeGraphe();

    // Sauvegarder
    gestionnaire.sauvegarderDansFichier("sauvegarde.txt", g);
    cout<<"Fichier bien sauvegarde !! \n";
}

int main()
{
    //tester();
    //test2();
    test3();

    return 0;
}
