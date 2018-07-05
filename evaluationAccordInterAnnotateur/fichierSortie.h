/**
 * \file fichierSortie.h
 * \author Dany Brégeon
 * \version 1.0
 * \date 4 juillet 2018
 * \brief Crée un fichier comportant les résultats
 *
 */
#ifndef FICHIERSORTIE_H_INCLUDED
#define FICHIERSORTIE_H_INCLUDED

#include <string>
#include <vector>
#include <map>

/** \brief crée un fichier comportant les résultats finaux (pourcentage de modification par rapport au nombre de classe, la valeur de la métrique et le nombre d'annotateurs)
 *
 * \param choixCorpus : les corpus choisi (opinion, émotion, coréférence...)
 * \param choixNbClasse : le nombre de classe
 * \param choixMetrique : le choix de la métrique (kappa, pi, alpha, alpha pondéré)
 * \param choixGold : le choix de la référence (idéale ou non)
 * \param nba : le nombre d'annotateurs
 * \param mapResultat : les résultats finaux (% de modification pour chaque nombre d'annotateurs et valeur de métrique)
 */
void fichierSortie (int choixCorpus, int choixNbClasse, std::string choixMetrique, int choixGold,
                   int nba, std::map<std::pair<int, float>, std::vector<float>> mapResultat);

#endif // FICHIERSORTIE_H_INCLUDED
