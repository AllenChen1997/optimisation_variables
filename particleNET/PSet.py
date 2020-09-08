#this fake PSET is needed for local test and for crab to figure the output filename
#you do not need to edit it unless you want to do a local test using a different input file than
#the one marked below
import FWCore.ParameterSet.Config as cms
process = cms.Process('NANO')
process.source = cms.Source("PoolSource", fileNames = cms.untracked.vstring(),
#	lumisToProcess=cms.untracked.VLuminosityBlockRange("254231:1-254231:24")
)
process.source.fileNames = [
	'root://cms-xrd-global.cern.ch//store/group/lpcjme/noreplica/NanoTuples/2016-ParticleNet-V00/mc/TT_TuneCUETP8M2T4_PSweights_13TeV-powheg-pythia8/NanoTuples-ParticleNetV0-12Dec2019_RunIISummer16MiniAODv3-94X_v3-v1/200129_033048/0000/nano_459.root' ##you can change only this line
]
process.maxEvents = cms.untracked.PSet(input = cms.untracked.int32(10))
#process.output = cms.OutputModule("PoolOutputModule", fileName = cms.untracked.string('tree.root'))
#process.out = cms.EndPath(process.output)

