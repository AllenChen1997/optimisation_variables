#!/usr/bin/env python
import os
import sys
from PhysicsTools.NanoAODTools.postprocessing.framework.postprocessor import * 

#this takes care of converting the input files from CRAB
from PhysicsTools.NanoAODTools.postprocessing.framework.crabhelper import inputFiles,runsAndLumis


myFiles = inputFiles()
outF = open("myOutFile.txt", "w")
for line in myFiles:
    line.replace('cms.untracked.vstring(\'','')
    line.replace('\')','')
    outF.write(line)
    outF.write("\n")
outF.close()

myInputFile = "myOutFile.txt"
print "PRINTING myOutFile.txt"
with open("myOutFile.txt") as f:
    print f.read()

JobNumber=sys.argv[1]
#histoFileName = "histo_"+str(JobNumber)+".root"
histoFileName = "histo.root"
print histoFileName

os.system("root -q -b 'correlated_plot.C++(\""+myInputFile+"\",\""+histoFileName+"\")'")


#from  PhysicsTools.NanoAODTools.postprocessing.examples.exampleModule import *
#p=PostProcessor(".",inputFiles(),"Jet_pt>200",modules=[exampleModuleConstr()],provenance=True,fwkJobReport=True,jsonInput=runsAndLumis())
#p.run()


print "DONE"

