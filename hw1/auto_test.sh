make > /dev/null
for iter in 1 5 10 50 100 500 1000 5000; do
	echo "Iteration: $iter"
	for i in 1 2 3 4 5; do
		echo "Training Model $i..."
		time ./train $iter model_init.txt seq_model_0${i}.txt model_0${i}.txt
		echo ""
	done
	echo "Testing with iteration: $iter"
	./test modellist.txt testing_data1.txt result1_iter_${iter}.txt
	./test modellist.txt testing_data2.txt result2_iter_${iter}.txt
	for i in seq_model_*; do
		mv $i ${i##.*}_iter_${iter}.txt
	done
	./accuracy result1_iter_${iter}.txt testing_answer.txt accu_iter_${iter}.txt
done
