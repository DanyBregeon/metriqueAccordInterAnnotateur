#include <iostream>
#include <vector>
#include <cstring>
#include <cstdlib>
#include <ctime>
#include <cmath>
#include <cstring>
#include <fstream>
#include <cstdio>
#include <sstream>
#include <ctime>
#include "calculMetrique.h"
#include "lectureFichier.h"

using namespace std;

void affichage(int nba, int nbobs, vector<vector<int>> & vAnnotObs){
    for (int i=0;i<nba;i++) {
        for (int j=0;j<nbobs;j++)
            printf("%3d",vAnnotObs[i][j]);
        cout << endl;
    }
}

void generationAleatoire1(vector<pair<int,int>> & vPasChoisi, vector<vector<int>> & vAnnotObs, int nba, int nbobs, int nbc){
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
            ofstream file;
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
            }
            it++;
    }
}

void generationAleatoire2(vector<vector<int>> & vAnnotObs, int nba, int nbobs, int nbc){
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
                ofstream file;
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
                }
            //}
        }
    }
}

int main()
{
    int nba = 9;
    int nbobs = 100;
    int nbc = 3;
    vector<vector<int>> vAnnotObs;
    vector<vector<int>> vObsAnnot;
    vector<pair<int,int>> vPasChoisi;

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
                vAnnotObs[i][j]=j % nbc;
                vPasChoisi.push_back(pair<int,int>(i,j));
            }
        }
    }else{
        choixTableau(307, vAnnotObs, nbobs, nba, nbc);
        for(int i=0; i<nba; i++){
            for(int j=0; j<nbobs; j++){
                vPasChoisi.push_back(pair<int,int>(i,j));
            }
        }
    }

    //affichage(nba, nbobs, vAnnotObs);

    vObsAnnot.resize(nbobs);
    for(int i=0; i<nbobs; i++){
        vObsAnnot[i].resize(nba);
    }

    for (int i=0;i<nba;i++){
        for (int j=0;j<nbobs;j++){
            vObsAnnot[j][i]=vAnnotObs[i][j];
        }
    }

    //generationAleatoire1(vPasChoisi, vAnnotObs, nba, nbobs, nbc);

    //generationAleatoire2(vAnnotObs, nba, nbobs, nbc);


    //le pourcentage de fois où un annotateur ne vote pas comme la référence pour chaque observable
    float pourcentageErreur[nbobs];
    //le vote majoritaire pour chaque observable
    int voteMajoritaire[nbobs];
    //le nombre d'occurence des classes dans les annotations d'un observable
    int tNbOcc[nbc];
    for(int c=0; c<nbc; c++){
        tNbOcc[c]=0;
    }

    for(int obs=0; obs<nbobs; obs++){
        for(int ann=0; ann<nba; ann++){
            tNbOcc[vObsAnnot[obs][ann]]++;
        }
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
        //cout << obs << ":" << voteMajoritaire[obs] << "(" << max << ") ";
        pourcentageErreur[obs] = ((float) (nba - max)/(float) nba)*100;
        cout << pourcentageErreur[obs] << endl;
    }

    //generation des nouveaux annotateurs
    int nbNewAnnot = nba;
    nba += nbNewAnnot;
    vAnnotObs.resize(nba);
    for(int i=0; i<nba; i++){
        vAnnotObs[i].resize(nbobs);
    }

    for(int ann=nba - nbNewAnnot; ann<nba; ann++){
        for(int obs=0; obs<nbobs; obs++){
            vAnnotObs[ann][obs]=voteMajoritaire[obs];
        }
    }
    //affichage(nba, nbobs, vAnnotObs);



    return 0;
}
