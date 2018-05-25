#ifndef CALCULRESULTATS_H_INCLUDED
#define CALCULRESULTATS_H_INCLUDED

#include <vector>
#include <map>

void resultatsPalier(std::map<std::pair<int, float>, std::vector<float>> & mapResultat,
                      std::map<std::pair<int, float>, std::vector<float>> & mapResultat2, float palierPas, int nbc, int nba);

#endif // CALCULRESULTATS_H_INCLUDED
