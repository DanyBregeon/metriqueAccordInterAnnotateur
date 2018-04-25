#include <iostream>
#include <cstring>
#include <cstdlib>
#include <ctime>
#include <cmath>
#include <fstream>
#include <cstdio>
#include <vector>
#include <sstream>
#include <iomanip>
#include <map>
#include <time.h>

using namespace std;

const int Max_obs=3000;//nb maximal d'observables
const int Max_classes=10;//nb maximal de classes
const int Max_annot=30;//nb maximal d'annotateurs


// I - CALCULS KRIPPENDORFF : TABLEAU DES COINCIDENCES C


//calcul de C[c1][c2] (C : tableau des coincidences)
float calcul(int T[Max_obs][Max_annot],int nbobs,int nbannot,int c1,int c2) {
  float temp,res=0;
  int nbannotvrai;
  for (int e=0;e<nbobs;e++) {//pour chaque observable
    for (int k1=0;k1<nbannot;k1++) {//pour chaque annotateur
      temp=0;
      if (T[e][k1]!=-1) { //si l'annotation existe
        nbannotvrai=nbannot;
        for (int k2=0;k2<nbannot;k2++) {
          if (k1!=k2) {
            if ((T[e][k1]==c1)&& (T[e][k2]==c2)) temp++;
            if (T[e][k2]==-1) nbannotvrai--;
          }
        }
        if (nbannotvrai>1) res=res+(temp*1.0)/(nbannotvrai-1);
      }
     }
  }
  return res;
}


//nb est le nombre d'observations effectives (nbannot X nblignes si pas de données manquantes)
void coincidences(int T[Max_obs][Max_annot],int nblignes,int nbannot,
		  float C[Max_classes][Max_classes],int nbclasses,float & nb) {
  int i,j;
  nb=0;
  for (i=0;i<nbclasses;i++)
    for (j=0;j<nbclasses;j++) {
    /*  if (i==j) C[i][j]=calcul1(T,nblignes,nbannot,i,nbclasses);
      else C[i][j]=calcul2(T,nblignes,nbannot,i,j,nbclasses);
      nb+=C[i][j];
    }*/
    C[i][j]=calcul(T,nblignes,nbannot,i,j);
    nb+=C[i][j];
  }
  //cout << "ici, nb="<< nb << endl;
}

void affiche_coincidences(float C[Max_classes][Max_classes],int nbclasses) {
  int i,j;
  float S=0;
  for (i=0;i<nbclasses;i++) {
    for (j=0;j<nbclasses;j++) {
      printf("%7.2f  ",C[i][j]);
      S=S+C[i][j];
    }
    cout << endl;
  }
  cout << "S=" << S << endl;
}


//  II - CALCUL DU ALPHA A PARTIR DU TABLEAU DES COINCIDENCES

//II - a : définition de la distance entre classes

//DISTANCE GROSSIERE
float delta_car(int i,int j,int nbclasses) {
  if (i!=j) return 1.0; else return 0.0;
}
//DISTANCE pour associative pronominale
/*   float delta_car(int i,int j,int nbclasses) {
  if (i==j)
    return 0;
  else {
    if ((i==4 && (j==2||j==3))||(j==4 && (i==2||i==3)))
      return 0.5;
    else return 1;
  }
  } */

  //DISTANCE EUCLIDIENNE :
float delta_carEuclidienne(int i,int j, int classes)  {
    float res;
    if (i>j) res=i-j; else res=j-i;//ou bien : res=sqrt((i-j)*(i-j))
    return res;
}
//bien d'autres distances sont possibles à définir suivant le pb traité


//CALCULS Kappa
//Artmann Poesio

//nb de fois où l'observable i est classé dans la catégorie k
int xikres(int i,int k,int Nbareel,int T[Max_obs][Max_annot]){
  int j,res=0;
  for (j=0;j<Nbareel;j++)
    if (T[i][j]==k) res++;
  //if (res!=0) printf("%d, ",res);
  return res;
}
//pkc=probas pour l'annotateur c de choisir la modalité k
double pkc(int c,int k,int nblignes,int T[Max_obs][Max_annot]){
  int i,nck=0;
  double res;
  for (i=0;i<nblignes;i++)
    if (T[i][c]==k) nck++;
  res=(1.0*nck)/nblignes;
  //printf("pkc(%d,%d)=%f\n",c,k,res);
  return res;
}

//II -b : désaccord observé et désaccord attendu

float Aepi(int nblignes,int nbclasses,int Nbareel, int T[Max_obs][Max_annot]){
  int i,j,Nbtemp[nbclasses];//pour stocker le nombre de votes pour chaque classe
  float res;
  for (i=0;i<nbclasses;i++)
    Nbtemp[i]=0;
  for (i=0;i<nblignes;i++)
    for (j=0;j<Nbareel;j++) {
      Nbtemp[T[i][j]]++;
    }
  res=0;
  for (i=0;i<nbclasses;i++)
    res+=Nbtemp[i]*Nbtemp[i];
  res=res/(nblignes*nblignes*Nbareel*Nbareel);
  //printf("Aepi=%f\n",res);
  return res;
}

