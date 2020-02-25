#include "/nfs/dust/cms/user/swieland/ttH_legacy/MEMdatabase/CodeforScriptGenerator/MEMDataBase/MEMDataBase/interface/DataBaseSample.h"



DataBaseSample::DataBaseSample(const TString sampleName_, const TString dataBaseDirectory_,const std::vector<TString> mem_strings_ ,const TString indexFile_){
  
  dataBaseDirectory=dataBaseDirectory_;
  sampleName=sampleName_;
  mem_strings=mem_strings_;
  br_p_vec = new Double_t[mem_strings.size()];
  
  if(indexFile_==""){indexFile=sampleName+TString("_index.txt"); std::cout<<indexFile<<std::endl; }
  else {
    indexFile=indexFile_;
  }
    std::ifstream idfilestream(dataBaseDirectory_+TString("/")+indexFile);
    TString dumpline;
     
//     bool readline=true;
    while(true){
//       std::cout<<"bla"<<std::endl;
      idfilestream>>dumpline;
      if(idfilestream.eof())break;

//       std::cout<<dumpline<<std::endl;
      FileNames.push_back(dumpline);
      TObjArray *obja=dumpline.Tokenize("_");
      Int_t arraylength=obja->GetEntries();
      
      TString thisrun=((TObjString*)(obja->At(arraylength-6)))->String();
      TString thisminlumi=((TObjString*)(obja->At(arraylength-5)))->String();
      TString thismaxlumi=((TObjString*)(obja->At(arraylength-4)))->String();
      TString thisminevent=((TObjString*)(obja->At(arraylength-3)))->String();
      TString thismaxevent=((TObjString*)(obja->At(arraylength-2)))->String();
      
//       std::cout<<thisrun<<" "<<thisminlumi<<" "<<thismaxlumi<<" "<<thisminevent<<" "<<thismaxevent<<std::endl;
      AddRunLumiEventCollection(thisrun.Atoll(), thisminlumi.Atoll(), thismaxlumi.Atoll(), thisminevent.Atoll(),thismaxevent.Atoll());
     }
   idfilestream.close();
   std::cout<<"Index file read: "<<indexFile<<std::endl; 
//   }
  
//   std::cout<<"empty Index file created: "<<indexFile<<std::endl; 
  
  currentOpenFileName="";
  openMode="READ";
}

void DataBaseSample::AddRunLumiEventCollection(const Long64_t runNumber_, const Long64_t minLumi_, const Long64_t maxLumi_,const Long64_t minEvent_, const Long64_t maxEvent_){
  bool elementExists=false;
  for(auto & irun : runCollection){
    if(irun.GetRunNumber()==runNumber_){
      irun.AddLumiAndEventInterval(minLumi_,maxLumi_,minEvent_,maxEvent_);
      elementExists=true;
      break;
    }
  }
  if(!elementExists){
   runCollection.push_back(RunCollection(runNumber_));
   runCollection.back().AddLumiAndEventInterval(minLumi_,maxLumi_,minEvent_,maxEvent_);
  }
}

bool DataBaseSample::RemoveEventCollection(const Long64_t runNumber_, const Long64_t minLumi_, const Long64_t maxLumi_,const Long64_t minEvent_, const Long64_t maxEvent_){
  bool removed=false;
  for(auto & irun : runCollection){
    if(irun.GetRunNumber()==runNumber_){
      removed=irun.RemoveEventInterval(minLumi_,maxLumi_,minEvent_,maxEvent_);
      break;
    }
  }
  return removed;
}

TString DataBaseSample::GetFileNameForEvent(const Long64_t runNumber, const Long64_t lumiSection, const Long64_t eventNumber){
  TString relevantFileName=sampleName+"_";
  
//   RunCollection* thisRunCollection=nullptr;
  
  for(auto & irun : runCollection){
    
    if(irun.GetRunNumber()==runNumber){
      relevantFileName+=TString::LLtoa(runNumber,10)+"_";
      TString substring=irun.GetLumiAndEventIntervalString(lumiSection,eventNumber);
      if(substring!=""){ 
	relevantFileName+=substring;
	relevantFileName+="_.root";
      }
      else{
	relevantFileName="";
      }
      
      break;    
    }
  }
  
  // std::cout<<" Event with "<<runNumber<<" "<<lumiSection<<" "<<eventNumber<<" is in "<<relevantFileName<<std::endl;
  return relevantFileName;
  
}

