# optimisation_variables

How to run the code:

	git clone https://github.com/AllenChen1997/optimisation_variables.git
	cd optimisation_variables/
	rm TH3_output.root
	root -b N2_study.C

you will see there is the TH3_output.root file, and lots of violin plot.

the number after the c_ means the selection eff. of QCD sample.

Next for getting the N2DDT or N2 distribution of Top, QCD, and signal, do

	root -b plot_3.C
	root -b plot_n2b2_3.C
	
Here we have new code for generating N2DDT weights:

you can follow the steps:

	git clone https://github.com/AllenChen1997/optimisation_variables.git
	cd optimisation_variables/
	root -b N2_study_new.C
	root -b cal_n2b1_3.C
	
you should get some png files, TH3_output_top.root as output maps, and punzi_result.txt as the comparing before and after applying N2DDT cut.