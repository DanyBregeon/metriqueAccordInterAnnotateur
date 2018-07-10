/**
 * \file calculMetrique.h
 * \author Dany Brégeon et Jeanne Villaneau
 * \version 1.0
 * \date 4 juillet 2018
 * \brief Calcul des métriques d'évaluation de l'accord inter-annotateur
 *
 */

#ifndef CALCULMETRIQUE_H_INCLUDED
#define CALCULMETRIQUE_H_INCLUDED

//const int Max_obs=3000;//nb maximal d'observables
//const int Max_classes=10;//nb maximal de classes
//const int Max_annot=30;//nb maximal d'annotateurs

#include <vector>

/** \brief calcul le pi de Scott
 *
 * \param nblignes : le nombre d'observables
 * \param nbclasses : le nombre de classes
 * \param Nbareel : le nombre d'annotateurs
 * \param vObsAnnot : les vecteurs représentant le tableau d'annotation
 * \return la valeur du pi
 */
float piAP(int nblignes,int nbclasses,int Nbareel, std::vector<std::vector<int>> & vObsAnnot);

/** \brief calcul le kappa de Cohen
 *
 * \param nblignes : le nombre d'observables
 * \param nbclasses : le nombre de classes
 * \param Nbareel : le nombre d'annotateurs
 * \param vObsAnnot : les vecteurs représentant le tableau d'annotation
 * \return la valeur du kappa
 */
float kappaAP(int nblignes,int nbclasses,int Nbareel, std::vector<std::vector<int>> & vObsAnnot);

/** \brief calcul l'alpha de Krippendorff, pondéré ou non
 *
 * \param nb : nombre d'annotations = nba*nbobs si pas de données manquantes
 * \param C : tableau de coïncidence
 * \param pondere : 0 pour calculer l'alpha non pondéré, 1 pour l'alpha pondéré par une distance euclidienne
 * \return la valeur de l'alpha
 */
float alpha(float nb, std::vector<std::vector<float>> & C, int nbclasses, int pondere);

/** \brief calcul le tableau de coïncidence
 *
 * \param vObsAnnot : les vecteurs représentant le tableau d'annotation
 * \param nblignes : le nombre d'observables
 * \param nbannot : le nombre d'annotateurs
 * \param C : tableau de coïncidence
 * \param nbclasses : le nombre de classes
 * \param nb : nombre d'annotations = nba*nbobs si pas de données manquantes
 */
void coincidences(std::vector<std::vector<int>> & vObsAnnot,int nblignes,int nbannot, std::vector<std::vector<float>> & C,int nbclasses,float & nb);

#endif // CALCULMETRIQUE_H_INCLUDED