double Aekappa(int nblignes,int nbclasses,int Nbareel, int T[Max_obs][Max_annot]){
  int comb2Nba=(Nbareel*(Nbareel-1))/2;
  //printf("comb2Nba=%d\n",comb2Nba);
  double res=0,den=0;
  for (int k=0;k<nbclasses;k++) {
    for (int a1=0;a1<Nbareel-1;a1++)
      for (int a2=a1+1;a2<Nbareel;a2++)
	res=res+pkc(a1,k,nblignes,T)*pkc(a2,k,nblignes,T);
  }
  return (res*1.0)/comb2Nba;
}

//on a nbclasses, exprimées dans le tableau T par un entier de 0 à nbclasses-1
float Aokappa(int nblignes,int nbclasses,int Nbareel, int T[Max_obs][Max_annot]){
  float res=0;
  int sum=0,i,nik;
  for (i=0;i<nblignes;i++) {
    for (int k=0;k<nbclasses;k++) {
      nik=xikres(i,k,Nbareel,T);
      sum=sum+nik*(nik-1);
    }
  }
  res=(sum*1.0)/(nblignes*Nbareel*(Nbareel-1));
  return res;
}

// Désaccord observé
float Do(float nb, float C[Max_classes][Max_classes],int nbclasses, int pondere){
  float res=0;
  int i,j;
  for (i=0;i<nbclasses;i++)
    for (j=0;j<nbclasses;j++)
        if (i!=j){
            if(pondere == 0){
                res=res+delta_car(i,j,nbclasses)*C[i][j];
            }else{
                res=res+delta_carEuclidienne(i,j,nbclasses)*C[i][j];
            }
        }
  res=res/nb;
  //cout << "Do=" << res << endl;
  return res;
}

float n_calc(float C[Max_classes][Max_classes],int l,int nbclasses) {
  float res=0;
  int i;
  for (i=0;i<nbclasses;i++) res=res+C[i][l];
  return res;
}

//Désaccord attendu (expected)
float De(float nb,float C[Max_classes][Max_classes],int nbclasses, int pondere) {
  float res=0;
  int i,j;
   for (i=0;i<nbclasses;i++)
    for (j=0;j<nbclasses;j++)
      if (i!=j){
          if(pondere == 0){
                res=res+delta_car(i,j,nbclasses)*n_calc(C,i,nbclasses)*n_calc(C,j,nbclasses);
            }else{
                res=res+delta_carEuclidienne(i,j,nbclasses)*n_calc(C,i,nbclasses)*n_calc(C,j,nbclasses);
            }
      }
   res=res/(nb*(nb-1));
   //cout << "De=" << res << endl;
   return res;
}

float piAP(int nblignes,int nbclasses,int Nbareel, int T[Max_obs][Max_annot]){
   float Ao,Ae;
   Ao=Aokappa(nblignes,nbclasses,Nbareel,T);
   //printf("Ao=%f\n",Ao);
   Ae=Aepi(nblignes,nbclasses,Nbareel,T);
   //printf("Aepi=%f\n",Ae);
   return (Ao-Ae)/(1.0-Ae);
}

float kappaAP(int nblignes,int nbclasses,int Nbareel, int T[Max_obs][Max_annot]){
   float Ao,Ae;
   Ao=Aokappa(nblignes,nbclasses,Nbareel,T);
   //printf("Ao=%f\n",Ao);
   Ae=Aekappa(nblignes,nbclasses,Nbareel,T);
   //printf("Aekappa=%f\n",Ae);
   return (Ao-Ae)/(1.0-Ae);
}

//nb = nb d'observations effectives = nb_experts*nb_observables si pas de données manquantes
float alpha(float nb,float C[Max_classes][Max_classes],int nbclasses, int pondere) {
  float res=1-(Do(nb,C,nbclasses, pondere)/De(nb,C,nbclasses, pondere));
  if (res<0) return 0; else return res;
}


/* float moyenne(float T[10],int nbfich) {
  float res=0;
  int i;
  for (i=0;i<nbfich;i++)
    res=res+T[i];
  return res/nbfich;
} */

/* float ecart_type(float moy,float T[],int nbfich) {
  float res=0;
  int i;
  for (i=0;i<nbfich;i++)
    res=res+((T[i]-moy)*(T[i]-moy));
  return sqrt(res/nbfich);
  }*/


// III - LECTURE D'UN FICHIER STANDARDISE
//Nbobs : nb d'observables, Nba : nb d'annotateurs, Nbc : nb de classes
// Les classes sont numérotées 0, 1, ..., Nbc-1

void lire(string nomfich,int T[Max_annot][Max_obs],int & Nbobs,int & Nba,int & Nbc) {
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
              T[an][ob]=-1;i++;
            }
            else {
              T[an][ob]=0;
              bool negatif = false;
              if (ligne[i]=='-') {
                if(!possedeDesValeursNegatives) possedeDesValeursNegatives=true;
                negatif = true;
                i++;
              }
              while ((i<lg)&&(ligne[i]!='\t')&&(ligne[i]!='\n')) {
                T[an][ob]=10*T[an][ob]+(ligne[i]-48);
                i++;
              }
              if(negatif){
                T[an][ob] = -T[an][ob];
              }
              if(possedeDesValeursNegatives){
                T[an][ob] += Nbc/2;
              }
            }
            i++;
          }
        }
        getline(file,ligne);
      }
    }
}

