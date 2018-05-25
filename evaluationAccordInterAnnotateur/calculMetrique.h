#ifndef CALCULMETRIQUE_H_INCLUDED
#define CALCULMETRIQUE_H_INCLUDED

//const int Max_obs=3000;//nb maximal d'observables
//const int Max_classes=10;//nb maximal de classes
//const int Max_annot=30;//nb maximal d'annotateurs

#include <vector>

float piAP(int nblignes,int nbclasses,int Nbareel, std::vector<std::vector<int>> & vObsAnnot/*int T[Max_obs][Max_annot]*/);

float kappaAP(int nblignes,int nbclasses,int Nbareel, std::vector<std::vector<int>> & vObsAnnot/*int T[Max_obs][Max_annot]*/);

float alpha(float nb, std::vector<std::vector<float>> & C/*float C[Max_classes][Max_classes]*/, int nbclasses, int pondere);

void coincidences(std::vector<std::vector<int>> & vObsAnnot,int nblignes,int nbannot, std::vector<std::vector<float>> & C,int nbclasses,float & nb);

#endif // CALCULMETRIQUE_H_INCLUDED
