#include "/nfs/dust/cms/user/swieland/ttH_legacy/MEMdatabase/CodeforScriptGenerator/MEMDataBase/MEMDataBase/interface/DataBaseMEMResult.h"

DataBaseMEMResult::DataBaseMEMResult(){
  p=-99;
  p_sig=-99;
  p_bkg=-99;
  p_err_sig=-99;
  p_err_bkg=-99;
  n_perm_sig=-99;
  n_perm_bkg=-99;
  blr_eth=-99.;
  blr_eth_transformed=-99.;
  
}

DataBaseMEMResult::DataBaseMEMResult(const std::vector<TString>& mem_strings_){
  p=-99;
  p_sig=-99;
  p_bkg=-99;
  p_err_sig=-99;
  p_err_bkg=-99;
  n_perm_sig=-99;
  n_perm_bkg=-99;
  blr_eth=-99.;
  blr_eth_transformed=-99.;
  p_vec=std::vector<double>(mem_strings_.size(),-99.);
}

DataBaseMEMResult::~DataBaseMEMResult(){}
