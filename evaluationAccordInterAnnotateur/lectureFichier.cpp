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
#include "lectureFichier.h"

using namespace std;

const int Max_obs=3000;//nb maximal d'observables
const int Max_classes=10;//nb maximal de classes
const int Max_annot=30;//nb maximal d'annotateurs

// III - LECTURE D'UN FICHIER STANDARDISE
//Nbobs : nb d'observables, Nba : nb d'annotateurs, Nbc : nb de classes
// Les classes sont numérotées 0, 1, ..., Nbc-1

void lire(string nomfich,vector<vector<int>> & vAnnotObs,int & Nbobs,int & Nba,int & Nbc) {
   const char * nom_fichier=nomfich.c_str();
    ifstream file(nom_fichier);
    if (!file){
      cout << "erreur ouverture fichier " << nomfich << endl;
    }
    else {
      string ligne;
      int lg;
      Nbobs=0;Nba=0;Nbc=0;
      while (((Nbobs==0)||(Nba==0)||(Nbc==0))&&(!file.eof())) {
        getline(file,ligne);
        if (ligne[0]!='%') {
          lg=ligne.size();
          int pos;
          pos=ligne.find("Nbannotateurs=");
          if ((Nba==0)&&(pos!=string::npos)) {
            pos+=14; //14 car c'est le nombre de caracteres dans "Nbannotateurs="
            while ((pos<lg)&&(ligne[pos]>=48)&&(ligne[pos]<=57)) { //entre 48 et 57 pour verifier que c'est un chiffre
              Nba=10*Nba+ligne[pos]-48; //on recupere le nombre d'annotateurs (10*Nba+ligne[pos] pour recuperer centaine/dizaine etc)
              pos++;
            }
            //cout << "Nba=" << Nba << endl;
          }
          else {//1
            pos=ligne.find("Nbclasses=");
            if ((Nbc==0)&&(pos!=string::npos)) {
              pos+=10;
              while ((pos<lg)&&(ligne[pos]>=48)&&(ligne[pos]<=57)) {
                Nbc=10*Nbc+ligne[pos]-48;
                pos++;
              }
              //cout << "Nbc=" << Nbc << endl;
            }
            else {//2
              pos=ligne.find("Nbobservables=");
              if ((Nbobs==0)&&(pos!=string::npos)) {
                pos+=14;
                while ((pos<lg)&&(ligne[pos]>=48)&&(ligne[pos]<=57)) {
                  Nbobs=10*Nbobs+ligne[pos]-48;
                  pos++;
                }
                //cout << "Nbobs=" << Nbobs << endl;
              }
            }//fin else 2
          }//fin else 1
        }//fin ligne qui n'est pas un commentaire
      }//fin while lecture paramètres

      vAnnotObs.resize(Nba);
      for(int i=0; i<Nba; i++){
        vAnnotObs[i].resize(Nbobs);
      }

      bool commentaire=true;
      while (commentaire) {
        getline(file,ligne);
        commentaire=(ligne[0]=='%') ;
      } //on n'est plus sur une ligne de commentaire
      // lecture du tableau proprement dit
      bool possedeDesValeursNegatives = false;
      for (int an=0;an<Nba;an++) {//une ligne par annotateur
        lg=ligne.size();
        int i=0; //indiceligne
        for (int ob=0;ob<Nbobs;ob++) {//Nbobs à lire
          if (i>=lg) cout << "erreur lecture ligne " << i << endl;
          else {
            if (ligne[i]=='.') {
                vAnnotObs[an][ob]=-1;i++;
              //T[an][ob]=-1;i++;
            }
            else {
              vAnnotObs[an][ob]=0;
              bool negatif = false;
              if (ligne[i]=='-') {
                if(!possedeDesValeursNegatives){
                    possedeDesValeursNegatives=true;
                    for(int a=0;a<=an;a++){
                        for(int o=0;o<ob;o++){
                            vAnnotObs[an][o] += Nbc/2;
                        }
                    }
                }
                negatif = true;
                i++;
              }
              while ((i<lg)&&(ligne[i]!='\t')&&(ligne[i]!='\n')) {
                vAnnotObs[an][ob]=10*vAnnotObs[an][ob]+(ligne[i]-48);
                i++;
              }
              if(negatif){
                vAnnotObs[an][ob] = -vAnnotObs[an][ob];
              }
              if(possedeDesValeursNegatives){
                vAnnotObs[an][ob] += Nbc/2;
              }
            }
            i++;
          }
        }
        getline(file,ligne);
      }
    }
}

