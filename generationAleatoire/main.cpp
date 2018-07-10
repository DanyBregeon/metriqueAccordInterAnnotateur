/**
 * \file main.cpp
 * \author Dany Brégeon
 * \version 1.0
 * \date 4 juillet 2018
 * \brief Génération aléatoire de données
 *
 */
#include <iostream>
#include <vector>
#include <cstring>
#include <cstdlib>
#include <ctime>
#include <cmath>
#include <string>
#include <algorithm>
#include <fstream>
#include <cstdio>
#include <sstream>
#include <ctime>
#include <stdio.h>
#include <io.h>
#include "calculMetrique.h"
#include "lectureFichier.h"


using namespace std;

int nba = 25; /**< \brief le nombre d'annotateurs */
int nbobs = 100; /**< \brief le nombre d'observables */
int nbc = 3; /**< \brief le nombre de classes */

int choixClasseErreurParRapportAuDonneesrelles = 0;
int choixIntervalleErreurAnnotateur = 0;
//int choixCalculP1 = 0;
int nbAnnotParGeneration = 9;
int choixGenerationAleatoire = 0;
int choixCalculRepartitionP1DonneesGenerees = 0;

/** \brief affiche le tableau d'annotation
 *
 * \param vAnnotObs : les vecteurs représentant le tableau d'annotation
 */
void affichage(vector<vector<int>> & vAnnotObs){
    for (int i=0;i<nba;i++) {
        for (int j=0;j<nbobs;j++)
            printf("%3d",vAnnotObs[i][j]);
        cout << endl;
    }
}

/** \brief crée des fichiers .csv avec le nombre d'annotateurs, d'observables, de classes ainsi que le tableau d'annotation. Ces fichiers peuvent etre utilisés en entrée par le programme "evaluationAccordInterAnnotateur"
 *
 * \param nba : le nombre d'annotateur
 * \param vAnnotObs : les vecteurs représentant le tableau d'annotation
 * \param ss : le nom du fichier crée
 */
void ecrireFichier(int nba, vector<vector<int>> & vAnnotObs, string ss){
    ofstream file;
    file.open(ss);

    file << "Nbannotateurs=" << nba << endl;
    file << "Nbclasses=" << nbc << endl;
    file << "Nbobservables=" << nbobs << endl;

    for(int i=0; i<nba; i++){
        for(int j=0; j<nbobs; j++){
            if(vAnnotObs[i][j] == -1){
                file << "." << "\t";
            }else{
                file << vAnnotObs[i][j] << "\t";
            }
        }
        file << endl;
    }
}

/** \brief génération aléatoire qui consiste à changer aléatoirement les annotations une à une
 *
 * \param vPasChoisi : toutes lse annotations pas encore modifiées
 * \param vAnnotObs : les vecteurs représentant le tableau d'annotation
 */
void generationAleatoire1(vector<pair<int,int>> & vPasChoisi, vector<vector<int>> & vAnnotObs){
    srand(time(0));
    int it = 0;
    while(vPasChoisi.size() > 0){
        cout << vPasChoisi.size() << endl;
        int rng = rand() % vPasChoisi.size();
        pair<int,int> *p = &vPasChoisi[rng];
        vAnnotObs[p->first][p->second] = rand() % nbc;
        vPasChoisi.erase(vPasChoisi.begin()+ rng);

        ostringstream ss;

            //cout << "i: " << i << " j: " << j << " " << (i*nbobs + j) << endl;
            ss << "generationAleatoireDonneesReelles/generationAleatoire" << it << ".csv";

            ecrireFichier(nba, vAnnotObs, ss.str());
            /*ofstream file;
            file.open(ss.str());

            file << "Nbannotateurs=" << nba << endl;
            file << "Nbclasses=" << nbc << endl;
            file << "Nbobservables=" << nbobs << endl;

            for(int i=0; i<nba; i++){
                for(int j=0; j<nbobs; j++){
                    if(vAnnotObs[i][j] == -1){
                        file << "." << "\t";
                    }else{
                        file << vAnnotObs[i][j] << "\t";
                    }
                }
                file << endl;
            }*/
            it++;
    }
}

/** \brief génération aléatoire qui consiste à changer les annotations une à une par annotateur
 *
 * \param vAnnotObs : les vecteurs représentant le tableau d'annotation
 */