DataBaseMEMResult DataBaseSample::GetMEMResult(const Long64_t runNumber, const Long64_t lumiSection, const Long64_t eventNumber){
  TString relevantFileName=GetFileNameForEvent(runNumber,lumiSection,eventNumber);
  
  DataBaseMEMResult thisMEM(mem_strings);

  if(relevantFileName=="" || relevantFileName==sampleName+"_"){
//     std::cout<<"did not find event in database"<<std::endl;
    return thisMEM;
  }
  
  if(relevantFileName!=currentOpenFileName){OpenTree(relevantFileName);}
  
  
  //getStart ievt
  
  Int_t startEvt=0;
  for(auto const & it : lookUpMap){
//     std::cout<<it.second<<" "<<it.first<<std::endl;
    if(it.second>=eventNumber){
      break;
    }
    else{ startEvt=it.first;}
  }
  
//   std::cout<<"starting at "<<startEvt<<std::endl;
  
  bool found=false;
//   for(Int_t ievt=startEvt; ievt<startEvt+LookUpStepSize+10; ievt++){
  for(Int_t ievt=startEvt; ievt<maxEventsPerTree; ievt++){
//     if(ievt==startEvt+LookUpStepSize){std::cout<<" -------------------------"<<std::endl;}
    currentOpenTree->GetEntry(ievt);
//     std::cout<<ievt<<" "<<brEvent<<" "<<brLumi<<" "<<brRun<<std::endl;
    if(brEvent==eventNumber and brLumi==lumiSection and brRun==runNumber){
//         std::cout<<"FOUND "<<ievt<<" "<<brEvent<<" "<<brLumi<<" "<<brRun<<std::endl;
      found=true;
      break;
    }
   if(brEvent>eventNumber){break;} 
  }
  
  if(found){
    thisMEM.p=br_p;
    thisMEM.p_sig=br_p_sig;
    thisMEM.p_bkg=br_p_bkg;
    thisMEM.p_err_sig=br_p_err_sig;
    thisMEM.p_err_bkg=br_p_err_bkg;
    thisMEM.n_perm_sig=br_n_perm_sig;
    thisMEM.n_perm_bkg=br_n_perm_bkg;
    thisMEM.blr_eth=br_blr_eth;
    thisMEM.blr_eth_transformed=br_blr_eth_transformed;
    for(uint i=0;i<mem_strings.size();i++) thisMEM.p_vec[i]=br_p_vec[i];
    
//     std::cout<<"FOUND p= "<<thisMEM.p<<" for "<<runNumber<<" "<<lumiSection<<" "<<eventNumber<<std::endl;  
  }
  else{
//     std::cout<<"NO ENTRY FOR: "<<runNumber<<" "<<lumiSection<<" "<<eventNumber<<std::endl;  
  }
    
  return thisMEM;
}

bool DataBaseSample::CloseTree(){
  if(currentOpenFileName=="" or currentOpenTreeFile==nullptr or currentOpenTree==nullptr){
//     std::cout<<"nothing to do"<<std::endl;
    return true;
  }
  
//   std::cout<<"closing"<<std::endl;
//      std::cout<<currentOpenFileName<<" "<<currentOpenTree<<" "<<currentOpenTreeFile<<std::endl;

  if(openMode=="UPDATE"){
//        std::cout<<currentOpenFileName<<" "<<currentOpenTree<<" "<<currentOpenTreeFile<<std::endl;

//     std::cout<<"sorting"<<std::endl;
//     std::cout<<currentOpenTree<<std::endl;
//     std::cout<<currentOpenTree->GetTitle()<<std::endl;
//     std::cout<<currentOpenTree->GetEntries()<<std::endl;

    Int_t nentries = currentOpenTree->GetEntries();
//     std::cout<<nentries<<std::endl;
    currentOpenTree->Draw("event","","goff");
    Int_t *myid = new Int_t[nentries];
    std::cout<<"starting sorting"<<std::endl;
    TMath::Sort(nentries,currentOpenTree->GetV1(),myid,false);    
    TFile* newtreefile = new TFile(dataBaseDirectory+"/"+currentOpenFileName+"_buffer","RECREATE");
    TTree *tsorted = (TTree*)currentOpenTree->CloneTree(0);
    std::cout<<"filling sorted events"<<std::endl;
    for (Int_t i=0;i<nentries;i++) {
      currentOpenTree->GetEntry(myid[i]);
      tsorted->Fill();
    }
    std::cout<<"DONE filling sorted events"<<std::endl;
    
    tsorted->Write();
    delete [] myid;
    
    newtreefile->Close();
    delete newtreefile;
  }
  currentOpenTreeFile->Close();
  delete currentOpenTreeFile;
  
  if(openMode=="UPDATE"){
    TString oldname=dataBaseDirectory+TString("/")+currentOpenFileName;
    TString newname=dataBaseDirectory+TString("/")+currentOpenFileName+TString("_buffer");
//     std::cout<<"names "<<oldname<<" "<<newname<<std::endl;
    remove(oldname);
    rename(newname, oldname);
  }
    
  currentOpenFileName="";
  currentOpenTreeFile=nullptr;
  currentOpenTree=nullptr;
  return true;
  
}

