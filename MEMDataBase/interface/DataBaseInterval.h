#ifndef DATABASEINTERVAL_H
#define DATABASEINTERVAL_H

#include "TFile.h"
#include "TString.h"
#include "TTree.h"
#include "TObjArray.h"
#include "TObjString.h"
#include <iostream>
#include <fstream>
#include <algorithm>
#include <map>

class DataBaseInterval{
  
public:
  DataBaseInterval();
  DataBaseInterval(Long64_t minVal_, Long64_t maxVal_);
  ~DataBaseInterval();
  
  bool ValueIsInInterval(Long64_t value);
  Long64_t GetMinVal() const;
  Long64_t GetMaxVal() const;
  
  static bool minIsSmaller(const DataBaseInterval& d1, const DataBaseInterval& d2){return (d1.GetMinVal() < d2.GetMinVal()); }

  
private:
  
  Long64_t minVal;
  Long64_t maxVal;
  
};

#endif