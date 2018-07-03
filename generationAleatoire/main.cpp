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

int nba = 25;
int nbobs = 100;
int nbc = 3;

void affichage(vector<vector<int>> & vAnnotObs){
    for (int i=0;i<nba;i++) {
        for (int j=0;j<nbobs;j++)
            printf("%3d",vAnnotObs[i][j]);
        cout << endl;
    }
}

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


void testPrevalence(vector<vector<int>> & vAnnotObs, int classe1, int classe2, int nba, int taille){
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

void probaErreurAnnotateur(vector<vector<int>> & tNbOcc, vector<int> & voteMajoritaire, int nbobs, int nbc, vector<vector<int>> & probaClasseErreur, vector<int> & totalPossibiliteClasse){
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

void repartitionP1(int nbNewAnnot, vector<int> & nbErreurObs){
    int taille = ((float)(nbc-1)/(float)nbc)*(float)nbNewAnnot + 1;
        int testOcc[taille];
        for(int i=0; i<taille; i++){
            testOcc[i]=0;
        }
        for(int obs=0; obs<nbobs; obs++){
            testOcc[nbErreurObs[obs]]++;
        }
        for(int i=0; i<taille; i++){
            cout << /*i << "  " <<*/ testOcc[i] << endl;
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

int main()
{
    srand(time(NULL));
    /*int nba = 25;
    int nbobs = 100;
    int nbc = 3;*/
    vector<vector<int>> vAnnotObs;
    vector<vector<int>> vObsAnnot;
    vector<pair<int,int>> vPasChoisi;

    int choix;
    cout << "Choix ? (0 pour artificielle, 1 pour reelle)";
    cin >> choix ;

    //initialise les vecteurs avec les données
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
        //testPrevalence(vAnnotObs, 1, 2, nba, nbobs/2);
    }

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

            cout << pourcentageErreur[obs] << endl;
            moyenneErreurP1 += pourcentageErreur[obs];

            // !!! test sur données artificielles !!!
            //pourcentageErreur[obs] = ((float) (( (float)obs/(float)nbobs)*(float)nba) /(float) (nba))*100;
            /*if(choix==0){
                if((float)obs < ((float)nbobs)*((float)30/(float)100)){
                    pourcentageErreur[obs] = ((nba/2) /(float) (nba))*100;
                }else if((float)obs < ((float)nbobs)*((float)55/(float)100)){
                    pourcentageErreur[obs] = ((nba/4) /(float) (nba))*100;
                }
                else if((float)obs < ((float)nbobs)*((float)75/(float)100)){
                    pourcentageErreur[obs] = ((nba/8) /(float) (nba))*100;
                }
                else if((float)obs < ((float)nbobs)*((float)90/(float)100)){
                    pourcentageErreur[obs] = ((nba/16) /(float) (nba))*100;
                }else{
                    pourcentageErreur[obs] = 0;
                }
            }*/
        }

        //test erreur pas totalement aléatoire
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


        probaErreurAnnotateur(tNbOcc, voteMajoritaire, nbobs, nbc, probaClasseErreur, totalPossibiliteClasse);

        // test répartition p1
        repartitionP1(nba, nbErreurObs);
        //fin test test répartition p1

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
    for(int occ=0; occ<130; occ++){
        //l'intervalle d'erreur des annotateurs (chaque annotateur fera entre intervalleMin et intervalleMax erreurs) (max exclu)
        //cas où l'intervalle est de 1
        int intervalleMin = occ%(int)((float)nbobs*sqrt((float)(nbc-1)/(float)(nbc)));//occ%(nbobs-1);//nbErreurMin;//occ;
        int intervalleMax = intervalleMin+1;//nbErreurMax;//occ+1;
        //cas où l'intervalle est le plus grand possible
        /*int intervalleMin = 0;
        int intervalleMax = (occ%(nbobs/2))*2;
        if(intervalleMax==0) intervalleMax=1;*/
        //cout << occ << endl;
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
                    //proba[obs]= 1.0 + ((pourcentageErreur[obs]/100.0)*((float) nbc/(float) (nbc-1))*10.0)*((pourcentageErreur[obs]/100.0)*((float) nbc/(float) (nbc-1))*10.0);

                    proba[obs]= 1.0 + (pourcentageErreur[obs]*((float) nbc/(float) (nbc-1)))*(pourcentageErreur[obs]*((float) nbc/(float) (nbc-1)))/100.0;
                    /*if(pourcentageErreur[obs]<1){
                        proba[obs]= 1;
                    }else{
                        proba[obs]=pourcentageErreur[obs];
                    }*/
                    //cout << obs << " proba : " << proba[obs] <<  " : " << (pourcentageErreur[obs]/100.0)*((float) nbc/(float) (nbc-1)) << endl;
                }else{
                    // //proba[obs]= proba[obs-1] + 1 + nbErreurObs[obs] * nbErreurObs[obs] * nbErreurObs[obs]; //1f/(float) nbobs;pourcentageErreur
                    //proba[obs]= proba[obs-1] + 1.0 + ((pourcentageErreur[obs]/100.0)*((float) nbc/(float) (nbc-1))*10.0)*((pourcentageErreur[obs]/100.0)*((float) nbc/(float) (nbc-1))*10.0);

                    proba[obs]= proba[obs-1] + 1.0 + (pourcentageErreur[obs]*((float) nbc/(float) (nbc-1)))*(pourcentageErreur[obs]*((float) nbc/(float) (nbc-1)))/100.0;
                    /*if(pourcentageErreur[obs]<1){
                        proba[obs]= proba[obs-1] + 1.0;
                    }else{
                        proba[obs]= proba[obs-1] + pourcentageErreur[obs];
                    }*/
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
                //l'annotateur ann fait une erreur à l'observable vObsErreur[itObs]
                //cas où l'erreur est choisi aléatoirement parmi les classes non majoritaire
                vAnnotObs2[ann][vObsErreur[itObs]]+= 1 + (rand() % (nbc-1));
                vAnnotObs2[ann][vObsErreur[itObs]] = vAnnotObs2[ann][vObsErreur[itObs]] % nbc;
                //cas où la classe de l'erreur dépend des données de départ
                /*int rngClasse = rand() % totalPossibiliteClasse[vObsErreur[itObs]];
                int itClasse = 0;
                while(rngClasse >= probaClasseErreur[vObsErreur[itObs]][itClasse]){
                    itClasse++;
                }
                vAnnotObs2[ann][vObsErreur[itObs]] = itClasse;*/
                //cas où l'erreur peut être choisi totalement au hasard (et peut donc ne pas en être une)
                /*int aleatoire = rand()%2;
                if(aleatoire==0){
                    vAnnotObs2[ann][vObsErreur[itObs]]= rand() % nbc;
                }else{
                    vAnnotObs2[ann][vObsErreur[itObs]]+= 1 + (rand() % (nbc-1));
                    vAnnotObs2[ann][vObsErreur[itObs]] = vAnnotObs2[ann][vObsErreur[itObs]] % nbc;
                }*/




                //totalPossibilite -= (1 + nbErreurObs[itObs]*nbErreurObs[itObs]*nbErreurObs[itObs]);
                //cout << "itObs : " << itObs << " , vObsErreur[itObs] : " << vObsErreur[itObs] << "  ";
                //cout << proba[itObs] << " vs ";

                //cout << "itObs-2 : " <<proba[itObs-2] << " itObs-1 : " <<proba[itObs-1] << " " << itObs << " : " << proba[itObs] <<
                //" itObs+1 " << proba[itObs+1] << " " <<  proba.size()-1 << " : " << proba[proba.size()-1] << endl;
                for(int i=itObs; i<proba.size()-1; i++){
                    // //proba[i] = proba[i+1] - (1 + nbErreurObs[vObsErreur[itObs]]*nbErreurObs[vObsErreur[itObs]]*nbErreurObs[vObsErreur[itObs]]);
                    //proba[i] = proba[i+1] - (int) (1.0 + ((pourcentageErreur[vObsErreur[itObs]]/100.0)*((float) nbc/(float) (nbc-1))*10.0)*((pourcentageErreur[vObsErreur[itObs]]/100.0)*((float) nbc/(float) (nbc-1))*10.0));

                    proba[i] = proba[i+1] - (int) (1.0 + (pourcentageErreur[vObsErreur[itObs]]*((float) nbc/(float) (nbc-1)))*(pourcentageErreur[vObsErreur[itObs]]*((float) nbc/(float) (nbc-1)))/100.0);
                    /*if(pourcentageErreur[vObsErreur[itObs]]<1){
                        proba[i]= proba[i+1] - 1;
                    }else{
                        proba[i]= proba[i+1] - (int)(pourcentageErreur[vObsErreur[itObs]]);
                    }*/
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
        cout << "Metrique: " << valeurMetrique << endl;


        //affichage(vAnnotObs);
        ostringstream ss;
        ss << "generationAleatoireV2/generationAleatoire" << occ << ".csv";
        ecrireFichier(nbNewAnnot, vAnnotObs2, ss.str());

        // !!!!!!!!!!!!!!!!!!!!!!! WIP !!!!!!!!!!!!!!!!!!!!!!!

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


    return 0;
}
