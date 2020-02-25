#include "/nfs/dust/cms/user/swieland/ttH_legacy/MEMdatabase/CodeforScriptGenerator/MEMDataBase/MEMDataBase/interface/DataBaseLumiCollection.h"

LumiCollection::LumiCollection(){
  lumiInterval=DataBaseInterval(0,0);
}

LumiCollection::~LumiCollection(){}

LumiCollection::LumiCollection(Long64_t minLumi_, Long64_t maxLumi_){
  lumiInterval=DataBaseInterval(minLumi_,maxLumi_);
}

void LumiCollection::AddEventInterval(const Long64_t minEvent_, const Long64_t maxEvent_){
  eventCollections.push_back(DataBaseInterval(minEvent_,maxEvent_));
  std::sort(eventCollections.begin(), eventCollections.end(), DataBaseInterval::minIsSmaller );
}

bool LumiCollection::RemoveEventInterval(const Long64_t minEvent_, const Long64_t maxEvent_){
  bool removed=false;
  int id=-1;
  
  for(unsigned int i=0; i<eventCollections.size();i++){
    if(eventCollections.at(i).GetMinVal()==minEvent_ and eventCollections.at(i).GetMaxVal()==maxEvent_ ){
     id=i;
     break;
    }
  }
  
  if(id>=0){
    eventCollections.erase(eventCollections.begin()+id);
    removed=true;
  }
  
  std::sort(eventCollections.begin(), eventCollections.end(), DataBaseInterval::minIsSmaller );
  return removed;
}

void LumiCollection::PrintStructure(){
  std::cout<<"  Lumi "<<lumiInterval.GetMinVal()<<" "<<lumiInterval.GetMaxVal()<<std::endl;
  for(auto & ievt : eventCollections){
    std::cout<<"    Event "<<ievt.GetMinVal()<<" "<<ievt.GetMaxVal()<<std::endl;
  }
}

TString LumiCollection::GetEventIntervalString(const Long64_t eventNumber){
  TString eventstring="";
  
  for(auto & ievt : eventCollections){
    if(ievt.ValueIsInInterval(eventNumber)){
      eventstring+=TString::LLtoa(ievt.GetMinVal(),10);
      eventstring+="_";
      eventstring+=TString::LLtoa(ievt.GetMaxVal(),10);
      break;
      
    }
   }
//   std::cout<<"eventstring "<<eventstring<<std::endl;
  return eventstring;
   
}

