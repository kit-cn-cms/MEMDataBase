
#include "/nfs/dust/cms/user/swieland/ttH_legacy/MEMdatabase/CodeforScriptGenerator/MEMDataBase/MEMDataBase/interface/DataBaseRunCollection.h"


//run collection members

RunCollection::RunCollection(const Long64_t runNumber_){
 runNumber=runNumber_; 
}

RunCollection::~RunCollection(){}


void RunCollection::AddLumiAndEventInterval(const Long64_t minLumi_, const Long64_t maxLumi_,const Long64_t minEvent_, const Long64_t maxEvent_){
  bool elementExists=false;
  for(auto & intvl : lumiCollections){
    if(intvl.GetMinLumi()==minLumi_ and intvl.GetMaxLumi()==maxLumi_){
      intvl.AddEventInterval(minEvent_,maxEvent_);
      elementExists=true;
      break;
    }
  }
  if(!elementExists){
   lumiCollections.push_back(LumiCollection(minLumi_, maxLumi_));
   lumiCollections.back().AddEventInterval(minEvent_,maxEvent_);
   std::sort(lumiCollections.begin(), lumiCollections.end(), LumiCollection::minIsSmaller );
  } 
}

bool RunCollection::RemoveEventInterval(const Long64_t minLumi_, const Long64_t maxLumi_,const Long64_t minEvent_, const Long64_t maxEvent_){
  bool removed = false;
  for(auto & intvl : lumiCollections){
    if(intvl.GetMinLumi()==minLumi_ and intvl.GetMaxLumi()==maxLumi_){
      removed=intvl.RemoveEventInterval(minEvent_,maxEvent_);
      break;
    }
  }
  return removed;
}

void RunCollection::PrintStructure(){
  std::cout<<"run "<<runNumber<<std::endl;
  for(auto & intvl : lumiCollections){
    intvl.PrintStructure(); 
  }
  
}


TString RunCollection::GetLumiAndEventIntervalString(const Long64_t lumiSection, const Long64_t eventNumber){
   TString lumistring=""; 
  
   for(auto & intvl : lumiCollections){
//      std::cout<<lumiSection<<" "<<intvl.GetMinLumi()<<" "<<intvl.GetMaxLumi()<<std::endl;
     if(lumiSection<=intvl.GetMaxLumi() and lumiSection>=intvl.GetMinLumi()){
//        std::cout<<"found lumi"<<std::endl;
       lumistring+=TString::LLtoa(intvl.GetMinLumi(),10);
       lumistring+="_";
       lumistring+=TString::LLtoa(intvl.GetMaxLumi(),10);
       lumistring+="_";
//        std::cout<<lumistring<<std::endl;
       TString substring = intvl.GetEventIntervalString(eventNumber);
       if(substring!=""){
	 lumistring+=substring;
       }
       else{
         lumistring="";
       }
       break;
     }
   }
  
//   std::cout<<"lumistring "<<lumistring<<std::endl;
  return lumistring;
}

bool RunCollection::GetRelevantLumi(const Long64_t lumiSection, Long64_t* minlumi, Long64_t* maxlumi){
   bool relevantLumiIntervalExists=false;
   for(auto & intvl : lumiCollections){
     if(lumiSection<=intvl.GetMaxLumi() and lumiSection>=intvl.GetMinLumi()){
       *minlumi=intvl.GetMinLumi();
       *maxlumi=intvl.GetMaxLumi();
       relevantLumiIntervalExists=true;
       break;
     }
   }
  return relevantLumiIntervalExists;
}
