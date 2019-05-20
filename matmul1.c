#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#define MASTER 0

int main(int argc, char* argv[]) {
	
	if (argc!=3){
		printf("\n Usage %s input file, output file \n",argv[0]);
		return -1;
	}

	int rank, size, n, istart, istop, chunk;
	MPI_Status status;
	//MPI_Datatype rowtype;
	MPI_Datatype coltype;
	
	MPI_Init (&argc, &argv);
	MPI_Comm_rank (MPI_COMM_WORLD, &rank);	
	MPI_Comm_size (MPI_COMM_WORLD, &size);

	//Read n and send to all
	FILE *input;
	if(rank==MASTER) {
		input = fopen(argv[1],"r");
		fscanf(input, "%d", &n);
	}

	MPI_Scatter(&n,1,MPI_INT,&n,1,MPI_INT,MASTER,MPI_COMM_WORLD);

	chunk  = n/size;
	printf("Rank: %d n: %d \n", rank, n);


	MPI_Type_vector(chunk, n, n, MPI_DOUBLE, &coltype);
	MPI_Type_commit(&coltype);



	double ** sub_array;
	double ** A;
/*Read the input file and create the matrices A and B*/
	//Stores the A matrix in master process
	printf("My rank is: %d chunk: %d \n", rank, chunk);
	if(rank == MASTER) {
		A = malloc(n*sizeof(double*));
		for (int i=0; i<n; i++){
			A[i] = malloc(n*sizeof(double));
		}

		for(int i=0;i<n;i++){
			for(int j=0;j<n;j++){
				fscanf(input,"%lf",&A[i][j]);	
			}		
		}

		for(int i=0;i<2;i++){
			for(int j=0;j<2;j++){
				printf("%lf ", A[i][j]);			
			}
			printf("\n ");	
		}

		for(int i=1; i<size; i++) {

			printf("My A is %lf \n", A[0][0]);
			MPI_Send(&A[0][0], 1, coltype, i, 111, MPI_COMM_WORLD);
		}
		printf("jag ar glad");
	}
	else {
		sub_array = malloc(chunk*sizeof(double*));
		for (int i=0; i<chunk; i++) {
			sub_array[i] = malloc(n*sizeof(double));
		}
		MPI_Recv(&sub_array[0][0],1,coltype,MASTER,111,MPI_COMM_WORLD,&status);
		printf("tung %lf \n", sub_array[0][0]);
	}
	
	

	//MPI_Type_vector(n,chunk,n, MPI_DOUBLE, &coltype);
	//MPI_Type_create_resized(rowtype, 0, sizeof(double), &rowtype);
	//MPI_Type_create_resized(coltype, 0, sizeof(double), &coltype);
	
	//MPI_Type_commit(&coltype); 
		
		/*double** block_A = (double **)malloc(n*sizeof(double*));	
		double** B = (double **)malloc(n*sizeof(double*));
		double** C = (double **)malloc(n*sizeof(double*));
		double** block_C = (double **)malloc(n*sizeof(double*));
		for(int i=0;i<n;i++){
			A[i] = (double*)malloc(n*sizeof(double));
			block_A[i] = (double*)malloc(chunk*sizeof(double));
			B[i] = (double*)malloc(n*sizeof(double));
			C[i] = (double*)malloc(n*sizeof(double));
			block_C[i] = (double*)malloc(chunk*sizeof(double));
		}*/

/*
		for(int i=0;i<n;i++){
			for(int j=0;j<n;j++){
				fscanf(input,"%lf",&A[i][j]);			
			}
		}

		for(int i=0;i<n;i++){
			for(int j=0;j<n;j++){
				fscanf(input,"%lf",&B[i][j]);			
			}
		}
   
		fclose(input);*/
	
	
/*print the input matrices*/
	/*if(rank==MASTER){
	for(int i=0;i<n;i++){
		for(int j=0;j<n;j++){
			printf("%lf ",A[i][j]);			
		}
			printf("\n ");	
	}
			printf("\n ");	
			printf("\n ");	

	for(int i=0;i<n;i++){
		for(int j=0;j<n;j++){
			printf("%lf ",B[i][j]);			
		}
			printf("\n ");	
	}


	}*/




/*partition A row wise and B column wise*/	
/*	double** block_B = (double **)malloc(chunk*sizeof(double*));
	for(int i=0;i<chunk;i++){
		block_B[i] = (double*)malloc(n*sizeof(double));
	}

	MPI_Type_vector(chunk,n,n, MPI_DOUBLE, &rowtype);
	MPI_Type_vector(n,chunk,n, MPI_DOUBLE, &coltype);
	MPI_Type_create_resized(rowtype, 0, sizeof(double), &rowtype);
	MPI_Type_create_resized(coltype, 0, sizeof(double), &coltype);
	MPI_Type_commit(&rowtype);
	MPI_Type_commit(&coltype); */




/*send blocks to processes*/
/*	int send_counts[size];
	int displs[size];

	for (int i=0; i<size; i++) {	
			displs[i] = rank*chunk;
			send_counts[i]=1;
	}


	MPI_Scatterv(A, send_counts,displs, rowtype, block_A, n*chunk, MPI_DOUBLE, MASTER, MPI_COMM_WORLD);
	MPI_Scatterv(B, send_counts,displs, coltype, block_B, n*chunk, MPI_DOUBLE, MASTER, MPI_COMM_WORLD);*/


/*
	if(rank==MASTER){
		printf("\n ");
		printf("\n ");
		for(int i=0;i<chunk;i++){
			for(int j=0;j<n;j++){
				printf("%lf ",block_A[i][j]);			
			}
		printf("\n ");	
		}

		printf("\n ");
		printf("\n ");


		for(int i=0;i<n;i++){
			for(int j=0;j<chunk;j++){
				printf("%lf ",block_B[i][j]);			
			}
			printf("\n ");	
		}
	}*/ 


	/*
	for (int i=0;i<chunk; i++) {
		for (int j=0;j<chunk;j++) {
			for(int k=0;k<n;k++){
				block_C[i][j]+= block_A[i][k]*block_B[k][j];
			}
		}
	}
                     

	MPI_Gatherv(block_C,chunk*chunk, MPI_DOUBLE, C,1,displs,coltype, MASTER, MPI_COMM_WORLD);*/

    /*                   
	if(rank==MASTER){
	for(int i=0;i<n;i++){
		for(int j=0;j<n;j++){
			printf("%lf ",C[i][j]);			
		}
			printf("\n ");	
	}


	}*/




	MPI_Finalize();
	return 0;
}