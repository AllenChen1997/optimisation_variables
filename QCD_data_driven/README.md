# optimisation_variables

# How to run the part of QCD data driven:
the analysis base on condor, the config file are at condor_submit on github
```
git clone https://github.com/AllenChen1997/optimisation_variables.git
cd optimisation_variables/QCD_data_driven/
git clone -b QCD_data_driven --single-branch  git@github.com:AllenChen1997/condor_submit.git
cd condor_submit
```
## run for trigger stugy
at optimisation_variables/QCD_data_driven/condor_submit/ do the submit of the jobs.
**before** you submit the condor job, please check the README.md first two steps in condor_submit for setting the enviroment and output directory for runAnalysis_testTrig.sh and submit_testTrig.sub

To submit one of period of datasets(B period for example)
```
bash file_split_signal_testTrig.sh ../filelist/JetHT-Run2017B-31Mar2018-v1.txt
```
To submit all of period of datasets
```
bash submit_all_signal_testTrig.sh
```
=================================================================
after condor job finished: go to optimisation_variables/QCD_data_driven/testTrig/
<your_output_dir> = /eos/user/k/kuchen/Trig_h_withHT_trigEff/ for me
```
bash auto_run_hadd.sh <your_output_dir>
root -b -q showTrigPrescales.C
bash runFull_draw_trig.sh
root -b -q draw_trigEff_step2.C
```
there will be the ratio plots and prescaleList.txt which can be used to decide which path to use.


## run getRfunction for MC
first thing is decide the using HTranges according the trigger efficiency plots and **change the array "HTUseRange" in the beginning of GetRfunction_MC.C**
in this step, different period signal data will have different HTranges

again go to optimisation_variables/QCD_data_driven/condor_submit/ to submit jobs to condor
**change the output directory in runAnalysis_getR_MC.sh to yours (the line with until)**
```
bash submit_all_QCDMC_getR.sh
```

after the job finished, go to optimisation_variables/QCD_data_driven/getR_MC/
<your_output_dir> = /eos/user/k/kuchen/getR_MC/ for me
```
bash Fullrun_hadd.sh <your_output_dir>
root -b -q DrawSRtest.C
root -b -q fit_curve_MC.C 
```

you should see lots of plots with name fit_h_withSR_x.png and fit_result.root

## run for data 
in case the range of selected HT is different in different period of data. there is no shell script which can submit jobs for all period data in same time.
as similar in MC, go to optimisation_variables/QCD_data_driven/condor_submit/ to submit jobs to condor
```
bash file_split_signal_getR.sh ../filelist/JetHT-Run2017B-31Mar2018-v1.txt
```
this is the example for period B. there are also other periods in filelist folder.
```
	root -b -q GetRfunction_signal.C\(\"<inputname>\",\"<outputname>\"\)
	root -b -q DrawSRtest.C\(false\)
```
if you want to run the next step, make sure you have these three output files, **fit_result.root(from MC part), keep_histo_cutFull.root(from data part), and the <outputname>** from data part.

	root -b -q root_output\fit_from_MC.C

	


