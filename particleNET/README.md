# submit crab job

	cmsrel CMSSW_11_0_0_pre11
	cd CMSSW_11_0_0_pre11/src
	cmsenv
	git cms-init
	git clone https://github.com/cms-nanoAOD/nanoAOD-tools.git PhysicsTools/NanoAODTools
	scram b
	git clone https://github.com/AllenChen1997/optimisation_variables.git
	cp optimisation_variables/particleNET/* PhysicsTools/NanoAODTools/crab/
	cd PhysicsTools/NanoAODTools/crab/
	source /cvmfs/cms.cern.ch/crab3/crab_standalone.sh
	voms-proxy-init --voms cms
	crab submit crab_cfg.py