#include <iostream>
#include <vector>
#include <cstring>
#include <cstdlib>
#include <ctime>
#include <cmath>
#include <cstring>
#include <fstream>
#include <cstdio>
#include "combinaison.h"

using namespace std;

// IV - génère toutes les annotations par vote majoritaires pour toutes les combinaisons de (n-1),
//      (n-2), (n-3) ... annotateurs, et donne les % de modifications par rapport à la référence

//calcul du vote majoritaire et le compare avec celui de référence

//n = 9 ou 8 ou 7 etc annotateurs, effectue des calculs pour chaque combinaison
void voteMajoritaire( int *p, int n, vector<vector<int>> & vObsAnnot,int nbobs,int nba, int nbc,
		  vector<int> & erreurVoteMaj, vector<int> & voteMajReference) {
    /*int i;
    for (i = 0; i < n; i++){
        cout << p[i];
    }
    cout << endl;
    //cout << " : ";*/

    //le vote majoritaire pour chaque observable
    int voteMajoritaire[nbobs];
    //le nombre d'occurence des classes dans les annotations d'un observable
    int tNbOcc[nbc];
    //la moyenne des classes pour chaque observable
    //float tMoyenne[nbobs];

    for(int obs=0; obs<nbobs; obs++){
        float moyenne = 0;
        for(int ann=0; ann<n; ann++){
            //p[ann] nous donne un annotateur de la combinaison actuelle, T[obs][p[ann]] nous donne la classe qu'à attribué cet annotateur à l'observable obs
            tNbOcc[vObsAnnot[obs][p[ann]]]++;
            moyenne += vObsAnnot[obs][p[ann]];
        }
        moyenne = moyenne / (float) n;
        int max=tNbOcc[0];
        int voteMaj = 0;
        tNbOcc[0] = 0;
        for(int i=1; i<nbc; i++){
            if(tNbOcc[i]>max){
                max = tNbOcc[i];
                voteMaj = i;
            }else if(tNbOcc[i]==max){
                int rng = rand() % 2;
                if(rng == 0){
                    max = tNbOcc[i];
                    voteMaj = i;
                }
            }
            tNbOcc[i]=0;
        }
        voteMajoritaire[obs]=voteMaj;

        /*for(int i=0; i<nbc; i++){
            tNbOcc[i]=0;
        }*/
    }

    //si c'est la référence
    if(n == nba){
        erreurVoteMaj[n]=0;
        //cout << "voteMajRef : ";
        for(int i=0; i<nbobs; i++){
            voteMajReference[i] = voteMajoritaire[i];
            //cout << voteMajReference[i] <<" ";
            //moyenneReference[i] = tMoyenne[i];
        }
    //sinon on compare avec la référence
    }else{
        //int test = erreurVoteMaj[n];
        for(int i=0; i<nbobs; i++){
            if(voteMajoritaire[i] != voteMajReference[i]){
                bool ponderation = false;
                if(!ponderation){
                    erreurVoteMaj[n]++;
                }else{
                    //si on veut pondéré l'erreur :
                    erreurVoteMaj[n]+= abs(voteMajoritaire[i] - voteMajReference[i]);
                }
                //"% de modifications par rapport à la référence" par nombre d'annotateur ou pour chaque vote majoritaire ?
                //break;
            }
        }
        //cout << "nbErreur " << n << ": " << erreurVoteMaj[n]-test << endl;
    }
}

//vote maj pour le gold en fonction du nombre d'annotateur de la combinaison
void voteMajoritaire2( int *p, int n, vector<vector<int>> & vObsAnnot,int nbobs,int nba, int nbc,
		  vector<int> & erreurVoteMaj, vector<int> & voteMajReference, vector<vector<vector<int>>> & voteMajOcc) {

    //le vote majoritaire pour chaque observable
    //int voteMajoritaire[nbobs];
    //le nombre d'occurence des classes dans les annotations d'un observable
    int tNbOcc[nbc];
    //la moyenne des classes pour chaque observable
    //float tMoyenne[nbobs];

    for(int obs=0; obs<nbobs; obs++){
        float moyenne = 0;
        for(int ann=0; ann<n; ann++){
            //p[ann] nous donne un annotateur de la combinaison actuelle, T[obs][p[ann]] nous donne la classe qu'à attribué cet annotateur à l'observable obs
            tNbOcc[vObsAnnot[obs][p[ann]]]++;
            moyenne += vObsAnnot[obs][p[ann]];
        }
        moyenne = moyenne / (float) n;
        int max=tNbOcc[0];
        int voteMaj = 0;
        tNbOcc[0] = 0;
        for(int i=1; i<nbc; i++){
            if(tNbOcc[i]>max){
                max = tNbOcc[i];
                voteMaj = i;
            }else if(tNbOcc[i]==max){
                int rng = rand() % 2;
                if(rng == 0){
                    max = tNbOcc[i];
                    voteMaj = i;
                }
            }
            tNbOcc[i]=0;
        }
        //voteMajoritaire[obs]=voteMaj;
        voteMajOcc[n][obs][voteMaj]++;
    }
}