void choixTableau(int choix,int T[Max_annot][Max_obs],int & Nbobs,int & Nba,int & Nbc) {
  std::ostringstream ss;
  if(choix > 100 && choix < 107){
    Nba=2;Nbobs=12;
    ss << (choix-100);
    lire("Exemple/Tableau" + ss.str() + ".csv",T,Nbobs,Nba,Nbc);
  }
  else if (choix>300 && choix<304) {
    Nba=2;Nbobs=12;
    ss << (choix-301);
    lire("classe3/TableauEmotion" + ss.str() +".csv",T,Nbobs,Nba,Nbc);
  }
  else if (choix>303 && choix<307) {
    Nba=2;Nbobs=12;
    ss << (choix-304);
    lire("classe3/TableauEmotionContexte" + ss.str() +".csv",T,Nbobs,Nba,Nbc);
  }
  else if (choix>306 && choix<310) {
    Nba=2;Nbobs=12;
    ss << (choix-307);
    lire("classe3/TableauOpinion" + ss.str() +".csv",T,Nbobs,Nba,Nbc);
  }
  else if (choix>309 && choix<313) {
    Nba=2;Nbobs=12;
    ss << (choix-310);
    lire("classe3/TableauOpinionContexte" + ss.str() +".csv",T,Nbobs,Nba,Nbc);
  }
  else if (choix>500 && choix<504) {
    Nba=2;Nbobs=12;
    ss << (choix-501);
    lire("classe5/TableauEmotion" + ss.str() +".csv",T,Nbobs,Nba,Nbc);
  }
  else if (choix>503 && choix<507) {
    Nba=2;Nbobs=12;
    ss << (choix-504);
    lire("classe5/TableauEmotionContexte" + ss.str() +".csv",T,Nbobs,Nba,Nbc);
  }
  else if (choix>506 && choix<510) {
    Nba=2;Nbobs=12;
    ss << (choix-507);
    lire("classe5/TableauOpinion" + ss.str() +".csv",T,Nbobs,Nba,Nbc);
  }
  else if (choix>509 && choix<513) {
    Nba=2;Nbobs=12;
    ss << (choix-510);
    lire("classe5/TableauOpinionContexte" + ss.str() +".csv",T,Nbobs,Nba,Nbc);
  }else if (choix==513) {
    Nba=2;Nbobs=12;
    lire("classe5/TableauTypageRelation.csv",T,Nbobs,Nba,Nbc);
  }else if(choix==514) {
    Nba=2;Nbobs=12;
    lire("classe5/TableauOpinion250.csv",T,Nbobs,Nba,Nbc);
  }else if(choix==314) {
    Nba=2;Nbobs=12;
    lire("classe3/TableauOpinion250.csv",T,Nbobs,Nba,Nbc);
  }else if(choix==515) {
    Nba=2;Nbobs=12;
    lire("classe5/TableauOpinionContexte250.csv",T,Nbobs,Nba,Nbc);
  }else if(choix==315) {
    Nba=2;Nbobs=12;
    lire("classe3/TableauOpinionContexte250.csv",T,Nbobs,Nba,Nbc);
  }else if(choix==516) {
    Nba=2;Nbobs=12;
    lire("classe5/TableauEmotion250.csv",T,Nbobs,Nba,Nbc);
  }else if(choix==316) {
    Nba=2;Nbobs=12;
    lire("classe3/TableauEmotion250.csv",T,Nbobs,Nba,Nbc);
  }else if(choix==517) {
    Nba=2;Nbobs=12;
    lire("classe5/TableauEmotionContexte250.csv",T,Nbobs,Nba,Nbc);
  }else if(choix==317) {
    Nba=2;Nbobs=12;
    lire("classe3/TableauEmotionContexte250.csv",T,Nbobs,Nba,Nbc);
  }

}

void afficheTableauLu(int T[Max_annot][Max_obs],int Nbobs,int Nba) {
  for (int i=0;i<Nba;i++) {
    for (int j=0;j<Nbobs;j++)
      printf("%3d",T[i][j]);
    cout << endl;
  }
}

void transposeT(int T[Max_annot][Max_obs],int TT[Max_obs][Max_annot],int nbl,int nbc) {
  int temp,i,j;
  for (i=0;i<nbl;i++)
    for (j=0;j<nbc;j++)
      TT[j][i]=T[i][j];
}

// IV - génère toutes les annotations par vote majoritaires pour toutes les combinaisons de (n-1),
//      (n-2), (n-3) ... annotateurs, et donne les % de modifications par rapport à la référence

//calcul du vote majoritaire et le compare avec celui de référence

