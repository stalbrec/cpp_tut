#include <iostream>
#include <vector>
#include <functional>
#include <string>

#include "Data.hh"

// generic function comparing two values of some type T
template <class T>
bool testEqual(const std::string& name, T expected, T real) {
  if (expected != real) {
    std::cout << "(" << name << ": <" << expected << ">"
              << ", " << real << ") ";
    return false;
  }
  return true;
}

bool testReadingSize() {
  std::cout << "testReadingSize...";
  Data datA("testA");
  return testEqual<int>("size", 1, datA.size());
}

bool testReadingMeasurement() {
  std::cout << "testReadingMeasurement...";
  Data datA("testA");
  return testEqual("measurement", 10., datA.measurement(0));
}

bool testReadingBinEdges() {
  std::cout << "testReadingBinEdges...";
  Data datA("testA");
  return testEqual("bin low", 0., datA.binLow(0)) &&
         testEqual("bin high", 1., datA.binHigh(0));
}

bool testReadingErrors() {
  std::cout << "testReadingErrors...";
  Data datA("testA");
  return testEqual("error", 2., datA.error(0));
}

bool testCopyConstructor() {
  std::cout << "testCopyConstructor...";
  Data a("testA");
  Data b(a);
  Data c = a;  // equivalent to Data c(a)
  return testEqual("measurement", 10., b.measurement(0)) &&
         testEqual("measurement", 10., c.measurement(0));
}

void runTests() {
  std::cout << "running tests...\n";
  std::vector<std::function<bool()> > tests(
      {testReadingSize, testReadingMeasurement, testReadingBinEdges,
       testReadingErrors, testCopyConstructor});
  for (auto test : tests)
    std::cout << (test() ? " ok" : " FAILED!") << std::endl;
}

int main() {
  using namespace std;

  cout << "******************************************************" << endl;
  runTests();
  cout << "******************************************************" << endl;
  // create an object which holds data of experiment A

  std::vector<std::string> datasets = {"A","B","C","D"};
  
  std::vector<Data*> dat = {};
  // dat.reserve(datasets.size());

  for(int i = 0 ; i<datasets.size(); i++){
    Data *this_dat = new Data("exp_"+datasets[i]);
    dat.push_back(this_dat);
  }
  
  // here is the data from experiment A
  cout << "bin 0: from " << dat[0]->binLow(0) << " to " << dat[0]->binHigh(1) << endl;
  cout << "measurement of experiment A in bin 0: " << dat[0]->measurement(0)
       << endl;


  for(int i = 0; i < datasets.size(); i++){
    for(int j = 0; j < datasets.size(); j++){
      if(i==j || i>j)continue;
      cout << "comparing datasets dat"<<datasets[i] <<" and dat"<< datasets[j]<< endl;
      cout << "number of not compatible bins (within 2sigma): " << dat[i]->CheckCompatibility(dat[j],2) << endl;
      cout << "number of not compatible bins (within 3sigma): " << dat[i]->CheckCompatibility(dat[j],3) << endl;
    }
  }

  Data* combined = new Data("exp_A");
  cout << "combining A and B" << endl;
  combined->Combine(dat[1]);
  cout << "combining AB and C" << endl;
  combined->Combine(dat[1]);
  cout << "combining ABC and D" << endl;
  combined->Combine(dat[1]);

  for(int i = 0; i < datasets.size(); i++){
    cout << "dataset " << datasets[i] << " bin27: " << dat[i]->measurement(27) << " +- " << dat[i]->error(27)<<endl;
  }

  cout << "combined dataset " << " bin27: " << combined->measurement(27) << " +- " << combined->error(27)<<endl;
  return 0;
}
