#!/bin/bash
if [ -z $1 ];then
	echo "use this bash script as following"
	echo "bash run_compare_plot_code.sh N"
	echo "N is the maximum number of the HT regions"
	exit 0
fi
Nregion=$1
for ((i=0; i < $1; i++));do
	echo "will look at region $i"
	root -b -q fit_from_MC.C\(\"JetHT-Run2017B-31Mar2018-v1.root\",$i\)
	sed -i '/lookingHTRegion =/c int lookingHTRegion = '$i';' combine_plots_corrPlot.C
	root -b -q combine_plots_corrPlot.C
	sed -i '/lookingHTRegion =/c int lookingHTRegion = '$i';' combine_plots_corrPlot_profileVer.C
	root -b -q combine_plots_corrPlot_profileVer.C
	mkdir -p region$i
	mv *.png region$i
done
