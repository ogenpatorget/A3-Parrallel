matmul: matmul.c
	mpicc matmul.c -o matmul -std=c99 -lm -lmpi
birko: matmul_birko.c
	mpicc matmul_birko.c -o birko -std=c99 -lm -lmpi
run:
	mpirun -n 2 ./birko input2.txt output.txt
old_run:
	mpirun -n 2 ./matmul input2.txt output.txt
clean:
	rm matmul
	rm output.txts