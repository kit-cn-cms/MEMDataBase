#
# example how to retrieve information from the database
#
from jet_corrections import *
import ROOT

CvectorTString = getattr(ROOT, "std::vector<TString>")

#load library
ROOT.gSystem.Load("libMEMDataBaseMEMDataBase.so")

mem_strings=["mem_p"]
mem_strings+=["mem_"+corr+ud+"_p" for corr in jet_corrections for ud in ["up","down"]]
mem_strings_vec = CvectorTString()
#print mem_strings
# fill the string in a vector to pass to the database
for mem_string in mem_strings:
    mem_strings_vec.push_back(ROOT.TString(mem_string))

# initialize with path to database
myDataBase=ROOT.MEMDataBase("/nfs/dust/cms/user/swieland/ttH_legacy/MEMdatabase/databases/2017_/",mem_strings_vec)

# load sample by identifier
# The second argument defaults to samplename_index.txt
# this text file simply holds a list of database files, nothing to concern you with
# myDataBase.AddSample("SingleElectron","SingleElectron_index.txt")
myDataBase.AddSample("ZZTuneCP513TeVpythia8","ZZTuneCP513TeVpythia8_index.txt.txt")

#print structure
myDataBase.PrintStructure()

run=276242
lumi=1074
event=7269012
# retrive MEM for this event
# return an object of class DataBaseMEMResult
# myDataBase.GetMEMResult( samplename, run, lumi, event )
result=myDataBase.GetMEMResult("ZZTuneCP513TeVpythia8", run, lumi, event)
# result=myDataBase.GetMEMResult("SingleElectron", run, lumi, event)

print "run lumi event, MEM p", run, lumi, event, result.p_vec[0]
print "MEM p_sig", result.p_sig
print "MEM p_bkg", result.p_bkg
print "MEM p_err_sig",  result.p_err_sig
print "MEM p_err_bkg",  result.p_err_bkg
print "MEM n_perm_sig",  result.n_perm_sig
print "MEM n_perm_bkg",  result.n_perm_bkg
#
