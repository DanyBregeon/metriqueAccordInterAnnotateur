/**
 * \file lectureFichier.h
 * \author    Dany Brégeon et Jeanne Villaneau
 * \version   1.0
 * \date       4 juillet 2018
 * \brief       Lecture de fichier .csv
 *
 */

#ifndef LECTUREFICHIER_H_INCLUDED
#define LECTUREFICHIER_H_INCLUDED

#include <vector>

/** \brief va lire le fichier et récupérer toutes les informations
 *
 * \param nomfich : le chemin du fichier à lire
 * \param vAnnotObs : les vecteurs représentant le tableau d'annotation
 * \param Nbobs : le nombre d'observable
 * \param Nba : le nombre d'annotateur
 * \param Nbc : le nombre de classe
 * \return 1 si le fichier a bien été lu, 0 sinon
 */
int lire(std::string nomfich,std::vector<std::vector<int> > & vAnnotObs,int & Nbobs,int & Nba,int & Nbc);

/** \brief va choisir le fichier à lire par rapport au choix, puis va lire le fichier et récupérer toutes les informations
 * \param choix : le fichier à lire
 * \param vAnnotObs : les vecteurs représentant le tableau d'annotation
 * \param Nbobs : le nombre d'observable
 * \param Nba : le nombre d'annotateur
 * \param Nbc : le nombre de classe
 */
void choixTableau(int choix,std::vector<std::vector<int> > & vAnnotObs,int & Nbobs,int & Nba,int & Nbc);

#endif // LECTUREFICHIER_H_INCLUDED
