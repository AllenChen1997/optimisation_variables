from WMCore.Configuration import Configuration
from CRABClient.UserUtilities import config, getUsernameFromSiteDB

config = Configuration()

config.section_("General")
config.General.requestName = 'NanoParticleNET'
config.General.transferLogs=True
config.section_("JobType")
config.JobType.pluginName = 'Analysis'
config.JobType.psetName = 'PSet.py'
config.JobType.scriptExe = 'crab_ana.sh'
config.JobType.inputFiles = ['FrameworkJobReport.xml','correlated_plot.C','NCU_ana.py'] #hadd nano will not be needed once nano tools are in cmssw
config.JobType.outputFiles = ['histo.root']
config.JobType.sendPythonFolder	 = True
config.JobType.allowUndistributedCMSSW = True
config.section_("Data")
config.Data.inputDataset ='/TT_TuneCUETP8M2T4_PSweights_13TeV-powheg-pythia8/hqu-NanoTuples-ParticleNetV0-12Dec2019_RunIISummer16MiniAODv3-94X_v3-v1-a9438db3cb3ef94689ad6f1f93da896e/USER'
config.Data.inputDBS = 'phys03'
#config.Data.inputDBS = 'global'
config.Data.splitting = 'FileBased'
config.Data.unitsPerJob = 4

#config.Data.splitting = 'Automatic'
#eventsPerLumi = 100
#timePerEv = 4
#desiredTime = 60*60*100
#config.Data.unitsPerJob = desiredTime / (eventsPerLumi*timePerEv)



#config.Data.outLFNDirBase = '/store/user/%s/NanoPost' % (getUsernameFromSiteDB())
config.Data.publication = False
config.Data.outputDatasetTag = 'NanoTestPost'
config.section_("Site")
#config.Site.storageSite = "T2_CH_CERN"
config.Site.storageSite = "T3_TW_NCU"

#config.Site.storageSite = "T2_TW_NCHC"
#config.section_("User")
#config.User.voGroup = 'dcms'

