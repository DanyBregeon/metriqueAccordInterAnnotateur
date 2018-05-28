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
    srand(time(NULL));
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
        choixTableau(314, vAnnotObs, nbobs, nba, nbc);
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

    float valeurMetriqueInit;
    valeurMetriqueInit = kappaAP(nbobs, nbc, nba, vObsAnnot);
    cout << "valeurMetrique: " << valeurMetriqueInit << endl;

    //generationAleatoire1(vPasChoisi, vAnnotObs, nba, nbobs, nbc);

    //generationAleatoire2(vAnnotObs, nba, nbobs, nbc);

    //generationAleatoire3(vPasChoisi, vAnnotObs, nba, nbobs, nbc);


    /*(2) GENERATION ALEATOIRE

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
    //le nombre d'erreur moyen de chaque annotateur
    float moyenneErreurP2;
    //le vote majoritaire pour chaque observable
    vector<int> voteMajoritaire;
    voteMajoritaire.resize(nbobs);
    //le nombre d'occurence des classes dans les annotations d'un observable
    int tNbOcc[nbc];
    //le nombre d'annotation manquante
    int nbInfoManquante = 0;

    //for(int occ=0; occ<10; occ++){
        for(int c=0; c<nbc; c++){
            tNbOcc[c]=0;
        }

        //calcul des votes majoritaires et de P1
        for(int obs=0; obs<nbobs; obs++){
            for(int ann=0; ann<nba; ann++){
                if(vObsAnnot[obs][ann]!=-1){
                    tNbOcc[vObsAnnot[obs][ann]]++;
                }else{
                    nbInfoManquante++;
                }
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
            cout << "voteMajoritaire : " << voteMaj << endl;
            //cout << obs << ":" << voteMajoritaire[obs] << "(" << max << ") ";
            pourcentageErreur[obs] = ((float) (nba - max - nbInfoManquante)/(float) (nba - nbInfoManquante))*100;
            nbErreurObs[obs] = nba - max - nbInfoManquante;
            cout << pourcentageErreur[obs] << endl;
            nbInfoManquante = 0;
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

    for(int occ=0; occ<2000; occ++){
        int intervalleMin = occ%(nbobs-1);//nbErreurMin;//occ;
        int intervalleMax = intervalleMin+1;//nbErreurMax;//occ+1;
        /*int intervalleMin = 0;
        int intervalleMax = nbobs;*/
        cout << occ << endl;
        //cout << endl << "P2= " << moyenneErreurP2 << "  (" << nbErreur << "/" << nba << ") , min=" << nbErreurMin << " , max=" << nbErreurMax << endl;
        /*cout << "choix intervalle :" << endl << "min=";
        cin >> intervalleMin;
        cout << "max=";
        cin >> intervalleMax;*/

        //generation des nouveaux annotateurs

        //I-on les ajoute à ceux déjà existant
        //ajouterNouveauAnnotateur(nba, nbobs, nbc, vAnnotObs, pourcentageErreur, voteMajoritaire);

        //II-on les met à la place de ceux déjà existant
        //remplacerParNouveauAnnotateur(nba, nbobs, nbc, vAnnotObs, pourcentageErreur, voteMajoritaire);

        //III-on les crée à part

        int nbNewAnnot = 9;//nba;
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
                    // //proba[obs]= 1 + nbErreurObs[obs] * nbErreurObs[obs] * nbErreurObs[obs];
                    //(pourcentageErreur[obs]/100)*(nbc/(nbc-1)) ->resultat entre 0 et 1, 0 entant tout le monde d'accord et 1 le plus de desaccord possible
                    proba[obs]= 1.0 + ((pourcentageErreur[obs]/100.0)*((float) nbc/(float) (nbc-1))*10.0)*((pourcentageErreur[obs]/100.0)*((float) nbc/(float) (nbc-1))*10.0);
                    //cout << obs << " proba : " << proba[obs] <<  " : " << (pourcentageErreur[obs]/100.0)*((float) nbc/(float) (nbc-1)) << endl;
                }else{
                    // //proba[obs]= proba[obs-1] + 1 + nbErreurObs[obs] * nbErreurObs[obs] * nbErreurObs[obs]; //1f/(float) nbobs;pourcentageErreur
                    proba[obs]= proba[obs-1] + 1.0 + ((pourcentageErreur[obs]/100.0)*((float) nbc/(float) (nbc-1))*10.0)*((pourcentageErreur[obs]/100.0)*((float) nbc/(float) (nbc-1))*10.0);
                    //cout << obs << " proba : " << proba[obs] <<  " : " << (pourcentageErreur[obs]/100.0)*((float) nbc/(float) (nbc-1)) << endl;
                    //cout << 1 + nbErreurObs[obs] * nbErreurObs[obs] << endl;
                    //cout << proba[obs] << endl;
                }
            }

            totalPossibilite = proba[nbobs-1];
            //on choisi un nombre aléatoire d'erreur compris dans l'intervalle
            int rng = intervalleMin + (rand() % (intervalleMax - intervalleMin));
            //cout << rng << ": ";
            for(int err=0; err<rng; err++){
                //int rngObs = rand() % vObsErreur.size();
                totalPossibilite = proba[proba.size()-1];
                if(totalPossibilite == 0){
                    cout << "BLBLLBLBBLLBLBLLBBBLLBLBLBBLBLLBLB";
                }
                int rngObs = rand() % (totalPossibilite);

                int itObs = 0;
                while(rngObs >= proba[itObs]){
                    itObs++;
                }
                vAnnotObs2[ann][vObsErreur[itObs]]+= 1 + (rand() % (nbc-1));
                vAnnotObs2[ann][vObsErreur[itObs]] = vAnnotObs2[ann][itObs] % nbc;
                //totalPossibilite -= (1 + nbErreurObs[itObs]*nbErreurObs[itObs]*nbErreurObs[itObs]);
                //cout << "itObs : " << itObs << " , vObsErreur[itObs] : " << vObsErreur[itObs] << "  ";
                //cout << proba[itObs] << " vs ";

                //cout << "itObs-2 : " <<proba[itObs-2] << " itObs-1 : " <<proba[itObs-1] << " " << itObs << " : " << proba[itObs] <<
                //" itObs+1 " << proba[itObs+1] << " " <<  proba.size()-1 << " : " << proba[proba.size()-1] << endl;
                for(int i=itObs; i<proba.size()-1; i++){
                    // //proba[i] = proba[i+1] - (1 + nbErreurObs[vObsErreur[itObs]]*nbErreurObs[vObsErreur[itObs]]*nbErreurObs[vObsErreur[itObs]]);
                    proba[i] = proba[i+1] - (int) (1.0 + ((pourcentageErreur[vObsErreur[itObs]]/100.0)*((float) nbc/(float) (nbc-1))*10.0)*((pourcentageErreur[vObsErreur[itObs]]/100.0)*((float) nbc/(float) (nbc-1))*10.0));
                }
                proba.pop_back();
                //cout << "itObs-2 : " <<proba[itObs-2] << " itObs-1 : " <<proba[itObs-1] << " " << itObs << " : " << proba[itObs] <<
                //" itObs+1 " << proba[itObs+1] << " " <<  proba.size()-1 << " : " << proba[proba.size()-1] << endl;
                //cout << proba[itObs] << endl;
                //cout << "test5" << endl; //erreur ici
                /*if(proba.begin()+ itObs == proba.end()){
                    cout << "AAAAAAAAA" << endl;
                }*/
                //cout << "itObs-2 : " <<vObsErreur[itObs-2] << " itObs-1 : " <<vObsErreur[itObs-1] << " itObs : " << vObsErreur[itObs] << " itObs+1 " << vObsErreur[itObs+1] << endl;
                //bilan : il ne faut pas mettre le -1 mais cela créer une erreur (proba.begin()+ itObs >= proba.end() -> pourquoi ?)
                //proba.erase(proba.begin()+ itObs -1);
                //cout << "test6" << endl;

                //vObsErreur.erase(vObsErreur.begin()+ itObs-1);
                for(int i=itObs; i<vObsErreur.size()-1; i++){
                    vObsErreur[i] = vObsErreur[i+1];
                }
                vObsErreur.pop_back();
                //cout << "itObs-2 : " <<vObsErreur[itObs-2] << " itObs-1 : " << vObsErreur[itObs-1] << " itObs : " << vObsErreur[itObs] << " itObs+1 " << vObsErreur[itObs+1] << endl << endl;
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
        cout << "valeurMetrique: " << valeurMetrique << endl;



        //affichage(nba, nbobs, vAnnotObs);
        ostringstream ss;
        ss << "generationAleatoireV2/generationAleatoire" << occ << ".csv";
        ecrireFichier(nbNewAnnot, nbobs, nbc, vAnnotObs2, ss.str());
    }

    return 0;
}