bool DataBaseSample::OpenTree(TString filename){
//   std::cout<<"about to open "<<filename<<std::endl;
  //std::cout << "test 6 " << std::endl;
  CloseTree();
  //std::cout << "test 7 " << std::endl;
  currentOpenFileName=filename;  
  currentOpenTreeFile = new TFile(dataBaseDirectory+"/"+filename, openMode);
//   std::cout<<currentOpenFileName<<" "<<currentOpenTreeFile<<std::endl;
  currentOpenTree=(TTree*)currentOpenTreeFile->Get("MVATree");
  //std::cout << "test 8 " << std::endl;
//   std::cout<<currentOpenTree<<std::endl;
//   std::cout<<currentOpenTree->GetEntries()<<std::endl;
  currentOpenTree->SetBranchAddress("run",&brRun);
  //std::cout << "test 9 " << std::endl;
  currentOpenTree->SetBranchAddress("lumi",&brLumi);
  currentOpenTree->SetBranchAddress("event",&brEvent);
  currentOpenTree->SetBranchAddress("mem_p_orig",&br_p);
  currentOpenTree->SetBranchAddress("mem_p_sig",&br_p_sig);
  currentOpenTree->SetBranchAddress("mem_p_bkg",&br_p_bkg);
  currentOpenTree->SetBranchAddress("mem_p_err_sig",&br_p_err_sig);
  currentOpenTree->SetBranchAddress("mem_p_err_bkg",&br_p_err_bkg);
  currentOpenTree->SetBranchAddress("mem_n_perm_sig",&br_n_perm_sig);
  currentOpenTree->SetBranchAddress("mem_n_perm_bkg",&br_n_perm_bkg);
  currentOpenTree->SetBranchAddress("blr_eth",&br_blr_eth);
  currentOpenTree->SetBranchAddress("blr_eth_transformed",&br_blr_eth_transformed);
  for(uint i=0;i<mem_strings.size();i++) {
    currentOpenTree->SetBranchAddress(mem_strings[i],&br_p_vec[i]);
  }

//   std::cout<<"opened tree "<<dataBaseDirectory<<"/"<<filename<<std::endl;
  lookUpMap.clear();
  
  currentTreeEntries=currentOpenTree->GetEntries();
  for(Int_t ievt=0; ievt<currentTreeEntries; ievt++){
    if(ievt%LookUpStepSize==0){
      currentOpenTree->GetEntry(ievt);
      Long64_t testEventNumber=brEvent;
      
//       std::cout<<"set up looUp table "<<ievt<<" "<<testEventNumber<<std::endl;
      lookUpMap[ievt]=testEventNumber;
    }
  }
  
  return true;
}

void DataBaseSample::PrintStructure(){
  std::cout<<"-------------------------------------"<<std::endl;
  std::cout<<"sample name "<<sampleName<<std::endl;
  std::cout<<"sample dataBaseDirectory "<<dataBaseDirectory<<std::endl;
  std::cout<<"sample index file "<<indexFile<<std::endl;
  
  for(auto & irun : runCollection){
    irun.PrintStructure();    
    
  }
}

