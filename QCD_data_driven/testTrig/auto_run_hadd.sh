#!bin/bash
inputdir="/eos/user/k/kuchen/Trig_h_withHT_trigEff/"
if [ ! -z $1 ]; then
	inputdir=$1
fi
hadd JetHT-Run2017B_output.root ${inputdir}/JetHT-Run2017B*
hadd JetHT-Run2017C_output.root ${inputdir}/JetHT-Run2017C*
hadd JetHT-Run2017D_output.root ${inputdir}/JetHT-Run2017D*
hadd JetHT-Run2017E_output.root ${inputdir}/JetHT-Run2017E*
hadd JetHT-Run2017F_output.root ${inputdir}/JetHT-Run2017F*
ls J*.root > testTrigOutputList.txt
