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

const int Max_obs=3000;//nb maximal d'observables
const int Max_classes=10;//nb maximal de classes
const int Max_annot=30;//nb maximal d'annotateurs



//CALCULS Kappa
//Artmann Poesio

//nb de fois où l'observable i est classé dans la catégorie k
int xikres(int i,int k,int Nbareel,vector<vector<int>> & vAnnotObs){
  int j,res=0;
  for (j=0;j<Nbareel;j++)
    if (vAnnotObs[i][j]==k) res++;
  return res;
}
//pkc=probas pour l'annotateur c de choisir la modalité k
double pkc(int c,int k,int nblignes,vector<vector<int>> & vAnnotObs){
  int i,nck=0;
  double res;
  for (i=0;i<nblignes;i++)
    if (vAnnotObs[i][c]==k) nck++;
  res=(1.0*nck)/nblignes;
  return res;
}



double Aekappa(int nblignes,int nbclasses,int Nbareel, vector<vector<int>> & vAnnotObs){
  int comb2Nba=(Nbareel*(Nbareel-1))/2;
  double res=0;//,den=0;
  for (int k=0;k<nbclasses;k++) {
    for (int a1=0;a1<Nbareel-1;a1++)
      for (int a2=a1+1;a2<Nbareel;a2++)
	res=res+pkc(a1,k,nblignes,vAnnotObs)*pkc(a2,k,nblignes,vAnnotObs);
  }
  return (res*1.0)/comb2Nba;
}

//on a nbclasses, exprimées dans le tableau T par un entier de 0 à nbclasses-1
float Aokappa(int nblignes,int nbclasses,int Nbareel, vector<vector<int>> & vAnnotObs){
  float res=0;
  int sum=0,i,nik;
  for (i=0;i<nblignes;i++) {
    for (int k=0;k<nbclasses;k++) {
      nik=xikres(i,k,Nbareel,vAnnotObs);
      sum=sum+nik*(nik-1);
    }
  }
  res=(sum*1.0)/(nblignes*Nbareel*(Nbareel-1));
  return res;
}

float Aepi(int nblignes,int nbclasses,int Nbareel, vector<vector<int>> & vAnnotObs){
  int i,j,Nbtemp[nbclasses];//pour stocker le nombre de votes pour chaque classe
  float res;
  for (i=0;i<nbclasses;i++)
    Nbtemp[i]=0;
  for (i=0;i<nblignes;i++)
    for (j=0;j<Nbareel;j++) {
      Nbtemp[vAnnotObs[i][j]]++;
    }
  res=0;
  for (i=0;i<nbclasses;i++)
    res+=Nbtemp[i]*Nbtemp[i];
  res=res/(nblignes*nblignes*Nbareel*Nbareel);
  return res;
}

float kappaAP(int nblignes,int nbclasses,int Nbareel, vector<vector<int>> & vAnnotObs){
   float Ao,Ae;
   Ao=Aokappa(nblignes,nbclasses,Nbareel,vAnnotObs);
   //printf("Ao=%f\n",Ao);
   Ae=Aekappa(nblignes,nbclasses,Nbareel,vAnnotObs);
   //printf("Aekappa=%f\n",Ae);
   return (Ao-Ae)/(1.0-Ae);
}

float piAP(int nblignes,int nbclasses,int Nbareel, vector<vector<int>> & vAnnotObs){
   float Ao,Ae;
   Ao=Aokappa(nblignes,nbclasses,Nbareel,vAnnotObs);
   printf("Ao=%f\n",Ao);
   Ae=Aepi(nblignes,nbclasses,Nbareel,vAnnotObs);
   printf("Aepi=%f\n",Ae);
   return (Ao-Ae)/(1.0-Ae);
}

//calcul de l'accord sur le tableau T à nblignes avec nbannot annotateurs,
// résultats  :kappa, pi
void kappa_pi_tableau(vector<vector<int>> & vAnnotObs,int nbannot, int nblignes, int nbclasses,
                      float & res_kappa) {
  res_kappa=kappaAP(nblignes,nbclasses,nbannot,vAnnotObs);
  //res_pi=piAP(nblignes,nbclasses,nbannot,vAnnotObs);
  //cout << "kappa=" << res_kappa << endl;
}



