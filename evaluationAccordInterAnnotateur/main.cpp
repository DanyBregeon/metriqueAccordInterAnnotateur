#include <iostream>
#include <vector>
#include <cstring>
#include <cstdlib>
#include <ctime>
#include <cmath>
#include <cstring>
#include <fstream>
#include <cstdio>
#include <map>
#include <iomanip>
#include "calculMetrique.h"
#include "lectureFichier.h"
#include "combinaison.h"
#include "fichierSortie.h"
#include "calculResultats.h"

using namespace std;

int main()
{
    //int tAnnotObs[Max_annot][Max_obs],tObsAnnot[Max_obs][Max_annot];
    vector<vector<int>> vAnnotObs;
    vector<vector<int>> vObsAnnot;
    //float C[Max_classes][Max_classes];
    vector<vector<float>> C;
    int nba, nbobs, nbc;//nb d'annotateurs, d'observables, de classes
    float nb;//observation réellement prises en compte

    int choix;
    int choixNbClasse;
    cout << "Choix ? (1 pour tous les corpus, 2 pour emotion, 3 pour opinion, 4 pour coreference, 0 pour les corpus generes aleatoirement)";
    cin >> choix ;
    //si >= 100, on test sur un seul fichier
    if(choix < 100){
        cout << "Choix ? (1 pour toutes les classes, 3 pour 3 classes, 5 pour 5 classes)";
        cin >> choixNbClasse ;
    }else{
        if(choix>=500){
            choixNbClasse = 5;
        }else{
            choixNbClasse = 3;
        }
    }

    string choixMetrique;
    cout << "Choix ? (a pour alpha, k pour kappa, pi pour pi, ap pour alpha pondere)";
    cin >> choixMetrique;

    int choixGold;
    cout << "Choix ? (gold: 0 pour verite ideale, 1 pour en fonction du nombre d'annotateurs retenus, 2 pareil mais sans annotateur commun, 3 moyenne par rapport au nombre P d'annotateurs, 4 moyenne par rapport a la verite)";
    cin >> choixGold;

    //tableau final avec les pourcentages de différences de votes majoritaires par rapport à la référence
    //en fonction du nombre de classes et de la valeur de la métrique
    std::map<std::pair<int, float>, std::vector<float>> mapResultat;



    if(choix<100){
        vector<int> lesChoix;
        if(choix==0){ //cas generation aleatoire
            for(int i=1000; i<3000/*2629/*1270*/; i++){
                lesChoix.push_back(i);
            }
        }
        if(choixNbClasse==1 || choixNbClasse==3){
            if(choix==1 || choix==2){
                for(int i=301; i<307; i++){
                    lesChoix.push_back(i);
                }
            }
            if(choix==1 || choix==3){
                for(int i=307; i<313; i++){
                    lesChoix.push_back(i);
                }
            }
        }
        if(choixNbClasse==1 || choixNbClasse==5){
            if(choix==1 || choix==2){
                for(int i=501; i<507; i++){
                    lesChoix.push_back(i);
                }
            }
            if(choix==1 || choix==3){
                for(int i=507; i<513; i++){
                    lesChoix.push_back(i);
                }
            }
            if(choix==1 || choix==4){
                lesChoix.push_back(513);
            }
        }
        for(int i=0; i<lesChoix.size(); i++){
            choixTableau(lesChoix[i],vAnnotObs,nbobs,nba,nbc);
            //afficheTableauLu(T,nbobs,nba);
            //vObsAnnot est vAnnotObs transposé
            vObsAnnot.resize(nbobs);
            for(int i=0; i<nbobs; i++){
                vObsAnnot[i].resize(nba);
            }
            C.resize(nbc);
            for(int i=0; i<nbc; i++){
                C[i].resize(nbc);
            }
            for (int i=0;i<nba;i++){
                for (int j=0;j<nbobs;j++){
                    vObsAnnot[j][i]=vAnnotObs[i][j];
                }
            }
            //cout << "Tableau des coincidences :\n";
            coincidences(vObsAnnot,nbobs,nba,C,nbc,nb);
            //affiche_coincidences(C,nbc);
            //cout << "nb d'observations prises en compte="<<nb <<endl;
            vector<float> vPourcentageErreur;
            //cout << "alpha=" << alpha(nb,C,nbc);
            float metrique;
            if(choixMetrique.compare("k")==0){
                metrique = kappaAP(nbobs, nbc, nba, vObsAnnot);
            }else if(choixMetrique.compare("ap")==0){
                metrique = alpha(nb,C,nbc, 1);
            }else if(choixMetrique.compare("pi")==0){
                metrique = piAP(nbobs, nbc, nba, vObsAnnot);
            }else{
                metrique = alpha(nb,C,nbc, 0);
            }
            pair<int, float> pairNbcAlpha(nbc, metrique);
            //cout << " pairNbcAlpha: " << pairNbcAlpha.first << "/" << pairNbcAlpha.second;
            //cout<< "combinaisons de n-p annotateurs :" <<endl;
            calculDifference(vObsAnnot,nbobs,nba,nbc, &vPourcentageErreur, choixGold);
            mapResultat.insert(pair<pair<int, float>, vector<float>>(pairNbcAlpha, vPourcentageErreur));
        }

        //affichage résultats
        for (map<std::pair<int, float>, vector<float>>::iterator it=mapResultat.begin(); it!=mapResultat.end(); ++it){
            pair<int, float> m = it->first;
            string s;
            if(choixMetrique.compare("k")==0){
                s = "kappa";
            }else if(choixMetrique.compare("ap")==0){
                s = "alpha(pondere)";
            }else if(choixMetrique.compare("pi")==0){
                s = "pi";
            }else{
                s = "alpha";
            }
            cout << m.first <<" classes, "<< s << "=" << fixed << setprecision (3) << m.second << "   ";
            for(int i=0; i<nba-1; i++){
                vector<float> valeurs = it->second;
                cout << nba-i << ": " << valeurs[i] <<"%   ";
            }
            cout << endl;
            cout << endl;
        }
        map<pair<int, float>, vector<float>> mapResultat2;
        resultatsPalier(mapResultat, mapResultat2, 0.05f, nbc, nba);
        fichierSortie(choix, choixNbClasse, choixMetrique, choixGold, nba, mapResultat2);
    }else{ //on test sur un seul fichier

        choixTableau(choix,vAnnotObs,nbobs,nba,nbc); //on rempli tous les parametres par les valeurs lu dans le fichier
        //afficheTableauLu(vAnnotObs,nbobs,nba); //on affiche le tableau d'annotations
        //coincidences
        //on transpose vAnnotObs dans vObsAnnot
        vObsAnnot.resize(nbobs);
        for(int i=0; i<nbobs; i++){
            vObsAnnot[i].resize(nba);
        }
        C.resize(nbc);
        for(int i=0; i<nbc; i++){
            C[i].resize(nbc);
        }
        for (int i=0;i<nba;i++){
            for (int j=0;j<nbobs;j++){
                vObsAnnot[j][i]=vAnnotObs[i][j];
            }
        }
        cout << "Tableau des coincidences :\n";
        coincidences(vObsAnnot,nbobs,nba,C,nbc,nb);
        //affiche_coincidences(C,nbc);
        cout << "nb d'observations prises en compte="<<nb <<endl;

        float metrique;
        if(choixMetrique.compare("k")==0){
            metrique = kappaAP(nbobs, nbc, nba, vObsAnnot);
            cout << "kappa= ";
        }else if(choixMetrique.compare("ap")==0){
            metrique = alpha(nb,C,nbc, 1);
            cout << "alpha(pondere)= ";
        }else if(choixMetrique.compare("pi")==0){
            metrique = piAP(nbobs, nbc, nba, vObsAnnot);
            cout << "pi= ";
        }else{
            metrique = alpha(nb,C,nbc, 0);
            cout << "alpha= ";
        }

        cout << metrique << endl;

        cout << "combinaisons de n-p annotateurs :" <<endl;

        std::vector<float> vPourcentageErreur;

        calculDifference(vObsAnnot,nbobs,nba,nbc, &vPourcentageErreur, choixGold);

        pair<int, float> pairNbcAlpha(nbc, metrique);
        mapResultat.insert(pair<pair<int, float>, vector<float>>(pairNbcAlpha, vPourcentageErreur));
        //affichage résultats
        for(int i=0; i<nba-1; i++){
            cout << nba-i << ": " << mapResultat.at(pairNbcAlpha)[i] <<"%   ";
        }
        fichierSortie(choix, choixNbClasse, choixMetrique, choixGold, nba, mapResultat);
    }


    /*choixTableau(307, vAnnotObs, nbobs, nba, nbc);


    //vAnnotObs.resize(nba);
      //for(int i=0; i<nba; i++){
        //vAnnotObs[i].resize(nbobs);
      //}
      vObsAnnot.resize(nbobs);
      for(int i=0; i<nbobs; i++){
        vObsAnnot[i].resize(nba);
      }
      C.resize(nbc);
      for(int i=0; i<nbc; i++){
        C[i].resize(nbc);
      }

    //for(int i=0; i<nba; i++){
        //for(int j=0; j<nbobs; j++){
          //  vAnnotObs[i][j]=j%nbc;
        //}
    //}
    //vAnnotObs[0][1]=2;

    //transpose
    for (int i=0;i<nba;i++){
        for (int j=0;j<nbobs;j++){
            vObsAnnot[j][i]=vAnnotObs[i][j];
        }
    }

    float valeurMetrique;
    valeurMetrique = piAP(nbobs, nbc, nba, vObsAnnot);
    cout << "pi: " << valeurMetrique;

    coincidences(vObsAnnot,nbobs,nba,C,nbc,nb);
    valeurMetrique = alpha(nb, C, nbc, 0);
    cout << " alpha: " << valeurMetrique;
    valeurMetrique = alpha(nb, C, nbc, 1);
    cout << " alpha pondere: " << valeurMetrique;
    valeurMetrique = kappaAP(nbobs, nbc, nba, vObsAnnot);
    cout << " kappa: " << valeurMetrique << endl;

    cout << "combinaisons de n-p annotateurs :" << endl;


    //tableau final avec les pourcentages de différences de votes majoritaires par rapport à la référence
    //en fonction du nombre de classes et de la valeur de la métrique
    map<pair<int, float>, vector<float>> mapResultat;

    vector<float> vPourcentageErreur;
    int choixGold=0;
    int choixNbClasse=3;
    int choix = 3;
    string choixMetrique="k";
    calculDifference(vObsAnnot,nbobs,nba,nbc, &vPourcentageErreur, choixGold);

    std::pair<int, float> pairNbcAlpha(nbc, valeurMetrique);
    mapResultat.insert(pair<pair<int, float>, vector<float>>(pairNbcAlpha, vPourcentageErreur));
    //affichage résultats
    for(int i=0; i<nba-1; i++){
        cout << nba-i << ": " << mapResultat.at(pairNbcAlpha)[i] <<"%   ";
    }
    fichierSortie(choix, choixNbClasse, choixMetrique, choixGold, nba, mapResultat);*/

    return 0;
}
