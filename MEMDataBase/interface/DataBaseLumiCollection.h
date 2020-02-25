#ifndef DATABASELUMICOLLECTION_H
#define DATABASELUMICOLLECTION_H

#include "/nfs/dust/cms/user/swieland/ttH_legacy/MEMdatabase/CodeforScriptGenerator/MEMDataBase/MEMDataBase/interface/DataBaseInterval.h"

class LumiCollection{
public:
  LumiCollection();
  ~LumiCollection();
  LumiCollection(Long64_t minLumi_, Long64_t maxLumi_);

  Long64_t GetMinLumi() const {return lumiInterval.GetMinVal();}
  Long64_t GetMaxLumi() const {return lumiInterval.GetMaxVal();}
  
  void AddEventInterval(const Long64_t minEvent_, const Long64_t maxEvent_);
  bool RemoveEventInterval(const Long64_t minEvent_, const Long64_t maxEvent_);
  void PrintStructure();

  static bool minIsSmaller(const LumiCollection& l1, const LumiCollection& l2){return (l1.GetMinLumi()<l2.GetMinLumi()); }

  TString GetEventIntervalString(const Long64_t eventNumber);
  
private:
  std::vector<DataBaseInterval> eventCollections;
  DataBaseInterval lumiInterval;
  
};


#endif