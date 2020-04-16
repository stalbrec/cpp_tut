#include "Data.hh"

#include <iostream>
#include <fstream>
#include <cassert>

using namespace std;

Data::Data(const std::string& filename) {
  // read data from file
  // return 1 if successful, 0 if otherwise

  // open the file
  ifstream file(filename);

  // check if file opened correctly
  if (!file.is_open()) {
    throw "Error could not open file with name: " + filename;
  }

  int size = -1;
  file >> size;

  // read in data from file: m_siz+1 bin edges
  for (int i = 0; i < size + 1; i++) {
    double edge;
    file >> edge;
    m_bins.push_back(edge);
  }
  // read in data from file: m_siz bin contents
  for (int i = 0; i < size; i++) {
    double entries;
    file >> entries;
    m_data.push_back(entries);
  }

  // done! close the file
  file.close();
};

void Data::assertSizes() { assert(m_data.size() + 1 == m_bins.size()); }
