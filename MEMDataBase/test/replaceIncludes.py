import sys
import os
import glob

print os.getcwd()
cwd=os.getcwd()
print cwd.replace("/MEMDataBase/test","")


filelist=[]

filelist+=glob.glob("../src/*.cc")
filelist+=glob.glob("../interface/*.h")
print filelist

for fn in filelist:
  newlist=[]
  f=open(fn,"r")
  oldlist=list(f)
  for line in oldlist:
    newline=line
    if "#include \"MEMDataBase/MEMDataBase/" in line:
      newline=line.replace("#include \"MEMDataBase/", "#include \""+cwd.replace("/MEMDataBase/test","")+"/")
      print newline
    newlist.append(newline)

  f.close()
  newfile=open(fn,"w")
  for line in newlist:
    newfile.write(line)
  newfile.close()

