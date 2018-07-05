/**
 * \file combinaison.h
 * \author Dany Brégeon
 * \version 1.0
 * \date 4 juillet 2018
 * \brief Calcul toutes les combinaisons de k parmi n annotateurs, en calculant leurs votes majoritaires et en les comparant à la référence pour obtenir des pourcentages de modification
 *
 */
#ifndef COMBINAISON_H_INCLUDED
#define COMBINAISON_H_INCLUDED

#include <vector>

/** \brief calcul toutes les combinaisons de k parmi n annotateurs, en calculant leurs votes majoritaires et en les comparant à la référence pour obtenir des pourcentages de modification
 *
 * \param vObsAnnot : les vecteurs représentant le tableau d'annotation
 * \param vPourcentageErreur : les pourcentages de modification pour chaque nombre d'annotateurs
 * \param choixGold : le choix de la référence (idéale ou non)
 */
void calculDifference(std::vector<std::vector<int>> & vObsAnnot,/*int nbobs,int nba,int nbc,*/ std::vector<float> *vPourcentageErreur, int choixGold);

#endif // COMBINAISON_H_INCLUDED
