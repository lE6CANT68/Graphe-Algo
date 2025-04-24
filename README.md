# 🧠 Projet Graphe Algo 2025

## 🎯 Objectif du projet

Ce projet a pour but de développer une **application interactive en C++** permettant de **créer, manipuler, visualiser et analyser des graphes orientés et non orientés**, à travers une interface utilisateur conviviale et une série d'algorithmes classiques en théorie des graphes.

L’interface permet à l’utilisateur de :
- Créer un graphe via **clavier**, **fenêtre graphique interactive**, ou **chargement depuis un fichier**.
- Visualiser le graphe sur la **console** ou de manière **graphique avec couleurs et flèches**.
- Appliquer une **large palette d’algorithmes** selon la nature du graphe.

---

## ⚙️ Fonctionnalités principales

### 🛠 Création & Chargement
- Ajout de sommets (avec ou sans étiquettes)
- Ajout d’arcs (pondérés ou non)
- Génération aléatoire des coordonnées graphiques
- Chargement depuis fichier externe

### 👁️ Visualisation
- Affichage console structuré (sommets, arcs, poids)
- Affichage graphique dynamique (sommets colorés, arcs orientés)
- Affichage FS/APS et Matrice d'adjacence

### 📚 Algorithmes intégrés

#### 📌 Pour graphes orientés
- Tarjan (Composantes fortement connexes)
- Dijkstra (plus courts chemins)
- Dantzig (tous plus courts chemins)
- Ordonnancement (tâches, tot/tard/chemin critique)
- Rang des sommets

#### 🧩 Pour graphes non orientés
- Points d’articulation
- Isthmes
- Arbre recouvrant minimal (Kruskal)

---

## ✅ Pré-requis techniques

- **C++11 ou supérieur**
- **Code::Blocks ou g++**
- **Bibliothèque graphique BGI / WinBGIm**
- [Graphviz](https://graphviz.org/) (optionnel – pour la génération de diagrammes UML via Doxygen)

---

## 📁 Structure du projet

```
PROJET_GRAPHES_ET_ALGO/
│
├── arc.{h,cpp}
├── sommet.{h,cpp}
├── graphe.{h,cpp}
├── grapheoriente.{h,cpp}
├── graphenonoriente.{h,cpp}
├── Interface.{h,cpp}
├── DessinateurGraphe.{h,cpp}
├── ConstructeurGraphique.{h,cpp}
├── GestionnaireFichierGraphe.{h,cpp}
├── AideAlgorithmique.{h,cpp}
├── Coordonnees.{h,cpp}
├── graphics.h
└── main.cpp
```

---

## 🧪 Tests & Exemples

Plusieurs fichiers de tests ont été créés pour vérifier les algorithmes :
- `test_dijkstra.txt` – graphe orienté pondéré
- `test_ordonnancement.txt` – graphe de tâches avec dépendances
- `test_tarjan.txt` – graphe orienté avec composantes fortement connexes
- `test_non_oriente.txt` – graphe pour détection de points d'articulation et isthmes

---

## 👨‍💻 Auteurs

- **Mathieu Schellenbaum [Chef de projet & Développeur]**
- Contributions à l’interface, aux structures de graphes, aux algorithmes et à la gestion graphique.
- **Cantin Fanger [Développeur]**
- Contributions à la création des algorithmes et aux tests
- **Logan Pommier [Développeur]**
- Contributions à la création des algorithmes et aux tests
- **Samy Zéroual [Développeur]**
- Contributions à la création des algorithmes et aux tests

---

## 📌 Remarques

Le projet a été conçu en **programmation orientée objet** avec une séparation claire des responsabilités :
- `Interface` gère la logique utilisateur.
- `graphe`, `GrapheOriente`, `GrapheNonOriente` représentent les structures de données.
- `DessinateurGraphe` et `ConstructeurGraphique` gèrent la visualisation et la création graphique.

---

## 📄 Licence

Projet académique – librement réutilisable à des fins pédagogiques.
