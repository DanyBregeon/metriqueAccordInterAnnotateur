#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <cmath>
#include <string>
#include <algorithm>
#include <fstream>
#include <cstdio>
#include <map>
#include <iomanip>
#include <dirent.h>
#include "calculMetrique.h"
#include "lectureFichier.h"
#include "combinaison.h"
#include "fichierSortie.h"
#include "calculResultats.h"

using namespace std;

int nba, nbobs, nbc;//nb d'annotateurs, d'observables, de classes

int choixGold = 0; //choix de la référence : 0 est pour la référence idéale au maximum d'annotateur
//int choixSortie = 0; //0 pour les pourcentages de modification pour chaque fichier, 1 pour la moyenne et l'écart type de ces pourcentages par palier, 2 pour les 2 en même temps
int choixErreurPondere = 0; //0 pour non, 1 pour oui

int main()
{
    //int tAnnotObs[Max_annot][Max_obs],tObsAnnot[Max_obs][Max_annot];
    vector<vector<int> > vAnnotObs;
    vector<vector<int> > vObsAnnot;
    //float C[Max_classes][Max_classes];
    vector<vector<float> > C;
    //int nba, nbobs, nbc;//nb d'annotateurs, d'observables, de classes
    float nb;//observation réellement prises en compte

    //tableau final avec les pourcentages de différences de votes majoritaires par rapport à la référence
    //en fonction du nombre de classes et de la valeur de la métrique
    std::map<std::pair<int, float>, std::vector<float> > mapResultat;

    string reponse;
    do{
        cout << "1 pour activer le mode avancee, 0 sinon : ";
        cin >> reponse;
    }while(reponse != "0" && reponse != "1");

    if(reponse.compare("1")==0){ //mode avancée : on peut changer la référence, si les erreurs sont pondérées et les résultats que l'on veut
        do{
            cout << "choix de la reference : 0 pour reference ideale, 1 pour reference par nombre d'annotateurs : ";
            cin >> reponse;
        }while(reponse != "0" && reponse != "1");
        choixGold = atoi(reponse.c_str());
        do{
            cout << "erreur pondere : 0 pour non, 1 pour oui : ";
            cin >> reponse;
        }while(reponse != "0" && reponse != "1");
        choixErreurPondere = atoi(reponse.c_str());
        /*do{
            cout << "resultats : 0 pour les pourcentages de modification pour chaque fichier, 1 pour la moyenne et l'ecart type de ces pourcentages par palier, 2 pour les 2 en meme temps : ";
            cin >> reponse;
        }while(reponse != "0" && reponse != "1" && reponse != "2");
        choixSortie = atoi(reponse.c_str());*/
    }

    bool erreur = false;
    do{
        string choixFichier;
        string choixMetrique;

        cout << "Chemin du fichier ou dossier : ";
        cin >> choixFichier;

        if(choixFichier.size() > 4 && choixFichier.substr(choixFichier.size()-4).compare(".csv")==0){ //cas un seul fichier
            if(lire(choixFichier, vAnnotObs, nbobs, nba, nbc) == 0){
                erreur = true;
            }else{
                erreur = false;
                do{
                    cout << "Choix de la metrique : a pour alpha, k pour kappa, pi pour pi, ap pour alpha pondere : ";
                    cin >> choixMetrique;
                }while(choixMetrique != "a" && choixMetrique != "k" && choixMetrique != "pi" && choixMetrique != "ap");
                //afficheTableauLu(vAnnotObs,nbobs,nba); //on affiche le tableau d'annotations
                //tableau de coincidences :
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
                //cout << "Tableau des coincidences :\n";
                coincidences(vObsAnnot,nbobs,nba,C,nbc,nb);
                //affiche_coincidences(C,nbc);
                //cout << "nb d'observations prises en compte="<< nb <<endl;

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

                cout << metrique << endl << endl;

                cout << "pourcentages de modification : (le pourcentage de votes majoritaires different avec k<" << nba;
                cout << " annotateurs par rapport a " << nba << " annotateurs)" << endl;

                std::vector<float> vPourcentageErreur;

                calculDifference(vObsAnnot, &vPourcentageErreur);

                pair<int, float> pairNbcAlpha(nbc, metrique);
                mapResultat.insert(pair<pair<int, float>, vector<float> >(pairNbcAlpha, vPourcentageErreur));
                //affichage résultats
                for(int i=0; i<nba-1; i++){
                    cout << nba-i << ": " << mapResultat.at(pairNbcAlpha)[i] <<"%   ";
                }
                choixFichier = choixFichier.substr(0, choixFichier.size()-4);
                replace(choixFichier.begin(), choixFichier.end(), '/', '_');
                fichierSortie(choixFichier, choixMetrique, nba, mapResultat);
            }
        }else{ //cas plusieurs fichiers

            DIR * rep = opendir(choixFichier.c_str());

            vector<string> files;

            if (rep == NULL){ //si ce n'était pas un répertoire
                erreur = true;
            }
            else
            {
                erreur = false;
                if(choixFichier[choixFichier.size()-1]!='/'){
                    choixFichier += "/";
                }

                struct dirent * ent;

                while ((ent = readdir(rep)) != NULL)
                {
                    string f = choixFichier + ent->d_name;
                    if(f.size() > 4 && f.substr(f.size()-4).compare(".csv")==0){
                        cout << f << endl;
                        files.push_back(f);
                    }
                }
                closedir(rep);

                replace(choixFichier.begin(), choixFichier.end(), '/', '_');


                do{
                    cout << "Choix de la metrique : a pour alpha, k pour kappa, pi pour pi, ap pour alpha pondere : ";
                    cin >> choixMetrique;
                }while(choixMetrique != "a" && choixMetrique != "k" && choixMetrique != "pi" && choixMetrique != "ap");
                //prevalence
                vector<pair<float, vector<float> >> vPrevalence;
                //vPrevalence.resize(files.size());
                for(int i=0; i<files.size(); i++){
                    cout << (i+1) << " / " << files.size() << endl;
                    //if(i%2000==0) cout << i << endl; //affichage
                    if(lire(files[i], vAnnotObs, nbobs, nba, nbc)==1){
                        //choixTableau(files[i],vAnnotObs,nbobs,nba,nbc);
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
                        //prevalence
                        calculPrevalence(vPrevalence, metrique, vAnnotObs);

                        pair<int, float> pairNbcAlpha(nbc, metrique);
                        //cout << " pairNbcAlpha: " << pairNbcAlpha.first << "/" << pairNbcAlpha.second;
                        //cout<< "combinaisons de n-p annotateurs :" <<endl;
                        calculDifference(vObsAnnot, &vPourcentageErreur);
                        mapResultat.insert(pair<pair<int, float>, vector<float> >(pairNbcAlpha, vPourcentageErreur));
                    }
                }

                //affichage résultats
                bool changement = false; //si le nombre de classes change
                for (map<std::pair<int, float>, vector<float> >::iterator it=mapResultat.begin(); it!=mapResultat.end(); ++it){
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
                    if(m.first != nbc){
                        changement = true;
                    }
                    cout << m.first <<" classes, "<< s << "=" << fixed << setprecision (3) << m.second << "   ";
                    for(int i=0; i<nba-1; i++){
                        vector<float> valeurs = it->second;
                        cout << nba-i << ": " << valeurs[i] <<"%   ";
                    }
                    cout << endl;
                    cout << endl;
                }
                map<pair<int, float>, vector<float> > mapResultat2;
                if(!changement){
                    resultatsPalier(mapResultat, mapResultat2, 0.05f, vPrevalence);
                    fichierSortie(choixFichier, choixMetrique, nba, mapResultat2);
                }else{
                    fichierSortie(choixFichier, choixMetrique, nba, mapResultat);
                }

            }
        }
    }while(erreur);

    system("PAUSE");

    /*
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
    cout << "Choix ? (gold: 0 pour verite ideale, 1 pour en fonction du nombre d'annotateurs retenus, 2 pareil mais sans annotateur commun, 3 prevalence)";
    cin >> choixGold;

    //tableau final avec les pourcentages de différences de votes majoritaires par rapport à la référence
    //en fonction du nombre de classes et de la valeur de la métrique
    std::map<std::pair<int, float>, std::vector<float> > mapResultat;



    if(choix<100){
        vector<int> lesChoix;
        if(choix==0){ //cas generation aleatoire
            for(int i=1000; i<3000; i++){
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
        //prevalence
        vector<pair<float, vector<float> >> vPrevalence;
        //vPrevalence.resize(lesChoix.size());
        for(int i=0; i<lesChoix.size(); i++){
            //if(i%2000==0) cout << i << endl; //affichage

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
            //prevalence
            calculPrevalence(vPrevalence, metrique, vAnnotObs);

            pair<int, float> pairNbcAlpha(nbc, metrique);
            //cout << " pairNbcAlpha: " << pairNbcAlpha.first << "/" << pairNbcAlpha.second;
            //cout<< "combinaisons de n-p annotateurs :" <<endl;
            calculDifference(vObsAnnot, &vPourcentageErreur, choixGold);
            mapResultat.insert(pair<pair<int, float>, vector<float> >(pairNbcAlpha, vPourcentageErreur));
        }

        //affichage résultats
        for (map<std::pair<int, float>, vector<float> >::iterator it=mapResultat.begin(); it!=mapResultat.end(); ++it){
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
        map<pair<int, float>, vector<float> > mapResultat2;
        resultatsPalier(mapResultat, mapResultat2, 0.05f, vPrevalence);
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

        if(choixGold == 3){
            cout << "prevalence :  ";
            vector<pair<float, vector<float> >> vPrevalence;
            calculPrevalence(vPrevalence, metrique, vAnnotObs);
            for(int c=0; c<nbc; c++){
                cout << c << " : " << (vPrevalence[0].second)[c] << endl;
            }
        }else{
            cout << "combinaisons de n-p annotateurs :" <<endl;

            std::vector<float> vPourcentageErreur;

            calculDifference(vObsAnnot, &vPourcentageErreur, choixGold);

            pair<int, float> pairNbcAlpha(nbc, metrique);
            mapResultat.insert(pair<pair<int, float>, vector<float> >(pairNbcAlpha, vPourcentageErreur));
            //affichage résultats
            for(int i=0; i<nba-1; i++){
                cout << nba-i << ": " << mapResultat.at(pairNbcAlpha)[i] <<"%   ";
            }
            fichierSortie(choix, choixNbClasse, choixMetrique, choixGold, nba, mapResultat);
        }
    }*/
    return 0;
}
