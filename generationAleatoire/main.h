#ifndef MAIN_H_INCLUDED
#define MAIN_H_INCLUDED

#include <vector>

/** \brief affiche le tableau d'annotation
 *
 * \param vAnnotObs : les vecteurs repr�sentant le tableau d'annotation
 */
void affichage(std::vector<std::vector<int> > & vAnnotObs);

/** \brief cr�e des fichiers .csv avec le nombre d'annotateurs, d'observables, de classes ainsi que le tableau d'annotation. Ces fichiers peuvent etre utilis�s en entr�e par le programme "evaluationAccordInterAnnotateur"
 *
 * \param nba : le nombre d'annotateur
 * \param vAnnotObs : les vecteurs repr�sentant le tableau d'annotation
 * \param ss : le nom du fichier cr�e
 */
void ecrireFichier(int nba, std::vector<std::vector<int> > & vAnnotObs, std::string ss);

/** \brief g�n�ration al�atoire qui consiste � changer al�atoirement les annotations une � une
 *
 * \param vPasChoisi : toutes lse annotations pas encore modifi�es
 * \param vAnnotObs : les vecteurs repr�sentant le tableau d'annotation
 */
void generationAleatoire1(std::vector<std::pair<int,int> > & vPasChoisi, std::vector<std::vector<int> > & vAnnotObs);

/** \brief g�n�ration al�atoire qui consiste � changer les annotations une � une par annotateur
 *
 * \param vAnnotObs : les vecteurs repr�sentant le tableau d'annotation
 */
void generationAleatoire2(std::vector<std::vector<int> > & vAnnotObs);

/** \brief g�n�ration al�atoire qui consiste � changer les annotations une � une par observable
 *
 * \param vAnnotObs : les vecteurs repr�sentant le tableau d'annotation
 */
void generationAleatoire3(std::vector<std::pair<int,int> > & vPasChoisi, std::vector<std::vector<int> > & vAnnotObs);


/** \brief permet de modifier la pr�valence des classes sans changer p1
 *
 * \param vAnnotObs : les vecteurs repr�sentant le tableau d'annotation
 * \param classe1 : la classe majoritaire
 * \param classe2 : la classe la moins majoritaire
 */
void testPrevalence(std::vector<std::vector<int> > & vAnnotObs, int classe1, int classe2, int taille);

/** \brief �tablit les probabilit�s de tomber sur telle ou telle classe lorq'un annotateur fait une erreur en fonction des erreurs des vrais annotateurs
 *
 * \param tNbOcc : le nombre d'annotation de telle classe pour tel observable
 * \param voteMajoritaire : le vote majoritaire de chaque observable
 * \param probaClasseErreur : la probabilit� de tomber sur telle classe en cas d'erreur sur tel observable
 * \param totalPossibiliteClasse : le nombre total de possibilit� par observable
 */
void probaErreurAnnotateur(std::vector<std::vector<int> > & tNbOcc, std::vector<int> & voteMajoritaire, std::vector<std::vector<int> > & probaClasseErreur, std::vector<int> & totalPossibiliteClasse);


/** \brief permet de calculer la r�partition de p1
 *
 * \param nbNewAnnot : le nombre d'annotateur
 * \param nbErreurObs : le nombre d'erreur par observable
 */
void repartitionP1(int nbNewAnnot, std::vector<int> & nbErreurObs);

#endif // MAIN_H_INCLUDED
