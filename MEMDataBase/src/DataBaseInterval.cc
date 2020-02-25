#include "/nfs/dust/cms/user/swieland/ttH_legacy/MEMdatabase/CodeforScriptGenerator/MEMDataBase/MEMDataBase/interface/DataBaseInterval.h"


DataBaseInterval::DataBaseInterval(){
 minVal=0;
 maxVal=0; 
  
}

DataBaseInterval::DataBaseInterval(Long64_t minVal_, Long64_t maxVal_){
  minVal=minVal_;
  maxVal=maxVal_;
}

DataBaseInterval::~DataBaseInterval(){}

Long64_t DataBaseInterval::GetMinVal() const {
 return minVal; 
}

Long64_t DataBaseInterval::GetMaxVal() const {
 return maxVal; 
}

bool DataBaseInterval::ValueIsInInterval(Long64_t value){
  if(value<=maxVal and value>=minVal) return true;
  else return false;
}