void generationAleatoire2(vector<vector<int>> & vAnnotObs){
    for (int i=0;i<nba;i++){
        for (int j=0;j<nbobs;j++){
            //for (int i=0;i<nba;i++){
                vAnnotObs[i][j] = rand() % nbc;
                //vObsAnnot[j][i] = vAnnotObs[i][j];
                  //  float valeurMetrique;
                    //valeurMetrique = kappaAP(nbobs, nbc, nba, vObsAnnot);
                    //cout << "valeurMetrique: " << valeurMetrique;

                ostringstream ss;

                //cout << "i: " << i << " j: " << j << " " << (i*nbobs + j) << endl;
                ss << "generationAleatoireDonneesReelles3/generationAleatoire" << (i*nbobs + j) << ".csv";

                ecrireFichier(nba, vAnnotObs, ss.str());
            //}
        }
    }
}

/** \brief génération aléatoire qui consiste à changer les annotations une à une par observable
 *
 * \param vAnnotObs : les vecteurs représentant le tableau d'annotation
 */
void generationAleatoire3(vector<pair<int,int>> & vPasChoisi, vector<vector<int>> & vAnnotObs){
    srand(time(0));
    int tChoisi[nbobs][nba];
    for (int j=0;j<nbobs;j++){
        for (int i=0;i<nba;i++){
            tChoisi[i][j]=0;
        }
    }
    int obs = 0;
    int ann = 0;
    for(int occ=0; occ < (nba*nbobs); occ++){
        int rng = rand() % vPasChoisi.size();
        pair<int,int> *p = &vPasChoisi[rng];
        vPasChoisi.erase(vPasChoisi.begin()+ rng);
        /*while(tChoisi[p->first][p->second]==1){
            rng = rand() % vPasChoisi.size();
            p = &vPasChoisi[rng];
            vPasChoisi.erase(vPasChoisi.begin()+ rng);
        }*/

        vAnnotObs[p->first][p->second] = rand() % nbc;
        //vPasChoisi.erase(vPasChoisi.begin()+ rng);
        tChoisi[p->first][p->second]=1;

        if(tChoisi[ann][obs]==0){
            vAnnotObs[ann][obs] = rand() % nbc;
            tChoisi[ann][obs]=1;
        }
        if(ann<nba-1){
            ann++;
        }else{
            ann=0;
            obs++;
        }
        cout << endl;
        ostringstream ss;

        //cout << "i: " << i << " j: " << j << " " << (i*nbobs + j) << endl;
        ss << "generationAleatoire4/generationAleatoire" << occ << ".csv";

        ecrireFichier(nba, vAnnotObs, ss.str());
    }
}

/*
void ajouterNouveauAnnotateur(vector<vector<int>> & vAnnotObs, vector<float> & pourcentageErreur, vector<int> & voteMajoritaire){
    int nbNewAnnot = nba;
    nba += nbNewAnnot;
    vAnnotObs.resize(nba);
    for(int i=0; i<nba; i++){
        vAnnotObs[i].resize(nbobs);
    }

    for(int ann=nba - nbNewAnnot; ann<nba; ann++){
        for(int obs=0; obs<nbobs; obs++){
            //on compare un nombre aleatoire entre 0 et 100 avec le pourcentage d'erreur
            int rng = rand() % 101;
            if(rng < pourcentageErreur[obs]){
                vAnnotObs[ann][obs]= rand() % nbc;
            }else{
                vAnnotObs[ann][obs]=voteMajoritaire[obs];
            }
        }
    }
}

void remplacerParNouveauAnnotateur(vector<vector<int>> & vAnnotObs, vector<float> & pourcentageErreur, vector<int> & voteMajoritaire){
    for(int ann=0; ann<nba; ann++){
        for(int obs=0; obs<nbobs; obs++){
            //on compare un nombre aleatoire entre 0 et 100 avec le pourcentage d'erreur
            int rng = rand() % 101;
            if(rng < pourcentageErreur[obs]){
                vAnnotObs[ann][obs]= rand() % nbc;
            }else{
                //vAnnotObs[ann][obs]=voteMajoritaire[obs];
            }
        }
    }
}
*/

