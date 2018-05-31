#ifndef CALCULRESULTATS_H_INCLUDED
#define CALCULRESULTATS_H_INCLUDED

#include <vector>
#include <map>

void calculPrevalence(std::vector<std::pair<float, std::vector<float>>> & vPrevalence, float valeurMetrique,
                      std::vector<std::vector<int>> & vAnnotObs, int nbc, int nba, int nbobs);

void resultatsPalier(std::map<std::pair<int, float>, std::vector<float>> & mapResultat,
                      std::map<std::pair<int, float>, std::vector<float>> & mapResultat2, float palierPas, int nbc, int nba,
                      std::vector<std::pair<float, std::vector<float>>> & vPrevalence);

#endif // CALCULRESULTATS_H_INCLUDED
