# optimisation_variables

# How to run the part of QCD data driven(local):

	git clone https://github.com/AllenChen1997/optimisation_variables.git
	cd optimisation_variables/QCD_data_driven/
	
## run for MC
Note that <inputname> and <outputname> are needed for run the code. 

inputname is the text file which contains the outputs root file from ExoPieElement

outputname is the output root file name like "myOutput.root"
	root -b -q GetRfunction_multi.C\(\"<inputname>\",\"<outputname>\"\)
you should see the <outputname> in your running folder

for local run the setup for the inputnames in the next step will become complex, the more easier way is running by condor. it can be seen in the section below.
**if you want to run local**
change inputMClist and xs array in DrawSRtest.C and run
	root -b -q DrawSRtest.C\(true\)
	root -b -q root_output\fit_curve_MC.C
you should see lots of plots with name fit_h_withSR_x.png

## run for data 
as similar in MC
	root -b -q GetRfunction_signal.C\(\"<inputname>\",\"<outputname>\"\)
	root -b -q DrawSRtest.C\(false\)
if you want to run the next step, make sure you have these three output files, fit_result.root(from MC part), keep_histo_cutFull.root(from data part), and the <outputname> from data part.
	root -b -q root_output\fit_from_MC.C

# How to run by condor 
	git clone git@github.com:AllenChen1997/condor_submit.git
	cd condor_submit
to be continue...