void choixTableau(int choix,vector<vector<int>> & vAnnotObs,int & Nbobs,int & Nba,int & Nbc) {
  std::ostringstream ss;
  if(choix >= 1000){
    Nba=2;Nbobs=12;
    ss << (choix-1000);
    lire("classe3/generationAleatoireV2/generationAleatoire" + ss.str() + ".csv",vAnnotObs,Nbobs,Nba,Nbc);
  }
  else if(choix > 100 && choix < 107){
    Nba=2;Nbobs=12;
    ss << (choix-100);
    lire("Exemple/Tableau" + ss.str() + ".csv",vAnnotObs,Nbobs,Nba,Nbc);
  }
  else if (choix>300 && choix<304) {
    Nba=2;Nbobs=12;
    ss << (choix-301);
    lire("classe3/TableauEmotion" + ss.str() +".csv",vAnnotObs,Nbobs,Nba,Nbc);
  }
  else if (choix>303 && choix<307) {
    Nba=2;Nbobs=12;
    ss << (choix-304);
    lire("classe3/TableauEmotionContexte" + ss.str() +".csv",vAnnotObs,Nbobs,Nba,Nbc);
  }
  else if (choix>306 && choix<310) {
    Nba=2;Nbobs=12;
    ss << (choix-307);
    lire("classe3/TableauOpinion" + ss.str() +".csv",vAnnotObs,Nbobs,Nba,Nbc);
  }
  else if (choix>309 && choix<313) {
    Nba=2;Nbobs=12;
    ss << (choix-310);
    lire("classe3/TableauOpinionContexte" + ss.str() +".csv",vAnnotObs,Nbobs,Nba,Nbc);
  }
  else if (choix>500 && choix<504) {
    Nba=2;Nbobs=12;
    ss << (choix-501);
    lire("classe5/TableauEmotion" + ss.str() +".csv",vAnnotObs,Nbobs,Nba,Nbc);
  }
  else if (choix>503 && choix<507) {
    Nba=2;Nbobs=12;
    ss << (choix-504);
    lire("classe5/TableauEmotionContexte" + ss.str() +".csv",vAnnotObs,Nbobs,Nba,Nbc);
  }
  else if (choix>506 && choix<510) {
    Nba=2;Nbobs=12;
    ss << (choix-507);
    lire("classe5/TableauOpinion" + ss.str() +".csv",vAnnotObs,Nbobs,Nba,Nbc);
  }
  else if (choix>509 && choix<513) {
    Nba=2;Nbobs=12;
    ss << (choix-510);
    lire("classe5/TableauOpinionContexte" + ss.str() +".csv",vAnnotObs,Nbobs,Nba,Nbc);
  }else if (choix==513) {
    Nba=2;Nbobs=12;
    lire("classe5/TableauTypageRelation.csv",vAnnotObs,Nbobs,Nba,Nbc);
  }else if(choix==514) {
    Nba=2;Nbobs=12;
    lire("classe5/TableauOpinion250.csv",vAnnotObs,Nbobs,Nba,Nbc);
  }else if(choix==314) {
    Nba=2;Nbobs=12;
    lire("classe3/TableauOpinion250.csv",vAnnotObs,Nbobs,Nba,Nbc);
  }else if(choix==515) {
    Nba=2;Nbobs=12;
    lire("classe5/TableauOpinionContexte250.csv",vAnnotObs,Nbobs,Nba,Nbc);
  }else if(choix==315) {
    Nba=2;Nbobs=12;
    lire("classe3/TableauOpinionContexte250.csv",vAnnotObs,Nbobs,Nba,Nbc);
  }else if(choix==516) {
    Nba=2;Nbobs=12;
    lire("classe5/TableauEmotion250.csv",vAnnotObs,Nbobs,Nba,Nbc);
  }else if(choix==316) {
    Nba=2;Nbobs=12;
    lire("classe3/TableauEmotion250.csv",vAnnotObs,Nbobs,Nba,Nbc);
  }else if(choix==517) {
    Nba=2;Nbobs=12;
    lire("classe5/TableauEmotionContexte250.csv",vAnnotObs,Nbobs,Nba,Nbc);
  }else if(choix==317) {
    Nba=2;Nbobs=12;
    lire("classe3/TableauEmotionContexte250.csv",vAnnotObs,Nbobs,Nba,Nbc);
  }else if(choix==318) {
    Nba=2;Nbobs=12;
    lire("classe3/TableauSimilariteConqueteSpatiale.csv",vAnnotObs,Nbobs,Nba,Nbc);
  }else if(choix==319) {
    Nba=2;Nbobs=12;
    lire("classe3/TableauSimilariteEpidemies.csv",vAnnotObs,Nbobs,Nba,Nbc);
  }else if(choix==518) {
    Nba=2;Nbobs=12;
    lire("classe5/TableauSimilariteConqueteSpatiale.csv",vAnnotObs,Nbobs,Nba,Nbc);
  }else if(choix==519) {
    Nba=2;Nbobs=12;
    lire("classe5/TableauSimilariteEpidemies.csv",vAnnotObs,Nbobs,Nba,Nbc);
  }

}
