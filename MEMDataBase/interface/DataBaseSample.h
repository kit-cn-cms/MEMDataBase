#ifndef DATABASESAMPLE_H
#define DATABASESAMPLE_H

#include "TFile.h"
#include "TString.h"
#include "TTree.h"
#include "TObjArray.h"
#include "TObjString.h"
#include <iostream>
#include <fstream>
#include <algorithm>
#include "/nfs/dust/cms/user/swieland/ttH_legacy/MEMdatabase/CodeforScriptGenerator/MEMDataBase/MEMDataBase/interface/DataBaseMEMResult.h"
#include "/nfs/dust/cms/user/swieland/ttH_legacy/MEMdatabase/CodeforScriptGenerator/MEMDataBase/MEMDataBase/interface/DataBaseRunCollection.h"
#include <map>
#include <cstdio>
#include "TMath.h"

class DataBaseSample{

public:
  DataBaseSample();
  DataBaseSample(const TString sampleName_, const TString dataBaseDirectory_,const std::vector<TString> mem_strings_ ,const TString indexFile_="");
  ~DataBaseSample();
  
  void AddRunLumiEventCollection(const Long64_t runNumber_, const Long64_t minLumi_, const Long64_t maxLumi_,const Long64_t minEvent_, const Long64_t maxEvent_);
  void PrintStructure();
  
  DataBaseMEMResult GetMEMResult(const Long64_t runNumber, const Long64_t lumiSection, const Long64_t eventNumber);
  
  bool AddEvent(Long64_t runNumber, Long64_t lumiSection, Long64_t eventNumber, Double_t p, Double_t p_sig, Double_t p_bkg, Double_t p_err_sig, Double_t p_err_bkg, Double_t n_perm_sig, Double_t n_perm_bkg);
  bool AddEvent(Long64_t runNumber, Long64_t lumiSection, Long64_t eventNumber, std::vector<Double_t> p_vec, Double_t p_sig, Double_t p_bkg, Double_t blr_4b, Double_t blr_2b);
  bool SaveDataBase();
private:

  TFile* currentOpenTreeFile;
  TTree* currentOpenTree;
  std::vector<RunCollection> runCollection;
  
  std::vector<TString> FileNames;
  
  TString currentOpenFileName;

  Int_t currentTreeEntries;
  TString openMode;
  
  static const int maxEventsPerTree=100000;
  static const int LookUpStepSize=1000;
  
  TString GetFileNameForEvent(const Long64_t runNumber, const Long64_t lumiSection, const Long64_t eventNumber);
//   bool GetRelevantRunAndLumi(const Long64_t lumiSection, const Long64_t eventNumber);
  void AddEventToTree(Long64_t runNumber, Long64_t lumiSection, Long64_t eventNumber, Double_t p, Double_t p_sig, Double_t p_bkg, Double_t p_err_sig, Double_t p_err_bkg, Double_t n_perm_sig, Double_t n_perm_bkg);
  void AddEventToTree(Long64_t runNumber, Long64_t lumiSection, Long64_t eventNumber, std::vector<Double_t> p_vec, Double_t p_sig, Double_t p_bkg, Double_t blr_4b, Double_t blr_2b);
  bool RemoveEventCollection(const Long64_t runNumber_, const Long64_t minLumi_, const Long64_t maxLumi_,const Long64_t minEvent_, const Long64_t maxEvent_);
  
  bool OpenTree(TString filename);
  bool CreateNewTree(TString filename);
  bool SplitCollection(TString filename);
  bool CloseTree();

  
  
  TString dataBaseDirectory;
  TString indexFile;
  TString sampleName;
  std::vector<TString> mem_strings;
//   std::vector<TString> sampleNames;
  
//   Long64_t lastRun;

  Long64_t brRun;
  Long64_t brLumi;
  Long64_t brEvent;
  
  Double_t br_p;
  Double_t br_p_sig;
  Double_t br_p_bkg;
  Double_t br_p_err_sig;
  Double_t br_p_err_bkg;
  Double_t br_n_perm_sig;
  Double_t br_n_perm_bkg;
  Double_t br_blr_eth;
  Double_t br_blr_eth_transformed;
  
  Double_t* br_p_vec;

  std::map<Int_t, Long64_t> lookUpMap;
  
  
  //stuff for creating the database
  Long64_t inputRun;
  Long64_t relevantRun;
  
  Long64_t inputMinLumi;
  Long64_t inputMaxLumi;
  Long64_t relevantMinLumi;
  Long64_t relevantMaxLumi;
  
  Long64_t inputMaxEvent;
  Long64_t inputMinEvent;
};



#endif
