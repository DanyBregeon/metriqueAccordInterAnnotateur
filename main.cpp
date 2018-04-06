#include <iostream>
using namespace std;
#include <cstring>
#include <cstdlib>
#include <ctime>
#include <cmath>
#include <cstring>
#include <fstream>
#include <cstdio>
#include <vector>
#include <sstream>


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
/*float delta_car(int i,int j, int classes)  {
    float res;
    if (i>j) res=i-j; else res=j-i;//ou bien : res=sqrt((i-j)*(i-j))
    return res;
}*/
//bien d'autres distances sont possibles à définir suivant le pb traité

//II -b : désaccord observé et désaccord attendu

// Désaccord observé
float Do(float nb, float C[Max_classes][Max_classes],int nbclasses){
  float res=0;
  int i,j;
  for (i=0;i<nbclasses;i++)
    for (j=0;j<nbclasses;j++)
      if (i!=j) res=res+delta_car(i,j,nbclasses)*C[i][j];
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
float De(float nb,float C[Max_classes][Max_classes],int nbclasses) {
  float res=0;
  int i,j;
   for (i=0;i<nbclasses;i++)
    for (j=0;j<nbclasses;j++)
      if (i!=j) res=res+delta_car(i,j,nbclasses)*n_calc(C,i,nbclasses)*n_calc(C,j,nbclasses);
   res=res/(nb*(nb-1));
   //cout << "De=" << res << endl;
   return res;
}



//nb = nb d'observations effectives = nb_experts*nb_observables si pas de données manquantes
float alpha(float nb,float C[Max_classes][Max_classes],int nbclasses) {
  float res=1-(Do(nb,C,nbclasses)/De(nb,C,nbclasses));
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
  if(choix < 10){
        Nba=2;Nbobs=12;
        ss << choix;
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
    lire("classe3/TableauOpinion" + ss.str() +".csv",T,Nbobs,Nba,Nbc);
  }
  else if (choix>509 && choix<513) {
    Nba=2;Nbobs=12;
    ss << (choix-510);
    lire("classe3/TableauOpinionContexte" + ss.str() +".csv",T,Nbobs,Nba,Nbc);
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


//calcul du vote majoritaire et le compare avec celui de référence
void voteMajoritaire(int T[Max_obs][Max_annot], int nblignes, int nbc, int n,
                     int erreurVoteMaj[Max_annot], int voteMajReference[Max_obs], int nba){
    int voteMajoritaire[nblignes];
    int tNbOcc[nbc];
    for(int i=0; i<nbc; i++){
        tNbOcc[i]=0;
    }
    for(int obs=0; obs<nblignes; obs++){
        for(int ann=0; ann<n; ann++){
            tNbOcc[T[obs][ann]]++;
        }
        int max=tNbOcc[0];
        int voteMaj = 0;
        for(int i=1; i<nbc; i++){
            if(tNbOcc[i]>max){
                max = tNbOcc[i];
                voteMaj = i;
            }
        }
        voteMajoritaire[obs]=voteMaj;

        for(int i=0; i<nbc; i++){
            tNbOcc[i]=0;
        }
    }
    if(n == nba){
        erreurVoteMaj[n]=0;
        for(int i=0; i<nblignes; i++){
            voteMajReference[i] = voteMajoritaire[i];
        }
    }else{
        for(int i=0; i<nblignes; i++){
            if(voteMajoritaire[i] != voteMajReference[i]){
                erreurVoteMaj[n]++;
                //"% de modifications par rapport à la référence" par nombre d'annotateur ou pour chaque vote majoritaire ?
                //break;
            }
        }

    }

    //affichage
    /*cout << "vote maj: ";
    for(int i=0; i<nblignes; i++){
        cout << voteMajoritaire[i] << " ";
    }*/
    //cout << endl;
}

//nblignes = nbobs, n = 9 ou 8 ou 7 etc
void alphaCombinaison( int *p, int n, int T[Max_obs][Max_annot],int nblignes,int nba,
		  float C[Max_classes][Max_classes],int nbc,float & nb, std::vector<float> *vAlpha,
		  int erreurVoteMaj[Max_annot], int voteMajReference[Max_obs]) {
    int i;
    for (i = 0; i < n; i++){
        //cout << p[i];
    }
    //cout << " : ";
    int TT[Max_obs][Max_annot];
    int l = 0;
    //récupère les lignes (les annotateurs) qui correspondent à la combinaison
    for(int y=0; y<n; y++){
        for(int x=0; x<nblignes; x++){
            TT[x][l] = T[x][p[y]];
            //cout << TT[x][l] << " ";
        }
        l++;
        //cout << endl;
    }
    coincidences(TT,nblignes,n,C,nbc,nb);
    //affiche_coincidences(C,nbc);
    float a = alpha(nb,C,nbc);
    //cout << "alpha=" << a << "  ";
    voteMajoritaire(TT, nblignes, nbc, n, erreurVoteMaj, voteMajReference, nba);
    vAlpha->push_back(a);
}

//trouve l'ensemble des combinaisons de p parmi n
void combinaisons(int *ens, int *combinaison, int n, int p, int i, int t,
                  int T[Max_obs][Max_annot],int nblignes,int nba,
		  float C[Max_classes][Max_classes],int nbc,float & nb, std::vector<float> *vAlpha,
		  int erreurVoteMaj[Max_annot], int voteMajReference[Max_obs], int nbCombinaison[Max_annot]) {
    if (i<p) {
        for (int k=t; k<n; k++) {
            combinaison[i] = ens[k];
            combinaisons(ens,combinaison,n,p,i+1,k+1, T,nblignes,nba,C,nbc,nb, vAlpha, erreurVoteMaj, voteMajReference, nbCombinaison);
        }
    }
    else {
        alphaCombinaison(combinaison,p, T,nblignes,nba,C,nbc,nb, vAlpha, erreurVoteMaj, voteMajReference);
        nbCombinaison[p]++;
    }
}

//fait la moyenne des alpha
float moyenne(std::vector<float> *vAlpha){
    float total = 0;
    for(unsigned int i=0; i<vAlpha->size(); i++){
        total += vAlpha->at(i);
    }
    return total/vAlpha->size();
}

//fait l'ecart-type des alpha
float ecart_type(std::vector<float> *vAlpha, float moy) {
    float total = 0;
    for(unsigned int i=0; i<vAlpha->size(); i++){
        total += (vAlpha->at(i)-moy)*(vAlpha->at(i)-moy);
    }
    return sqrt(total/vAlpha->size());
}

void calculDifference(int T[Max_obs][Max_annot],int nblignes,int nba,
		  float C[Max_classes][Max_classes],int nbc,float & nb/*, int TPourcentageErreur[Max_annot]*/){
    int ens[nba];
    for(int i=0; i<nba; i++){
        ens[i] = i;
    }

    int erreurVoteMaj[Max_annot];
    int nbCombinaison[Max_annot];
    int voteMajReference[Max_obs];

    for(int p=nba; p>1; p--){
        //cout << p << " annotateurs / " << nba << endl;
        //cout << endl;
        int combi[p];
        std::vector<float> vAlpha;
        combinaisons(ens,combi,nba,p,0,0, T,nblignes,nba,C,nbc,nb, &vAlpha, erreurVoteMaj, voteMajReference, nbCombinaison);
        float moy = moyenne(&vAlpha);
        //cout << "moyenne : " << moy << endl;
        //cout << "ecart-type : " << ecart_type(&vAlpha, moy) << endl;
        //cout << endl;
        //int combi[nba-1];
    }
    cout << "poucentage d'erreur :  ";
    for(int i=nba; i>1; i--){
        //cout << i << ": " << erreurVoteMaj[i] << "/" << nbCombinaison[i] << "   ";
        //cout << i << ": " << ((float) erreurVoteMaj[i]/(float) nbCombinaison[i])*100 << "%   ";
        float pourcentage = ((float) erreurVoteMaj[i]/(float) (nbCombinaison[i]*nblignes))*100;
        cout << i << ": " << pourcentage << "%   ";
        //TPourcentageErreur[i] = pourcentage;
    }
    cout << endl;
    //return vAlpha.at(0);
}


int main() {
  // a priori 9 experts, le nb d'enonces se déduit du fichier
  int T[Max_annot][Max_obs],T1[Max_obs][Max_annot];
  float C[Max_classes][Max_classes];
  int choix;
  cout << "Choix ? (O pour auto, 1-6 pour les exemples, 301-312 pour emotions/opinions avec 3 classes, 501-512 avec 5 classes)";
  cin >> choix ;
  int nba,nbobs,nbc;//nb d'annotateurs, d'observables, de classes
  float nb;//observation réellement prises en compte

  if(choix==0){
    //int TFinal;
    //int TPourcentageErreur[Max_annot];
    choix = 300;
    cout << "3 classes :" << endl;
    for(int i=0; i<24; i++){
      if(i == 12){
        choix=500;
        cout << "5 classes :" << endl;
      }
      choix++;
      choixTableau(choix,T,nbobs,nba,nbc);
      //afficheTableauLu(T,nbobs,nba);
      transposeT(T,T1,nba,nbobs);
      //cout << "Tableau des coincidences :\n";
      coincidences(T1,nbobs,nba,C,nbc,nb);
      //affiche_coincidences(C,nbc);
      //cout << "nb d'observations prises en compte="<<nb <<endl;
      cout << "alpha=" << alpha(nb,C,nbc)<< endl;
      //cout<< "combinaisons de n-p annotateurs :" <<endl;
      calculDifference(T1,nbobs,nba,C,nbc,nb);
    }
  }else{

      choixTableau(choix,T,nbobs,nba,nbc); //on rempli tous les parametres par les valeurs lu dans le fichier
      afficheTableauLu(T,nbobs,nba); //on affiche le tableau d'annotations
      //coincidences
      transposeT(T,T1,nba,nbobs); //on inverse les i et j du tableau
      cout << "Tableau des coincidences :\n";
      coincidences(T1,nbobs,nba,C,nbc,nb);
      affiche_coincidences(C,nbc);
      cout << "nb d'observations prises en compte="<<nb <<endl;
      cout << "alpha=" << alpha(nb,C,nbc)<< endl;

      cout<< "combinaisons de n-p annotateurs :" <<endl;
      calculDifference(T1,nbobs,nba,C,nbc,nb);
  }
  return 0;
}