//nblignes = nbobs, n = 9 ou 8 ou 7 etc annotateurs, effectue des calculs pour chaque combinaison
void voteMajoritaire( int *p, int n, int T[Max_obs][Max_annot],int nblignes,int nba, int nbc,
		  int erreurVoteMaj[Max_annot], int voteMajReference[Max_obs], int egalite) {
    /*int i;
    for (i = 0; i < n; i++){
        cout << p[i];
    }
    cout << endl;
    //cout << " : ";*/

    //tableau contenant les annotateurs choisi par la combinaison
    //int TT[Max_obs][Max_annot];

    //le vote majoritaire pour chaque observable
    int voteMajoritaire[nblignes];
    //le nombre d'occurence des classes dans les annotations d'un observable
    int tNbOcc[nbc];
    //la moyenne des classes pour chaque observable
    //float tMoyenne[nblignes];

    for(int obs=0; obs<nblignes; obs++){
        float moyenne = 0;
        for(int ann=0; ann<n; ann++){
            //p[ann] nous donne un annotateur de la combinaison actuelle, T[obs][p[ann]] nous donne la classe qu'à attribué cet annotateur à l'observable obs
            tNbOcc[T[obs][p[ann]]]++;
            moyenne += T[obs][p[ann]];
        }
        moyenne = moyenne / (float) n;
        int max=tNbOcc[0];
        int voteMaj = 0;
        tNbOcc[0] = 0;
        for(int i=1; i<nbc; i++){
            if(tNbOcc[i]>max){
                max = tNbOcc[i];
                voteMaj = i;
            }else if(tNbOcc[i]==max){
                if(egalite==1){
                    int rng = rand() % 2;
                    if(rng == 0){
                        max = tNbOcc[i];
                        voteMaj = i;
                    }
                }else{
                    //en cas d'égalité pour le vote majoritaire, c'est la classe la plus proche de la moyenne des classes qui l'emporte
                    if(abs((float)i-moyenne) < abs((float)voteMaj-moyenne)){
                        max = tNbOcc[i];
                        voteMaj = i;
                    }
                }
                /*//en cas d'égalité pour le vote majoritaire, c'est la classe la plus proche de la moyenne de référence des classes qui l'emporte
                if(abs((float)i-moyenneReference[obs]) < abs((float)voteMaj-moyenneReference[obs])){
                    max = tNbOcc[i];
                    voteMaj = i;
                }*/

            }
            tNbOcc[i]=0;
        }
        voteMajoritaire[obs]=voteMaj;
        //tMoyenne[obs]=moyenne;

        /*for(int i=0; i<nbc; i++){
            tNbOcc[i]=0;
        }*/
    }

    //si c'est la référence
    if(n == nba){
        erreurVoteMaj[n]=0;
        //cout << "voteMajRef : ";
        for(int i=0; i<nblignes; i++){
            voteMajReference[i] = voteMajoritaire[i];
            //cout << voteMajReference[i] <<" ";
            //moyenneReference[i] = tMoyenne[i];
        }
    //sinon on compare avec la référence
    }else{
        int test = erreurVoteMaj[n];
        for(int i=0; i<nblignes; i++){
            if(voteMajoritaire[i] != voteMajReference[i]){
                erreurVoteMaj[n]++;
                //"% de modifications par rapport à la référence" par nombre d'annotateur ou pour chaque vote majoritaire ?
                //break;
            }
        }
        //cout << "nbErreur " << n << ": " << erreurVoteMaj[n]-test << endl;
    }

    /*if(choixMetrique.compare("a")==0 || choixMetrique.compare("ap")==0){
        coincidences(TT,nblignes,n,C,nbc,nb);
    }*/


    //affiche_coincidences(C,nbc);
    //float a = alpha(nb,C,nbc);
    //cout << "alpha=" << a << "  ";
    //float k = kappaAP(nblignes, nbc, n, TT);
    //cout << "kappa=" << k << "  " << endl;

    //voteMajoritaire(TT, nblignes, nbc, n, erreurVoteMaj, voteMajReference, nba, /*moyenneReference,*/ egalite);

    //vAlpha->push_back(a);
}

//vote maj pour le gold en fonction du nombre d'annotateur de la combinaison
void voteMajoritaire2( int *p, int n, int T[Max_obs][Max_annot],int nblignes,int nba, int nbc,
		  int erreurVoteMaj[Max_annot], int voteMajReference[Max_obs], int egalite, vector<vector<vector<int> > > &voteMajOcc) {

    //le vote majoritaire pour chaque observable
    //int voteMajoritaire[nblignes];
    //le nombre d'occurence des classes dans les annotations d'un observable
    int tNbOcc[nbc];
    //la moyenne des classes pour chaque observable
    //float tMoyenne[nblignes];

    for(int obs=0; obs<nblignes; obs++){
        float moyenne = 0;
        for(int ann=0; ann<n; ann++){
            //p[ann] nous donne un annotateur de la combinaison actuelle, T[obs][p[ann]] nous donne la classe qu'à attribué cet annotateur à l'observable obs
            tNbOcc[T[obs][p[ann]]]++;
            moyenne += T[obs][p[ann]];
        }
        moyenne = moyenne / (float) n;
        int max=tNbOcc[0];
        int voteMaj = 0;
        tNbOcc[0] = 0;
        for(int i=1; i<nbc; i++){
            if(tNbOcc[i]>max){
                max = tNbOcc[i];
                voteMaj = i;
            }else if(tNbOcc[i]==max){
                if(egalite==1){
                    int rng = rand() % 2;
                    if(rng == 0){
                        max = tNbOcc[i];
                        voteMaj = i;
                    }
                }else{
                    //en cas d'égalité pour le vote majoritaire, c'est la classe la plus proche de la moyenne des classes qui l'emporte
                    if(abs((float)i-moyenne) < abs((float)voteMaj-moyenne)){
                        max = tNbOcc[i];
                        voteMaj = i;
                    }
                }
            }
            tNbOcc[i]=0;
        }
        //voteMajoritaire[obs]=voteMaj;
        voteMajOcc[n][obs][voteMaj]++;
    }
}


