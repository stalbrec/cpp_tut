#include "Data.hh"

#include <iostream>
#include <cmath>
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

  for (int i = 0; i < size; i++){
    double error;
    file >> error;
    m_error.push_back(error);
  }

  // done! close the file
  file.close();
};

void Data::assertSizes() { assert(m_data.size() + 1 == m_bins.size()); }

int Data::CheckCompatibility(Data* in, int N){
  int n_deviating_points=0;  
  for (int i = 0; i<this->size(); i++ ){
    double diff = std::abs(this->measurement(i) - in->measurement(i));
    if(diff > N * (this->error(i)+in->error(i)) ) n_deviating_points++;
    }
  return n_deviating_points;
};

void Data::Combine(Data* in){
  bool compatible = this->CheckCompatibility(in,3) == 0;
  cout << "Datasets are " << (compatible ? "compatible":"incompatible (Are you sure you want to combine these?)") << endl;
  for(int i = 0; i < this->size(); i++){
    double y_1 = this->measurement(i);
    double y_2 = in->measurement(i);
    double w_1 = 1./(this->error(i)*this->error(i));
    double w_2 = 1./(in->error(i)*in->error(i));
    double y = (y_1*w_1 + y_2*w_2)/(w_1+w_2);
    double sigma = std::sqrt(1./(w_1+w_2) );
    m_data[i] = y;
    m_error[i] = sigma;
  }
};
