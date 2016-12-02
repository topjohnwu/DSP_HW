#!/bin/bash

source set_htk_path.sh

config=lib/config.cfg
data_list=scripts/training.scp

mmf_dir=hmm
macro=$mmf_dir/macros
model=$mmf_dir/models

label=labels/Clean08TR.mlf
model_list=lib/models.lst

#################################################
# re-adjust mean, var
echo "step 01 [HErest]: adjust mean, var..."
for i in 0 1 2 3 4 5 6 7 8 9 10 11 12 13 14 ;
do
	echo "iteration $i"
	HERest -C $config -I $label \
		-t 250.0 150.0 1000.0 -S $data_list \
		-H $macro -H $model -M $mmf_dir $model_list
done

#################################################
# add short pause model, change model_list and label file
echo "step 02 [HHEd]: add \"sp\" model"

if [ ! -e bin/spmodel_gen ]; then
	cd bin/; make ; cd ..;
fi
bin/spmodel_gen $model $model
label=labels/Clean08TR_sp.mlf
model_list=lib/models_sp.lst
HHEd -T 2 -H $macro -H $model -M $mmf_dir lib/sil1.hed $model_list

# re-adjust mean, var
echo "step 03 [HErest]: adjust mean, var..."
for i in 0 1 2 ;
do
	echo "iteration $i"
	HERest -C $config -I $label \
		-t 250.0 150.0 1000.0 -S $data_list \
		-H $macro -H $model -M $mmf_dir $model_list
done


#################################################
# increase mixture
echo "step 04 [HHEd]: split gaussian mixture..."
HHEd -T 2 -H $macro -H $model -M $mmf_dir lib/mix2_10.hed $model_list

# re-adjust mean, var
echo "step 05 [HERest]: adjust mean, var..."
for i in 0 1 2 3 4 5 ;
do
	HERest -C $config -I $label \
		-t 250.0 150.0 1000.0 -S $data_list \
		-H $macro -H $model -M $mmf_dir $model_list
done

