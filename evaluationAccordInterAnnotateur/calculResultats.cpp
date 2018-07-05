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
#include <map>
#include <algorithm>
#include "calculResultats.h"

using namespace std;

extern int nba;
extern int nbobs;
extern int nbc;

void calculPrevalence(vector<pair<float, vector<float>>> & vPrevalence, float valeurMetrique, vector<vector<int>> & vAnnotObs/*,
                      int nbc, int nba, int nbobs*/){
    int occClasse[nbc];
    for(int i=0; i<nbc; i++){
        occClasse[i]=0;
    }
    for(int ann=0; ann<nba; ann++){
        for(int obs=0; obs<nbobs; obs++){
            occClasse[vAnnotObs[ann][obs]]++;
        }
    }
    vector<float> prevalences;
    prevalences.resize(nbc);
    for(int i=0; i<nbc; i++){
        prevalences[i]=((float)occClasse[i]/(float)(nba*nbobs))*100;
    }
    vPrevalence.push_back(pair<float, vector<float>>(valeurMetrique, prevalences));
    //affichage
    /*for(int i=0; i<nbc; i++){
        cout << i << " : " << ((float)occClasse[i]/(float)(nba*nbobs))*100 << "%    ";
    }
    cout << endl;*/
}

void calculPalier(map<pair<int, float>, vector<float>> & mapResultat, map<pair<int, float>, vector<float>> & mapResultat2, float palierPas,
                  vector<vector<float>> & vResultats, vector<vector<float>> & vResultatsEcartType, int moyenneEcartType, /*int nbc, int nba,*/
                  vector<pair<float, vector<float>>> & vPrevalence, vector<vector<float>> & vPrevalencePalier){
    float palier = palierPas;
    int occPalier = 0;
    int indexPalier = 0;
    if(moyenneEcartType == 0){
        for(int i=0; i<vPrevalence.size(); i++){
            if(vPrevalence[i].first < palier-(palierPas/2)){
                occPalier++;
                for(int j=0; j<nbc; j++){
                    vPrevalencePalier[indexPalier][j] += (vPrevalence[i].second)[j];
                }
            }else{
                if(occPalier>0){
                        cout << palier-palierPas << " : ";
                    for(int j=0; j<nbc; j++){
                        vPrevalencePalier[indexPalier][j] = vPrevalencePalier[indexPalier][j] / (float) occPalier;
                        cout << j << ": " << vPrevalencePalier[indexPalier][j] << "  ";
                    }
                    cout << endl;
                    //pair<int, float> pairNbcAlpha(-1, palier-palierPas);
                    //mapResultat2.insert(pair<pair<int, float>, vector<float>>(pairNbcAlpha, vPrevalencePalier[indexPalier]));
                }
                palier += palierPas;
                occPalier = 0;
                indexPalier++;
            }
        }
        if(occPalier>0){
            cout << palier-palierPas << " : ";
            for(int j=0; j<nbc; j++){
                vPrevalencePalier[indexPalier][j] = vPrevalencePalier[indexPalier][j] / (float) occPalier;
                cout << j << ": " << vPrevalencePalier[indexPalier][j] << "  ";
            }
            cout << endl;
            //pair<int, float> pairNbcAlpha(-1, palier-palierPas);
            //mapResultat2.insert(pair<pair<int, float>, vector<float>>(pairNbcAlpha, vPrevalencePalier[indexPalier]));
        }
    }


    palier = palierPas;
    occPalier = 0;
    indexPalier = 0;
    for (map<pair<int, float>, vector<float>>::iterator it=mapResultat.begin(); it!=mapResultat.end(); ++it){
        pair<int, float> m = it->first;
        vector<float> valeurs = it->second;
        //pour chaque génération aléatoire, on regarde s'il est dans le palier actuel: tant qu'il ne l'est pas on change de palier
        while(m.second>=/*palier*/palier-(palierPas/2)){
            //s'il y a au moins une génération aléatorie dans ce palier
            if(occPalier > 0){
                //on calcul la moyenne/l'éécart type pour chaque groupe de N annotateurs
                for(int i=0; i<nba-1; i++){
                    if(moyenneEcartType == 0){
                        vResultats[indexPalier][i] = vResultats[indexPalier][i] / (float) occPalier;
                        if(i==0){
                            cout << indexPalier*palierPas << ": " << occPalier << " occurences" << endl;
                        }
                    }else{
                        vResultatsEcartType[indexPalier][i] = sqrt(vResultatsEcartType[indexPalier][i] / (float) occPalier);
                    }
                }
                if(moyenneEcartType == 0){
                    pair<int, float> pairNbcAlpha(nbc, palier-palierPas);
                    mapResultat2.insert(pair<pair<int, float>, vector<float>>(pairNbcAlpha, vResultats[indexPalier]));
                }else{
                    pair<int, float> pairNbcAlpha(0, palier-palierPas);
                    mapResultat2.insert(pair<pair<int, float>, vector<float>>(pairNbcAlpha, vResultatsEcartType[indexPalier]));
                }
            }
            palier += palierPas;
            occPalier = 0;
            indexPalier++;
        }
        occPalier++;
        for(int i=0; i<nba-1; i++){
            if(moyenneEcartType == 0){
                vResultats[indexPalier][i] += valeurs[i];
            }else{
                vResultatsEcartType[indexPalier][i] += (valeurs[i]-vResultats[indexPalier][i]) * (valeurs[i]-vResultats[indexPalier][i]);
            }
        }
    }
    if(occPalier > 0){
        for(int i=0; i<nba-1; i++){
            if(moyenneEcartType == 0){
                vResultats[indexPalier][i] = vResultats[indexPalier][i] / (float) occPalier;
                if(i==0){
                    cout << indexPalier*palierPas << ": " << occPalier << " occurences" << endl;
                }
            }else{
                vResultatsEcartType[indexPalier][i] = sqrt(vResultatsEcartType[indexPalier][i] / (float) occPalier);
            }
        }
        if(moyenneEcartType == 0){
            pair<int, float> pairNbcAlpha(nbc, palier-palierPas);
            mapResultat2.insert(pair<pair<int, float>, vector<float>>(pairNbcAlpha, vResultats[indexPalier]));
        }else{
            pair<int, float> pairNbcAlpha(0, palier-palierPas);
            mapResultat2.insert(pair<pair<int, float>, vector<float>>(pairNbcAlpha, vResultatsEcartType[indexPalier]));
        }
    }

    //affichage
    int nbPalier = 2 + (int)(1.0f/palierPas);
    for(int i=0; i<nbPalier; i++){
        cout << i*palierPas << ": ";
        for(int j=0; j<nba-1; j++){
            if(moyenneEcartType == 0){
                cout << vResultats[i][j] << " ";
            }else{
                cout << vResultatsEcartType[i][j] << " ";
            }
        }
        cout << endl;
    }
}