/** \brief permet de modifier la prévalence des classes sans changer p1
 *
 * \param vAnnotObs : les vecteurs représentant le tableau d'annotation
 * \param classe1 : la classe majoritaire
 * \param classe2 : la classe la moins majoritaire
 */
void testPrevalence(vector<vector<int>> & vAnnotObs, int classe1, int classe2, int taille){
    for(int ann=0; ann<nba; ann++){
        for(int obs=0; obs<taille; obs++){
            if(vAnnotObs[ann][obs]==classe1){
                vAnnotObs[ann][obs]=classe2;
            }else if(vAnnotObs[ann][obs]==classe2){
                vAnnotObs[ann][obs]=classe1;
            }
        }
    }
}

/** \brief établit les probabilités de tomber sur telle ou telle classe lorq'un annotateur fait une erreur en fonction des erreurs des vrais annotateurs
 *
 * \param tNbOcc : le nombre d'annotation de telle classe pour tel observable
 * \param voteMajoritaire : le vote majoritaire de chaque observable
 * \param probaClasseErreur : la probabilité de tomber sur telle classe en cas d'erreur sur tel observable
 * \param totalPossibiliteClasse : le nombre total de possibilité par observable
 */
void probaErreurAnnotateur(vector<vector<int>> & tNbOcc, vector<int> & voteMajoritaire, vector<vector<int>> & probaClasseErreur, vector<int> & totalPossibiliteClasse){
    for(int obs=0; obs<nbobs; obs++){
        for(int c=0; c<nbc; c++){
            if(c != voteMajoritaire[obs]){
                if(tNbOcc[obs][c]==0){
                    probaClasseErreur[obs][c] = totalPossibiliteClasse[obs] + 1;
                    totalPossibiliteClasse[obs] += 1;
                }else{
                    probaClasseErreur[obs][c] = totalPossibiliteClasse[obs] + tNbOcc[obs][c]*2;
                    totalPossibiliteClasse[obs] += tNbOcc[obs][c]*2;
                }
            }else{
                probaClasseErreur[obs][c]=0;
            }
            cout << c << " : " << probaClasseErreur[obs][c] << "(" << tNbOcc[obs][c] <<")        ";
        }
        cout << "   / " << totalPossibiliteClasse[obs] << endl;
    }
}


/** \brief permet de calculer la répartition de p1
 *
 * \param nbNewAnnot : le nombre d'annotateur
 * \param nbErreurObs : le nombre d'erreur par observable
 */
void repartitionP1(int nbNewAnnot, vector<int> & nbErreurObs){
    int taille = ((float)(nbc-1)/(float)nbc)*(float)nbNewAnnot + 1;
        int testOcc[taille];
        for(int i=0; i<taille; i++){
            testOcc[i]=0;
        }
        for(int obs=0; obs<nbobs; obs++){
            testOcc[nbErreurObs[obs]]++;
        }
        cout << "nombre d'observables ayant X erreurs : " << endl;
        for(int i=0; i<taille; i++){
            cout << i << " erreurs : " << testOcc[i] << endl;
        }
        //avec palier
        /*int testOcc[11];
        for(int i=0; i<11; i++){
            testOcc[i]=0;
        }
        for(int obs=0; obs<nbobs; obs++){
            int i=0;
            while(pourcentageErreur[obs] > i*10){
                i++;
            }
            testOcc[i]++;
        }
        for(int i=0; i<11; i++){
            cout << i*10 << " " << testOcc[i] << endl;
        }*/
}

/** \brief entrée du programme
 * \return EXIT_SUCCESS - Arrêt normal du programme.
 */
