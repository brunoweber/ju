#include <algorithm>
#include <iostream>
#include <fstream>
#include <vector>
#include <cmath>
#include <cstdlib>
#include <set>
#include <string>
#include <utility>


std::string StrList(const std::vector<int>& list) {
  std::string s;

  if (!list.empty()) {
    std::vector<std::vector<int> > groups(1);
    groups[0].push_back(list[0]);
    int igrp = 0;

    for (int i = 1; i < list.size(); ++i) {
      if (list[i] != list[i - 1] + 1) {
        groups.resize(groups.size() + 1);
        igrp++;
      }
      groups[igrp].push_back(list[i]);
    }

    for (int i = 0; i < groups.size(); ++i) {
      if (i != 0) {
        if (i == groups.size() - 1 && groups[i].size() != 2) {
          // before last group, if 1 or more than 2 numbers in last group
          s += " et ";
        } else {
          s += ", ";
        }
      }

      if (groups[i].size() > 2) {
        // more than 2 numbers in group
        s += std::to_string(groups[i][0]) + " à " + std::to_string(groups[i].back());
      } else if (groups[i].size() > 1) {
        // 2 numbers in group and...
        if (i == groups.size() - 1) {
          // ... last group
          s += std::to_string(groups[i][0]) + " et " + std::to_string(groups[i].back());
        } else {
          // ... not last group
          s += std::to_string(groups[i][0]) + ", " + std::to_string(groups[i].back());
        }
      } else {
        // 1 number in group
        s += std::to_string(groups[i][0]);
      }
    }
  }

  return s;
}


int main() {

  std::cout << "============================================================" << std::endl;
  std::cout << " Calculs formatés à copier/coller dans Word :" << std::endl;
  std::cout << "   1. Générer des tables de multiplication" << std::endl;
  std::cout << "   2. Copier/coller le contenu du fichier \"tables.txt\" généré" << std::endl;
  std::cout << "   3. Word : Insertion > Tableau > Convertir le texte en tableau" << std::endl;
  std::cout << "   4. Sélectionner le séparateur \";\"" << std::endl;
  std::cout << "============================================================" << std::endl;

  int nb_tables = 0;
  std::cout << "Nombre de tables : ";
  std::cin >> nb_tables;

  std::vector<int> tables;
  for (int i = 0; i < nb_tables; ++i) {
    std::cout << "Table " << i << " : ";
    int value;
    std::cin >> value;
    tables.push_back(value);
  }
  std::sort(tables.begin(), tables.end());

  int nb_calc = 0;
  std::cout << "Nombre de calculs : ";
  std::cin >> nb_calc;

  int width = 5; //(int)std::sqrt((double)nb_calc);
  int height = nb_calc / width;
  if (width * height != nb_calc) height++;

  int reload = 0;
  do {
    reload = 0;

    std::ofstream file("./tables.txt");

    std::set<std::pair<int,int> > operations;

    while (operations.size() < nb_calc) {
      int table = (double)std::rand() / RAND_MAX * nb_tables;
      if (table > (nb_tables - 1)) table = (nb_tables - 1);
      int value = (double)std::rand() / RAND_MAX * 11;
      if (value > 10) value = 10;
      int tab[2] = {tables[table], value};
      int side = (double)std::rand() / RAND_MAX * 2;
      if (side > 1) side = 1;
      operations.insert(std::pair<int,int>(tab[side], tab[(side + 1) % 2]));
    }

    std::vector<std::pair<int,int> > sorted_operations(operations.begin(), operations.end());
    operations.clear();

    std::cout << "============================================================" << std::endl;
    std::cout << " Calculs proposés :" << std::endl;
    std::cout << "============================================================" << std::endl;

    std::vector<int> left;
    std::vector<int> right;
    for (int i = 0; i < nb_calc; ++i) {
      if (i > 0 && i % width == 0)
        std::cout << std::endl;

      int id = (double)std::rand() / RAND_MAX * sorted_operations.size();
      if (id >= sorted_operations.size()) id = sorted_operations.size() - 1;

      std::vector<std::pair<int,int> >::iterator it = sorted_operations.begin() + id;
      std::cout << it->first << " x " << it->second << '\t';

      left.push_back(it->first);
      right.push_back(it->second);

      sorted_operations.erase(it);
    }

    std::cout << std::endl;

    // Write to file

    file << nb_calc << " calcul" << (nb_calc > 1 ? "s" : "") << " en ___ minutes !" << std::endl;
    file << "Table" << (nb_tables > 1 ? "s" : "") << " de " << StrList(tables) << "." << std::endl;
    file << std::endl;

    for (int i = 0; i < width * height; ++i) {
      if (i > 0 && i % width == 0)
        file << std::endl;

      if (i < nb_calc)
        file << left[i] << " x " << right[i] << " = ___";

      if ((i + 1) % width != 0)
        file << ";";
    }

    file << std::endl;
    file << std::endl;
    file << "Correction" << std::endl;
    file << std::endl;

    for (int i = 0; i < width * height; ++i) {
      if (i > 0 && i % width == 0)
        file << std::endl;

      if (i < nb_calc)
        file << left[i] << " x " << right[i] << " = " << (left[i] * right[i]);

      if ((i + 1) % width != 0)
        file << ";";
    }

    file << std::endl;

    // Reloader

    std::cout << "Recharger (o/n) ? ";
    char c;
    std::cin >> c;
    if (c == 'o') reload = 1;
  } while (reload == 1);

  return 0;
}