void resultatsPalier(map<pair<int, float>, vector<float>> & mapResultat, map<pair<int, float>, vector<float>> & mapResultat2,
                     float palierPas, /*int nbc, int nba,*/ vector<pair<float, vector<float>>> & vPrevalence){

    int nbPalier = 2;
    nbPalier += (int)(1.0f/palierPas);
    cout << "nbPalier : " << nbPalier << endl;

    sort(vPrevalence.begin(), vPrevalence.end());
    //affichage pérvalence
    /*for(int i=0; i<vPrevalence.size(); i++){
            cout << vPrevalence[i].first << " :  ";
        for(int j=0; j<nbc; j++){
            cout << j << " : " << (vPrevalence[i].second)[j] << "  ";
        }
        cout << endl;
    }*/

    vector<vector<float>> vResultats;
    vector<vector<float>> vResultatsEcartType;
    vector<vector<float>> vPrevalencePalier;
    vResultats.resize(nbPalier);
    vResultatsEcartType.resize(nbPalier);
    vPrevalencePalier.resize(nbPalier);
    for(int i=0; i<nbPalier; i++){
        vResultats[i].resize(nba-1);
        vResultatsEcartType[i].resize(nba-1);
        vPrevalencePalier[i].resize(nbc);
    }
    for(int i=0; i<nbPalier; i++){
        for(int j=0; j<nba-1; j++){
            vResultats[i][j]=0;
            vResultatsEcartType[i][j]=0;
        }
        for(int j=0; j<nbc; j++){
            vPrevalencePalier[i][j]=0;
        }
    }

    //moyenne
    calculPalier(mapResultat, mapResultat2, palierPas, vResultats, vResultatsEcartType, 0, /*nbc, nba,*/ vPrevalence, vPrevalencePalier);
    //ecart type
    calculPalier(mapResultat, mapResultat2, palierPas, vResultats, vResultatsEcartType, 1, /*nbc, nba,*/ vPrevalence, vPrevalencePalier);
    /*float palier = palierPas:
    int occPalier = 0;
    int indexPalier=0;
    for (map<pair<int, float>, vector<float>>::iterator it=mapResultat.begin(); it!=mapResultat.end(); ++it){
        pair<int, float> m = it->first;
        vector<float> valeurs = it->second;
        while(m.second>=palier){
            if(occPalier > 0){
                for(int i=0; i<nba-1; i++){
                    vResultats[indexPalier][i] = vResultats[indexPalier][i] / (float) occPalier;
                    //vResultatsEcartType[indexPalier][i] = sqrt(vResultatsEcartType[indexPalier][i] / (float) occPalier);
                }
                pair<int, float> pairNbcAlpha(nbc, palier-palierPas);
                mapResultat2.insert(pair<pair<int, float>, vector<float>>(pairNbcAlpha, vResultats[indexPalier]));
                //mapResultat2.insert(pair<pair<int, float>, vector<float>>(pairNbcAlpha, vResultatsEcartType[indexPalier]));
            }
            palier += palierPas;
            occPalier = 0;
            indexPalier++;
        }
        occPalier++;
        for(int i=0; i<nba-1; i++){
            vResultats[indexPalier][i] += valeurs[i];
            //vResultatsEcartType[indexPalier][i] += (valeurs[i]-vResultats[indexPalier][i]) * (valeurs[i]-vResultats[indexPalier][i]);
        }

    }
    if(occPalier > 0){
        for(int i=0; i<nba-1; i++){
            vResultats[indexPalier][i] = vResultats[indexPalier][i] / (float) occPalier;
            //vResultatsEcartType[indexPalier][i] = sqrt(vResultatsEcartType[indexPalier][i] / (float) occPalier);
        }
        pair<int, float> pairNbcAlpha(nbc, palier-0.05f);
        mapResultat2.insert(pair<pair<int, float>, vector<float>>(pairNbcAlpha, vResultats[indexPalier]));
        //mapResultat2.insert(pair<pair<int, float>, vector<float>>(pairNbcAlpha, vResultatsEcartType[indexPalier]));
    }

    //affichage
    for(int i=0; i<nbPalier; i++){
        for(int j=0; j<nba-1; j++){
            cout << vResultats[i][j] << " ";
        }
        cout << endl;
    }

    //ecart-type
    vector<vector<float>> vResultatsEcartType;
    vResultatsEcartType.resize(nbPalier);
    for(int i=0; i<nbPalier; i++){
        vResultatsEcartType[i].resize(nba-1);
      }

    for(int i=0; i<nbPalier; i++){
        for(int j=0; j<nba-1; j++){
            vResultatsEcartType[i][j]=0;
        }
    }

    palier = palierPas;
    occPalier = 0;
    indexPalier=0;
    for (map<pair<int, float>, vector<float>>::iterator it=mapResultat.begin(); it!=mapResultat.end(); ++it){
        pair<int, float> m = it->first;
        vector<float> valeurs = it->second;
        while(m.second>=palier){
            if(occPalier > 0){
                for(int i=0; i<nba-1; i++){
                    vResultatsEcartType[indexPalier][i] = sqrt(vResultatsEcartType[indexPalier][i] / (float) occPalier);
                }
                pair<int, float> pairNbcAlpha(10, palier-0.05f);
                mapResultat2.insert(pair<pair<int, float>, vector<float>>(pairNbcAlpha, vResultatsEcartType[indexPalier]));
            }
            palier += 0.05f;
            occPalier = 0;
            indexPalier++;
        }
        occPalier++;
        for(int i=0; i<nba-1; i++){
            vResultatsEcartType[indexPalier][i] += (valeurs[i]-vResultats[indexPalier][i]) * (valeurs[i]-vResultats[indexPalier][i]);
        }

    }
    if(occPalier > 0){
        for(int i=0; i<nba-1; i++){
            vResultatsEcartType[indexPalier][i] = sqrt(vResultatsEcartType[indexPalier][i] / (float) occPalier);
        }
        pair<int, float> pairNbcAlpha(10, palier-0.05f);
        mapResultat2.insert(pair<pair<int, float>, vector<float>>(pairNbcAlpha, vResultatsEcartType[indexPalier]));
    }*/

}