int main()
{
    srand(time(NULL));
    vector<vector<int>> vAnnotObs;
    vector<vector<int>> vObsAnnot;
    vector<pair<int,int>> vPasChoisi;

    //configuration des paramètres par l'utilisateur
    string reponse;
    do{
        cout << "1 pour activer le mode avancee, 0 sinon : ";
        cin >> reponse;
    }while(reponse != "0" && reponse != "1");

    if(reponse.compare("1")==0){ //mode avancée
        do{
            cout << "choix de la methode de generation aleatoire : 0 par defaut, 1 aleatoire, 2 aleatoire annot par annot, 3 aleatoire obs par obs : ";
            cin >> reponse;
        }while(reponse != "0" && reponse != "1" && reponse != "2" && reponse != "3");
        choixGenerationAleatoire = atoi(reponse.c_str());
        if(choixGenerationAleatoire==0){
            do{
                cout << "choix intervalle du nombre d'erreurs des annotateurs : 0 pour minimal, 1 pour maximal : ";
                cin >> reponse;
            }while(reponse != "0" && reponse != "1");
            choixIntervalleErreurAnnotateur = atoi(reponse.c_str());
            do{
                cout << "choix erreurs des annotateurs : 0 pour aleatoire, 1 par rapport aux erreurs des annotateurs reelles : ";
                cin >> reponse;
            }while(reponse != "0" && reponse != "1");
            choixClasseErreurParRapportAuDonneesrelles = atoi(reponse.c_str());
            do{
                cout << "choix calcul de la repartition du nombre d'erreurs par observable : 0 pour non, 1 pour oui : ";
                cin >> reponse;
            }while(reponse != "0" && reponse != "1");
            choixCalculRepartitionP1DonneesGenerees = atoi(reponse.c_str());
            /*do{
                cout << "choix de la maniere dont p1 va inluencer les resultats : 0 ou 1 : ";
                cin >> reponse;
            }while(reponse != "0" && reponse != "1");
            choixCalculP1 = atoi(reponse.c_str());*/
            do{
                cout << "choix du nombre d'annotateurs par generation aleatoire : ";
                cin >> nbAnnotParGeneration;
            }while(nbAnnotParGeneration < 1);
        }

    }

    //initialise les vecteurs avec les données
    string choixFichier;
    do{
        cout << "Chemin du fichier ou dossier des donnees de depart : ";
        cin >> choixFichier;
    }while(lire(choixFichier, vAnnotObs, nbobs, nba, nbc) == 0);

    for(int i=0; i<nba; i++){
        for(int j=0; j<nbobs; j++){
            vPasChoisi.push_back(pair<int,int>(i,j));
        }
    }
    //testPrevalence(vAnnotObs, 1, 2, nbobs/2);

    //pour plus tard pour nom fichier en sortie
    choixFichier = choixFichier.substr(0, choixFichier.size()-4);
    replace(choixFichier.begin(), choixFichier.end(), '/', '_');

    //choix du nombre de génération aléatoire
    int nbGenerationAleatoire = 2000;
    if(choixGenerationAleatoire == 0){
        do{
            cout << "choix du nombre de generation aleatoire : ";
            cin >> nbGenerationAleatoire;
        }while(nbGenerationAleatoire < 1);
    }


    /*
    int choix;
    cout << "Choix ? (0 pour artificielle, 1 pour reelle)";
    cin >> choix ;
    if(choix==0){
        vAnnotObs.resize(nba);
        for(int i=0; i<nba; i++){
            vAnnotObs[i].resize(nbobs);
        }

        for(int i=0; i<nba; i++){
            for(int j=0; j<nbobs; j++){
                //if(j<nbobs/4){
                    vAnnotObs[i][j]=rand() % nbc;
                //}else{
                  //  vAnnotObs[i][j]=j % nbc;
                //}
                vPasChoisi.push_back(pair<int,int>(i,j));
            }
        }
    }else{
        choixTableau(314, vAnnotObs, nbobs, nba, nbc);
        for(int i=0; i<nba; i++){
            for(int j=0; j<nbobs; j++){
                vPasChoisi.push_back(pair<int,int>(i,j));
            }
        }
        //testPrevalence(vAnnotObs, 1, 2, nbobs/2);
    }*/

    //affichage(vAnnotObs);

    vObsAnnot.resize(nbobs);
    for(int i=0; i<nbobs; i++){
        vObsAnnot[i].resize(nba);
    }

    //transpose
    for (int i=0;i<nba;i++){
        for (int j=0;j<nbobs;j++){
            vObsAnnot[j][i]=vAnnotObs[i][j];
        }
    }

    //calcul la métrique sur les données
    float valeurMetriqueInit;
    valeurMetriqueInit = kappaAP(nbobs, nbc, nba, vObsAnnot);
    cout << "Metrique: " << valeurMetriqueInit << endl;

    //premiers tests de générations aléatoires
    if(choixGenerationAleatoire==1){
        generationAleatoire1(vPasChoisi, vAnnotObs);
    }else if(choixGenerationAleatoire==2){
        generationAleatoire2(vAnnotObs);
    }else if(choixGenerationAleatoire==3){
        generationAleatoire3(vPasChoisi, vAnnotObs);
    }else{

    /*GENERATION ALEATOIRE

Or donc, imaginons que nous ayons p annotateurs (a_i, i de 1 à p) qui ont donné chacun leur avis sur n observables (o_j, j de 1 à n)

Pour p1, on étudie ce qui se passe pour chaque observable o_j : on calcule la référence pour cet observable (r_j) ; le nombre d'observateurs qui n'ont pas choisi cette référence (n_j). Ensuite, on calcule la moyenne du nombre d'"erreurs" pour cet observable particulier (eobs_j=n_j/p).

Pour p2, on étudie ce qui se passe pour un annotateur (a_i) : on calcule la référence pour l'ensemble des observable (r_j, j de 1 à n), puis le nombre de fois où l'annotateur a_i, sur l'ensemble des observables, n'a pas choisi la référence (eannot_i).  La somme des (eannot_i) divisée par p nous donne la moyenne du nombre d'erreurs, par annotateur, ce que j'ai appelé p2.

En fait, l'idée qui sous-tend le calcul de p2, c'est de pouvoir créer des annotateurs fictifs plus ou moins "bons", i.e. qui s'écartent plus ou moins de la référence par rapport à nos annotateurs réels, ce qui, en principe, devrait nous permettre de pouvoir obtenir des valeurs d'accords plus ou moins élevées suivant la façon dont on déciderait de faire varier ce paramètre... Si on choisit pour un groupe d'annotateurs fictifs, un taux d'erreurs globalement < à p2, on devrait voir augmenter la mesure d'accord, alors que dans le cas contraire, on devrait la voir baisser...

Je pense aussi qu'il faut combiner p1 et p2.

Un exemple possible de protocole :

1 - on choisit, en fonction du taux d'accord que l'on souhaite, un intervalle dans lequel on veut faire varier le taux d'erreurs par annotateur. Par exemple, si on a p2=20% sur les observateurs réels et qu'on a 100 observables, on peut choisir [10;25] si on veut améliorer l'accord et [15;30] si on veut au contraire le dégrader...

2- Pour chaque annotateur fictif créé, on tire au sort le nombre d'"erreurs" (nbe) qu'il va commettre (par rapport à la référence calculée avec les annotateurs réels) dans cet intervalle

3 - on tire au sort nbe observations pour lesquelles l'annotateur fictif va s'écarter de la référence (en tenant compte ou non des erreurs constatées sur les observations : par exemple, on pourrait avoir un tirage qui favoriserait les observations ayant eu le plus fort taux d'erreurs - eobs_j-). Pour ces observations, on choisit une valeur différente de la référence (en tenant compte ou non des erreurs constatées sur ces observations...)

4 - bien sûr, pour les autres observations, l'annotateur fictif vote comme la référence.*/


    //le pourcentage de fois où un annotateur ne vote pas comme la référence pour chaque observable
    vector<float> pourcentageErreur;
    pourcentageErreur.resize(nbobs);
    //le nombre d'erreur pour chaque observable
    vector<int> nbErreurObs;
    nbErreurObs.resize(nbobs);
    //le nombre d'erreur moyen des observables
    float moyenneErreurP1 = 0;
    //le nombre d'erreur moyen de chaque annotateur
    float moyenneErreurP2;
    //le vote majoritaire pour chaque observable
    vector<int> voteMajoritaire;
    voteMajoritaire.resize(nbobs);
    //le nombre d'occurence des classes dans les annotations d'un observable
    vector<vector<int>> tNbOcc;
    tNbOcc.resize(nbobs);
    for(int o=0; o<nbobs; o++){
        tNbOcc[o].resize(nbc);
    }
    //le nombre d'annotation manquante
    int nbInfoManquante = 0;

        //calcul des votes majoritaires et de P1
        for(int obs=0; obs<nbobs; obs++){
            for(int c=0; c<nbc; c++){
                tNbOcc[obs][c]=0;
            }
            for(int ann=0; ann<nba; ann++){
                if(vObsAnnot[obs][ann]!=-1){
                    tNbOcc[obs][vObsAnnot[obs][ann]]++;
                }else{
                    nbInfoManquante++;
                }
            }
            int max=tNbOcc[obs][0];
            int voteMaj = 0;
            for(int i=1; i<nbc; i++){
                if(tNbOcc[obs][i]>max){
                    max = tNbOcc[obs][i];
                    voteMaj = i;
                }else if(tNbOcc[obs][i]==max){
                    int rng = rand() % 2;
                        if(rng == 0){
                            max = tNbOcc[obs][i];
                            voteMaj = i;
                        }

                }
            }
            voteMajoritaire[obs]=voteMaj;
            cout << "voteMajoritaire : " << voteMaj << endl;
            for(int c=0; c<nbc; c++){
                cout << c << " : " << tNbOcc[obs][c] << "       ";
            }
            cout << endl;
            //cout << obs << ":" << voteMajoritaire[obs] << "(" << max << ") ";
            pourcentageErreur[obs] = ((float) (nba - max - nbInfoManquante)/(float) (nba - nbInfoManquante))*100;
            nbErreurObs[obs] = nba - max - nbInfoManquante;
            nbInfoManquante = 0;

            cout << pourcentageErreur[obs] << "%" << endl;
            moyenneErreurP1 += pourcentageErreur[obs];
        }

        //test classe des erreurs pas totalement aléatoire mais en fonction des données réelles
        //la probabilité de tomber sur tel classe lorsqu'un annotateur fait une erreur
        vector<vector<int>> probaClasseErreur;
        probaClasseErreur.resize(nbobs);
        //le total de possibilité pour choisir de quelle classe est l'erreur
        vector<int> totalPossibiliteClasse;
        totalPossibiliteClasse.resize(nbobs);
        for(int o=0; o<nbobs; o++){
            probaClasseErreur[o].resize(nbc);
            totalPossibiliteClasse[o]=0;
        }

        if(choixClasseErreurParRapportAuDonneesrelles==1){
            probaErreurAnnotateur(tNbOcc, voteMajoritaire, probaClasseErreur, totalPossibiliteClasse);
        }


        // test répartition p1
        repartitionP1(nba, nbErreurObs);

        //calcul de P2
        int nbErreur = 0;
        int nbErreurMin = nbobs;
        int nbErreurMax = 0;
        for(int ann=0; ann<nba; ann++){
            int nbErreurAnn = 0;
            for(int obs=0; obs<nbobs; obs++){
                if(vAnnotObs[ann][obs] != voteMajoritaire[obs]){
                    nbErreurAnn++;
                    nbErreur++;
                }
            }
            if(nbErreurAnn < nbErreurMin) nbErreurMin = nbErreurAnn;
            if(nbErreurAnn > nbErreurMax) nbErreurMax = nbErreurAnn;
        }
        moyenneErreurP2 = (float) nbErreur/(float) nba;
        moyenneErreurP1 = moyenneErreurP1 / (float) nbobs;
        cout << "moyenne % P1= " << moyenneErreurP1 << "%" << endl; //<< " , ";
        float ecartType = 0;
        for(int obs = 0; obs < nbobs; obs++){
           ecartType += (pourcentageErreur[obs]-moyenneErreurP1)*(pourcentageErreur[obs]-moyenneErreurP1);
        }
        ecartType = ecartType/(float) nbobs;
        ecartType = sqrt(ecartType);
        cout << "ecart type : " << ecartType << endl;
        //cout << "moyenne % P2= " << ((float) moyenneErreurP2/(float) nbobs)*100.0 << " min=" << ((float) nbErreurMin/(float) nbobs)*100.0 << " , max=" << ((float) nbErreurMax/(float) nbobs)*100.0 << endl;


    //création des groupes d'annotateurs
    for(int occ=0; occ<nbGenerationAleatoire; occ++){
        //l'intervalle d'erreur des annotateurs (chaque annotateur fera entre intervalleMin et intervalleMax erreurs) (max exclu)
        int intervalleMin;
        int intervalleMax;
        if(choixIntervalleErreurAnnotateur==0){
            //cas où l'intervalle est de 0
            intervalleMin = occ%(int)((float)nbobs*sqrt((float)(nbc-1)/(float)(nbc)));//occ%(nbobs-1);//nbErreurMin;//occ;
            intervalleMax = intervalleMin+1;//nbErreurMax;//occ+1;
        }else{
            //cas où l'intervalle est le plus grand possible
            intervalleMin = 0;
            intervalleMax = (occ%(nbobs/2))*2;
            if(intervalleMax==0) intervalleMax=1;
        }

        //cout << occ << endl;
        /*cout << "choix intervalle :" << endl << "min=";
        cin >> intervalleMin;
        cout << "max=";
        cin >> intervalleMax;*/


        //generation des nouveaux annotateurs
        int nbNewAnnot = nbAnnotParGeneration;
        vector<vector<int>> vAnnotObs2;
        vAnnotObs2.resize(nbNewAnnot);
        for(int i=0; i<nbNewAnnot; i++){
            vAnnotObs2[i].resize(nbobs);
        }
        vector<vector<int>> vObsAnnot2;
        vObsAnnot2.resize(nbobs);
        for(int i=0; i<nbobs; i++){
            vObsAnnot2[i].resize(nbNewAnnot);
        }

        for(int ann=0; ann<nbNewAnnot; ann++){
            //on calcul les proba de tomber sut tel ou tel annotateur
            vector<int> vObsErreur;
            vObsErreur.resize(nbobs);
            vector<int> proba;
            proba.resize(nbobs);
            int totalPossibilite = 0;
            for(int obs=0; obs<nbobs; obs++){
                vAnnotObs2[ann][obs]=voteMajoritaire[obs];
                vObsErreur[obs] = obs;
                if(obs==0){
                    //plusieurs test de la prise en compte de p1 dans les proba

                    //(pourcentageErreur[obs]/100)*(nbc/(nbc-1)) -> resultat entre 0 et 1, 0 entant tout le monde d'accord et 1 le plus de desaccord possible
                    //proba[obs]= 1.0 + ((pourcentageErreur[obs]/100.0)*((float) nbc/(float) (nbc-1))*10.0)*((pourcentageErreur[obs]/100.0)*((float) nbc/(float) (nbc-1))*10.0);

                    proba[obs]= 1.0 + (pourcentageErreur[obs]*((float) nbc/(float) (nbc-1)))*(pourcentageErreur[obs]*((float) nbc/(float) (nbc-1)))/100.0;
                    /*if(pourcentageErreur[obs]<1){
                        proba[obs]= 1;
                    }else{
                        proba[obs]=pourcentageErreur[obs];
                    }*/
                }else{
                    // //proba[obs]= proba[obs-1] + 1 + nbErreurObs[obs] * nbErreurObs[obs] * nbErreurObs[obs]; //1f/(float) nbobs;pourcentageErreur
                    //proba[obs]= proba[obs-1] + 1.0 + ((pourcentageErreur[obs]/100.0)*((float) nbc/(float) (nbc-1))*10.0)*((pourcentageErreur[obs]/100.0)*((float) nbc/(float) (nbc-1))*10.0);

                    proba[obs]= proba[obs-1] + 1.0 + (pourcentageErreur[obs]*((float) nbc/(float) (nbc-1)))*(pourcentageErreur[obs]*((float) nbc/(float) (nbc-1)))/100.0;
                    /*if(pourcentageErreur[obs]<1){
                        proba[obs]= proba[obs-1] + 1.0;
                    }else{
                        proba[obs]= proba[obs-1] + pourcentageErreur[obs];
                    }*/
                }
            }

            totalPossibilite = proba[nbobs-1];
            //on choisi un nombre aléatoire d'erreur compris dans l'intervalle
            int rng = intervalleMin + (rand() % (intervalleMax - intervalleMin));

            //pour chaque erreur, on choisi l'observable sur lequel elle va être ainsi que sa classe
            for(int err=0; err<rng; err++){

                totalPossibilite = proba[proba.size()-1];

                int rngObs = rand() % (totalPossibilite);

                int itObs = 0;
                while(rngObs >= proba[itObs]){
                    itObs++;
                }
                //l'annotateur ann fait une erreur à l'observable vObsErreur[itObs]
                if(choixClasseErreurParRapportAuDonneesrelles==0){
                    //cas où l'erreur est choisi aléatoirement parmi les classes non majoritaire
                    vAnnotObs2[ann][vObsErreur[itObs]]+= 1 + (rand() % (nbc-1));
                    vAnnotObs2[ann][vObsErreur[itObs]] = vAnnotObs2[ann][vObsErreur[itObs]] % nbc;
                }else{
                    //cas où la classe de l'erreur dépend des données de départ
                    int rngClasse = rand() % totalPossibiliteClasse[vObsErreur[itObs]];
                    int itClasse = 0;
                    while(rngClasse >= probaClasseErreur[vObsErreur[itObs]][itClasse]){
                        itClasse++;
                    }
                    vAnnotObs2[ann][vObsErreur[itObs]] = itClasse;
                }


                //cas où l'erreur peut être choisi totalement au hasard (et peut donc ne pas en être une)
                /*int aleatoire = rand()%2;
                if(aleatoire==0){
                    vAnnotObs2[ann][vObsErreur[itObs]]= rand() % nbc;
                }else{
                    vAnnotObs2[ann][vObsErreur[itObs]]+= 1 + (rand() % (nbc-1));
                    vAnnotObs2[ann][vObsErreur[itObs]] = vAnnotObs2[ann][vObsErreur[itObs]] % nbc;
                }*/

                //les erreurs suivantes ne pourrons pas retomber sur cet observable
                for(int i=itObs; i<proba.size()-1; i++){
                    proba[i] = proba[i+1] - (int) (1.0 + (pourcentageErreur[vObsErreur[itObs]]*((float) nbc/(float) (nbc-1)))*(pourcentageErreur[vObsErreur[itObs]]*((float) nbc/(float) (nbc-1)))/100.0);
                }
                proba.pop_back();

                for(int i=itObs; i<vObsErreur.size()-1; i++){
                    vObsErreur[i] = vObsErreur[i+1];
                }
                vObsErreur.pop_back();
            }

        }
        //fin de cette génération aléatoire

        //transpose
        for (int i=0;i<nbNewAnnot;i++){
            for (int j=0;j<nbobs;j++){
                vObsAnnot2[j][i]=vAnnotObs2[i][j];
            }
        }
        //on calcul la métrique sur cette génération aléatoire
        float valeurMetrique;
        valeurMetrique = kappaAP(nbobs, nbc, nbNewAnnot, vObsAnnot2);
        cout << "Metrique: " << valeurMetrique << endl;


        //crée les fichiers de sortie
        ostringstream ss;
        //s'il n'y a pas de dossier resultats le crée
        mkdir("resultats");
        ss << "resultats/" << choixFichier << occ << ".csv";
        ecrireFichier(nbNewAnnot, vAnnotObs2, ss.str());

        //calcul de la répartition p1 sur les données générées
        if(choixCalculRepartitionP1DonneesGenerees == 1){
            //le nombre d'erreur pour chaque observable
            vector<int> nbErreurObs2;
            nbErreurObs2.resize(nbobs);
            //le nombre d'occurence des classes dans les annotations d'un observable
            vector<vector<int>> tNbOcc2;
            tNbOcc2.resize(nbobs);
            for(int o=0; o<nbobs; o++){
                tNbOcc2[o].resize(nbc);
            }
            //le nombre d'annotation manquante
            int nbInfoManquante2 = 0;

            //calcul des votes majoritaires et de P1
            for(int obs=0; obs<nbobs; obs++){
                for(int c=0; c<nbc; c++){
                    tNbOcc2[obs][c]=0;
                }
                for(int ann=0; ann<nbNewAnnot; ann++){
                    if(vObsAnnot[obs][ann]!=-1){
                        tNbOcc2[obs][vObsAnnot2[obs][ann]]++;
                    }else{
                        nbInfoManquante2++;
                    }
                }
                int max=tNbOcc2[obs][0];
                int voteMaj = 0;
                for(int i=1; i<nbc; i++){
                    if(tNbOcc2[obs][i]>max){
                        max = tNbOcc2[obs][i];
                        voteMaj = i;
                    }else if(tNbOcc2[obs][i]==max){
                        int rng = rand() % 2;
                        if(rng == 0){
                            max = tNbOcc2[obs][i];
                            voteMaj = i;
                        }
                    }
                }
                nbErreurObs2[obs] = nbNewAnnot - max - nbInfoManquante2;
                nbInfoManquante2 = 0;
            }
            repartitionP1(nbNewAnnot, nbErreurObs2);
        }
    }
    }

    return 0;
}
