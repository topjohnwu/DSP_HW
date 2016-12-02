#!/bin/bash

source set_htk_path.sh;

config=lib/config.cfg
proto=lib/proto

# set output path and file of HCompV
init_mmf=hmmdef
mmf_dir=hmm

data_list=scripts/training.scp

HCompV -T 2 -D -C $config -o $init_mmf -f 0.01 \
	-m -S $data_list -M $mmf_dir $proto


############################################
# if you cannot run these binary file,     #
# use a c compiler to re-complie them.     #
# source codes are in the directory: bin/  #
############################################
out_macro=hmm/macros
out_model=hmm/models

if [ ! -e bin/macro ]; then
	cd bin/; make; cd ..
fi

if [ ! -e bin/models_1mixsil ]; then
	cd bin/; make; cd ..
fi

bin/macro 39 MFCC_Z_E_D_A $mmf_dir/vFloors $out_macro
bin/models_1mixsil $mmf_dir/$init_mmf $out_model