bool DataBaseSample::SaveDataBase(){
  if(openMode=="UPDATE"){
   std::ofstream idfilestream(dataBaseDirectory+TString("/")+indexFile);
   for(unsigned int ib=0; ib<FileNames.size(); ib++){
     idfilestream<<FileNames.at(ib);
     idfilestream<<"\n";
   }
   idfilestream.close();
   std::cout<<"index file written"<<std::endl;
   }
   
   CloseTree();
   
   return true;
}

DataBaseSample::~DataBaseSample(){
//    if(currentOpenTreeFile!=nullptr){
//     currentOpenTreeFile->Close();
//     delete currentOpenTreeFile;
//    }  
  
//    std::cout<<"destructor called"<<std::endl;
   
   CloseTree();
   delete[] br_p_vec;
//    std::cout<<"destructor succeeded"<<std::endl;

}


bool DataBaseSample::AddEvent(Long64_t runNumber, Long64_t lumiSection, Long64_t eventNumber, Double_t p, Double_t p_sig, Double_t p_bkg, Double_t p_err_sig, Double_t p_err_bkg, Double_t n_perm_sig, Double_t n_perm_bkg){
  TString relevantFileName=GetFileNameForEvent(runNumber,lumiSection,eventNumber);
//   std::cout<<eventNumber<<" "<<relevantFileName<<std::endl;
  
  if(relevantFileName=="" or relevantFileName==sampleName+"_"){
//     std::cout<<"collection for event does not exist yes. Creating it"<<std::endl;
    //get ranges for lumiinterval
    Long64_t maxRangeForLumi=100000;
    Long64_t lowerlumival=lumiSection-lumiSection%maxRangeForLumi;
    Long64_t upperlumival=lowerlumival+maxRangeForLumi-1;
    AddRunLumiEventCollection(runNumber, lowerlumival, upperlumival, -99999999999,99999999999);

    
    relevantFileName=GetFileNameForEvent(runNumber,lumiSection,eventNumber);
    CreateNewTree(relevantFileName);
  }
  if(relevantFileName!=""){
    if(relevantFileName!=currentOpenFileName){openMode="UPDATE"; OpenTree(relevantFileName);}
    AddEventToTree(runNumber, lumiSection, eventNumber, p, p_sig, p_bkg, p_err_sig, p_err_bkg,n_perm_sig,n_perm_bkg);   
  }

//   CloseTree();
  return true; 
}

bool DataBaseSample::AddEvent(Long64_t runNumber, Long64_t lumiSection, Long64_t eventNumber, std::vector<Double_t> p_vec, Double_t p_sig, Double_t p_bkg, Double_t blr_4b, Double_t blr_2b){
  //br_p_vec.clear();
  //std::cout << "test 1 " << std::endl;
  TString relevantFileName=GetFileNameForEvent(runNumber,lumiSection,eventNumber);
//   std::cout<<eventNumber<<" "<<relevantFileName<<std::endl;
  
  if(relevantFileName=="" or relevantFileName==sampleName+"_"){
//     std::cout<<"collection for event does not exist yes. Creating it"<<std::endl;
    //get ranges for lumiinterval
    //std::cout << "test 2 " << std::endl;
    Long64_t maxRangeForLumi=100000;
    Long64_t lowerlumival=lumiSection-lumiSection%maxRangeForLumi;
    Long64_t upperlumival=lowerlumival+maxRangeForLumi-1;
    
    AddRunLumiEventCollection(runNumber, lowerlumival, upperlumival, -99999999999,99999999999);

    
    relevantFileName=GetFileNameForEvent(runNumber,lumiSection,eventNumber);
    CreateNewTree(relevantFileName);
  }
  if(relevantFileName!=""){
    //std::cout << "test 3 " << std::endl;
    if(relevantFileName!=currentOpenFileName){openMode="UPDATE"; OpenTree(relevantFileName);}
    //std::cout << "test 4 " << std::endl;
    AddEventToTree(runNumber, lumiSection, eventNumber, p_vec, p_sig, p_bkg, blr_4b, blr_2b);   
    //std::cout << "test 5 " << std::endl;
  }

//   CloseTree();
  return true; 
}

