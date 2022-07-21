#!bin/bash
dir="/eos/user/k/kuchen/getR_MC/"
if [ ! -z $1 ]; then
	dir=$1
fi
hadd QCD_2000toInf.root $dir/QCD_HT2000toInf*.root
hadd QCD_1500to2000.root $dir/QCD_HT1500to2000*.root
hadd QCD_1000to1500.root $dir/QCD_HT1000to1500*.root
hadd QCD_700to1000.root $dir/QCD_HT700to1000*.root
hadd QCD_500to700.root $dir/QCD_HT500to700*.root
hadd QCD_300to500.root $dir/QCD_HT300to500*.root
hadd QCD_200to300.root $dir/QCD_HT200to300*.root
hadd QCD_100to200.root $dir/QCD_HT100to200*.root
