/**
 * \file calculMetrique.h
 * \author Dany Brégeon et Jeanne Villaneau
 * \version 1.0
 * \date 4 juillet 2018
 * \brief Calcul le kappa de Cohen
 *
 */
#ifndef CALCULMETRIQUE_H_INCLUDED
#define CALCULMETRIQUE_H_INCLUDED

#include <vector>

/*void kappa_pi_tableau(std::vector<std::vector<int> > & vAnnotObs,int nbannot, int nblignes, int nbclasses,
                      float & res_kappa);*/


/** \brief calcul le kappa de Cohen
 *
 * \param nblignes : le nombre d'observable
 * \param nbclasses : le nombre de classe
 * \param Nbareel : le nombre d'annotateur
 * \param vAnnotObs : les vecteurs représentant le tableau d'annotation
 * \return la valeur du kappa
 */
float kappaAP(int nblignes,int nbclasses,int Nbareel, std::vector<std::vector<int> > & vAnnotObs);

#endif // CALCULMETRIQUE_H_INCLUDED
