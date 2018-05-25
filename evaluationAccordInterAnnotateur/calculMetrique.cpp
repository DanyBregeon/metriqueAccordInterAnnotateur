#include <iostream>
#include <vector>
#include <cstring>
#include <cstdlib>
#include <ctime>
#include <cmath>
#include <cstring>
#include <fstream>
#include <cstdio>
#include "calculMetrique.h"

using namespace std;

//const int Max_obs=3000;//nb maximal d'observables
//const int Max_classes=10;//nb maximal de classes
//const int Max_annot=30;//nb maximal d'annotateurs


// I - CALCULS KRIPPENDORFF : TABLEAU DES COINCIDENCES C


//calcul de C[c1][c2] (C : tableau des coincidences)
float calcul(vector<vector<int>> & vObsAnnot,int nbobs,int nbannot,int c1,int c2) {
  float temp,res=0;
  int nbannotvrai;
  for (int e=0;e<nbobs;e++) {//pour chaque observable
    for (int k1=0;k1<nbannot;k1++) {//pour chaque annotateur
      temp=0;
      if (vObsAnnot[e][k1]!=-1) { //si l'annotation existe
        nbannotvrai=nbannot;
        for (int k2=0;k2<nbannot;k2++) {
          if (k1!=k2) {
            if ((vObsAnnot[e][k1]==c1)&& (vObsAnnot[e][k2]==c2)) temp++;
            if (vObsAnnot[e][k2]==-1) nbannotvrai--;
          }
        }
        if (nbannotvrai>1) res=res+(temp*1.0)/(nbannotvrai-1);
      }
     }
  }
  return res;
}


//nb est le nombre d'observations effectives (nbannot X nblignes si pas de données manquantes)
void coincidences(vector<vector<int>> & vObsAnnot,int nblignes,int nbannot,
		 vector<vector<float>> & C,int nbclasses,float & nb) {
  int i,j;
  nb=0;
  for (i=0;i<nbclasses;i++)
    for (j=0;j<nbclasses;j++) {
    /*  if (i==j) C[i][j]=calcul1(T,nblignes,nbannot,i,nbclasses);
      else C[i][j]=calcul2(T,nblignes,nbannot,i,j,nbclasses);
      nb+=C[i][j];
    }*/
    C[i][j]=calcul(vObsAnnot,nblignes,nbannot,i,j);
    nb+=C[i][j];
  }
  //cout << "ici, nb="<< nb << endl;
}

void affiche_coincidences(vector<vector<float>> & C,int nbclasses) {
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
int xikres(int i,int k,int Nbareel,vector<vector<int>> & vObsAnnot){
  int j,res=0;
  for (j=0;j<Nbareel;j++)
    if (vObsAnnot[i][j]==k) res++;
  //if (res!=0) printf("%d, ",res);
  return res;
}
//pkc=probas pour l'annotateur c de choisir la modalité k
double pkc(int c,int k,int nblignes,vector<vector<int>> & vObsAnnot){
  int i,nck=0;
  double res;
  for (i=0;i<nblignes;i++)
    if (vObsAnnot[i][c]==k) nck++;
  res=(1.0*nck)/nblignes;
  //printf("pkc(%d,%d)=%f\n",c,k,res);
  return res;
}

//II -b : désaccord observé et désaccord attendu

float Aepi(int nblignes,int nbclasses,int Nbareel, vector<vector<int>> & vObsAnnot){
  int i,j,Nbtemp[nbclasses];//pour stocker le nombre de votes pour chaque classe
  float res;
  for (i=0;i<nbclasses;i++)
    Nbtemp[i]=0;
  for (i=0;i<nblignes;i++)
    for (j=0;j<Nbareel;j++) {
      Nbtemp[vObsAnnot[i][j]]++;
    }
  res=0;
  for (i=0;i<nbclasses;i++)
    res+=Nbtemp[i]*Nbtemp[i];
  res=res/(nblignes*nblignes*Nbareel*Nbareel);
  //printf("Aepi=%f\n",res);
  return res;
}

double Aekappa(int nblignes,int nbclasses,int Nbareel, vector<vector<int>> & vObsAnnot){
  int comb2Nba=(Nbareel*(Nbareel-1))/2;
  //printf("comb2Nba=%d\n",comb2Nba);
  double res=0,den=0;
  for (int k=0;k<nbclasses;k++) {
    for (int a1=0;a1<Nbareel-1;a1++)
      for (int a2=a1+1;a2<Nbareel;a2++)
	res=res+pkc(a1,k,nblignes,vObsAnnot)*pkc(a2,k,nblignes,vObsAnnot);
  }
  return (res*1.0)/comb2Nba;
}

//on a nbclasses, exprimées dans le tableau T par un entier de 0 à nbclasses-1
float Aokappa(int nblignes,int nbclasses,int Nbareel, vector<vector<int>> & vObsAnnot){
  float res=0;
  int sum=0,i,nik;
  for (i=0;i<nblignes;i++) {
    for (int k=0;k<nbclasses;k++) {
      nik=xikres(i,k,Nbareel,vObsAnnot);
      sum=sum+nik*(nik-1);
    }
  }
  res=(sum*1.0)/(nblignes*Nbareel*(Nbareel-1));
  return res;
}

// Désaccord observé
float Do(float nb, vector<vector<float>> & C,int nbclasses, int pondere){
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

float n_calc(vector<vector<float>> & C,int l,int nbclasses) {
  float res=0;
  int i;
  for (i=0;i<nbclasses;i++) res=res+C[i][l];
  return res;
}

//Désaccord attendu (expected)
float De(float nb, vector<vector<float>> & C,int nbclasses, int pondere) {
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

float piAP(int nblignes,int nbclasses,int Nbareel, vector<vector<int>> & vObsAnnot){
   float Ao,Ae;
   Ao=Aokappa(nblignes,nbclasses,Nbareel,vObsAnnot);
   //printf("Ao=%f\n",Ao);
   Ae=Aepi(nblignes,nbclasses,Nbareel,vObsAnnot);
   //printf("Aepi=%f\n",Ae);
   return (Ao-Ae)/(1.0-Ae);
}

float kappaAP(int nblignes,int nbclasses,int Nbareel, vector<vector<int>> & vObsAnnot){
   float Ao,Ae;
   Ao=Aokappa(nblignes,nbclasses,Nbareel,vObsAnnot);
   //printf("Ao=%f\n",Ao);
   Ae=Aekappa(nblignes,nbclasses,Nbareel,vObsAnnot);
   //printf("Aekappa=%f\n",Ae);
   return (Ao-Ae)/(1.0-Ae);
}

//nb = nb d'observations effectives = nb_experts*nb_observables si pas de données manquantes
float alpha(float nb, vector<vector<float>> & C, int nbclasses, int pondere) {
  float res=1-(Do(nb,C,nbclasses, pondere)/De(nb,C,nbclasses, pondere));
  if (res<0) return 0; else return res;
}