void DataBaseSample::AddEventToTree(Long64_t runNumber, Long64_t lumiSection, Long64_t eventNumber, Double_t p, Double_t p_sig, Double_t p_bkg, Double_t p_err_sig, Double_t p_err_bkg, Double_t n_perm_sig, Double_t n_perm_bkg){
    
   brRun=runNumber;
   brLumi=lumiSection;
   brEvent=eventNumber;
  
   br_p=p;
   br_p_sig=p_sig;
   br_p_bkg=p_bkg;
   br_p_err_sig=p_err_sig;
   br_p_err_bkg=p_err_bkg;
   br_n_perm_sig=n_perm_sig;
   br_n_perm_bkg=n_perm_bkg;
   
   currentOpenTree->Fill();
//    std::cout<<"filled "<<eventNumber<<std::endl;

   currentTreeEntries=currentOpenTree->GetEntries();
//    std::cout<<currentOpenTree<<std::endl;
//    std::cout<<currentOpenTree->GetEntries()<<std::endl;
   if(currentTreeEntries>=maxEventsPerTree){
     SplitCollection(currentOpenFileName);
     
   }


}

void DataBaseSample::AddEventToTree(Long64_t runNumber, Long64_t lumiSection, Long64_t eventNumber, std::vector<Double_t> p_vec, Double_t p_sig, Double_t p_bkg, Double_t blr_4b, Double_t blr_2b){
    
   brRun=runNumber;
   brLumi=lumiSection;
   brEvent=eventNumber;
  
   br_p=-1.;
   //br_p_vec=p_vec;
   for(uint i=0;i<p_vec.size();i++) br_p_vec[i]=p_vec[i];
   if(blr_4b!=0.&&blr_2b!=0.) {
       br_blr_eth=blr_4b/(blr_4b+blr_2b);
   }
   else {
       br_blr_eth=0.;
   }
   if(br_blr_eth!=0.) {
       br_blr_eth_transformed=log(br_blr_eth/(1-br_blr_eth));
   }
   else {
       br_blr_eth_transformed=0.;
   }
   br_p_sig=p_sig;
   br_p_bkg=p_bkg;
   br_p_err_sig=-1.;
   br_p_err_bkg=-1.;
   br_n_perm_sig=-1.;
   br_n_perm_bkg=-1.;
   //std::cout << "test 10" << std::endl;
   currentOpenTree->Fill();
   //std::cout << "test 11" << std::endl;
//    std::cout<<"filled "<<eventNumber<<std::endl;

   currentTreeEntries=currentOpenTree->GetEntries();
//    std::cout<<currentOpenTree<<std::endl;
//    std::cout<<currentOpenTree->GetEntries()<<std::endl;
   if(currentTreeEntries>=maxEventsPerTree){
     SplitCollection(currentOpenFileName);
     
   }


}


bool DataBaseSample::CreateNewTree(TString filename){
//   TString newname=dataBaseDirectory+TString("/")+filename;
//   remove(newname);
  TFile* newtreefile= new TFile(dataBaseDirectory+"/"+filename,"RECREATE");

  TTree* newtree = new TTree("MVATree","MVATree");  
  newtree->Branch("run",&brRun,"run/L");
  newtree->Branch("lumi",&brLumi,"lumi/L");
  newtree->Branch("event",&brEvent,"event/L");
  newtree->Branch("mem_p_orig",&br_p,"p/D");
  newtree->Branch("mem_p_sig",&br_p_sig,"p_sig/D");
  newtree->Branch("mem_p_bkg",&br_p_bkg,"p_bkg/D");
  newtree->Branch("mem_p_err_sig",&br_p_err_sig,"p_err_sig/D");
  newtree->Branch("mem_p_err_bkg",&br_p_err_bkg,"p_err_bkg/D");
  newtree->Branch("mem_n_perm_sig",&br_n_perm_sig,"n_perm_sig/D");
  newtree->Branch("mem_n_perm_bkg",&br_n_perm_bkg,"n_perm_bkg/D");
  newtree->Branch("blr_eth",&br_blr_eth,"blr_eth/D");
  newtree->Branch("blr_eth_transformed",&br_blr_eth_transformed,"blr_eth_transformed/D");
  for(uint i=0;i<mem_strings.size();i++) {
    newtree->Branch(mem_strings[i],&br_p_vec[i],mem_strings[i]+"/D");
  }
  newtree->AutoSave();
  newtreefile->Close();
  delete newtreefile;
  
//   std::cout<<"created new Tree "<<filename<<std::endl;
  FileNames.push_back(filename);
  return true;
}