void calculMoyenne( int *p, int n, int T[Max_obs][Max_annot],int nblignes,int nba, int nbc,
		  vector<vector<float>> & moyennes, int egalite)
{
    float moyenne = 0;
    for(int obs=0; obs<nblignes; obs++){
        for(int ann=0; ann<n; ann++){
            //p[ann] nous donne un annotateur de la combinaison actuelle, T[obs][p[ann]] nous donne la classe qu'à attribué cet annotateur à l'observable obs
            moyenne += T[obs][p[ann]];
        }
    }
    moyennes[n].push_back(moyenne/(float) (n*nblignes)) ;


}
//cas comparaison vote maj sans annotateurs communs
void combinaisons2(vector<int> & ens, int *combinaison, int n, int p, int i, int t,
                int T[Max_obs][Max_annot],int nblignes,int nba, int nbc,
                int erreurVoteMaj[Max_annot], int voteMajReference[Max_obs], int egalite, vector<vector<vector<int> > > & voteMajOcc){
    //if(nbCombinaison[p] < 500000){
        if (i<p) {
            for (int k=t; k<n; k++) {
                combinaison[i] = ens[k];
                combinaisons2(ens,combinaison,n,p,i+1,k+1, T,nblignes,nba,nbc, erreurVoteMaj, voteMajReference, egalite, voteMajOcc);
            }
        }
        else {
            voteMajoritaire2(combinaison,p, T,nblignes,nba,nbc, erreurVoteMaj, voteMajReference, egalite, voteMajOcc);
        }
    //}
}


