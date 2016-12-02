#!/bin/bash

source set_htk_path.sh;

feature_dir=MFCC

[ -d $feature_dir/training ] || mkdir -p $feature_dir/training;
[ -d $feature_dir/testing ]  || mkdir -p $feature_dir/testing;

config=lib/hcopy.cfg
training_list=scripts/training_hcopy.scp
testing_list=scripts/testing_hcopy.scp

HCopy -T 1 -C $config -S $training_list
HCopy -T 1 -C $config -S $testing_list
