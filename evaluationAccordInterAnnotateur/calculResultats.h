/**
 * \file calculResultats.h
 * \author Dany Brégeon
 * \version 1.0
 * \date 4 juillet 2018
 * \brief Calcul les résultats en faisant la moyenne et l'écart-type des pourcentages de modification par valeur de métrique proche
 *
 */
#ifndef CALCULRESULTATS_H_INCLUDED
#define CALCULRESULTATS_H_INCLUDED

#include <vector>
#include <map>

/** \brief calcul la prévalence des classes
 *
 * \param vPrevalence : vecteur ayant la prévalence de chaque classe pour une certaine valeur de métrique
 * \param valeurMetrique : la valeur de la métrique
 * \param vAnnotObs : les vecteurs représentant le tableau d'annotation
 */
void calculPrevalence(std::vector<std::pair<float, std::vector<float> > > & vPrevalence, float valeurMetrique,
                      std::vector<std::vector<int> > & vAnnotObs);

/** \brief calcul les résultats en faisant la moyenne et l'écart-type des pourcentages de modification par valeur de métrique proche
 *
 * \param mapResultat : les résultats finaux (% de modification pour chaque nombre d'annotateurs et valeur de métrique)
 * \param mapResultat2 : les résultats finaux avec le calcul de la moyenne et de l'écart-type
 * \param palierPas : la taille des palier pour lesquels les résultats seront regroupés
 * \param vPrevalence : vecteur ayant la prévalence de chaque classe pour une certaine valeur de métrique
 */
void resultatsPalier(std::map<std::pair<int, float>, std::vector<float> > & mapResultat,
                      std::map<std::pair<int, float>, std::vector<float> > & mapResultat2, float palierPas,
                      std::vector<std::pair<float, std::vector<float> > > & vPrevalence);

#endif // CALCULRESULTATS_H_INCLUDED
