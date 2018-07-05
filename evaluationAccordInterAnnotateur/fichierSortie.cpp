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
#include <iomanip>
#include "lectureFichier.h"

using namespace std;

// retourne la date actuelle, format: YYYY-MM-DD.HH:mm:ss
const std::string currentDateTime() {
    time_t now = time(0);
    struct tm tstruct;
    char buf[80];
    tstruct = *localtime(&now);
    strftime(buf, sizeof(buf), "%Y-%m-%d.%X", &tstruct);

    return buf;
}

void fichierSortie (int choixCorpus, int choixNbClasse, string choixMetrique, int choixGold,
                   int nba, map<pair<int, float>, vector<float>> mapResultat)
{
    //création fichier de sortie
    ostringstream ss;
    switch (choixCorpus)
    {
        case 0:
            ss<<"generationAleatoire_";
            break;
        case 1:
            ss<<"All_";
            break;
        case 2:
            ss<<"Emotion_";
            break;
        case 3:
             ss<<"Opinion_";
            break;
        case 4:
             ss<<"Coreference_";
            break;
        default:
            ss<< choixCorpus << "_";
    }
    if(choixNbClasse==1){
        ss << "Allclasses_";
    }else{
        ss << choixNbClasse << "classes_";
    }

    if(choixMetrique.compare("a")==0){
        ss<<"alpha_";
    }else if(choixMetrique.compare("k")==0){
        ss<<"kappa_";
    }else if(choixMetrique.compare("pi")==0){
        ss<<"pi_";
    }else if(choixMetrique.compare("ap")==0){
        ss<<"alphaPondéré_";
    }

    if(choixGold==1){
        ss<<"goldParAnnot";
    }/*else if(choixGold==2){
        ss<<"goldParAnnotSansAnnotCommun";
    }else if(choixGold==3){
        ss<<"moyenneParAnnot";
    }else if(choixGold==4){
        ss<<"moyenne";
    }*/

    ss << nba;
    ofstream myfile;
    myfile.open ("resultats/"+ss.str()+".txt");

    //myfile << currentDateTime() << endl << endl;

    for (map<pair<int, float>, vector<float>>::iterator it=mapResultat.begin(); it!=mapResultat.end(); ++it){
        pair<int, float> m = it->first;
        string s;
        if(choixMetrique.compare("k")==0){
            s = "kappa";
        }else if(choixMetrique.compare("ap")==0){
            s = "alpha(pondéré)";
        }else if(choixMetrique.compare("pi")==0){
            s = "pi";
        }else{
            s = "alpha";
        }
        myfile << m.first <<" classes, "<< s << "=" << fixed << setprecision (3) << m.second << "   ";
        myfile << fixed << setprecision (3);
        for(int i=0; i<nba-1; i++){
            vector<float> valeurs = it->second;
             myfile << valeurs[i] <<"\t";
             /*if(choixGold==3 || choixGold==4){
                myfile << nba-i << ":  " << valeurs[i] <<"   ";
             }else{
                if(valeurs[i]<10){
                    myfile << nba-i << ":  " << valeurs[i] <<"%   ";
                }else{
                    myfile << nba-i << ": " << valeurs[i] <<"%   ";
                }
             }*/
        }
        myfile << endl;
        //myfile << endl;
    }
    myfile.close();
}
