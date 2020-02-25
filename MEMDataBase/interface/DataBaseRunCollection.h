#ifndef DATABASERUNCOLLECTION_H
#define DATABASERUNCOLLECTION_H

#include "/nfs/dust/cms/user/swieland/ttH_legacy/MEMdatabase/CodeforScriptGenerator/MEMDataBase/MEMDataBase/interface/DataBaseLumiCollection.h"


class RunCollection{
public:
  RunCollection();
  ~RunCollection();
  RunCollection(const Long64_t runNumber_);
  
  Long64_t GetRunNumber(){return runNumber;}
  
  void AddLumiAndEventInterval(const Long64_t minLumi_, const Long64_t maxLumi_,const Long64_t minEvent_, const Long64_t maxEvent_);
  void PrintStructure();
  
  TString GetLumiAndEventIntervalString(const Long64_t lumiSection, const Long64_t eventNumber);
  bool GetRelevantLumi(const Long64_t lumiSection, Long64_t* minlumi, Long64_t* maxlumi);
  bool RemoveEventInterval(const Long64_t minLumi_, const Long64_t maxLumi_,const Long64_t minEvent_, const Long64_t maxEvent_);
  
  
private:
  std::vector<LumiCollection> lumiCollections;
  Long64_t runNumber;
  
  
};

#endif