void calculCombiSansAnnotCommun(int *ens, int *combinaison, int p, int n,
                int T[Max_obs][Max_annot],int nblignes,int nba, int nbc, int erreurVoteMaj[Max_annot], int voteMajReference[Max_obs],
                int egalite, vector<vector<vector<int> > > &voteMajOcc){
    //on crée un ensemble qui contient tous les annotateurs sauf ceux présent dans cette combinaison
    vector<int> ens2;
    bool b;
    for (int i = 0; i < n; i++){
        b = false;
        for (int j = 0; j < p; j++){
            if(ens[i]==combinaison[j]){
                b = true;
                break;
            }
        }
        if(!b){
            ens2.push_back(ens[i]);
        }
    }
    //calcul du vote maj de de cette combinaison
    int tNbOcc[nbc];
    for(int obs=0; obs<nblignes; obs++){
        float moyenne = 0;
        for(int ann=0; ann<p; ann++){
            tNbOcc[T[obs][ens[ann]]]++;
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
        voteMajReference[obs]=voteMaj;
    }

    //on calcul toutes les combinaisons avec le même nombre d'annotateurs et qui ne contiennent pas les même annotateurs
    int combi[p];
    combinaisons2(ens2, combi, n-p, p, 0, 0, T,nblignes,nba,nbc, erreurVoteMaj, voteMajReference, egalite, voteMajOcc);
    //on fait le vote maj des votes maj de toutes les combi sans annotateurs communs
    for(int obs=0; obs<nblignes; obs++){
        int max = voteMajOcc[p][obs][0];
        int voteMaj = 0;
        for(int c=1; c<nbc; c++){
            if(voteMajOcc[p][obs][c]>max){
                max = voteMajOcc[p][obs][c];
                voteMaj = c;
            }else if(voteMajOcc[p][obs][c]==max){
                int rng = rand() % 2;
                if(rng == 0){
                    max = voteMajOcc[p][obs][c];
                    voteMaj = c;
                }
            }
            voteMajOcc[p][obs][c]=0;
        }
        //on compare le vote maj des votes maj de toutes les combi sans annotateurs communs avec le vote maj de cette combinaison
        if(voteMajReference[obs] != voteMaj){
            erreurVoteMaj[p]++;
        }
    }
    //cout << "p: " << p << " " << erreurVoteMaj[p]<< endl;
}


//trouve l'ensemble des combinaisons de p parmi n (de manière récursive)
void combinaisons(int *ens, int *combinaison, int n, int p, int i, int t,
                  int T[Max_obs][Max_annot],int nblignes,int nba,int nbc, /*std::vector<float> *vAlpha,*/
		  int erreurVoteMaj[Max_annot], int voteMajReference[Max_obs], int nbCombinaison[Max_annot], /*float moyenneReference[Max_obs],*/ int egalite,
		  int choixGold, vector<vector<vector<int> > > &voteMajOcc, vector<vector<float>> & moyennes) {
    if (i<p) {
        for (int k=t; k<n; k++) {
            combinaison[i] = ens[k];
            combinaisons(ens,combinaison,n,p,i+1,k+1, T,nblignes,nba,nbc, /*&vAlpha,*/ erreurVoteMaj, voteMajReference, nbCombinaison, /*moyenneReference,*/ egalite, choixGold, voteMajOcc, moyennes);
        }
    }
    else {
        if(choixGold==0){
            voteMajoritaire(combinaison,p, T,nblignes,nba,nbc, /*&vAlpha,*/ erreurVoteMaj, voteMajReference, /*moyenneReference,*/ egalite);
        }else if(choixGold==1){
            voteMajoritaire2(combinaison,p, T,nblignes,nba,nbc, /*&vAlpha,*/ erreurVoteMaj, voteMajReference, /*moyenneReference,*/ egalite, voteMajOcc);
        }else if(choixGold==2){
            if(nbCombinaison[p] < 100){
                calculCombiSansAnnotCommun(ens, combinaison,p, n, T,nblignes,nba,nbc, erreurVoteMaj, voteMajReference, egalite, voteMajOcc);
            }else{
                nbCombinaison[p]=99;
            }
        }else if(choixGold==3 || choixGold ==4){
            calculMoyenne(combinaison, p, T,nblignes,nba,nbc, moyennes, egalite);
        }
        nbCombinaison[p]++;
    }
}


void calculDifference(int T[Max_obs][Max_annot],int nblignes,int nba,int nbc, std::vector<float> *vPourcentageErreur, int egalite, int choixGold){
    int ens[nba];
    for(int i=0; i<nba; i++){
        ens[i] = i;
    }
    //le nombre de votes majoritaires différent de la référence par nombre d'annotateurs
    int erreurVoteMaj[Max_annot];
    //le nombre de combinaisons par nombre d'annotateurs
    int nbCombinaison[Max_annot];
    //les votes majoritaires de la référence pour chaque observable
    int voteMajReference[Max_obs];
    //les moyennes de votes de la référence pour chaque observable
    //float moyenneReference[Max_obs];
    //les moyennes pour chaque combinaisons de p annotateurs
    vector<vector<float>> moyennes;
    moyennes.resize(Max_annot);

    for(int i=0; i<nba+1; i++){
        erreurVoteMaj[i]=0;
        nbCombinaison[i]=0;
    }

    vector<vector<vector<int> > > voteMajOcc;

    // Set up sizes. (HEIGHT x WIDTH)
    voteMajOcc.resize(Max_annot);
    for (int i = 0; i < Max_annot; ++i) {
        voteMajOcc[i].resize(Max_obs);

        for (int j = 0; j < Max_obs; ++j)
            voteMajOcc[i][j].resize(Max_classes);
    }

    if(choixGold==2){
        for(int p=nba/2; p>1; p--){
            int combi[p];
            combinaisons(ens,combi,nba,p,0,0, T,nblignes,nba,nbc, erreurVoteMaj, voteMajReference, nbCombinaison, egalite, choixGold, voteMajOcc, moyennes);
        }
    }else{
        for(int p=nba; p>1; p--){
            //cout << p << " annotateurs / " << nba << endl;
            //cout << endl;
            int combi[p];
            //std::vector<float> vAlpha;
            combinaisons(ens,combi,nba,p,0,0, T,nblignes,nba,nbc, /*&vAlpha,*/ erreurVoteMaj, voteMajReference, nbCombinaison, /*moyenneReference,*/ egalite, choixGold, voteMajOcc, moyennes);
            //float moy = moyenne(&vAlpha);
            //cout << "moyenne : " << moy << endl;
            //cout << "ecart-type : " << ecart_type(&vAlpha, moy) << endl;
            //cout << endl;
            //int combi[nba-1];
        }
    }

    if(choixGold==1){
        for(int i=nba; i>1; i--){
            int nbErreur = 0;
            for(int obs=0; obs<nblignes; obs++){
                int max = voteMajOcc[i][obs][0];
                int voteMaj = 0;
                for(int c=1; c<nbc; c++){
                    if(voteMajOcc[i][obs][c]>max){
                        max = voteMajOcc[i][obs][c];
                        voteMaj = c;
                    }else if(voteMajOcc[i][obs][c]==max){
                        int rng = rand() % 2;
                        if(rng == 0){
                            max = voteMajOcc[i][obs][c];
                            voteMaj = c;
                        }
                    }
                }
                for(int c=0; c<nbc; c++){
                    if(c != voteMaj){
                        nbErreur += voteMajOcc[i][obs][c];
                    }
                }
            }
            //cout << nbErreur << "/(" << nbCombinaison[i] << "*" << nblignes << ")    ";
            float pourcentage = ((float) nbErreur/(float) (nbCombinaison[i]*nblignes))*100;
            //cout << i << ": " << fixed << setprecision (3) << pourcentage << "%   ";
            vPourcentageErreur->push_back(pourcentage);
        }

    }else if(choixGold==2){
        for(int i=nba; i>1; i--){
            if(i>nba/2){
                vPourcentageErreur->push_back(0);
            }else{
                cout << i << ": " << erreurVoteMaj[i] << "/(" << nbCombinaison[i] << "*" << nblignes << ")    ";
                float pourcentage = ((float) erreurVoteMaj[i]/(float) (nbCombinaison[i]*nblignes))*100;
                vPourcentageErreur->push_back(pourcentage);
            }
        }
    }else if(choixGold==3){
        for(int i=nba; i>1; i--){
            float minMoyenne = moyennes[i][0];
            float minIndex = 0;
            float maxMoyenne = moyennes[i][0];
            float maxIndex = 0;
            for(int j=1; j<moyennes[i].size(); j++){
                if(moyennes[i][j] < minMoyenne){
                    minMoyenne = moyennes[i][j];
                    minIndex = j;
                }else if(moyennes[i][j] > maxMoyenne){
                    maxMoyenne = moyennes[i][j];
                    maxIndex = j;
                }
            }
            cout << i << ": min=" << minMoyenne << " max=" << maxMoyenne << "    ";
            float amplitude = maxMoyenne-minMoyenne;
            vPourcentageErreur->push_back(amplitude);
        }
    }else if(choixGold==4){
        float moyenneRef = moyennes[nba][0];
        vPourcentageErreur->push_back(0);
        for(int i=nba-1; i>1; i--){
            float moyenne = 0;
            for(int j=1; j<moyennes[i].size(); j++){
                moyenne += moyennes[i][j];
            }
            moyenne = moyenne / (float) moyennes[i].size();
            cout << i << ": " << moyenne << "    ";
            float amplitude = abs(moyenneRef-moyenne);
            vPourcentageErreur->push_back(amplitude);
        }
    }else{
        //cout << "poucentage d'erreur :  ";
        for(int i=nba; i>1; i--){
            cout << i << ": " << erreurVoteMaj[i] << "/(" << nbCombinaison[i] << "*" << nblignes << ")    ";
            //cout << i << ": " << ((float) erreurVoteMaj[i]/(float) nbCombinaison[i])*100 << "%   ";
            float pourcentage = ((float) erreurVoteMaj[i]/(float) (nbCombinaison[i]*nblignes))*100;
            vPourcentageErreur->push_back(pourcentage);
            //cout << i << ": " << fixed << setprecision (3) << pourcentage << "%   ";
            //TPourcentageErreur[i] = pourcentage;
        }
        cout << endl;
        //return vAlpha.at(0);
    }
}

// V - CREATION FICHIER DE SORTIE

// retourne la date actuelle, format: YYYY-MM-DD.HH:mm:ss
const std::string currentDateTime() {
    time_t now = time(0);
    struct tm tstruct;
    char buf[80];
    tstruct = *localtime(&now);
    strftime(buf, sizeof(buf), "%Y-%m-%d.%X", &tstruct);

    return buf;
}

int fichierSortie (int choixCorpus, int choixNbClasse, string choixMetrique, string egalite, int choixGold,
                   int nba, std::map<std::pair<int, float>, std::vector<float>> mapResultat)
{
    //lit le fichier numeroExperience.txt et incrémente son contenu pour savoir le numéro du test
    int numeroEperience = 0;
    int tailleLigne = 0;
    ifstream file("resultats/numeroExperience.txt");
    if (file){
        string ligne;
        getline(file,ligne);
        tailleLigne = ligne.size();
        for(int i=0; i<tailleLigne; i++){
            numeroEperience = 10*numeroEperience + ((int)ligne[i])-48;
        }
    }
    file.close();
    ofstream fileNumIncrement;
    fileNumIncrement.open ("resultats/numeroExperience.txt");
    fileNumIncrement << (numeroEperience+1);
    fileNumIncrement.close();

    //création fichier de sortie
    std::ostringstream ss;
    switch (choixCorpus)
    {
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

    if(egalite.compare("a")==0){
        ss<<"A";
    }else if(egalite.compare("m")==0){
        ss<<"M";
    }

    if(choixGold==1){
        ss<<"goldParAnnot";
    }else if(choixGold==2){
        ss<<"goldParAnnotSansAnnotCommun";
    }else if(choixGold==3){
        ss<<"moyenneParAnnot";
    }else if(choixGold==4){
        ss<<"moyenne";
    }

    ss << nba;
    //ss << numeroEperience;
    ofstream myfile;
    myfile.open ("resultats/"+ss.str()+".txt");

    myfile << currentDateTime() << endl << endl;

    for (std::map<std::pair<int, float>, std::vector<float>>::iterator it=mapResultat.begin(); it!=mapResultat.end(); ++it){
        std::pair<int, float> m = it->first;
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
            std::vector<float> valeurs = it->second;
             //myfile << valeurs[i] <<"\t";
             if(choixGold==3 || choixGold==4){
                myfile << nba-i << ":  " << valeurs[i] <<"   ";
             }else{
                if(valeurs[i]<10){
                    myfile << nba-i << ":  " << valeurs[i] <<"%   ";
                }else{
                    myfile << nba-i << ": " << valeurs[i] <<"%   ";
                }
             }
        }
        myfile << endl;
        //myfile << endl;
    }
    myfile.close();
    return 0;
}


int main() {
    // a priori 9 experts, le nb d'enonces se déduit du fichier
    int T[Max_annot][Max_obs],T1[Max_obs][Max_annot];
    float C[Max_classes][Max_classes];
    int choix;
    int choixNbClasse;
    //cout << "Choix ? (0 pour auto, 1-6 pour les exemples, 301-312 pour emotions/opinions avec 3 classes, 501-512 avec 5 classes, 513 pour coreference)";
    cout << "Choix ? (1 pour tous les corpus, 2 pour emotion, 3 pour opinion, 4 pour coreference)";
    cin >> choix ;
    //si >= 100, on test sur un seul fichier: on a plus de détail mais pas de fichier de sortie
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
    //choix = 516;
    //choixNbClasse = 5;

    string egalite;
    cout << "Choix ? (en cas d'egalite au vote majoritaire on departage par: m pour le plus proche de la moyenne des classes, a pour aleatoire)";
    cin >> egalite;
    //egalite = "a";

    int egaliteInt;
    if(egalite.compare("m")==0){
        egaliteInt = 0;
    }else{
        egaliteInt = 1;
    }

    int nba,nbobs,nbc;//nb d'annotateurs, d'observables, de classes
    float nb;//observation réellement prises en compte

    string choixMetrique;
    cout << "Choix ? (a pour alpha, k pour kappa, pi pour pi, ap pour alpha pondere)";
    cin >> choixMetrique;
    //choixMetrique = "k";

    int choixGold;
    cout << "Choix ? (gold: 0 pour verite ideale, 1 pour en fonction du nombre d'annotateurs retenus, 2 pareil mais sans annotateur commun, 3 moyenne par rapport au nombre P d'annotateurs, 4 moyenne par rapport a la verite)";
    cin >> choixGold;
    //choixGold  = 0;

    //tableau final avec les pourcentages de différences de votes majoritaires par rapport à la référence (9 annotateurs)
    //en fonction du nombre de classes et de la valeur de la métrique
    std::map<std::pair<int, float>, std::vector<float>> mapResultat;

    if(choix<100){
        std::vector<int> lesChoix;
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
            choixTableau(lesChoix[i],T,nbobs,nba,nbc);
            //afficheTableauLu(T,nbobs,nba);
            transposeT(T,T1,nba,nbobs);
            //cout << "Tableau des coincidences :\n";
            coincidences(T1,nbobs,nba,C,nbc,nb);
            //affiche_coincidences(C,nbc);
            //cout << "nb d'observations prises en compte="<<nb <<endl;
            std::vector<float> vPourcentageErreur;
            //cout << "alpha=" << alpha(nb,C,nbc);
            float metrique;
            if(choixMetrique.compare("k")==0){
                metrique = kappaAP(nbobs, nbc, nba, T1);
            }else if(choixMetrique.compare("ap")==0){
                metrique = alpha(nb,C,nbc, 1);
            }else if(choixMetrique.compare("pi")==0){
                metrique = piAP(nbobs, nbc, nba, T1);
            }else{
                metrique = alpha(nb,C,nbc, 0);
            }
            std::pair<int, float> pairNbcAlpha(nbc, metrique);
            //cout << " pairNbcAlpha: " << pairNbcAlpha.first << "/" << pairNbcAlpha.second;
            //cout<< "combinaisons de n-p annotateurs :" <<endl;
            calculDifference(T1,nbobs,nba,nbc, &vPourcentageErreur, egaliteInt, choixGold);
            mapResultat.insert(std::pair<std::pair<int, float>, std::vector<float>>(pairNbcAlpha, vPourcentageErreur));
        }
        //affichage résultats
        for (std::map<std::pair<int, float>, std::vector<float>>::iterator it=mapResultat.begin(); it!=mapResultat.end(); ++it){
            std::pair<int, float> m = it->first;
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
            std::vector<float> valeurs = it->second;
            cout << nba-i << ": " << valeurs[i] <<"%   ";
            }
            cout << endl;
            cout << endl;
        }
        fichierSortie(choix, choixNbClasse, choixMetrique, egalite, choixGold, nba, mapResultat);
    }else{
        choixTableau(choix,T,nbobs,nba,nbc); //on rempli tous les parametres par les valeurs lu dans le fichier
        afficheTableauLu(T,nbobs,nba); //on affiche le tableau d'annotations
        //coincidences
        transposeT(T,T1,nba,nbobs); //on inverse les i et j du tableau
        cout << "Tableau des coincidences :\n";
        coincidences(T1,nbobs,nba,C,nbc,nb);
        affiche_coincidences(C,nbc);
        cout << "nb d'observations prises en compte="<<nb <<endl;

        float metrique;
        if(choixMetrique.compare("k")==0){
            metrique = kappaAP(nbobs, nbc, nba, T1);
            cout << "kappa= ";
        }else if(choixMetrique.compare("ap")==0){
            metrique = alpha(nb,C,nbc, 1);
            cout << "alpha(pondere)= ";
        }else if(choixMetrique.compare("pi")==0){
            cout << "pi= ";
        }else{
            metrique = alpha(nb,C,nbc, 0);
            cout << "alpha= ";
        }

        cout << metrique << endl;

        cout << "combinaisons de n-p annotateurs :" <<endl;

        std::vector<float> vPourcentageErreur;
        calculDifference(T1,nbobs,nba,nbc, &vPourcentageErreur, egaliteInt, choixGold);

        std::pair<int, float> pairNbcAlpha(nbc, metrique);
        mapResultat.insert(std::pair<std::pair<int, float>, std::vector<float>>(pairNbcAlpha, vPourcentageErreur));
        //affichage résultats
        for(int i=0; i<nba-1; i++){
            cout << nba-i << ": " << mapResultat.at(pairNbcAlpha)[i] <<"%   ";
        }
        fichierSortie(choix, choixNbClasse, choixMetrique, egalite, choixGold, nba, mapResultat);

    }
    return 0;
}