//trouve l'ensemble des combinaisons de p parmi n (de manière récursive)
void combinaisons(int *ens, int *combinaison, int n, int p, int i, int t,
                  vector<vector<int>> & vObsAnnot,int nbobs,int nba,int nbc,
		  vector<int> & erreurVoteMaj, vector<int> & voteMajReference, vector<int> & nbCombinaison,
		  int choixGold, vector<vector<vector<int> > > &voteMajOcc, vector<vector<float>> & moyennes) {
    if (i<p) {
        for (int k=t; k<n; k++) {
            combinaison[i] = ens[k];
            combinaisons(ens,combinaison,n,p,i+1,k+1, vObsAnnot,nbobs,nba,nbc, erreurVoteMaj, voteMajReference, nbCombinaison, choixGold, voteMajOcc, moyennes);
        }
    }
    else {
        if(choixGold==0){
            voteMajoritaire(combinaison,p, vObsAnnot,nbobs,nba,nbc, erreurVoteMaj, voteMajReference);
        }else if(choixGold==1){
            voteMajoritaire2(combinaison,p, vObsAnnot,nbobs,nba,nbc, erreurVoteMaj, voteMajReference, voteMajOcc);
        }
        nbCombinaison[p]++;
    }
}


void calculDifference(vector<vector<int>> & vObsAnnot,int nbobs,int nba,int nbc, vector<float> *vPourcentageErreur, int choixGold){
    int ens[nba];
    for(int i=0; i<nba; i++){
        ens[i] = i;
    }
    //le nombre de votes majoritaires différent de la référence par nombre d'annotateurs
    vector<int> erreurVoteMaj;
    erreurVoteMaj.resize(nba+1);
    //le nombre de combinaisons par nombre d'annotateurs
    vector<int> nbCombinaison;
    nbCombinaison.resize(nba+1);
    //les votes majoritaires de la référence pour chaque observable
    vector<int> voteMajReference;
    voteMajReference.resize(nbobs);
    //les moyennes pour chaque combinaisons de p annotateurs
    vector<vector<float>> moyennes;
    moyennes.resize(nba);

    for(int i=0; i<nba+1; i++){
        erreurVoteMaj[i]=0;
        nbCombinaison[i]=0;
    }

    vector<vector<vector<int> > > voteMajOcc;

    voteMajOcc.resize(nba);
    for (int i = 0; i < nba; ++i) {
        voteMajOcc[i].resize(nbobs);

        for (int j = 0; j < nbobs; ++j)
            voteMajOcc[i][j].resize(nbc);
    }

    for(int p=nba; p>1; p--){
        //cout << p << " annotateurs / " << nba << endl;
        //cout << endl;
        int combi[p];
        //std::vector<float> vAlpha;
        combinaisons(ens,combi,nba,p,0,0, vObsAnnot,nbobs,nba,nbc, erreurVoteMaj, voteMajReference, nbCombinaison, choixGold, voteMajOcc, moyennes);
        //float moy = moyenne(&vAlpha);
        //cout << "moyenne : " << moy << endl;
        //cout << "ecart-type : " << ecart_type(&vAlpha, moy) << endl;
        //cout << endl;
        //int combi[nba-1];
    }

    if(choixGold==1){
        for(int i=nba; i>1; i--){
            int nbErreur = 0;
            for(int obs=0; obs<nbobs; obs++){
                int max = voteMajOcc[i][obs][0];
                int voteMaj = 0;
                for(int c=1; c<nbc; c++){
                    if(voteMajOcc[i][obs][c]>max){
                        max = voteMajOcc[i][obs][c];
                        voteMaj = c;
                    }else if(voteMajOcc[i][obs][c]==max){
                        int rng = rand() % 2;
                        if(rng == 0){
                            max = voteMajOcc[i][obs][c];
                            voteMaj = c;
                        }
                    }
                }
                for(int c=0; c<nbc; c++){
                    if(c != voteMaj){
                        nbErreur += voteMajOcc[i][obs][c];
                    }
                }
            }
            //cout << nbErreur << "/(" << nbCombinaison[i] << "*" << nblignes << ")    ";
            float pourcentage = ((float) nbErreur/(float) (nbCombinaison[i]*nbobs))*100;
            //cout << i << ": " << fixed << setprecision (3) << pourcentage << "%   ";
            vPourcentageErreur->push_back(pourcentage);
        }

    }else{
        //cout << "poucentage d'erreur :  ";
        for(int i=nba; i>1; i--){
            //cout << i << ": " << erreurVoteMaj[i] << "/(" << nbCombinaison[i] << "*" << nblignes << ")    ";
            //cout << i << ": " << ((float) erreurVoteMaj[i]/(float) nbCombinaison[i])*100 << "%   ";
            float pourcentage;
            bool ponderation = false;
            if(!ponderation){
                pourcentage = ((float) erreurVoteMaj[i]/(float) (nbCombinaison[i]*nbobs))*100;
            }else{
                pourcentage = (((float) erreurVoteMaj[i]/(nbc-1))  /(float) (nbCombinaison[i]*nbobs))*100;
            }

            vPourcentageErreur->push_back(pourcentage);
            //cout << i << ": " << fixed << setprecision (3) << pourcentage << "%   ";
            //TPourcentageErreur[i] = pourcentage;
        }
        cout << endl;
    }
}