bool DataBaseSample::SplitCollection(TString oldfilename){
  
  CloseTree();
  openMode="UPDATE";
  OpenTree(oldfilename);
  
  Int_t oldentries=currentOpenTree->GetEntries();
  
  //get splitting point
  Long64_t midevent;
  currentOpenTree->GetEntry(int(oldentries/2));
//   TString oldfilename=currentOpenFileName;
  midevent=brEvent;
  
  std::cout<<"splitting tree "<<oldfilename<<" at "<<midevent<<std::endl;
  
  TObjArray *obja=oldfilename.Tokenize("_");
  Int_t arraylength=obja->GetEntries();

  
  TString thissamplename=sampleName;
  TString thisrun=((TObjString*)(obja->At(arraylength-6)))->String();
  TString thisminlumi=((TObjString*)(obja->At(arraylength-5)))->String();
  TString thismaxlumi=((TObjString*)(obja->At(arraylength-4)))->String();
  TString thisminevent=((TObjString*)(obja->At(arraylength-3)))->String();
  TString thismaxevent=((TObjString*)(obja->At(arraylength-2)))->String();
  
//   AddRunLumiEventCollection(thisrun.Atoll(), thisminlumi.Atoll(), thismaxlumi.Atoll(), thisminevent.Atoll(),thismaxevent.Atoll());
  TString newfilenameDown=thissamplename+TString("_")+thisrun+TString("_")+thisminlumi+TString("_")+thismaxlumi+TString("_")+thisminevent+TString("_")+TString::LLtoa(midevent,10)+TString("_.root");
  TString newfilenameUp=thissamplename+TString("_")+thisrun+TString("_")+thisminlumi+TString("_")+thismaxlumi+TString("_")+TString::LLtoa(midevent+1,10)+TString("_")+thismaxevent+TString("_.root");
  std::cout<<"into "<<newfilenameDown<<" and "<<newfilenameUp<<std::endl;
  
  bool removedCollection=RemoveEventCollection(thisrun.Atoll(), thisminlumi.Atoll(), thismaxlumi.Atoll(), thisminevent.Atoll(),thismaxevent.Atoll());
  if(removedCollection and false){std::cout<<"removed event interval"<<std::endl;}
  AddRunLumiEventCollection(thisrun.Atoll(), thisminlumi.Atoll(), thismaxlumi.Atoll(), thisminevent.Atoll(),midevent);
  AddRunLumiEventCollection(thisrun.Atoll(), thisminlumi.Atoll(), thismaxlumi.Atoll(), midevent+1,thismaxevent.Atoll());

  for(unsigned int fni=0; fni<FileNames.size(); fni++){
    if(FileNames.at(fni)==oldfilename){
      FileNames.erase(FileNames.begin()+fni);
//       std::cout<<"erased filename from list"<<std::endl;
      break;
    } 
  }
  FileNames.push_back(newfilenameDown);
  FileNames.push_back(newfilenameUp);
  
  TFile* newFileUp = new TFile(dataBaseDirectory+TString("/")+newfilenameUp,"RECREATE");
  TTree* newTreeUp=(TTree*)currentOpenTree->CloneTree(0);
  TFile* newFileDown = new TFile(dataBaseDirectory+TString("/")+newfilenameDown,"RECREATE");
  TTree* newTreeDown=(TTree*)currentOpenTree->CloneTree(0);
  
  for(int ievt=0; ievt<oldentries; ievt++){
    currentOpenTree->GetEntry(ievt);
    if(brEvent<=midevent){
//       std::cout<<brEvent<<" down"<<std::endl;
      newTreeDown->Fill(); 
    }
    else{
//       std::cout<<brEvent<<" up"<<std::endl;
      newTreeUp->Fill();
    }
  }
  
  newFileDown->cd();
  newTreeDown->Write();
  newFileDown->Close();
  newFileUp->cd();
  newTreeUp->Write();
  newFileUp->Close();
  
  CloseTree();
  
  delete newFileDown;
  delete newFileUp;
  delete obja;
  
  TString oldfn=dataBaseDirectory+TString("/")+oldfilename;
  remove(oldfn);
  
  std::cout<<"splitting done"<<std::endl;
  return true;
}
