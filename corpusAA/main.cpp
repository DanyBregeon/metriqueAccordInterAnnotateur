#include <iostream>
#include <cstring>
#include <cstdlib>
#include <sstream>
#include <iomanip>
#include <fstream>
#include <dirent.h>
#include <string>
#include <vector>
using namespace std;

const int Max_obs=3000;//nb maximal d'observables
const int Max_classes=10;//nb maximal de classes
const int Max_annot=30;//nb maximal d'annotateurs

void lire(string nomfich, string T[Max_annot][Max_obs], int an, int &indexObs){
    const char * nom_fichier=nomfich.c_str();
    ifstream file(nom_fichier);
    if (!file){
      cout << "erreur ouverture fichier " << nomfich << endl;
    }else{
        string ligne;
        int lg;
        //int indexObs = *indexObsP;
        bool relation = false;
        while ((!file.eof())){
            getline(file,ligne);
            lg=ligne.size();
            if(ligne.find("<relation") != string::npos){
                relation = true;
            }
            if(ligne.find("<schema") != string::npos){
                relation = false;
            }
            if(relation && (ligne.find("<type>") != string::npos)){
                int pos = 6;
                while ((pos<lg)&&(ligne[pos]!=60)){
                    pos++;
                }
                string reponse = ligne.substr(6, pos-6);
                //directe = 0, indirecte = 1, assoc = 2, assoc pronom = 3, anaphore = 4
                string classe;
                if(reponse.find("DIRECTE")==0){
                    classe = "0";
                }else if(reponse.find("INDIRECTE")==0){
                    classe = "1";
                }else if(reponse.find("ASSOC_PRONOM")==0){
                    classe = "3";
                }else if(reponse.find("ASSOC")==0){
                    classe = "2";
                }else if(reponse.find("ANAPHORE")==0){
                    classe = "4";
                }else{
                    classe = ".";
                }
                /*cout << ligne.substr(6, pos-6) << " ";
                cout << indexObs << endl;*/
                T[an][indexObs] = classe;
                indexObs++;
            }
        }
        //*indexObsP = indexObs;
        cout << indexObs << endl;
    }
    file.close();
}


//parcours tous les fichiers d'un dossier
std::vector<std::string> open(std::string path = ".") {

    DIR*    dir;
    dirent* pdir;
    std::vector<std::string> files;

    dir = opendir(path.c_str());

    while (pdir = readdir(dir)) {
        files.push_back(pdir->d_name);
    }

    return files;
}


void recupDataAnnotateur(string cheminDossier, string T[Max_annot][Max_obs], int numAnnot){
    int indexObs = 0;
    std::vector<std::string> f;

    f = open(cheminDossier);

    for(int i=0; i<f.size(); i++){
        //si le fichier commence par une lettre majuscule on le lit
        if(f[i].at(0)>64 && f[i].at(0)<91){
            lire(cheminDossier + "/" + f[i], T, numAnnot, indexObs);
        }
    }
}

void writeTabCsv(int nba, int nbobs, int nbc, string T[Max_annot][Max_obs]){

    ofstream file;
    file.open("TableauTypageRelation.csv");

    file << "Nbannotateurs=" << nba << endl;
    file << "Nbclasses=" << nbc << endl;
    file << "Nbobservables=" << nbobs << endl;

    for(int i=0; i<nba; i++){
        for(int j=0; j<nbobs; j++){
            file << T[i][j] << "\t";
        }
        file << endl;
    }


}


int main()
{
    int nba=9, nbobs=384, nbc=5;
    string T[Max_annot][Max_obs];
    recupDataAnnotateur("corpusTypageRelations/AL", T, 0);
    recupDataAnnotateur("corpusTypageRelations/AS", T, 1);
    recupDataAnnotateur("corpusTypageRelations/DM", T, 2);
    recupDataAnnotateur("corpusTypageRelations/ES", T, 3);
    recupDataAnnotateur("corpusTypageRelations/FS", T, 4);
    recupDataAnnotateur("corpusTypageRelations/IE", T, 5);
    recupDataAnnotateur("corpusTypageRelations/JM", T, 6);
    recupDataAnnotateur("corpusTypageRelations/JV", T, 7);
    recupDataAnnotateur("corpusTypageRelations/JYA", T, 8);

    for(int i=0; i<nba; i++){
        for(int j=0; j<nbobs; j++){
            cout << T[i][j] << "    ";
        }
        cout << endl;
    }

    writeTabCsv(nba, nbobs, nbc, T);

    /*int indexObs = 0;
    std::vector<std::string> f;

    f = open("corpusTypageRelations/AL");

    for(int i=0; i<f.size(); i++){
        //si le fichier commence par une lettre majuscule on le lit
        if(f[i].at(0)>64 && f[i].at(0)<91){
            lire("corpusTypageRelations/AL/" + f[i], T, 0, &indexObs);
        }
    }*/

    //lire("corpusTypageRelations/AL/A_549Anais3.aa");
    return 0;
}
