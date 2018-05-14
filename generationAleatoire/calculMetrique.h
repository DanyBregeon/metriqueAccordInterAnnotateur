#ifndef CALCULMETRIQUE_H_INCLUDED
#define CALCULMETRIQUE_H_INCLUDED

#include <vector>

/*void kappa_pi_tableau(std::vector<std::vector<int>> & vAnnotObs,int nbannot, int nblignes, int nbclasses,
                      float & res_kappa);*/

float kappaAP(int nblignes,int nbclasses,int Nbareel, std::vector<std::vector<int>> & vAnnotObs);

#endif // CALCULMETRIQUE_H_INCLUDED
