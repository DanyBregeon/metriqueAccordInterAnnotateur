#ifndef FICHIERSORTIE_H_INCLUDED
#define FICHIERSORTIE_H_INCLUDED

#include <string>
#include <vector>
#include <map>

int fichierSortie (int choixCorpus, int choixNbClasse, std::string choixMetrique, int choixGold,
                   int nba, std::map<std::pair<int, float>, std::vector<float>> mapResultat);

#endif // FICHIERSORTIE_H_INCLUDED
