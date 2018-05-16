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

void ecrireFichier(int nba, int nbobs, int nbc, vector<vector<int>> & vAnnotObs, string ss){
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

            ecrireFichier(nba, nbobs, nbc, vAnnotObs, ss.str());
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

                ecrireFichier(nba, nbobs, nbc, vAnnotObs, ss.str());
            //}
        }
    }
}

void generationAleatoire3(vector<pair<int,int>> & vPasChoisi, vector<vector<int>> & vAnnotObs, int nba, int nbobs, int nbc){
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

        ecrireFichier(nba, nbobs, nbc, vAnnotObs, ss.str());
    }
}

void ajouterNouveauAnnotateur(int & nba, int nbobs, int nbc, vector<vector<int>> & vAnnotObs, vector<float> & pourcentageErreur, vector<int> & voteMajoritaire){
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

void remplacerParNouveauAnnotateur(int nba, int nbobs, int nbc, vector<vector<int>> & vAnnotObs, vector<float> & pourcentageErreur, vector<int> & voteMajoritaire){
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

int main()
{
    int nba = 9;
    int nbobs = 30;
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

    //transpose
    for (int i=0;i<nba;i++){
        for (int j=0;j<nbobs;j++){
            vObsAnnot[j][i]=vAnnotObs[i][j];
        }
    }

    //generationAleatoire1(vPasChoisi, vAnnotObs, nba, nbobs, nbc);

    //generationAleatoire2(vAnnotObs, nba, nbobs, nbc);

    //generationAleatoire3(vPasChoisi, vAnnotObs, nba, nbobs, nbc);


    //le pourcentage de fois où un annotateur ne vote pas comme la référence pour chaque observable
    vector<float> pourcentageErreur;
    pourcentageErreur.resize(nbobs);
    //le nombre d'erreur moyen de chaque annotateur
    float moyenneErreurP2;
    //le vote majoritaire pour chaque observable
    vector<int> voteMajoritaire;
    voteMajoritaire.resize(nbobs);
    //le nombre d'occurence des classes dans les annotations d'un observable
    int tNbOcc[nbc];

    //for(int occ=0; occ<10; occ++){
        for(int c=0; c<nbc; c++){
            tNbOcc[c]=0;
        }

        //calcul des votes majoritaires et de P1
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

        int intervalleMin = 0;
        int intervalleMax = nbobs;
        cout << "P2= " << moyenneErreurP2 << "  (" << nbErreur << "/" << nba << ") , min=" << nbErreurMin << " , max=" << nbErreurMax << endl;
        cout << "choix intervalle :" << endl << "min=";
        cin >> intervalleMin;
        cout << "max=";
        cin >> intervalleMax;

        //generation des nouveaux annotateurs

        //I-on les ajoute à ceux déjà existant
        //ajouterNouveauAnnotateur(nba, nbobs, nbc, vAnnotObs, pourcentageErreur, voteMajoritaire);

        //II-on les met à la place de ceux déjà existant
        //remplacerParNouveauAnnotateur(nba, nbobs, nbc, vAnnotObs, pourcentageErreur, voteMajoritaire);

        //III-on les crée à part

        int nbNewAnnot = nba;
        nba += nbNewAnnot;
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
            for(int obs=0; obs<nbobs; obs++){
                vAnnotObs2[ann][obs]=voteMajoritaire[obs];
            }
            //on choisi un nombre aléatoire d'erreur compris dans l'intervalle
            int rng = intervalleMin + (rand() % (intervalleMax - intervalleMin));
            for(int err=0; err<rng; err++){
                int rngObs = rand() % nbobs;
                vAnnotObs2[ann][rngObs]= rand() % nbc;
            }
        }

        //transpose
        for (int i=0;i<nbNewAnnot;i++){
            for (int j=0;j<nbobs;j++){
                vObsAnnot2[j][i]=vAnnotObs2[i][j];
            }
        }
        float valeurMetrique;
        valeurMetrique = kappaAP(nbobs, nbc, nbNewAnnot, vObsAnnot2);
        cout << "valeurMetrique: " << valeurMetrique;



        //affichage(nba, nbobs, vAnnotObs);
        //ostringstream ss;
        //ss << "generationAleatoireDonneesReelles/generationAleatoireTest" << /*occ <<*/ ".csv";
        //ecrireFichier(nba, nbobs, nbc, vAnnotObs, ss.str());
    //}

    return 0;
}
