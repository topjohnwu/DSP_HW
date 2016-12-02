#!/bin/bash

source set_htk_path.sh

config=lib/config.cfg
macro=hmm/macros
model=hmm/models

test_data_list=scripts/testing.scp
dictionary=lib/dict

out_mlf=result/result.mlf

out_acc=result/accuracy
answer_mlf=labels/answer.mlf

model_list=lib/models_sp.lst
word_net=lib/wdnet_sp

HVite -D -H $macro -H $model -S $test_data_list -C $config -w $word_net \
	-l '*' -i $out_mlf -p 0.0 -s 0.0 $dictionary $model_list
	
HResults -e "???" sil -e "???" sp -I $answer_mlf $model_list \
	$out_mlf >> $out_acc
