#ifndef AIDEALGORITHMIQUE_H
#define AIDEALGORITHMIQUE_H

#include <string>
#include <vector>

class AideAlgorithmique {
    private:

        void afficherTexteDansFenetre(const std::vector<std::string>& lignes, const std::string& titre);

        std::vector<std::string> getAideUtilisationInterface();
        std::vector<std::string> getTexteDijkstra();
        std::vector<std::string> getTexteDantzig();
        std::vector<std::string> getTexteTarjan();
        std::vector<std::string> getTexteKruskal();
        std::vector<std::string> getTexteOrdonnancement();
        std::vector<std::string> getTexteRang();
        std::vector<std::string> getTextePointsArticulation();
        std::vector<std::string> getTexteIsthmes();
        std::vector<std::string> getTexteGrapheReduit();

    public:
        AideAlgorithmique();

        void afficherAideInterface();

        void afficherDijkstra();
        void afficherDantzig();
        void afficherTarjan();
        void afficherOrdonnancement();
        void afficherRang();

        void afficherPointsArticulation();
        void afficherIsthmes();
        void afficherGrapheReduit();
};

#endif // AIDEALGORITHMIQUE_H
