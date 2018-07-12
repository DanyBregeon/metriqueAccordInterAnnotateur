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
#include <stdio.h>
#include "lectureFichier.h"

#ifdef _WIN32
    #include <io.h>
    #define OS_Windows
#else
    #include <sys/stat.h>
#endif

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

void fichierSortie(string nomFichier, string choixMetrique, int nba, map<pair<int, float>, vector<float> > mapResultat){
    //création fichier de sortie
    ostringstream ss;
    ss << "resultats_";
    ss << "" + nomFichier;
    if(choixMetrique.compare("a")==0){
        ss<<"_alpha";
    }else if(choixMetrique.compare("k")==0){
        ss<<"_kappa";
    }else if(choixMetrique.compare("pi")==0){
        ss<<"_pi";
    }else if(choixMetrique.compare("ap")==0){
        ss<<"_alphaPondéré";
    }

    #ifdef OS_Windows
        mkdir("resultats");
    #else
        mkdir("resultats", S_IRUSR | S_IWUSR | S_IXUSR | S_IRGRP | S_IWGRP | S_IXGRP | S_IROTH | S_IWOTH | S_IXOTH);
    #endif
    ofstream myfile;
	string s = "resultats/"+ss.str()+".txt";
    myfile.open (s.c_str());
    //cout << ss.str();

    for (map<pair<int, float>, vector<float> >::iterator it=mapResultat.begin(); it!=mapResultat.end(); ++it){
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
        if(m.first != 0){ //si = 0 alors il s'agit de l'ecart type
            myfile << m.first <<" classes, "<< s << "=" << fixed << setprecision (3) << m.second << "   ";
            myfile << fixed << setprecision (3);
            for(int i=0; i<nba-1; i++){
                vector<float> valeurs = it->second;
                 myfile << valeurs[i] <<"\t";
            }
            myfile << endl;
        }
    }
    myfile.close();
}



//deprecated, ne sert plus
void fichierSortieCorpus (int choixCorpus, int choixNbClasse, string choixMetrique, int choixGold,
                   int nba, map<pair<int, float>, vector<float> > mapResultat)
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

    /*if(choixGold==1){
        ss<<"goldParAnnot";
    }else if(choixGold==2){
        ss<<"goldParAnnotSansAnnotCommun";
    }else if(choixGold==3){
        ss<<"moyenneParAnnot";
    }else if(choixGold==4){
        ss<<"moyenne";
    }*/

    ss << nba;
    ofstream myfile;
	string s = "resultats/"+ss.str()+".txt";
    myfile.open (s.c_str());

    //myfile << currentDateTime() << endl << endl;

    for (map<pair<int, float>, vector<float> >::iterator it=mapResultat.begin(); it!=mapResultat.end(); ++it){
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
